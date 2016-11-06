/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abombard <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2014/11/06 22:43:27 by abombard          #+#    #+#             */
/*   Updated: 2014/11/10 17:11:41 by abombard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char		*ft_strtrim(char const *str)
{
	size_t	start;
	size_t	len;

	if (str == NULL)
		return (NULL);
	start = 0;
	while (ft_isspace(str[start]))
		start++;
	len = ft_strlen(str) - 1;
	while (ft_isspace(str[len]))
		len--;
	return (ft_strsub(str, start, len));
}
