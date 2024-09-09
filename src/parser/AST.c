/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rocky <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/12 19:35:45 by rocky             #+#    #+#             */
/*   Updated: 2024/08/28 20:11:50 by rocky            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tokens.h"

t_ast	*create_redir_node(t_token *token);
int		is_redir_node(t_token *tokens);
t_ast	*manage_redirs(t_token **tokens, t_ms_data *data);
t_ast	*manage_pipe(t_token **tokens, t_ms_data *data);
t_ast	*new_ast_node(void);
t_ast	*create_redir(t_token **tokens, t_token *tmp, t_ms_data *data);
int		cmd_arg_len(t_token *current);
void	set_command_args(t_ast *command_node, t_token **tokens, int cmd_arg_count);
t_ast	*manage_commands(t_token **tokens, t_ms_data *data);


t_ast	*parse_tokens(t_token **tokens, t_ms_data *data)
{
	if (!tokens || !*tokens)
		return (NULL);
	return (manage_pipe(tokens, data));
}

t_ast	*manage_pipe(t_token **tokens, t_ms_data *data)
{
	t_token		*tmp;
	t_token		*next_token;
	t_ast		*pipe_node;

	tmp = *tokens;
	while (*tokens && (*tokens)->next)
	{
		next_token = (*tokens)->next;
		if (next_token->type == PIPE)
		{
			pipe_node = new_ast_node();
			pipe_node->type = next_token->type;
			pipe_node->args = malloc(sizeof(char *) * 2);
			if (!pipe_node->args)
			{
				free(pipe_node);
				return (NULL);
			}
			pipe_node->args[0] = ft_strdup(next_token->data);
			pipe_node->args[1] = NULL;
			pipe_node->left = manage_redirs(&tmp, data);
			*tokens = next_token->next;
			if (next_token->next == NULL)
				pipe_node->right = NULL;
			else
				pipe_node->right = manage_pipe(tokens, data);
			return (pipe_node);
		}
		*tokens = next_token;
	}
	return (manage_redirs(&tmp, data));
}

t_ast	*manage_redirs(t_token **tokens, t_ms_data *data)
{
	t_ast		*command_node;
	t_ast		*redirect_node;
	t_token		*current_token;

	if (!tokens || !*tokens)
		return (NULL);
	command_node = manage_commands(tokens, data);
	current_token = *tokens;
	while (current_token && is_redir_node(current_token))
	{
		redirect_node = create_redir_node(current_token);
		redirect_node->left = command_node;
		*tokens = current_token->next;
		if (*tokens)
		{
			redirect_node->right = create_redir_node(*tokens);
			*tokens = (*tokens)->next;
		}
		else
			redirect_node->right = NULL;
		command_node = redirect_node;
		current_token = *tokens;
	}
	return (command_node);
}

t_ast	*manage_commands(t_token **tokens, t_ms_data *data)
{
	t_ast		*command_node;
	int			cmd_arg_count;

	command_node = new_ast_node();
	command_node->type = PHRASE;
	cmd_arg_count = cmd_arg_len(*tokens);
	command_node->args = malloc(sizeof(char *) * (cmd_arg_count + 1));
	if (!command_node->args)
		return (NULL);
	set_command_args(command_node, tokens, cmd_arg_count);
	(void)data;
	//post_process_command_args(command_node, cmd_arg_count, data);
	return (command_node);
}

void	set_command_args(t_ast *command_node, t_token **tokens, int arg_count)
{
	int		i;

	i = 0;
	while (i < arg_count)
	{
		command_node->args[i] = ft_strdup((*tokens)->data);
		*tokens = (*tokens)->next;
		i++;
	}
	command_node->args[arg_count] = NULL;
}


t_ast	*create_redir_node(t_token *token)
{
	t_ast			*node;

	node = new_ast_node();
	node->type = token->type;
	node->args = malloc(sizeof(char *) * 2);
	if (!node->args)
	{
		free(node);
		return (NULL);
	}
	node->args[0] = ft_strdup(token->data);
	node->args[1] = NULL;
	return (node);
}


int	is_redir_node(t_token *tokens)
{
	if (tokens->type == REDIR_IN
		|| tokens->type == REDIR_OUT
		|| tokens->type == REDIR_APPEND
		|| tokens->type == REDIR_HEREDOC)
		return (1);
	return (0);
}

t_ast	*new_ast_node(void)
{
	t_ast		*node;

	node = malloc(sizeof(t_ast));
	if (!node)
		return (NULL);
	node->type = NONE;
	node->args = NULL;
	node->left = NULL;
	node->right = NULL;
	return (node);
}

int	cmd_arg_len(t_token *current)
{
	int	i;

	i = 0;
	while (current && current->type == PHRASE)
	{
		i++;
		current = current->next;
	}
	return (i);
}