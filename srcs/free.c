#include "context.h"
#include "intern_malloc.h"

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

extern bool   quantum_release(
    t_list *region_head,
    t_list *quantum_free_head,
    t_quantum *quantum)
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
  free_list_add(quantum, quantum_free_head);
  if (region->size_free == region->size)
  {
    if (region_too_many(region_head))
      CHECK(region_del(region));
  }
  return (true);
}

extern void   free(void *addr)
{
  t_context *context;
  t_quantum *quantum;
  t_list    *region_head;
  t_list    *quantum_free_head;

  if (addr == NULL)
    return ;
  quantum = CONTAINER_OF(addr, t_quantum, chunk);
  LOG_DEBUG("free quantum %p", quantum);
  if (quantum->magic_number != MAGIC_NUMBER_NODE)
    LOG_ERROR("Invalid magic_number %x", quantum->magic_number);
  get_context(&context);
  if (quantum->info.size < TINY_QUANTUM_SIZE)
  {
    region_head = &context->tiny;
    quantum_free_head = &context->tiny_free;
  }
  else if (quantum->info.size < SMALL_QUANTUM_SIZE)
  {
    region_head = &context->small;
    quantum_free_head = &context->small_free;
  }
  else
  {
    region_head = &context->large;
    quantum_free_head = &context->large_free;
  }
  if (!quantum_release(region_head, quantum_free_head, quantum))
    LOG_ERROR("quantum_release failed");

  show_mem();
}
