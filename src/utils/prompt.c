/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmikhayl <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/05 13:58:23 by rmikhayl          #+#    #+#             */
/*   Updated: 2024/06/19 16:19:42 by rocky            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tokens.h"
#include "env.h"

/*
todo: 
extract and print only after LOGNAME

!wrong example:
🌴 dmdemirk@maxishell:~/home/dmdemirk/Documents/minishell>

correct example:
🌴 dmdemirk@maxishell:~/Documents/minishell>
 */

char	*generate_prompt(t_minishell_data *data);
char	*generate_prompt_string(t_minishell_data *data, char *pwd, \
				char *home_dir);

char	*generate_prompt(t_minishell_data *data)
{
	char	*prompt;
	size_t	prompt_len;
	char	*pwd;
	char	*home_dir;

	home_dir = NULL;
	pwd = NULL;
	pwd = generate_prompt_string(data, pwd, home_dir);
	prompt_len = ft_strlen("🌴\e[1m @maxishell:~> \e[m") + \
			ft_strlen(get_env(data->envp, "LOGNAME")) + \
			ft_strlen(pwd) + \
			1;
	prompt = (char *)malloc(prompt_len);
	if (!prompt)
		exit(EXIT_FAILURE);
	ft_strcpy(prompt, "🌴\e[1m ");
	ft_strcat(prompt, get_env(data->envp, "LOGNAME"));
	ft_strcat(prompt, "@maxishell:");
	ft_strcat(prompt, pwd);
	ft_strcat(prompt, "> \e[m");
	return (prompt);
}

char	*generate_prompt_string(t_minishell_data *data, char *pwd, \
				char *home_dir)
{
	char	*temp_pwd;

	pwd = get_env(data->envp, "PWD");
	if (pwd)
	{
		home_dir = get_env(data->envp, "HOME");
		if (home_dir && ft_strncmp(pwd, home_dir, ft_strlen(home_dir)) == 0)
		{
			temp_pwd = ft_strjoin("~", pwd + ft_strlen(home_dir));
			free(pwd);
			pwd = temp_pwd;
		}
	}
	return (pwd);
}

void	print_maxishell(void)
{
	ft_printf("\033[1;33m\n\n\n\n\n");
	ft_printf("███╗░░░███╗░█████╗░██╗░░██╗██╗░██████╗██╗░"
		"░██╗███████╗██╗░░░░░██╗░░░░░\n");
	ft_printf("████╗░████║██╔══██╗╚██╗██╔╝██║██╔════╝██║░"
		"░██║██╔════╝██║░░░░░██║░░░░░\n");
	ft_printf("██╔████╔██║███████║░╚███╔╝░██║╚█████╗░█████"
		"██║█████╗░░██║░░░░░██║░░░░░\n");
	ft_printf("██║╚██╔╝██║██╔══██║░██╔██╗░██║░╚═══██╗██╔══"
		"██║██╔══╝░░██║░░░░░██║░░░░░\n");
	ft_printf("██║░╚═╝░██║██║░░██║██╔╝╚██╗██║██████╔╝██║░░"
		"██║███████╗███████╗███████╗\n");
	ft_printf("╚═╝░░░░░╚═╝╚═╝░░╚═╝╚═╝░░╚═╝╚═╝╚═════╝░╚═╝░░"
		"╚═╝╚══════╝╚══════╝╚══════╝\n");
	ft_printf("\n");
	ft_printf("			   \033[1;34m"
		"Version 0.0.0\033[0m\n");
	ft_printf("			  \033[1;36m"
		"By Димас и Ромас\033[0m\n");
	ft_printf("\n");
}
