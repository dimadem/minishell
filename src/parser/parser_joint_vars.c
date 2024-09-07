/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_joint_vars.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmikhayl <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/04 16:46:53 by rmikhayl          #+#    #+#             */
/*   Updated: 2024/09/04 16:46:57 by rmikhayl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <tokens.h>

char *str_start_adj(char *arg)
{
    if (!strcmp(arg, "?") || !strcmp(arg, "?\""))
        return strdup("");
	else if (!strcmp(arg, "?\'"))
        return strdup("\'");
    return arg;
}

char	*tmp_adj(char *arg)
{
	char	*ptr;

	if (*arg == '\"')
	{
		ptr = arg + 1;
		while (*ptr)
		{
			if (!ft_isdigit(*ptr))
				return (arg);
			ptr++;
		}
		return (ft_strdup(arg + 1));
	}
	return (arg);
}

char	*expand_variable(char **start, t_ms_data *data)
{
	char	*expanded_str;
	char	*var_name;
	char	*expanded_var;
	char	*result;
	char	*var_start;
	char	*str_start;
	int		len_before_var;

	str_start = *start;
	expanded_str = ft_strdup("");
	while (**start && **start != '\0')
	{
		if (**start == '$')
		{
			var_start = *start;
			(*start)++;

			while (**start && (ft_isalnum(**start) || **start == '_'))
				(*start)++;
			var_name = ft_substr(var_start, 0, *start - var_start);
			expanded_var = expand_env_and_loc_var(var_name, data);
			free(var_name);
			printf("expanded_var: %s\n", expanded_var);
			len_before_var = var_start - str_start;
			result = ft_strjoin_free(expanded_str, ft_substr(str_start, 0, len_before_var));
			printf("result: %s\n", result);
			expanded_str = ft_strjoin_free(result, expanded_var);
			printf("expanded_str: %s\n", expanded_str);
			str_start = *start;
			str_start = str_start_adj(str_start);
			printf("str_start: %s\n", str_start);
			free(expanded_var);
		}
		else
			(*start)++;
	}
	if (str_start != *start)
	{
		result = ft_substr(str_start, 0, *start - str_start);
		printf("result: %s\n", result);
		expanded_str = ft_strjoin_free(expanded_str, result);
		printf("expanded_str2: %s\n", expanded_str);
		free(result);
	}
	return (expanded_str);
}

char	*append_literal(char **start, char *processed_arg)
{
	char	*literal_part;
	char	*literal_start;
	char	*tmp;

	literal_start = *start;
	while (**start != '\0' && **start != '$')
		(*start)++;
	literal_part = ft_substr(literal_start, 0, *start - literal_start);
	tmp = ft_strjoin(processed_arg, literal_part);
	free(literal_part);
	free(processed_arg);
	return (tmp);
}

char	*process_argument(char *arg, t_ms_data *data)
{
	char	*start;
	char	*processed_arg;
	char	*tmp;
	char	*expanded_var;

	processed_arg = ft_strdup("");
	start = arg;
	printf(RED"arg: %s\n"RESET, arg);
	while (*start != '\0')
	{
		if (*start == '$')
		{
			expanded_var = expand_variable(&start, data);
			printf("expanded_var: %s\n", expanded_var);
			if (expanded_var != NULL)
			{
				printf(BLU"processed_arg: %s\n"RESET, processed_arg);
				printf(BLU"expanded_var: %s\n"RESET, expanded_var);
				tmp = tmp_adj(ft_strjoin(processed_arg, expanded_var));
				printf(BLU"tmp: %s\n"RESET, tmp);
				free(processed_arg);
				processed_arg = tmp;
				printf(BLU"shouldn't be here. processed_arg: %s\n"RESET, processed_arg);
				free(expanded_var);
			}
			printf("processed_arg1: %s\n", processed_arg);
		}
		else
			processed_arg = append_literal(&start, processed_arg);
	}
	printf("processed_arg2: %s\n", processed_arg);
	return (processed_arg);
}
