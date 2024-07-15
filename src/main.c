/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmikhayl <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/05 13:23:26 by rmikhayl          #+#    #+#             */
/*   Updated: 2024/07/11 18:02:20 by dmdemirk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tokens.h"
#include "execute.h"
#include "shell.h"
#include "pipe.h"
#include "signals.h"

int	status_handler(int status, t_loop_data *loop_data)
{
	if (status == WAIT_NEXT_COMMAND)
	{
		loop_cleanup(loop_data->trimmed_input, loop_data->tokens, \
				loop_data->prompt, loop_data->tree);
		return (0);
	}
	return (1);
}

void	main_loop(t_minishell_data *data, t_loop_data *loop_data)
{
	int	status;

	while (1)
	{	
		if(g_signo == SIGINT || g_signo == SIGQUIT)
		{
			signals_handler(g_signo, data);
			g_signo = 0;
			continue ;
		}
		loop_data->prompt = generate_prompt(data);
		loop_data->input = readline(loop_data->prompt);
		if (loop_data->input == NULL)
		{
			write(STDOUT_FILENO, "exit\n", 5);
			break ;
		}
		make_history(loop_data->input);
		loop_data->trimmed_input = trim_input(loop_data->input);
		input_error_checks(loop_data->trimmed_input);
		loop_data->tokens = tokenise(loop_data->trimmed_input);
		loop_data->tree = parse_tokens(&loop_data->tokens);
		print_ast_root(loop_data->tree);
		status = execute_ast(loop_data->tree, data);
		if (status_handler(status, loop_data))
		{
			handle_io_fd(data);
			loop_cleanup(loop_data->trimmed_input, loop_data->tokens, \
					loop_data->prompt, loop_data->tree);
		}
	}
}

int	main(int argc, char **argv, char **envp)
{
	t_minishell_data	data;
	t_loop_data			loop_data;

	print_maxishell();
	init_minishell_data(&data, envp);
	init_signals();
	initialise(argc, argv);
	main_loop(&data, &loop_data);
	free_minishell_data(&data);
	free_env(data.envp);
	return (0);
}
