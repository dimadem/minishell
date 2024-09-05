/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_checker.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rocky <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/11 16:51:08 by rocky             #+#    #+#             */
/*   Updated: 2024/06/11 16:53:40 by rocky            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tokens.h"

const char	*ft_strcat_const(const char *dest, const char *src)
{
	int		i;
	int		j;
	char	*result;

	result = malloc(sizeof(char) * (strlen(dest) + strlen(src) + 1));
	if (!result)
		return (NULL);
	i = 0;
	while (dest[i] != '\0')
	{
		result[i] = dest[i];
		i++;
	}
	j = 0;
	while (src[j] != '\0')
	{
		result[i + j] = src[j];
		j++;
	}
	result[i + j] = '\0';
	return (result);
}


int	check_operators_loop(const char **str, int *single_quotes, \
			int *double_quotes, int *cmd_flag)
{
	if (**str == '\'')
		(*single_quotes)++;
	if (**str == '\"')
		(*double_quotes)++;
	if (**str == '|' && !(*single_quotes % 2) && !(*double_quotes % 2) && 
			*(*str + 1) != '\0')
	{
		if (*cmd_flag)
			return (1);
		*cmd_flag = 1;
	}
	else if (ft_strchr(" \t\n\r\v\f", **str) != NULL)
		*cmd_flag = 0;
	(*str)++;
	return (0);
}

int	check_operators(const char *str)
{
	int	single_quotes;
	int	double_quotes;
	int	cmd_flag;
	// const char *new;
	const char *tail = "yeeeeeeeeeee";
	const char *res;
	

	// new = str;
	single_quotes = 0;
	double_quotes = 0;
	cmd_flag = 0;
	if (*str == '&' || *str == '|')
		return (1);
	// while (*new)
	// {
	// 	printf("new: %s\n", new);
	// 	// if (*new == '|' && (*new + 1) == '\0')
	// 	// {
	// 	// 	printf("YEEEEEEEEEEEEEE");
	// 	// 	fflush(stdout);
	// 	// }
	// 	new++;
	// }
	res = ft_strcat_const(str, tail);
	ft_printf("res: %s\n", res);
	while (*str)
	{
		printf("*str: %s\n", str);
		if (check_operators_loop(&str, &single_quotes, \
			&double_quotes, &cmd_flag) == 1)
			return (1);
	}
	if (cmd_flag)
		return (1);
	return (0);
}

int	check_redirections(const char *str)
{
	int	single_quotes;
	int	double_quotes;

	single_quotes = 0;
	double_quotes = 0;
	while (*str)
	{
		if (*str == '\'')
			single_quotes++;
		if (*str == '\"')
			double_quotes++;
		if ((!(single_quotes % 2) && !(double_quotes % 2))
			&& (*str == '>' || *str == '<'))
		{
			if (!valid_operator(&str))
				return (1);
		}
		else
			str++;
	}
	return (0);
}

int	check_open_quotes(const char *str)
{
	int	single_quote_open;
	int	double_quote_open;

	single_quote_open = 0;
	double_quote_open = 0;
	while (*str)
	{
		if (*str == '\'' && !double_quote_open)
		{
			single_quote_open = !single_quote_open;
		}
		else if (*str == '"' && !single_quote_open)
			double_quote_open = !double_quote_open;
		str++;
	}
	return (single_quote_open || double_quote_open);
}

int	input_error_checks(const char *str)
{
	if (check_redirections(str))
		ft_printf("Input error: invalid redirection.\n");
	else if (check_operators(str))
		ft_printf("Input error: invalid operator.\n");
	else if (check_open_quotes(str))
		ft_printf("Input error: open quote.\n");
	else
		return (0);
	return (1);
}
