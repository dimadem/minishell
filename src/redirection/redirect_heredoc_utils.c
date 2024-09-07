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

int			redirect_here_doc(t_ast *node, t_ms_data *data);

/**
  - @brief This function handle "<<" heredoc functionality
  - 
  - @param node current node in the AST
  - @param data minishell data structure
  - @return status:
  - 0: success
  - 1: error
 */

static char	*assemble_result(char **tokens, size_t result_len)
{
	char	*result;
	int		i;

	result = malloc(result_len + 1);
	if (!result)
		return (NULL);
	*result = '\0';
	i = 0;
	while (tokens[i])
	{
		ft_strcat(result, tokens[i]);
		if (tokens[i + 1])
			ft_strcat(result, " ");
		i++;
	}
	free(tokens);
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
				return (arg);
			ptr++;
		}
		if (*ptr == '\0')
		{
			new_arg = malloc(strlen(arg) + 2);
			if (!new_arg)
				return (NULL);
			strcpy(new_arg, arg);
			strcat(new_arg, "\"");
			return (new_arg);
		}
		if (*ptr == '\"' && *(ptr + 1) == '\0')
			return (arg);
	}
	return (arg);
}


char	*process_and_reassemble(char *line, t_ms_data *data)
{
	char	**tokens;
	size_t	result_len;
	int		i;
	char	*processed_token;
	char	*result;

	tokens = ft_split(line, ' ');
	if (!tokens)
		return (NULL);
	result_len = 0;
	i = 0;
	while (tokens[i])
	{
		processed_token = expand_variable(&tokens[i], data);
		processed_token = token_adj(processed_token);
		tokens[i] = processed_token;
		result_len += ft_strlen(tokens[i]) + 1;
		i++;
	}
	result = assemble_result(tokens, result_len);
	return (result);
}

void	write_heredoc_lines(char **line, int file_fd, char *eof, \
			t_ms_data *data)
{
	while (*line && (ft_strcmp(*line, eof) != 0) && !g_heredoc_interrupted)
	{
		write(file_fd, *line, ft_strlen(*line));
		write(file_fd, "\n", 1);
		free(*line);
		*line = process_and_reassemble(readline("> "), data);
	}
}
