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

# define STACK_FREE       0
# define STACK_USED       1

typedef enum	e_quantum_type
{
  QUANTUM_TYPE_TINY = 1,
  QUANTUM_TYPE_SMALL,
  QUANTUM_TYPE_LARGE
}             t_quantum_type;

typedef struct	s_info
{
  uint8_t stack:1;
  uint8_t type:3;
	size_t  size;
  size_t  offset;
}				t_info;

# define MAGIC_NUMBER_HEAD        0x4bed
# define MAGIC_NUMBER_NODE			  0xdeadbeef

# define TINY_QUANTUM_SIZE		256
# define SMALL_QUANTUM_SIZE		1024

typedef struct	s_quantum
{
	uint32_t  magic_number;

	t_list		list;
	t_info		info;
	uint8_t		chunk[];
}				t_quantum;

typedef struct	s_region
{
  size_t    size;
  size_t    size_free;
	t_list		list;
	t_list		*quantum;
  t_quantum head;
}				t_region;

typedef struct	s_context
{
	uint8_t		is_initialized;

  size_t    pagesize;

  t_list    tiny;
  t_list    tiny_free;
  size_t    tiny_region_size;

  t_list    small;
  t_list    small_free;
  size_t    small_region_size;

  t_list    large;
  t_list    large_free;

}				t_context;

void  get_context(t_context **context);

#endif
