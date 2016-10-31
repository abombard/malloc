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

static bool	init(t_context *context)
{
	INIT_LIST_HEAD(&context->tiny);
	INIT_LIST_HEAD(&context->small);
	INIT_LIST_HEAD(&context->large);
	return (true);
}

extern bool	get_context(t_context **out_context)
{
	static t_context	context = { .is_initialized = false };

	*out_context = NULL;
	if (context.is_initialized == false)
	{
		CHECK(init(&context));
		context.is_initialized = true;
	}
	*out_context = &context;
	return (true);
}
