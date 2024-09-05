/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmdemirk <dmdemirk@student.42london.c      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/17 11:10:03 by dmdemirk          #+#    #+#             */
/*   Updated: 2024/06/17 11:10:25 by dmdemirk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"
#include "env.h"
#include "libft.h"
#include <unistd.h>
#include "exit_status.h"

#include <stdio.h>

/*
   todo
   set the exit status to ERROR if the variable is not a valid identifier
Functionality:
- Unset the environment variable
- Return ERROR if the variable is not a valid identifier    
 */

int	builtin_unset(t_ms_data *data)
{
	int		i;
	char	*key;

	i = 0;
	while (data->args[++i])
	{
		key =  data->args[i];
		printf("key: %s\n", key);
		if (unset_env(&data->envp, key) == -1)
		{
			// ft_putstr_fd("bash: unset: `", STDERR_FILENO);
			// ft_putstr_fd(key, STDERR_FILENO);
			// ft_putendl_fd("': not a valid identifier", STDERR_FILENO);
			// set_exit_status(&data->exit_status, NOT_VALID_IDENTIFIER);
			// set_shell_var(&data->shell_variables, "?", ft_itoa(data->exit_status));
			return (EXIT_SUCCESS);
		}
	}
	return (EXIT_SUCCESS);
}
