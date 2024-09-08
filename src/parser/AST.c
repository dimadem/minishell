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
t_ast	*new_ast_node(t_token_type type);
t_ast	*create_redir(t_token **tokens, t_token *tmp, t_ms_data *data);
int		arg_len(t_token *current);
void	set_command_args(t_ast *command_node, t_token **tokens, int arg_count);
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
		if ((*tokens)->next->type == PIPE)
		{
			pipe_node = new_ast_node((*tokens)->next->type);
			(*tokens)->next = NULL;
			pipe_node->left = manage_redirs(&tmp, data);
			if (next_token->next == NULL)
				pipe_node->right = NULL;
			else
				pipe_node->right = manage_pipe(&(next_token->next), data);
			free(next_token->data);
			free(next_token);
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
		// ft_printf(YEL"freeing curr_token (redir): %s		(manage_redirs)\n"RESET, current_token->data);
		// free(current_token->data);
		redirect_node->left = command_node;
		print_ast_args(command_node);
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
	int			arg_count;

	command_node = new_ast_node(PHRASE);
	arg_count = arg_len(*tokens);
	command_node->args = malloc(sizeof(char *) * (arg_count + 1));
	if (!command_node->args)
		return (NULL);
	set_command_args(command_node, tokens, arg_count);
	(void)data;
	//post_process_command_args(command_node, arg_count, data);
	return (command_node);
}

void	set_command_args(t_ast *command_node, t_token **tokens, int arg_count)
{
	int		i;

	i = 0;
	while (i < arg_count)
	{
		// ft_printf("set_command_args tokens:\n");
		// print_tokens(*tokens);
		command_node->args[i] = ft_strdup((*tokens)->data);
		*tokens = (*tokens)->next;
		i++;
	}
	command_node->args[arg_count] = NULL;
}














t_ast	*new_ast_node(t_token_type type)
{
	t_ast		*node;

	node = malloc(sizeof(t_ast));
	if (!node)
		return (NULL);
	node->type = type;
	node->args = NULL;
	node->left = NULL;
	node->right = NULL;
	return (node);
}

t_ast	*create_redir(t_token **tokens, t_token *tmp, t_ms_data *data)
{
	t_ast	*redirect_node;

	redirect_node = new_ast_node((*tokens)->type);
	*tokens = (*tokens)->next->next;
	redirect_node->left = manage_redirs(tokens, data);
	redirect_node->right = create_redir_node(tmp->next);
	free(tmp->data);
	free(tmp);
	return (redirect_node);
}

int	arg_len(t_token *current)
{
	int	arg_count;

	arg_count = 0;
	while (current && current->type == PHRASE)
	{
		arg_count++;
		current = current->next;
	}
	return (arg_count);
}

t_ast	*create_redir_node(t_token *token)
{
	t_ast			*node;

	node = malloc(sizeof(t_ast));
	if (!node)
		return (NULL);
	node->type = token->type;
	node->args = malloc(sizeof(char *) * 2);
	if (!node->args)
	{
		free(node);
		return (NULL);
	}
	node->args[0] = token->data;
	node->args[1] = NULL;
	node->left = NULL;
	node->right = NULL;
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

