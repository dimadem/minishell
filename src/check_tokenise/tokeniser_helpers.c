/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokeniser_helpers.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rocky <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/14 10:38:28 by rocky             #+#    #+#             */
/*   Updated: 2024/06/14 10:38:30 by rocky            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tokens.h"

void	print_tokens(t_token *tokens)
{
	t_token	*token;
	int		i;				

	i = 0;
	token = tokens;
	while (token != NULL)
	{
		ft_printf("input[%d] ->  %s \n", i, token->data);
		token = token->next;
		i++;
	}
}

void	print_ast_args(t_ast *node)
{
	int		i;				

	i = 0;
	while (node->args[i] != NULL)
	{
		ft_printf("ast arg[%d] ->  %s \n", i, node->args[i]);
		i++;
	}
}