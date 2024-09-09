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
char		*token_adj(char *arg);

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
	result_len = 0;
	i = 0;
	while (tokens[i])
	{
		orig_token = tokens[i];
		tmp_token = expand_variable(&tokens[i], data);
		if (tmp_token)
		{
			processed_token = token_adj(tmp_token);
			if (processed_token != tmp_token)
			{
				new_token = ft_strdup(processed_token);
				free(tmp_token);
			}
			else
				new_token = tmp_token;
			free(processed_token);
			free(orig_token);
			tokens[i] = new_token;
			result_len += ft_strlen(new_token) + 1;
		}
		i++;
	}
	result = assemble_result(tokens, result_len);
	ft_free_2d_arr(tokens);
	return (result);
}

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
				return (ft_strdup(arg));
			ptr++;
		}
		if (*ptr == '\0')
		{
			new_arg = malloc(strlen(arg) + 2);
			if (!new_arg)
				return (NULL);
			ft_strcpy(new_arg, arg);
			ft_strcat(new_arg, "\"");
			return (new_arg);
		}
		if (*ptr == '\"' && *(ptr + 1) == '\0')
			return (ft_strdup(arg));
	}
	return (ft_strdup(arg));
}
