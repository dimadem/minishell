/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   loc_env_var_handler_utils.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmikhayl <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/10 01:45:18 by rmikhayl          #+#    #+#             */
/*   Updated: 2024/09/10 01:45:21 by rmikhayl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tokens.h"

char	*ev_loop(char **start, t_ms_data *data, char *str_start, \
			char *expanded_str)
{
	char	*var_name;
	char	*var_start;
	char	*expanded_var;
	char	*tmp;
	int		len_before_var;

	var_start = *start;
	(*start)++;
	while (**start && (ft_isalnum(**start) || **start == '_'))
		(*start)++;
	var_name = ft_substr(var_start, 0, *start - var_start);
	expanded_var = expand_env_and_loc_var(var_name, data);
	free(var_name);
	len_before_var = var_start - str_start;
	tmp = ft_substr(str_start, 0, len_before_var);
	expanded_str = ft_strjoin_free(expanded_str, tmp);
	free(tmp);
	expanded_str = ft_strjoin_free(expanded_str, expanded_var);
	free(expanded_var);
	return (expanded_str);
}

char	*expand_variable(char **start, t_ms_data *data)
{
	char	*expanded_str;
	char	*str_start;
	char	*result;

	result = NULL;
	str_start = *start;
	expanded_str = ft_strdup("");
	while (**start && **start != '\0')
	{
		if (**start == '$')
		{
			expanded_str = ev_loop(start, data, str_start, expanded_str);
			str_start = *start;
			str_start = str_start_adj(str_start);
		}
		else
			(*start)++;
	}
	if (str_start != *start)
	{
		result = ft_substr(str_start, 0, *start - str_start);
		expanded_str = ft_strjoin_free(expanded_str, result);
		free(result);
	}
	return (expanded_str);
}

void	handle_local_vars(t_ms_data *data, char *arg)
{
	char	*p;
	int		valid_var;

	p = arg;
	valid_var = 1;
	if (arg[0] != '=' && ft_strchr(arg, '='))
	{
		while (p < ft_strchr(arg, '='))
		{
			if (!ft_isalnum(*p++))
			{
				valid_var = 0;
				break ;
			}
		}
		if (valid_var)
			handle_add_set_shell_variable(&data->shell_variables, arg);
	}
}

char	**ft_add_segment(char **result, char *start, size_t length, int *count)
{
	result = realloc(result, (*count + 1) * sizeof(char *));
	result[*count] = ft_strndup(start, length);
	(*count)++;
	return (result);
}

char	**ft_split_preserve_quotes(char *str, char delimiter)
{
	char	**result;
	int		count;
	int		in_quotes;
	char	*start;

	result = NULL;
	count = 0;
	in_quotes = 0;
	start = str;
	while (*str)
	{
		if (*str == '"' || *str == '\'')
			in_quotes = !in_quotes;
		else if (*str == delimiter && !in_quotes)
		{
			result = ft_add_segment(result, start, str - start, &count);
			start = str + 1;
		}
		str++;
	}
	if (str != start)
		result = ft_add_segment(result, start, str - start, &count);
	result[count] = NULL;
	return (result);
}
