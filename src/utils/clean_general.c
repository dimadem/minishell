/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clean_general.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rocky <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/11 14:56:25 by rocky             #+#    #+#             */
/*   Updated: 2024/07/17 14:45:30 by dmdemirk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tokens.h"
#include "env.h"
#include "exit_status.h"

void	free_args(char **args)
{
	int	i;

	i = 0;
	if (args)
	{
		while (args[i] != NULL)
			free(args[i++]);
		free(args);
	}
}

void	free_env_list(t_env *env)
{
	t_env	*temp;

	while (env)
	{
		temp = env;
		env = env->next;
		free(env->key);
		free(env->value);
		free(temp);
	}
}

void	free_ms_data(t_ms_data *data)
{
	if (data)
	{
		free_shell_var_list(data->envp);
		free_shell_var_list(data->shell_variables);
		free(data->current_dir);
		if (data->std_in >= 0)
			close(data->std_in);
		if (data->std_out >= 0)
			close(data->std_out);
		if (data->std_err >= 0)
			close(data->std_err);
	}
}

void	loop_cleanup(t_loop_data *loop_data,  t_token *tokens_head)
{
	free(loop_data->trimmed_input);
	free_all_tokens(tokens_head);
	free(loop_data->prompt);
	free_ast(loop_data->tree);
}

void	free_ast(t_ast *node)
{
	int				i;

	i = 0;
	if (!node)
		return ;
	if (node->args)
	{
		while (node->args && node->args[i])
		{
			// ft_printf(GRN"t_ast node arg free'd: %s		at add: 	%p\n"RESET, node->args[i], node->args[i]);
			free(node->args[i]);
			i++;
		}
		free(node->args);
	}
	free_ast(node->left);
	free_ast(node->right);
	free(node);
}

void	free_all_tokens(t_token *tokens)
{
	t_token	*temp;

	while (tokens)
	{
		temp = tokens;
		tokens = tokens->next;
		if (temp)
		{
			if (temp->data)
			{
				// ft_printf(GRN"token free'd: %s		at add:		%p\n"RESET, temp->data, temp->data);
				free(temp->data);
				temp->data = NULL;
			}
		}
		free(temp);
		temp = NULL;
	}
}