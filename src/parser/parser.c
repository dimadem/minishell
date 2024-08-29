/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rocky <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/12 19:35:45 by rocky             #+#    #+#             */
/*   Updated: 2024/07/11 18:44:23 by dmdemirk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tokens.h"

t_ast	*create_redir_node(t_token *token);
int		is_redir_node(t_token *tokens);
t_ast	*clr_node(t_token **tokens, t_token *next_token, t_ast *redirect_node, t_ms_data *data);
t_ast	*manage_redirs(t_token **tokens, t_ms_data *data);
t_ast	*manage_pipe(t_token **tokens, t_ms_data *data);

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
	free(token);
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

t_ast	*clr_node(t_token **tokens, t_token *next_token, t_ast *redirect_node, t_ms_data *data)
{
	(*tokens)->next = next_token->next->next;
	redirect_node->left = manage_redirs(tokens, data);
	redirect_node->right = create_redir_node((next_token->next));
	free(next_token->data);
	free(next_token);
	return (redirect_node);
}

t_ast	*manage_redirs(t_token **tokens, t_ms_data *data)
{
	t_token		*head;
	t_ast		*redirect_node;
	t_token		*next_token;

	if (!*tokens)
		return (NULL);
	head = *tokens;
	if (is_redir_node(*tokens))
		return (create_redir(tokens, head, data));
	while (*tokens && (*tokens)->next)
	{
		next_token = (*tokens)->next;
		if (is_redir_node(next_token))
		{
			redirect_node = new_ast_node((*tokens)->next->type);
			redirect_node->left = manage_commands(&head, data);
			*tokens = next_token->next;
			redirect_node->right = create_redir_node(next_token->next);
			return (redirect_node);
		}
		*tokens = next_token;
	}
	return (manage_commands(&head, data));
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