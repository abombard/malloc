#include "context.h"
#include "intern_malloc.h"
#include <assert.h>

extern void	*realloc(void *addr, size_t size)
{
	t_quantum	*quantum;
	uint8_t		*new;

	if (addr == NULL)
		return (malloc(size));
	quantum = CONTAINER_OF(addr, t_quantum, chunk);
	assert(quantum->magic_number == MAGIC_NUMBER_NODE);
	assert(quantum->info.stack == STACK_USED);
	new = malloc(size);
	if (new == NULL)
		return (NULL);
	ft_memcpy(new, addr, quantum->info.size);
	free(addr);
	return (new);
}
