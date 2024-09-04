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

char	*expand_env_and_loc_var(char *arg, t_ms_data *data)
{
	char	*env_value;

	if (ft_strcmp(arg, "$?") == 0)
		return (ft_itoa(data->exit_status));
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

void	post_process_command_args(t_ast *command_node, int arg_count, t_ms_data *data)
{
	int		i;
	char	*arg;
	char	*processed_arg;
	char	*start;
	char	*var_name;
	char	*expanded_var;
	char	*tmp;

	i = 0;
	while (i < arg_count)
	{
		arg = command_node->args[i];
		handle_local_vars(data, arg);  // This keeps working as expected

		processed_arg = ft_strdup("");  // Start with an empty string
		start = arg;  // Pointer to traverse through the argument string

		while (*start != '\0')  // Traverse until end of string
		{
			if (*start == '$')  // Found a variable
			{
				// We want to keep the '$' as part of the variable name
				char *var_start = start;  // Start of variable (including '$')

				// Move past the '$' and collect the alphanumeric characters or underscores after it
				start++;  // Move past the '$'
				while (*start != '\0' && (ft_isalnum(*start) || *start == '_'))
					start++;  // Move past the variable name

				// Get the variable name including the '$'
				var_name = ft_substr(var_start, 0, start - var_start);

				// Expand the variable using the provided function (with '$' included)
				expanded_var = expand_env_and_loc_var(var_name, data);
				free(var_name);  // Free the variable name string

				// Append the expanded variable value to processed_arg
				if (expanded_var != NULL)
				{
					tmp = ft_strjoin(processed_arg, expanded_var);
					free(processed_arg);
					processed_arg = tmp;
					free(expanded_var);
				}
			}
			else
			{
				// Handle literal characters (until the next '$')
				char *literal_start = start;
				while (*start != '\0' && *start != '$')
					start++;  // Move until we find a '$' or the end of string

				// Extract the literal part and append it to processed_arg
				char *literal_part = ft_substr(literal_start, 0, start - literal_start);
				tmp = ft_strjoin(processed_arg, literal_part);
				free(processed_arg);
				processed_arg = tmp;
				free(literal_part);
			}
		}

		// Replace the old argument with the newly processed one
		free(command_node->args[i]);
		command_node->args[i] = processed_arg;

		i++;
	}

	// This should remain unchanged as it handles the final cleanup
	final_quote_removal(arg_count, command_node);
}
