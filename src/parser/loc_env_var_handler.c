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

void	handle_local_vars(t_ms_data *data, char *arg);
char	*process_argument(char *arg, t_ms_data *data);
char	*expand_variable(char **start, t_ms_data *data);
void	final_quote_removal(int arg_count, t_ast *command_node);
char	*exit_status_adj(char *arg);
char	*tmp_adj(char *arg);
char	*expand_env_and_loc_var(char *arg, t_ms_data *data);
char	*str_start_adj(char *arg);

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
			handle_local_vars(data, command_node->args[i]); // no mallocs --> noleaks poss
			processed_arg = process_argument(command_node->args[i], data);
		}
		else
		{
			processed_arg = ft_substr(command_node->args[i], 1, \
				ft_strlen(command_node->args[i]) - 2);
			ft_printf(RED"malloc'd processed_arg: %s	at ad: %p\n"RESET, processed_arg, processed_arg);
		}
		ft_printf(GRN"freed command_node->args[i]: %s	at ad: %p\n"RESET, command_node->args[i], command_node->args[i]);
		free(command_node->args[i]);
		ft_printf(RED"malloc'd command_node->args[i]: %s	at ad: %p\n"RESET, command_node->args[i], command_node->args[i]);
		command_node->args[i] = ft_strdup(processed_arg);
		i++;
	}
	ft_printf(GRN"freed new processed_arg: %s	at ad: %p\n"RESET, processed_arg, processed_arg);
	free(processed_arg);
	final_quote_removal(arg_count, command_node);
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

char	*process_argument(char *arg, t_ms_data *data)
{
	char	*start;
	char	*processed_arg;
	char	*tmp;
	char	*expanded_var;

	processed_arg = ft_strdup("");
	ft_printf(RED"malloc'd processed_arg: %s	at ad: %p\n"RESET, processed_arg, processed_arg);
	start = arg;
	while (*start != '\0')
	{
		if (*start == '$')
		{
			expanded_var = expand_variable(&start, data);
			if (expanded_var != NULL)
			{
				tmp = tmp_adj(ft_strjoin(processed_arg, expanded_var));
				ft_printf(GRN"freed processed_arg: %s	at ad: %p\n"RESET, processed_arg, processed_arg);
				free(processed_arg);
				ft_printf(GRN"freed expanded_var: %s	at ad: %p\n"RESET, expanded_var, expanded_var);
				free(expanded_var);
				processed_arg = tmp;
				ft_printf(GRN"freed tmp_adj_dup: %s	at ad: %p\n"RESET, tmp, tmp);
				free(tmp);
			}
		}
		else
			processed_arg = append_literal(&start, processed_arg);
	}
	return (processed_arg);
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
	ft_printf(RED"malloc'd expanded_str: %s	at ad: %p\n"RESET, expanded_str, expanded_str);
	
	while (**start && **start != '\0')
	{
		if (**start == '$')
		{
			var_start = *start;
			(*start)++;

			while (**start && (ft_isalnum(**start) || **start == '_'))
				(*start)++;
			var_name = ft_substr(var_start, 0, *start - var_start);
			ft_printf(RED"malloc'd var_name: %s	at ad: %p\n"RESET, var_name, var_name);
			expanded_var = expand_env_and_loc_var(var_name, data);
			ft_printf(RED"malloc'd expanded_var: %s	at ad: %p\n"RESET, expanded_var, expanded_var);
			ft_printf(GRN"freed var_name: %s	at ad: %p\n"RESET, var_name, var_name);
			free(var_name);
			len_before_var = var_start - str_start;
			result = ft_strjoin_free(expanded_str, ft_substr(str_start, 0, len_before_var));
			ft_printf(RED"malloc'd result: %s	at ad: %p\n"RESET, result, result);
			expanded_str = ft_strjoin_free(result, expanded_var);
			ft_printf(RED"malloc'd expanded_str: %s	at ad: %p\n"RESET, expanded_str, expanded_str);
			str_start = *start;
			str_start = str_start_adj(str_start);
			ft_printf(RED"malloc'd str_start: %s	at ad: %p\n"RESET, str_start, str_start);
			ft_printf(GRN"freed expanded_var: %s	at ad: %p\n"RESET, expanded_var, expanded_var);
			free(expanded_var);
		}
		else
			(*start)++;
	}
	if (str_start != *start)
	{
		result = ft_substr(str_start, 0, *start - str_start);
		ft_printf(RED"malloc'd result: %s	at ad: %p\n"RESET, result, result);
		expanded_str = ft_strjoin_free(expanded_str, result);
		ft_printf(RED"malloc'd expanded_str: %s	at ad: %p\n"RESET, expanded_str, expanded_str);
		ft_printf(GRN"freed result: %s	at ad: %p\n"RESET, result, result);
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
		ft_printf(RED"malloc'd (exit status) env_value_dup: %s	at ad: %p\n"RESET, env_value_dup, env_value_dup);
		return (env_value_dup);
	}
	else if (arg[0] == '$')
	{
		if (arg[ft_strlen(arg) - 1] == '"')
			arg[ft_strlen(arg) - 1] = '\0';
		env_value = get_env(data->envp, arg + 1);
		if (!env_value)
		{
			env_value = get_env(data->shell_variables, arg + 1); // is strdup necessary?
			env_value_dup = ft_strdup(env_value);
			ft_printf(RED"malloc'd env_value_dup: %s	at ad: %p\n"RESET, env_value_dup, env_value_dup);
		}
		if (env_value)
		{
			env_value_dup = ft_strdup(env_value);
			ft_printf(RED"malloc'd env_value_dup: %s	at ad: %p\n"RESET, env_value_dup, env_value_dup);
			return (env_value_dup);
		}
	}
	env_value_dup = ft_strdup("");
	ft_printf(RED"malloc'd empty env_value_dup: %s	at ad: %p\n"RESET, env_value_dup, env_value_dup);
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
	char *exit_status;

    if (strcmp(arg, "$") == 0)
	{
		free(arg);
		exit_status = strdup("$?");
		ft_printf(RED"malloc'd exit_status: %s	at ad: %p\n"RESET, exit_status, exit_status);
        return exit_status;
	}
    return arg;
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
	char	*tmp_adj_dup;

	if (*arg == '\"')
	{
		ptr = arg + 1;
		while (*ptr)
		{
			if (!ft_isdigit(*ptr))
				return (arg);
			ptr++;
		}
		tmp_adj_dup = ft_strdup(arg + 1);
		ft_printf(RED"malloc'd tmp_adj_dup: %s	at ad: %p\n"RESET, tmp_adj_dup, tmp_adj_dup);
		return (tmp_adj_dup);
	}
	tmp_adj_dup = ft_strdup(arg);
	ft_printf(RED"malloc'd tmp_adj_dup: %s	at ad: %p\n"RESET, tmp_adj_dup, tmp_adj_dup);
	return (tmp_adj_dup);
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
	ft_printf(RED"malloc'd literal_part: %s	at ad: %p\n"RESET, literal_part, literal_part);
	tmp = ft_strjoin(processed_arg, literal_part);
	ft_printf(RED"malloc'd tmp_append_literal (new processed_arg): %s	at ad: %p\n"RESET, tmp, tmp);
	ft_printf(GRN"freed literal_part: %s	at ad: %p\n"RESET, literal_part, literal_part);
	free(literal_part);
	ft_printf(GRN"freed processed_arg: %s	at ad: %p\n"RESET, processed_arg, processed_arg);
	free(processed_arg);

	return (tmp);
}

