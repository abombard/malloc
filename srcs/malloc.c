#include "context.h"
#include "intern_malloc.h"

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
  t_quantum *quantum;
  void      *addr;

  get_context(&context);
  size = size < 16 ? 16 : get_multiple(size, G_MEM_ALIGN);
  if (size < context->tiny_quantum_size)
  {
    LOG_DEBUG("size %zu TINY", size);
    quantum = quantum_retrieve(MEMORY_TYPE_TINY, size);
  }
  else if (size < context->small_quantum_size)
  {
    LOG_DEBUG("size %zu SMALL", size);
    quantum = quantum_retrieve(MEMORY_TYPE_SMALL, size);
  }
  else
  {
    LOG_DEBUG("size %zu LARGE", size);
    quantum = quantum_retrieve(MEMORY_TYPE_LARGE, size);
  }
  addr = (void *)quantum->chunk;
  return (addr);
}
