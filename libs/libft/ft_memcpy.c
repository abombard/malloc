/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memccpy.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abombard <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2014/11/04 21:02:13 by abombard          #+#    #+#             */
/*   Updated: 2014/11/19 14:16:43 by abombard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void			*ft_memcpy(void *dst, const void *src, size_t n)
{
	char		*p;
	size_t	i;

	p = (char *)dst;
	i = 0;
	while (i < n)
	{
		p[i] = ((char *)src)[i];
		i++;
	}
	return (dst);
}
