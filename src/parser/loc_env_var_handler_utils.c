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

char	*expand_variable(char **start, t_ms_data *data);
char	*expand_env_and_loc_var(char *arg, t_ms_data *data);
void	final_quote_removal(int arg_count, t_ast *command_node);

char	*expand_variable(char **start, t_ms_data *data)
{
	char	*expanded_str;
	char	*var_name;
	char	*expanded_var;
	char	*result;
	char	*var_start;
	char	*str_start;
	int		len_before_var;
	char	*tmp;

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
			len_before_var = var_start - str_start;
			tmp = ft_substr(str_start, 0, len_before_var);
			result = ft_strjoin_free(expanded_str, tmp);
			free(tmp);
			expanded_str = ft_strjoin_free(result, expanded_var);
			str_start = *start;
			str_start = str_start_adj(str_start);
			free(expanded_var);
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

char	*expand_env_and_loc_var(char *arg, t_ms_data *data)
{
	char	*env_value;
	char	*exit_status;
	char	*env_value_dup;

	arg = exit_status_adj(arg);
	if (ft_strcmp(arg, "$?") == 0)
	{
		exit_status = get_shell_variable(data->shell_variables, "?");
		env_value_dup = ft_strdup(exit_status);
		return (env_value_dup);
	}
	else if (arg[0] == '$')
	{
		if (arg[ft_strlen(arg) - 1] == '"')
			arg[ft_strlen(arg) - 1] = '\0';
		env_value = get_env(data->envp, arg + 1);
		if (!env_value)
		{
			env_value = get_env(data->shell_variables, arg + 1);
		}
		if (env_value)
		{
			env_value_dup = ft_strdup(env_value);
			return (env_value_dup);
		}
	}
	env_value_dup = ft_strdup("");
	return (env_value_dup);
}

void	final_quote_removal(int arg_count, t_ast *command_node)
{
	int		i;
	size_t	len;
	char	*arg;
	char	*trimmed_arg;

	i = 0;
	while (i < arg_count)
	{
		arg = command_node->args[i];
		len = ft_strlen(arg);
		if ((arg[0] == '"' && arg[len - 1] == '"') || (arg[0] == '\''
				&& arg[len - 1] == '\''))
		{
			trimmed_arg = ft_strndup(arg + 1, len - 2);
			free(command_node->args[i]);
			command_node->args[i] = trimmed_arg;
		}
		i++;
	}
}

