#include "context.h"
#include "intern_malloc.h"

#include <assert.h>

static bool   region_too_many(t_list *head)
{
  t_region  *region;
  t_list    *pos;
  int       count;

  count = 0;
  pos = head;
  while ((pos = pos->next) && pos != head)
  {
    region = CONTAINER_OF(pos, t_region, list);
    if (region->size_free == region->size)
      count++;
  }
  return (count > 1 ? true : false);
}

static bool   isfree(t_quantum *quantum)
{
  if (quantum->magic_number != MAGIC_NUMBER_NODE)
    return (false);
  return (quantum->info.stack == STACK_FREE ? true : false);
}

extern bool   quantum_release(t_quantum *quantum, t_handle *handle)
{
  t_region    *region;
  t_quantum   *prev;
  t_quantum   *next;

  region = (void *)quantum - quantum->info.offset;
  region->size_free += sizeof(t_quantum) + quantum->info.size;
  prev = CONTAINER_OF(quantum->list.prev, t_quantum, list);
  next = CONTAINER_OF(quantum->list.next, t_quantum, list);
  if (isfree(next))
  {
    LOG_DEBUG("merging quantum %p offset %zu size %zu with next %p offset %zu size %zu", quantum, quantum->info.offset, quantum->info.size, next, next->info.offset, next->info.size);
    free_list_del(next);
    list_del(&next->list);
    quantum->info.size += sizeof(t_quantum) + next->info.size;
  }
  if (isfree(prev))
  {
    LOG_DEBUG("merging quantum %p offset %zu size %zu with prev %p offset %zu size %zu", quantum, quantum->info.offset, quantum->info.size, prev, prev->info.offset, prev->info.size);
    free_list_del(prev);
    list_del(&quantum->list);
    prev->info.size += sizeof(t_quantum) + quantum->info.size;
    quantum = prev;
  }
  free_list_add(quantum, &handle->quantum);
  if (region->size_free == region->size && region_too_many(&handle->region))
      CHECK(del_region(region));
  return (true);
}

extern void   free(void *addr)
{
  t_handle  *handle;
  t_quantum *quantum;

  if (addr == NULL)
    return ;
  quantum = CONTAINER_OF(addr, t_quantum, chunk);
  assert(quantum->magic_number == MAGIC_NUMBER_NODE);
  assert(quantum->info.stack == STACK_USED);
  if (quantum->info.type == MEMORY_TYPE_LARGE)
  {
    list_del(&quantum->list);
    call_munmap(quantum, sizeof(t_quantum) + quantum->info.size);
  }
  else
  {
    if (!get_handle(quantum->info.type, &handle))
    {
      LOG_ERROR("get_handle failed");
      return ;
    }
    quantum_release(quantum, handle);
  }
}
