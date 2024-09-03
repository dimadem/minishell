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
#include "errors.h"

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
		if (unset_env(&data->envp, key) == -1)
		{
			ft_putstr_fd("bash: unset: `", STDERR_FILENO);
			ft_putstr_fd(key, STDERR_FILENO);
			ft_putendl_fd("': not a valid identifier", STDERR_FILENO);
			return (ERROR);
		}
	}
	return (SUCCESS);
}
