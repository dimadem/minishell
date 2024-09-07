/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmikhayl <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/05 13:58:23 by rmikhayl          #+#    #+#             */
/*   Updated: 2024/08/28 20:10:27 by rocky            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tokens.h"
#include "env.h"

char	*generate_prompt(t_ms_data *data);
char	*generate_prompt_string(t_ms_data *data, char *pwd, \
				char *home_dir);

char	*generate_prompt(t_ms_data *data)
{
	char	*prompt;
	size_t	prompt_len;

	prompt_len = ft_strlen("🌴 @maxishell$ ") + \
			ft_strlen(getenv("LOGNAME"));
	prompt = (char *)malloc(prompt_len + 1);
	if (!prompt)
		exit(EXIT_FAILURE);
	ft_strcpy(prompt, "🌴 ");
	ft_strcat(prompt, getenv("LOGNAME"));
	ft_strcat(prompt, "@maxishell$ ");
	(void)data;
	return (prompt);
}

char	*generate_prompt_string(t_ms_data *data, char *pwd, \
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
		"Version 1.0.0\033[0m\n");
	ft_printf("			  \033[1;36m"
		"By Димас и Ромас\033[0m\n");
	ft_printf("\n");
}
