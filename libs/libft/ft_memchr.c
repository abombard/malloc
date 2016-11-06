/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abombard <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2014/11/04 19:51:31 by abombard          #+#    #+#             */
/*   Updated: 2015/03/25 14:04:42 by abombard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memchr(const void *s, int c, size_t n)
{
	char		*p;
	size_t	i;

	if (s == NULL)
		return (NULL);
	p = (char *)s;
	i = 0;
	while (i < n)
	{
		if (p[i] == (char)c)
			return (p + i);
		i++;
	}
	return (NULL);
}
