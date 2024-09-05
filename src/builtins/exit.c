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
<<<<<<< HEAD
#include "exit_status.h"
=======
#include "errors.h"
>>>>>>> fd09487 (new: errors)

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
    if (data->args[1] && ft_isnumber(data->args[1]) == 0)
    {
		exit_status_handler(data, NUMERIC_REQUIRED, ft_strjoin("exit: ", data->args[1]));
        exit(NUMERIC_REQUIRED);
    }
    else if (data->args[1] && data->args[2])
    {
		exit_status_handler(data, TOO_MANY_ARGS, "exit");
        exit(TOO_MANY_ARGS);
    }
    else if (data->args[1])
    {
        number = ft_atoi(data->args[1]);
        ft_putendl_fd("exit", STDOUT_FILENO);
		data->exit_status = number;
        set_shell_var(&data->shell_variables, "?", ft_itoa(data->exit_status));
        exit(number);
    }
    else
    {
        ft_putendl_fd("exit", STDOUT_FILENO);
        exit(0);
    }
}
