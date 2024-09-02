#include "shell.h"
#include "env.h"
#include "libft.h"
#include <stddef.h>
#include <stdlib.h>

#include <stdio.h>

void	set_shell_var(t_env **shell_var, const char *key, const char *value);
void    add_shell_var_node(t_env **shell_var, const char *line);
char	*get_shell_variable(t_env *shell_var, const char *key);

void add_shell_var_node(t_env **shell_var, const char *line)
{
    t_env	*new_node;
	t_env	*curr_node;
	char	*key;
	char	*value;

	new_node = (t_env *)malloc(sizeof(t_env) + 1);
	if (!new_node)
		return ;
	key = ft_strcdup(line, '=');
	value = ft_strchr(line, '=') + 1;
	new_node->key = ft_strdup(key);
	new_node->value = ft_strdup(value);
	free(key);
	new_node->next = NULL;
	if (*shell_var == NULL)
	{
		*shell_var = new_node;
		return ;
	}
	curr_node = *shell_var;
	while (curr_node->next != NULL)
		curr_node = curr_node->next;
	curr_node->next = new_node;
}


void	set_shell_var(t_env **shell_var, const char *key, const char *value)
{
	t_env	*current;
	t_env	*new_env;

	printf("KEY -> %s\nVALUE -> %s\n", key, value);
    current = *shell_var;
	while (current)
	{
		if (ft_strcmp(current->key, key) == 0)
		{
			free(current->value);
			current->value = ft_strdup(value);
			return ;
		}
		current = current->next;
	}
	new_env = malloc(sizeof(t_env));
	new_env->key = ft_strdup(key);
	new_env->value = ft_strdup(value);
	new_env->next = *shell_var;
	*shell_var = new_env;
}

char	*get_shell_variable(t_env *shell_var, const char *key)
{
	t_env	*curr_node;

	curr_node = shell_var;
	while (curr_node)
	{
		if (ft_strcmp(curr_node->key, key) == 0)
			return (curr_node->value);
		curr_node = curr_node->next;
	}
	return (NULL);
}