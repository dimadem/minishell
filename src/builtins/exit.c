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
#include "errors.h"

/*
Functionality:
- Exit the shell
- Free the environment
- Exit with the exit status
 */

int builtin_exit(t_ms_data *data)
{

    int     number;

	number = 0;
    if (!ft_isnumber(data->args[1]))
    {
		shell_error_handler(NUMERIC_REQUIRED, ft_strjoin("exit: ", data->args[1]));
		set_exit_status(NUMERIC_REQUIRED);
        exit(NUMERIC_REQUIRED);
    }
    else if (data->args[2])
    {
		shell_error_handler(TOO_MANY_ARGS, "exit");
		data->exit_status = TOO_MANY_ARGS;
        exit(TOO_MANY_ARGS);
    }
    else
    {
        number = ft_atoi(data->args[1]);
        ft_putendl_fd("exit", STDOUT_FILENO);
		data->exit_status = number;
        exit(number);
    }
}
