/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmdemirk <dmdemirk@student.42london.c      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/03 16:31:33 by dmdemirk          #+#    #+#             */
/*   Updated: 2024/08/28 14:20:26 by dmdemirk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "shell.h"
#include "errors.h"

/*
	Functionality:
		- Print the argument
*/

int	builtin_echo(t_ms_data *data)
{
	int	newline;
	int	i;

	newline = 1;
	if (data->args[1] && (ft_strcmp(data->args[1], "-n") == 0))
	{
		newline = 0;
		data->args++;
	}
	i = 0;
	while (data->args[++i])
	{
		ft_putstr_fd(data->args[i], STDOUT_FILENO);
		if (data->args[i + 1])
			ft_putstr_fd(" ", STDOUT_FILENO);
	}
	if (newline)
		write(STDOUT_FILENO, "\n", 1);
	return (SUCCESS);
}
