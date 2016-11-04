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

# include <unistd.h>
# define PAGESIZE           (size_t)sysconf(_SC_PAGESIZE)

# define G_MEM_ALIGN			  sizeof(void *)

/*
** info
*/

# define STACK_FREE       0
# define STACK_USED       1

typedef enum	e_memory_type
{
  MEMORY_TYPE_TINY = 0,
  MEMORY_TYPE_SMALL,
  MEMORY_TYPE_LARGE
}             t_memory_type;

typedef struct	s_info
{
  uint8_t stack:1;
  uint8_t type:3;
	size_t  size;
  size_t  offset;
}				t_info;

/*
** quantum
*/

# define MAGIC_NUMBER_HEAD        0x4bed
# define MAGIC_NUMBER_NODE			  0xdeadbeef

typedef struct	s_quantum
{
	uint32_t  magic_number;

	t_list		list;
	t_info		info;
	uint8_t		chunk[];
}				t_quantum;

/*
** region
*/

typedef struct	s_region
{
  size_t    size;
  size_t    size_free;
	t_list		list;
	t_list		*quantum;
  t_quantum head;
}				t_region;

/*
** handle
*/

typedef struct	s_handle
{
  t_memory_type type;
  t_list        region;
  size_t        region_size;
  t_list        quantum;
}               t_handle;

/*
** context
*/

typedef struct	s_context
{
	uint8_t		is_initialized;

  size_t    pagesize;
  size_t    tiny_quantum_size;
  size_t    small_quantum_size;

  t_handle  tiny;
  t_handle  small;
  t_list    large;

}				t_context;

void  get_context(t_context **context);
bool  get_handle(t_memory_type memory_type, t_handle **handle);

#endif
