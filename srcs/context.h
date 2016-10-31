/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   context.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abombard <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/10/31 16:47:53 by abombard          #+#    #+#             */
/*   Updated: 2016/10/31 18:20:46 by abombard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONTEXT_H
# define CONTEXT_H

# include "malloc.h"

# include <stddef.h>
# include <stdint.h>

# include "log.h"
# include "list.h"

# define MAGIC_NUMBER			0xdeadbeaf

# define TINY_QUANTUM_SIZE		16
# define SMALL_QUANTUM_SIZE		512

# define G_MEM_ALIGN			(sizeof(void *))

typedef struct	s_info
{
	size_t		used:1;
	size_t		size:sizeof(size_t) - 1;
}				t_info;

typedef struct	s_block
{
	uint32_t	magic_number;

	t_list		list;
	t_list		stack;
	t_info		info;
	uint8_t		user[];
}				t_block;

typedef struct	s_handle
{
	t_list		list;
	t_list		block;
	t_list		stack_free;
	t_list		stack_used;
}				t_handle;

typedef struct	s_context
{
	uint8_t		is_initialized;
	t_list		tiny;
	t_list		small;
	t_list		large;
}				t_context;

#endif
