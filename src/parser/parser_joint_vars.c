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

char	*expand_variable(char **start, t_ms_data *data)
{
	char	*var_name;
	char	*expanded_var;
	char	*var_start;

	printf("expand_variable start: %s\n", *start);
	var_start = *start;
	if (**start == '\"')
	{
		(*start)++;
		var_start++;
	}
	(*start)++;
	while (**start != '\0' && (ft_isalnum(**start) || **start == '_' \
		|| **start == '?'))
		(*start)++;
	printf("expand_variable start2: %s\n", *start);
	var_name = ft_substr(var_start, 0, *start - var_start);
	printf("expand_variable start3: %s\n", var_name);
	expanded_var = expand_env_and_loc_var(var_name, data);
	printf("expand_variable start4: %s\n", expanded_var);
	free(var_name);
	if (ft_strlen(expanded_var) != 0)
		return (expanded_var);
	else
		return (ft_strdup(var_start));
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
	while (*start != '\0')
	{
		printf("arg: %s\n", arg);
		printf("start: %s\n", start);
		if (*start == '$')
		{
			expanded_var = expand_variable(&start, data);
			printf("expanded_var in process-argument (should be non << only): %s\n", expanded_var);
			if (expanded_var != NULL)
			{
				tmp = ft_strjoin(processed_arg, expanded_var);
				free(processed_arg);
				processed_arg = tmp;
				free(expanded_var);
			}
		}
		else
			processed_arg = append_literal(&start, processed_arg);
	}
	return (processed_arg);
}
