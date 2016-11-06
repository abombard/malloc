/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abombard <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2014/11/03 15:26:14 by abombard          #+#    #+#             */
/*   Updated: 2016/03/31 13:18:47 by abombard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strdup(const char *src)
{
	char	*s;
	int		size;

	if (src == NULL)
		return (NULL);
	size = ft_strlen(src) + 1;
	s = malloc(size);
	if (s == NULL)
		return (NULL);
	ft_memcpy(s, src, size);
	return (s);
}
