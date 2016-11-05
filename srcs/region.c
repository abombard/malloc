#include "context.h"
#include "intern_malloc.h"

extern t_region *new_region(t_handle *handle)
{
	t_region		*region;
	t_quantum		*quantum;

	region = call_mmap(handle->region_size);
	if (region == NULL)
		return (NULL);
	region->size = handle->region_size - sizeof(t_region);
	region->size_free = region->size;
	region->head.magic_number = MAGIC_NUMBER_HEAD;
	INIT_LIST_HEAD(&region->head.list);
	region->quantum = &region->head.list;
	list_add_tail(&region->list, &handle->region);
	quantum = (void *)region->head.chunk;
	quantum_setup(quantum, handle->type, (size_t)sizeof(t_region), region->size);
	list_add_tail(&quantum->list, region->quantum);
	free_list_add(quantum, &handle->quantum);
	return (region);
}

extern bool del_region(t_region *region)
{
	t_quantum *quantum;
	t_list		*pos;

	LOG_DEBUG("region %p size %zu", region, region->size);
	list_del(&region->list);
	pos = region->quantum;
	while ((pos = pos->next) && pos != region->quantum)
	{
		quantum = CONTAINER_OF(pos, t_quantum, list);
		free_list_del(quantum);
	}
	CHECK(call_munmap(region, sizeof(t_region) + region->size));
	return (true);
}
