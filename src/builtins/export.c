/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmdemirk <dmdemirk@student.42london.c      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/17 11:10:52 by dmdemirk          #+#    #+#             */
/*   Updated: 2024/06/17 15:34:56 by dmdemirk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "env.h"
#include "libft.h"
#include <stdlib.h>
#include "shell.h"
#include <stdio.h>

/*
Functionalities:
- If no arguments are passed or "-p", print all environment variables
- If arguments are passed, set the environment variables
 */

int			builtin_export(t_ms_data *data);
static void	add_env(t_ms_data *data);

int	builtin_export(t_ms_data *data)
{
	t_env	*curr_node;
	char	*key;

	key = NULL;
	if ((data->args[1] == NULL) \
			|| ft_strncmp(data->args[1], "-p", 1) == 0)
	{
		curr_node = data->envp;
		while (curr_node)
		{
			printf("declare -x %s=%s\n", curr_node->key, curr_node->value);
			curr_node = curr_node->next;
		}
	}
	else
		add_env(data);
	return (0);
}

static void	add_env(t_ms_data *data)
{
	int		i;
	char*	key;

	i = 0;
	key = NULL;
	while (data->args[++i])
	{
		if (ft_strchr(data->args[i], '='))
		{
			printf("we are here boom\n");
			char* res;
			res = data->args[i];
			printf("curr arg -> %s\n", ft_strchr(res, '$') + 1);
			set_env(&data->envp, data->args[i], \
					ft_strchr(data->args[i], '=') + 1);
		}
		else
		{
			key = ft_strcdup(data->args[i], '=');
			if (!key)
				continue ;
			if (get_env(data->envp, key))
				set_env(&data->envp, key, "");
			else
				set_env(&data->envp, key, NULL);
			free(key);
		}
	}
}
