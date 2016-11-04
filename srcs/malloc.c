#include "context.h"
#include "intern_malloc.h"

static void     *addr_retrieve(
    t_quantum_type type,
    t_list *region_head,
    t_list *quantum_free_head,
    size_t region_size,
    size_t quantum_size)
{
  t_region  *region;
  t_quantum *quantum;
  void      *addr;

  quantum = quantum_find(quantum_free_head, quantum_size);
  if (quantum == NULL)
  {
    region = region_add(region_head, type, region_size);
    CHECK(region != NULL);
    quantum = CONTAINER_OF(region->quantum->next, t_quantum, list);
    free_list_add(quantum, quantum_free_head);
  }
  else
    LOG_DEBUG("Found a free block of size %zu", quantum_size);
  quantum_retrieve(quantum, quantum_size, quantum_free_head);
  addr = quantum->chunk;
  LOG_DEBUG("new quantum allocated stack %s size %zu", quantum->info.stack ? "USED":"FREE", quantum->info.size);
  return (addr);
}

extern size_t get_multiple(size_t size, size_t mult)
{
  size_t    mod;

  mod = size % mult;
  if (mod != 0)
    size += mult - mod;
  return (size);
}

extern void *malloc(size_t size)
{
  t_context *context;
  void      *addr;

  get_context(&context);

  if (size < 16)
    size = 16;
  else
    size = get_multiple(size, 8);

  if (size < TINY_QUANTUM_SIZE)
  {
    LOG_DEBUG("size %zu TINY", size);
    addr = addr_retrieve(
        QUANTUM_TYPE_TINY,
        &context->tiny,
        &context->tiny_free,
        context->tiny_region_size,
        size);
  }
  else if (size < SMALL_QUANTUM_SIZE)
  {
    LOG_DEBUG("size %zu SMALL", size);
    addr = addr_retrieve(
        QUANTUM_TYPE_SMALL,
        &context->small,
        &context->small_free,
        context->small_region_size,
        size);
  }
  else
  {
    LOG_DEBUG("size %zu LARGE", size);
    addr = addr_retrieve(
        QUANTUM_TYPE_LARGE,
        &context->large,
        &context->large_free,
        size,
        size);
  }

  show_mem();

  return (addr);
}
