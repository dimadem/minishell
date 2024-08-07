/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   stack_control_2.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmikhayl <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/05 13:32:45 by rmikhayl          #+#    #+#             */
/*   Updated: 2024/07/11 18:45:52 by dmdemirk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tokens.h"

void	build_linked_list(t_token **tokens, char **argv)
{
	int	i;

	i = 0;
	while (argv[i])
	{
		add_node(tokens, argv[i]);
		i++;
	}
}
