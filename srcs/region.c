#include "context.h"
#include "intern_malloc.h"

#include <sys/mman.h>

static void	*call_mmap(size_t size)
{
  void  *addr;

	addr = mmap(0, size, PROT_READ | PROT_WRITE, MAP_ANON | MAP_PRIVATE, -1, 0);
	if (addr == MAP_FAILED)
  {
		LOG_ERROR("mmap failed size %zu", size);
    return (NULL);
  }
  LOG_DEBUG("addr %p size %zu", addr, size);
	return (addr);
}

extern bool call_munmap(void *addr, size_t size)
{
	if (munmap(addr, size) != 0)
	{
		LOG_ERROR("munmap() failed addr %p size %zu", (void *)addr, size);
		return (false);
	}
  LOG_DEBUG("addr %p size %zu", addr, size);
  return (true);
}

extern t_region *new_region(size_t size)
{
  t_region    *region;
	t_quantum	  *quantum;

  region = call_mmap(size);
  if (region == NULL)
    return (NULL);
  region->size = size - sizeof(t_region);
  region->size_free = region->size;
  LOG_DEBUG("region %p size %zu", region, region->size);
  INIT_LIST_HEAD(&region->list);
  region->head.magic_number = MAGIC_NUMBER_HEAD;
  region->head.info.stack = STACK_USED;
  region->head.info.size = 0;
  region->quantum = &region->head.list;
	INIT_LIST_HEAD(region->quantum);
	quantum = (void *)region->head.chunk;
	quantum_setup(quantum, (size_t)sizeof(t_region), region->size);
	list_add_tail(&quantum->list, region->quantum);
  return (region);
}

extern t_region *region_add(t_list *head, size_t size)
{
  t_context *context;
  t_region  *region;

  get_context(&context);
  LOG_DEBUG("pagesize %zu size %zu", context->pagesize, size);
  if (size % context->pagesize != 0)
  {
    size += sizeof(t_region) + sizeof(t_quantum);
    size = get_multiple(size, context->pagesize);
  }
  region = new_region(size);
  if (region == NULL)
    return (NULL);
  list_add_tail(&region->list, head);
  return (region);
}

extern bool region_del(t_region *region)
{
  list_del(&region->list);
  CHECK(call_munmap(region, sizeof(t_region) + region->size));
  return (true);
}
