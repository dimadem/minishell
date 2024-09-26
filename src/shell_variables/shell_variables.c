/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_variables.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmdemirk <dmdemirk@student.42london.c      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/06 11:51:16 by dmdemirk          #+#    #+#             */
/*   Updated: 2024/09/09 12:41:39 by dmdemirk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"
#include "env.h"
#include "libft.h"
#include "tokens.h"
#include <stddef.h>

int		handle_add_set_shell_variable(t_env **shell_var, char *line);
int		handle_get_shell_variable(t_ms_data *data, const char *key);
int		handle_shell_variable(t_ast *node, t_ms_data *data);
void	shell_variable_update(t_ms_data *data, int status);




#include <stdlib.h>
#include <string.h>

char *ft_remove_quotes(char *str) {
    int len = strlen(str);
	char *new_str = NULL;

    // Check if the string has at least 2 characters and starts and ends with the same quote
    if (len > 1 && ((str[0] == '\'' && str[len - 1] == '\'') || (str[0] == '"' && str[len - 1] == '"'))) {
        // Create a new string without the quotes
        new_str = strndup(str + 1, len - 2);
        return new_str;
    }
    // Return the original string if no quotes to remove
    return strdup(str);
}


int	handle_add_set_shell_variable(t_env **shell_var, char *line)
{
	char	*key;
	char	*value;
	char	*nq_value= NULL;

	key = NULL;
	value = NULL;
	if (*shell_var == NULL)
		add_shell_var_node(shell_var, line);
	else
	{
		key = ft_strcdup(line, '=');
		value = ft_strchr(line, '=') + 1;
		nq_value = ft_remove_quotes(value);
		// free(value);
		set_shell_var(shell_var, key, nq_value);
		// free(nq_value);
		free(key);
	}
	return (0);
}

int	handle_get_shell_variable(t_ms_data *data, const char *key)
{
	char	*value;
	t_env	*shell_var;

	shell_var = data->shell_variables;
	value = get_env(shell_var, key);
	if (value)
	{
		ft_putstr_fd(value, data->std_out);
		ft_putstr_fd("\n", data->std_out);
		free(value);
		return (0);
	}
	return (-1);
}

int	handle_shell_variable(t_ast *node, t_ms_data *data)
{
	if (node->left && node->right == NULL)
		return (handle_get_shell_variable(data, node->args[0]));
	return (-1);
}

void	shell_variable_update(t_ms_data *data, int status)
{
	char	*status_str;

	data->exit_status = status;
	status_str = ft_itoa(status);
	set_shell_var(&data->shell_variables, "?", status_str);
	free(status_str);
}
