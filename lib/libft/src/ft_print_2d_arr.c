/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_print_2d_arr.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmikhayl <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/09 19:37:13 by rmikhayl          #+#    #+#             */
/*   Updated: 2024/09/09 19:37:16 by rmikhayl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_print_2d_arr(char **arr)
{
	int	i;

	i = -1;
	while (arr[++i] != NULL)
		ft_printf("arr[%d]:	%s		at add:	%p	\n", i, arr[i], arr[i]);
}