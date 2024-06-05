/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmdemirk <dmdemirk@student.42london.c      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/31 11:02:00 by dmdemirk          #+#    #+#             */
/*   Updated: 2024/06/05 18:20:29 by rmikhayl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"
#include "libft.h"
#include <stdio.h>
#include <sys/wait.h>
#include "shell.h"
#include "execute.h"

int	new_process(t_minishell_data *data);

int	execute(t_minishell_data *data)
{
	size_t	i;
	char	*builtin_commands[7];
	int		(*builtin_functions[7])(t_minishell_data *);

	builtin_commands[0] = "cd";
	builtin_commands[1] = "echo";
	builtin_commands[2] = "env";
	builtin_commands[3] = "exit";
	builtin_functions[0] = &builtin_cd;
	builtin_functions[1] = &builtin_echo;
	builtin_functions[2] = &builtin_env;
	builtin_functions[3] = &builtin_exit;
	if (data->args[0] == NULL)
		return (1);
	i = -1;
	while (++i < sizeof(builtin_commands) / sizeof(char *))
	{
		if (ft_strcmp(data->args[0], builtin_commands[i]) == 0)
			return ((*builtin_functions[i])(data));
		else
			return (new_process(data));
	}
	return (1);
}

int	new_process(t_minishell_data *data)
{
	pid_t	pid;
	int		status;
	char	*path;

	path = ft_find_path(data->args[0], data->envp);
	pid = fork();
	if (pid == 0)
	{
		if (execve(path, data->args, data->envp) == -1)
			perror("minishell");
	}
	else if (pid < 0)
		perror("minishell");
	else
		waitpid(pid, &status, 0);
	return (1);
}
