/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmikhayl <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/05 13:23:26 by rmikhayl          #+#    #+#             */
/*   Updated: 2024/06/11 15:22:12 by rocky            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tokens.h"
#include "execute.h"
#include "shell.h"

void	main_loop(t_minishell_data data)
{
	char				*prompt;
	char				*line;
	t_token				*tokens;
	char				*trimmed_input;
	t_ast				*tree;

	prompt = generate_prompt(&data);
	line = readline(prompt);
	if (!line)
		exit(EXIT_FAILURE);
	make_history(line);
	trimmed_input = trim_input(line);
	input_error_checks(trimmed_input);
	tokens = tokenise(trimmed_input);
	if (!tokens)
	{
		ft_printf("Error: tokens could not be allocated");
		exit(EXIT_FAILURE);
	}
	print_tokens(tokens);
	ft_printf("^tokens before entering the tree\n");
	tree = parse_tokens(&tokens);
	//data.args = list_to_array(tokens);
	//execute(&data);
	loop_cleanup(trimmed_input, tokens, prompt, tree);
}

int	main(int argc, char **argv, char **envp)
{
	t_minishell_data	data;

	print_maxishell();
	init_minishell_data(&data, envp);
	initialise(argc, argv);
	while (1)
		main_loop(data);
	free_minishell_data(&data);
	free_env(data.envp);
	return (0);
}
