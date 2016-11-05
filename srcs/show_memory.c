#include "context.h"
#include "intern_malloc.h"

#include <stdlib.h>

extern bool quantum_show(t_quantum *quantum)
{
	CHECK(quantum != NULL);
	if (quantum->magic_number != MAGIC_NUMBER_HEAD && quantum->magic_number != MAGIC_NUMBER_NODE)
	{
		LOG_ERROR("Invalid quantum %p magic_number %x stack %s size %zu", quantum, quantum->magic_number, quantum->info.stack == STACK_FREE ? "FREE":"USED", quantum->info.size);
		exit(1);
	}
	log_info("quantum %p magic_number %x stack %s offset %zu size %zu", quantum, quantum->magic_number, quantum->info.stack == STACK_FREE ? "FREE":"USED", quantum->info.offset, quantum->info.size);
	return (true);
}

extern bool quantum_show_list(t_list *head)
{
	t_quantum *quantum;
	t_list		*pos;

	CHECK(head != NULL);
	pos = head;
	while ((pos = pos->next) && pos != head)
	{
		quantum = CONTAINER_OF(pos, t_quantum, list);
		CHECK(quantum_show(quantum));
	}
	return (true);
}

extern bool region_show(t_region *region)
{
	CHECK(region != NULL);
	log_info("region %p", region);
	log_info("size %zu size_free %zu", region->size, region->size_free);
	CHECK(quantum_show_list(region->quantum));
	return (true);
}

extern bool region_show_list(t_list *head)
{
	t_region	*region;
	t_list		*pos;

	CHECK(head != NULL);
	pos = head;
	while ((pos = pos->next) && pos != head)
	{
		region = CONTAINER_OF(pos, t_region, list);
		CHECK(region_show(region));
	}
	return (true);
}

extern void show_memory(void)
{
	t_context *context;

	get_context(&context);
	log_info("Tiny regions");
	region_show_list(&context->tiny.region);
	log_info("Small regions");
	region_show_list(&context->small.region);
	log_info("Large regions");
	quantum_show_list(&context->large);
}
