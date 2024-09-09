/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect_heredoc.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmdemirk <dmdemirk@student.42london.c      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/11 14:32:29 by dmdemirk          #+#    #+#             */
/*   Updated: 2024/07/11 16:00:30 by dmdemirk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"
#include "tokens.h"
#include "redirection.h"
#include "execute.h"
#include <fcntl.h>
#include <sys/wait.h>
#include "signals.h"

static char	*assemble_result(char **tokens, size_t result_len);
char	*token_adj(char *arg);

char	*process_and_reassemble(char *line, t_ms_data *data)
{
	char	**tokens;
	size_t	result_len;
	int		i;
	char	*processed_token;
	char	*result;
	char	*tmp_token;
	char	*new_token;
	char	*orig_token;

	tokens = ft_split(line, ' ');
	free(line);
	if (!tokens)
		return (NULL);
	ft_print_2d_arr(tokens);
	result_len = 0;
	i = 0;
	while (tokens[i])
	{
		orig_token = tokens[i];  // Store the original token
		tmp_token = expand_variable(&tokens[i], data);  // Get expanded token (may be the same or new)
		if (tmp_token)
		{
			printf("post-expand_variable &tokens[i] below:\n");
			ft_print_2d_arr(tokens);
			processed_token = token_adj(tmp_token);  // Adjust the token
			if (processed_token != tmp_token)
			{
				new_token = ft_strdup(processed_token);  // Duplicate the adjusted token
				free(tmp_token);  // Free the temporary token returned by expand_variable
			}
			else
			{
				new_token = tmp_token;  // No changes, use the token as-is
			}
			free(orig_token);  // Free the original token from ft_split
			tokens[i] = new_token;  // Assign the newly adjusted or expanded token
			result_len += ft_strlen(new_token) + 1;
		}
		i++;
	}
	printf("post-loop &tokens[i] below:\n");
	ft_print_2d_arr(tokens);
	result = assemble_result(tokens, result_len);
	printf("post-assemble_result &tokens[i] below:\n");
	ft_print_2d_arr(tokens);
	ft_free_2d_arr(tokens);  // Free the tokens array after use
	return (result);
}


static char	*assemble_result(char **tokens, size_t result_len)
{
	char	*result;
	int		i;

	result = malloc(result_len + 1);
	if (!result)
		return (NULL);
	*result = '\0';  // Start with an empty string
	i = 0;
	while (tokens[i])
	{
		ft_strcat(result, tokens[i]);
		if (tokens[i + 1])
			ft_strcat(result, " ");  // Add space between tokens
		i++;
	}
	return (result);
}

char	*token_adj(char *arg)
{
	char	*ptr;
	char	*new_arg;

	if (*arg == '\"')
	{
		ptr = arg + 1;
		while (*ptr && *ptr != '\"')
		{
			if (!ft_isdigit(*ptr))
				return (arg);  // Return the original argument if not all digits inside quotes
			ptr++;
		}
		if (*ptr == '\0')  // If there's no closing quote, add one
		{
			new_arg = malloc(strlen(arg) + 2);
			if (!new_arg)
				return (NULL);
			ft_strcpy(new_arg, arg);
			ft_strcat(new_arg, "\"");
			return (new_arg);
		}
		if (*ptr == '\"' && *(ptr + 1) == '\0')  // If it's a properly quoted string, return as-is
			return (arg);
	}
	return (arg);  // Return the original
}