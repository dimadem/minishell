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

char	*process_and_reassemble(char *line, t_ms_data *data)
{
	char	**tokens;
	size_t	result_len;
	int		i;
	char	*processed_token;

	tokens = ft_split(line, ' ');
	if (!tokens)
		return (NULL);
	result_len = 0;
	i = 0;
	while (tokens[i])
	{
		processed_token = expand_env_and_loc_var(tokens[i], data);
		free(tokens[i]);
		tokens[i] = processed_token;
		result_len += ft_strlen(tokens[i]) + 1;
		i++;
	}
	return (assemble_result(tokens, result_len));
}