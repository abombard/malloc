/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abombard <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2014/11/06 22:04:28 by abombard          #+#    #+#             */
/*   Updated: 2015/03/25 14:05:27 by abombard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char		*ft_strjoin(char const *s1, char const *s2)
{
	char		*s;
	size_t	ss1;
	size_t	ss2;

	if (s1 == NULL || s2 == NULL)
		return (NULL);
	ss1 = ft_strlen(s1);
	ss2 = ft_strlen(s2);
	s = malloc(ss1 + ss2 + 1);
	if (s == NULL)
		return (NULL);
	ft_memcpy(s, s1, ss1);
	ft_memcpy(s + ss1, s2, ss2);
	s[ss1 + ss2] = '\0';
	return (s);
}
