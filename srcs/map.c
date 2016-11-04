#include "context.h"
#include "intern_malloc.h"

#include <sys/mman.h>

extern void	*call_mmap(size_t size)
{
  void  *addr;

	addr = mmap(0, size, PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);
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
  LOG_DEBUG("addr %p size %zu", addr, size);
	if (munmap(addr, size) != 0)
	{
		LOG_ERROR("munmap() failed addr %p size %zu", (void *)addr, size);
		return (false);
	}
  return (true);
}

