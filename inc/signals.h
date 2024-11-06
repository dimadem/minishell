/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmdemirk <dmdemirk@student.42london.c      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/17 14:07:19 by dmdemirk          #+#    #+#             */
/*   Updated: 2024/09/09 13:20:52 by dmdemirk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SIGNALS_H
# define SIGNALS_H
# define _GNU_SOURCE

# include <termios.h>
# include <sys/ioctl.h>
# include <signal.h>
# include "shell.h"

typedef struct s_signal_context {
    t_ms_data *data_cxt;
} t_signal_context;

// GLOBAL VAR
extern volatile sig_atomic_t	g_heredoc_interrupted;

/*      signals     */
void signal_reset_prompt(int signo, siginfo_t *info, void *ucontext);
void	set_signals_interactive(t_ms_data *data);
void	signal_print_newline(int signal);
void	sigquit_ignore(void);
void	set_signals_noninteractive(void);
void	handle_sigint_heredoc(int signo);

int		ft_perror(char *str);

t_signal_context *get_context(t_ms_data *data);

#endif
