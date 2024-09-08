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

char	*handle_special_chars(char *str, t_token **tokens);
void	append_word_if_valid(char *start, char *str, t_token **tokens);
char	*handle_phrase(char *str, t_token **tokens);

t_token	*tokenise(char *str)
{
	t_token	*tokens;

	tokens = NULL;
	while (*str)
	{
		while (*str && ft_strchr(" \t\n\r\v\f", *str) != NULL)
			str++;
		if (ft_strchr("<|>", *str) != NULL)
			str = handle_special_chars(str, &tokens);
		else
			str = handle_phrase(str, &tokens);
	}
	return (tokens);
}

char	*handle_special_chars(char *str, t_token **tokens)
{
	if (*str == '<')
	{
		if (*(str + 1) == '<')
		{
			append_token(tokens, new_token("<<", REDIR_HEREDOC));
			(str)++;
		}
		else
			append_token(tokens, new_token("<", REDIR_IN));
	}
	else if (*str == '>')
	{
		if (*(str + 1) == '>')
		{
			append_token(tokens, new_token(">>", REDIR_APPEND));
			(str)++;
		}
		else
			append_token(tokens, new_token(">", REDIR_OUT));
	}
	else if (*str == '|')
		append_token(tokens, new_token("|", PIPE));
	(str)++;
	return (str);
}

t_token	*new_token(char *value, t_token_type type)
{
	t_token	*token;

	token = malloc(sizeof(t_token));
	if (!token)
		return (NULL);
	token->data = ft_strdup(value);
	ft_printf(RED"token malloc'd:	%s		at add:		%p\n"RESET, token->data, token->data);
	if (!token->data)
	{
		free(token);
		return (NULL);
	}
	token->type = type;
	token->next = NULL;
	return (token);
}

void	append_token(t_token **tokens, t_token *new_token)
{
	t_token	*curr;

	if (!*tokens)
		*tokens = new_token;
	else
	{
		curr = *tokens;
		while (curr->next)
			curr = curr->next;
		curr->next = new_token;
	}
}

char	*handle_phrase(char *str, t_token **tokens)
{
	char	*start;
	char	quote_flag;
	char	quote_type;

	start = str;
	quote_flag = 0;
	quote_type = 0;
	while (*str)
	{
		if (!quote_flag && (*str == '\'' || *str == '\"'))
		{
			quote_flag = 1;
			quote_type = *str;
		}
		else if (quote_flag && *str == quote_type)
			quote_flag = 0;
		if (!quote_flag && ft_strchr(" \t\n\r\v\f<|>", *str) != NULL)
			break ;
		str++;
	}
	append_word_if_valid(start, str, tokens);
	return (str);
}

void	append_word_if_valid(char *start, char *str, t_token **tokens)
{
	char	*word;

	if (str > start)
	{
		word = ft_strndup(start, str - start);
		if (word)
		{
			append_token(tokens, new_token(word, PHRASE));
			free(word);
		}
		else
			ft_printf("Error: unable to allocate memory for token\n");
	}
}
