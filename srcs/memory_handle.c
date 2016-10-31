#include "context.h"

#include <sys/mman.h>

extern void	*call_mmap(size_t size)
{
	void	*map;

	map = mmap(0, size, PROT_READ | PROT_WRITE, MAP_ANON | MAP_PRIVATE, -1, 0);
	if (map == MAP_FAILED)
		LOG_ERROR("mmap failed size %zu", size);
	return (map);
}

extern void		block_init(t_block *block, size_t size)
{
	block->magic_number = MAGIC_NUMBER;
	INIT_LIST_HEAD(&block->list);
	INIT_LIST_HEAD(&block->stack);
	block->info.used = false;
	block->info.size = size;
}

t_handle		*handle_new(size_t size)
{
	t_handle	*new;
	t_block		*block;

	new = call_mmap(size);
	if (new == NULL)
		FATAL("call_mmap failed size %zu", size);
	INIT_LIST_HEAD(&new->list);
	INIT_LIST_HEAD(&new->block);
	INIT_LIST_HEAD(&new->stack_free);
	INIT_LIST_HEAD(&new->stack_used);
	size -= sizeof(t_handle);
	block = (void *)new + sizeof(t_handle);
	block_init(&block, size - sizeof(t_block));
	list_add_tail(&block->list, &new->block);
	list_add_tail(&block->stack, &new->stack_free);
	return (new);
}
