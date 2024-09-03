/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmdemirk <dmdemirk@student.42london.c      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/03 16:32:42 by dmdemirk          #+#    #+#             */
/*   Updated: 2024/07/11 14:59:10 by dmdemirk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"
#include "libft.h"
#include <stdlib.h>
#include <stdio.h>
#include "env.h"

/*
Functionality:
- Exit the shell
- Free the environment
- Exit with the exit status
 */

int	builtin_exit(t_ms_data *data)
{
	char	*message;

	message = "exit\n";
	ft_putstr_fd(message, STDOUT_FILENO);
	exit(data->exit_status);
}
