/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmdemirk <dmdemirk@student.42london.c      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/04 11:19:58 by dmdemirk          #+#    #+#             */
/*   Updated: 2024/07/17 17:02:13 by dmdemirk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SHELL_H
# define SHELL_H

# include "env.h"
# include <signal.h>
# include <termios.h>

typedef struct s_minishell_data
{
	struct termios			terminal;
	char					**args;
	t_env					*envp;
	t_env					*local_env;
	int						std_in;
	int						std_out;
	int						std_err;
	char					*current_dir;
	int						exit_status;
}				t_minishell_data;

#endif
