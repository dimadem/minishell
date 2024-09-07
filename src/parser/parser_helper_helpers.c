/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_helper_helpers.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rocky <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/28 20:36:33 by rocky             #+#    #+#             */
/*   Updated: 2024/08/28 20:36:39 by rocky            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tokens.h"
#include "execute.h"
#include "exit_status.h"



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
