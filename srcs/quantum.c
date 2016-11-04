#include "context.h"
#include "intern_malloc.h"

extern t_quantum  *quantum_setup(
    t_quantum *quantum,
    t_memory_type type,
    size_t offset,
    size_t size)
{
  t_context *context;

  LOG_DEBUG("quantum %p offset %zu size %zu", quantum, offset, size);
  get_context(&context);
  if (size < sizeof(t_quantum) + sizeof(t_list))
    return (NULL);
  size -= sizeof(t_quantum);
	quantum->magic_number = MAGIC_NUMBER_NODE;
	INIT_LIST_HEAD(&quantum->list);
  quantum->info.type = type;
	quantum->info.size = size;
  quantum->info.offset = offset;
  return (quantum);
}

extern void     free_list_del(t_quantum *quantum)
{
  t_list    *free;

  LOG_DEBUG("del quantum from free list");
  quantum->info.stack = STACK_USED;
  free = (t_list *)quantum->chunk;
  list_del(free);
}

extern void     free_list_add(t_quantum *quantum, t_list *quantum_free_head)
{
  t_list    *free;
  t_quantum *quantum_cur;
  t_list    *pos;

  LOG_DEBUG("add quantum to free list");
  quantum->info.stack = STACK_FREE;
  free = (t_list *)quantum->chunk;
  pos = quantum_free_head;
  while ((pos = pos->next) && pos != quantum_free_head)
  {
    quantum_cur = CONTAINER_OF((void *)pos, t_quantum, chunk);
    if (quantum_cur->info.size > quantum->info.size)
      break ;
  }
  list_add_tail(free, pos);
}

static t_quantum  *quantum_find(t_list *head, size_t size)
{
  t_quantum *quantum;
  t_list    *pos;

  LOG_DEBUG("size %zu", size);
  pos = head;
  while ((pos = pos->next) && pos != head)
  {
    quantum = CONTAINER_OF((void *)pos, t_quantum, chunk);
    if (quantum->info.size >= size)
      return (quantum);
  }
  return (NULL);
}

static t_quantum  *quantum_split(t_quantum *quantum, size_t quantum_new_size)
{
  size_t    quantum_total_size;
  t_quantum *new_quantum;
  size_t    new_quantum_offset;
  size_t    new_quantum_size;

  LOG_DEBUG("quantum %p offset %zu size %zu new_size %zu", quantum, quantum->info.offset, quantum->info.size, quantum_new_size);
  quantum_total_size = sizeof(t_quantum) + quantum_new_size;
  new_quantum = (void *)quantum + quantum_total_size;
  new_quantum_offset = quantum->info.offset + quantum_total_size;
  new_quantum_size = quantum->info.size - quantum_new_size;
  new_quantum = quantum_setup(new_quantum, quantum->info.type, new_quantum_offset, new_quantum_size);
  if (new_quantum == NULL)
    return (NULL);
  quantum->info.size = quantum_new_size;
  list_add(&new_quantum->list, &quantum->list);
  return (new_quantum);
}

static t_quantum  *quantum_retrieve_large(size_t size)
{
  t_context *context;
  t_quantum *quantum;
  size_t    quantum_size;

  get_context(&context);
  quantum_size = sizeof(t_quantum) + size;
  quantum_size = get_multiple(quantum_size, context->pagesize);
  quantum = call_mmap(quantum_size);
  if (quantum == NULL)
    return (NULL);
  quantum_setup(quantum, MEMORY_TYPE_LARGE, sizeof(t_quantum), quantum_size);
  quantum->info.stack = STACK_USED;
  list_add_tail(&quantum->list, &context->large);
  return (quantum);
}

extern t_quantum  *quantum_retrieve(t_memory_type type, size_t size)
{
  t_handle  *handle;
  t_region  *region;
  t_quantum *quantum;
  t_quantum *new_quantum;

  if (type == MEMORY_TYPE_LARGE)
    return (quantum_retrieve_large(size));
  CHECK(get_handle(type, &handle));
  quantum = quantum_find(&handle->quantum, size);
  if (quantum == NULL)
  {
    region = new_region(handle);
    CHECK(region != NULL);
    quantum = CONTAINER_OF(region->quantum->next, t_quantum, list);
  }
  LOG_DEBUG("quantum %p size %zu size needed %zu", quantum, quantum->info.size, size);
  new_quantum = quantum_split(quantum, size);
  LOG_DEBUG("%s to split quantum", new_quantum ? "Succeed":"Failed");
  if (new_quantum)
  {
    free_list_add(new_quantum, &handle->quantum);
  }
  free_list_del(quantum);
  region = (void *)quantum - quantum->info.offset;
  region->size_free -= sizeof(t_quantum) + quantum->info.size;
  return (quantum);
}
