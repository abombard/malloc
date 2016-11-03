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

static void	init(t_context *context)
{
  context->pagesize = PAGESIZE;
	INIT_LIST_HEAD(&context->tiny);
	INIT_LIST_HEAD(&context->tiny_free);
  context->tiny_region_size = context->pagesize;
	INIT_LIST_HEAD(&context->small);
	INIT_LIST_HEAD(&context->small_free);
  context->small_region_size = context->tiny_region_size * 4;
	INIT_LIST_HEAD(&context->large);
	INIT_LIST_HEAD(&context->large_free);
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
