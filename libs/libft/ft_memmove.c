/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abombard <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2014/11/05 17:58:16 by abombard          #+#    #+#             */
/*   Updated: 2014/11/21 09:15:11 by abombard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memmove(void *dst, const void *src, size_t len)
{
	if (!dst || !src || !len)
		return (NULL);
  if (dst < src)
    ft_memcpy(dst, src, len);
  else
  {
    while (1)
    {
      len--;
      ((unsigned char *)dst)[len] = ((unsigned char *)src)[len];
      if (len == 0)
        break ;
    }
  }
	return (dst);
}
