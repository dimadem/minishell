/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_checker.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rocky <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/11 16:51:08 by rocky             #+#    #+#             */
/*   Updated: 2024/06/11 16:53:40 by rocky            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tokens.h"

int	check_operators_loop(const char **str, int *single_quotes, \
			int *double_quotes, int *cmd_flag)
{
	if (**str == '\'')
		(*single_quotes)++;
	if (**str == '\"')
		(*double_quotes)++;
	
	const char *start = *str; 
	printf("start: %s\n", start);
	if (**str == '|' && !(*single_quotes % 2) && !(*double_quotes % 2))
	{
		printf("*str: %s\n", *str);
		if (*(*str + 1) == '\0')
		{
			// Prompt for more input after the pipe
			char *additional_input = readline("> ");
			printf("additional_input: %s\n", additional_input);
			if (additional_input && *additional_input != '\0')
			{
				// Allocate space to append the new input to the existing input
				size_t original_length = *str - start + 1;
				size_t new_length = original_length + strlen(additional_input) + 2; // +2 for space and null terminator
				// ^ here strlen(*str) is just 1 because *str is |, since that's waht we check for above. To get everything from the start of the string, we need to make a start pointer to keep track of the start of the striing after all the quotes are skipped, and make strlen the difference between start and str (accounting for |). Then we need to feed through start where necessary in the code. I think you understand
				printf("new_length: %ld\n", new_length);
				char *new_str = malloc(new_length);
				if (!new_str)
					exit(EXIT_FAILURE);

				// Copy current input and append a space and the new input
				strcpy(new_str, *str);  // Copy existing input
				printf("new_str: %s.\n", new_str);
				strcat(new_str, " ");   // Add space after '|'
				printf("new_str2: %s.\n", new_str);
				strcat(new_str, additional_input);  // Append additional input
				printf("new_str3: %s.\n", new_str);

				// Free the old input
				//free((char *)*str);  // Free old input

				*str = new_str;  // Update str to point to the combined input
				printf("new input: %s\n", *str);

				free(additional_input);  // Free the readline() result
			}
			else
			{
				free(additional_input);
				return (1);  // If no additional input is provided, return error
			}
			return (0);
		}
		if (*cmd_flag)
			return (1);
		*cmd_flag = 1;
	}
	else if (ft_strchr(" \t\n\r\v\f", **str) != NULL)
		*cmd_flag = 0;
	(*str)++;
	return (0);
}



int	check_operators(const char *str)
{
	int	single_quotes;
	int	double_quotes;
	int	cmd_flag;

	single_quotes = 0;
	double_quotes = 0;
	cmd_flag = 0;
	if (*str == '&' || *str == '|')
		return (1);
	while (*str)
		if (check_operators_loop(&str, &single_quotes, \
			&double_quotes, &cmd_flag) == 1)
			return (1);
	if (cmd_flag)
		return (1);
	return (0);
}

int	check_redirections(const char *str)
{
	int	single_quotes;
	int	double_quotes;

	single_quotes = 0;
	double_quotes = 0;
	while (*str)
	{
		if (*str == '\'')
			single_quotes++;
		if (*str == '\"')
			double_quotes++;
		if ((!(single_quotes % 2) && !(double_quotes % 2))
			&& (*str == '>' || *str == '<'))
		{
			if (!valid_operator(&str))
				return (1);
		}
		else
			str++;
	}
	return (0);
}

int	check_open_quotes(const char *str)
{
	int	single_quote_open;
	int	double_quote_open;

	single_quote_open = 0;
	double_quote_open = 0;
	while (*str)
	{
		if (*str == '\'' && !double_quote_open)
		{
			single_quote_open = !single_quote_open;
		}
		else if (*str == '"' && !single_quote_open)
			double_quote_open = !double_quote_open;
		str++;
	}
	return (single_quote_open || double_quote_open);
}

int	input_error_checks(const char *str)
{
	if (check_redirections(str))
		ft_printf("Input error: invalid redirection.\n");
	else if (check_operators(str))
		ft_printf("Input error: invalid operator.\n");
	else if (check_open_quotes(str))
		ft_printf("Input error: open quote.\n");
	else
		return (0);
	return (1);
}
