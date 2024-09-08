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
		if ((arg[0] == '"' && arg[len - 1] == '"') || (arg[0] == '\'' \
			&& arg[len - 1] == '\''))
		{
			trimmed_arg = ft_strndup(arg + 1, len - 2);
			free(command_node->args[i]);
			command_node->args[i] = trimmed_arg;
		}
		i++;
	}
}

char *exit_status_adj(char *arg)
{
    if (strcmp(arg, "$") == 0)
	{
		free(arg);
        return strdup("$?");
	}
    return arg;
}

char	*expand_env_and_loc_var(char *arg, t_ms_data *data)
{
	char	*env_value;
	char	*exit_status;

	arg = exit_status_adj(arg);
	if (ft_strcmp(arg, "$?") == 0)
	{
		exit_status = get_shell_variable(data->shell_variables, "?");
		return (ft_strdup(exit_status));
	}
	else if (arg[0] == '$')
	{
		if (arg[ft_strlen(arg) - 1] == '"')
			arg[ft_strlen(arg) - 1] = '\0';
		env_value = get_env(data->envp, arg + 1);
		if (!env_value)
			env_value = get_env(data->shell_variables, arg + 1);
		if (env_value)
			return (ft_strdup(env_value));
	}
	return (ft_strdup(""));
}

void	handle_local_vars(t_ms_data *data, char *arg)
{
	char	*p;
	int		valid_var;

	p = arg;
	valid_var = 1;
	if (arg[0] != '=' && strchr(arg, '='))
	{
		while (p < strchr(arg, '='))
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

int	is_in_single_quotes(char *arg)
{
	int	len;

	if (!arg || arg[0] != '\'')
		return (0);
	len = 0;
	while (arg[len] != '\0')
		len++;
	if (arg[len - 1] == '\'')
		return (1);
	return (0);
}

void	post_process_command_args(t_ast *command_node, int arg_count \
			, t_ms_data *data)
{
	int		i;
	char	*processed_arg;

	i = 0;
	while (i < arg_count)
	{
		if (!is_in_single_quotes(command_node->args[i]))
		{
			handle_local_vars(data, command_node->args[i]);
			processed_arg = process_argument(command_node->args[i], data);
		}
		else
		{
			processed_arg = ft_substr(command_node->args[i], 1, \
				ft_strlen(command_node->args[i]) - 2);
		}
		free(command_node->args[i]);
		command_node->args[i] = processed_arg;
		i++;
	}
	final_quote_removal(arg_count, command_node);
}


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
			len_before_var = var_start - str_start;
			result = ft_strjoin_free(expanded_str, ft_substr(str_start, 0, len_before_var));
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
		if (*start == '$')
		{
			expanded_var = expand_variable(&start, data);
			if (expanded_var != NULL)
			{
				tmp = tmp_adj(ft_strjoin(processed_arg, expanded_var));
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
