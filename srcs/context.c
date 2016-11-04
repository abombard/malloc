/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   context.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abombard <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/10/31 14:18:01 by abombard          #+#    #+#             */
/*   Updated: 2016/10/31 18:20:48 by abombard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "context.h"
#include "intern_malloc.h"

static void init_handle(t_handle *handle, t_memory_type type, size_t region_size)
{
  handle->type = type;
  INIT_LIST_HEAD(&handle->region);
  INIT_LIST_HEAD(&handle->quantum);
  handle->region_size = region_size;
}

static void	init(t_context *context)
{
  context->pagesize = PAGESIZE;
  context->tiny_quantum_size = context->pagesize / 8;
  context->small_quantum_size = context->pagesize;
  init_handle(&context->tiny, MEMORY_TYPE_TINY, context->tiny_quantum_size * 16);
  init_handle(&context->small, MEMORY_TYPE_SMALL, context->small_quantum_size * 16);
	INIT_LIST_HEAD(&context->large);
}

extern bool get_handle(t_memory_type memory_type, t_handle **handle)
{
  t_context *context;

  *handle = NULL;
  get_context(&context);
  if (memory_type == MEMORY_TYPE_TINY)
    *handle = &context->tiny;
  else if (memory_type == MEMORY_TYPE_SMALL)
    *handle = &context->small;
  return (*handle == NULL ? false : true);
}

extern void	get_context(t_context **out_context)
{
	static t_context	context = { .is_initialized = false };

	if (context.is_initialized == false)
	{
		init(&context);
		context.is_initialized = true;
	}
	*out_context = &context;
}
