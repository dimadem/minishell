/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmdemirk <dmdemirk@student.42london.c      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/04 11:19:58 by dmdemirk          #+#    #+#             */
/*   Updated: 2024/06/26 15:28:37 by dmdemirk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SHELL_H
# define SHELL_H

# include "env.h"

typedef struct s_minishell_data
{
	char	**args;
	t_env	*envp;
	t_env	*local_env;
	int		temp_fd;
	char	*current_dir;
	int		exit_status;
}				t_minishell_data;

#endif
