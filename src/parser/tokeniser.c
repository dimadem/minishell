/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokeniser.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rocky <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/12 15:00:23 by rocky             #+#    #+#             */
/*   Updated: 2024/06/12 15:00:25 by rocky            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tokens.h"
#include <string.h>

void	update_single_quotes_count(int *quote_count, char c)
{
	if (c == '\'')
		(*quote_count)++;
}

void	handle_env_vars(char **str, t_token **tokens)
{
	char	*env_var;
	size_t	env_var_len;

	env_var = NULL;
	if (**str == '$' && *(*str + 1) == '(')
	{
		(*str) += 2;
		env_var_len = 0;
		while (**str && *(*str + env_var_len) != ')')
			env_var_len++;
		env_var = (char *)malloc(env_var_len + 1);
		if (!env_var)
			return ;
		strncpy(env_var, *str, env_var_len);
		env_var[env_var_len] = '\0';
		append_token(tokens, new_token(env_var, ENV_VAR));
		free(env_var);
		*str += env_var_len;
	}
	(*str)++;
}

void	handle_special_chars(char **str, t_token **tokens, int *quote_count)
{

	update_single_quotes_count(quote_count, **str);
	if (**str == '<')
	{
		if (*(*str + 1) == '<')
		{
			append_token(tokens, new_token("<<", REDIR_HEREDOC));
			(*str)++;
		}
		else
			append_token(tokens, new_token("<", REDIR_IN));
	}
	else if (**str == '>')
	{
		if (*(*str + 1) == '>')
		{
			append_token(tokens, new_token(">>", REDIR_APPEND));
			(*str)++;
		}
		else
			append_token(tokens, new_token(">", REDIR_OUT));
	}
	else if (**str == '|')
		append_token(tokens, new_token("|", PIPE));
	(*str)++;
}

void	append_phrase_if_valid(char **start, char **str, t_token **tokens)
{
	char	*phrase;

	if (*str > *start)
	{
		phrase = ft_strndup(*start, *str - *start);
		if (phrase)
		{
			append_token(tokens, new_token(phrase, PHRASE));
			free(phrase);
		}
		else
			ft_printf("Error: unable to allocate memory for token\n");
	}
}

void	handle_phrase(char **str, t_token **tokens, int *quote_count)
{
	char	*start;
	char	quote_flag;
	char	quote_type;

	start = *str;
	quote_flag = 0;
	quote_type = 0;
	while (**str)
	{
		update_single_quotes_count(quote_count, **str);
		if (!quote_flag && (**str == '\'' || **str == '\"'))
		{
			quote_flag = 1;
			quote_type = **str;
		}
		else if (quote_flag && **str == quote_type)
			quote_flag = 0;
		if (!quote_flag && ft_strchr(" \t\n\r\v\f<|>", **str) != NULL)
			break ;
		(*str)++;
	}
	append_phrase_if_valid(&start, str, tokens);
}
void	print_tokens(t_token *tokens)
{
	t_token	*token;
	int		i;				

	i = 0;
	token = tokens;
	while (token != NULL)
	{
		printf("input[%d] ->  %s: %d \n", i, token->data, token->type);
		token = token->next;
		i++;
	}
}

t_token	*tokenise(char *str)
{
	t_token	*tokens;
	int	quote_count;

	tokens = NULL;
	quote_count = 0;
	while (*str)
	{
		while (*str && ft_strchr(" \t\n\r\v\f", *str) != NULL)
			str++;
		if (ft_strchr("<|>", *str) != NULL)
			handle_special_chars(&str, &tokens, &quote_count);
		else if (ft_strchr("$", *str) != NULL)
			handle_env_vars(&str, &tokens);
		else
			handle_phrase(&str, &tokens, &quote_count);

	}
	print_tokens(tokens);
	printf("quote count: %d\n", quote_count);
	return (tokens);
}
