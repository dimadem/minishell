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

void	loop_cleanup(char *line, t_token *tokens, char *prompt, t_ast *tree)
{
	free(line);
	free(prompt);
	(void)tokens;
	free_ast(tree);
}

