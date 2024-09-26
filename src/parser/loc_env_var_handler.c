/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   loc_env_var_handler.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmikhayl <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/04 16:46:53 by rmikhayl          #+#    #+#             */
/*   Updated: 2024/09/09 16:19:56 by dmdemirk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <tokens.h>

void	handle_local_vars(t_ms_data *data, char *arg);
char	*process_argument(char *arg, t_ms_data *data);

int count_elements(char **array) {
    int count = 0;
    while (array && array[count]) {
        count++;
    }
    return count;
}

void	post_process_command_args(t_ast *command_node, int arg_count, \
			t_ms_data *data)
{
	int		i;
	char	*processed_arg;
	char	**split_arg = NULL;

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
			processed_arg = ft_substr(command_node->args[i], 1,
					ft_strlen(command_node->args[i]) - 2);
		}
		free(command_node->args[i]);
		// here we split it by ' ' and add 2 args
		split_arg = ft_split(processed_arg, ' ');
		ft_print_2d_arr(split_arg, "split_arg");
		if (count_elements(split_arg) > 1)
		{
			int j = 0;
			while (j < count_elements(split_arg))
			{
				command_node->args[i+j] = ft_strdup(split_arg[j]);
				j++;
			}
			i += j;
			command_node->args[i] = NULL;
		}
		else
		{
			command_node->args[i] = ft_strdup(processed_arg);
			i++;
		}
		ft_print_2d_arr(command_node->args, "cmd->args");

		free(processed_arg);
		
	}
	
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

char	*append_expanded_var(char *processed_arg, char *tmp_ad, \
			char *expanded_var)
{
	char	*tmp;
	char	*tmp_2;

	tmp = ft_strjoin_free(processed_arg, expanded_var);
	tmp_ad = tmp_adj(tmp);
	free(expanded_var);
	tmp_2 = ft_strdup(tmp_ad);
	free(tmp_ad);
	return (tmp_2);
}

char	*process_argument(char *arg, t_ms_data *data)
{
	char	*start;
	char	*processed_arg;
	char	*tmp_ad;
	char	*expanded_var;

	tmp_ad = NULL;
	processed_arg = ft_strdup("");
	start = arg;
	while (*start != '\0')
	{
		if (*start == '$')
		{
			expanded_var = expand_variable(&start, data);
			if (expanded_var != NULL)
			{
				processed_arg = append_expanded_var(processed_arg, \
									tmp_ad, expanded_var);
				free(tmp_ad);
			}
		}
		else
			processed_arg = append_literal(&start, processed_arg);
	}
	ft_printf(YEL"processed_arg: '%s'\n"RESET, processed_arg);
	fflush(stdout);
	return (processed_arg);
}
