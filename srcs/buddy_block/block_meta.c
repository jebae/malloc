#include "malloc.h"

void		set_block_stat(t_uint16 idx, t_int32 flag, t_uint8 *stats)
{
	stats += idx / 8;

	if (flag == 0)
		*stats &= ~(1 << (idx % 8));
	else
		*stats |= 1 << (idx % 8);
}

t_uint32	get_block_stat(t_uint16 idx, t_uint8 *stats)
{
	stats += idx / 8;
	return ((*stats) & (1 << (idx % 8)) ? 1 : 0);
}

void		set_block_size(
	void *ptr, t_uint16 size, t_uint32 smallest_block_size, t_mem_pool *pool)
{
	t_uint64	relative_addr;

	relative_addr = (t_uint8 *)ptr - pool->data;
	if (relative_addr % smallest_block_size > 0)
		return ;
	pool->sizes[relative_addr / smallest_block_size] = size;
}

t_uint16	get_block_size(
	void *ptr, t_uint32 smallest_block_size, t_mem_pool *pool)
{
	t_uint64	relative_addr;
	t_uint32	size;

	relative_addr = (t_uint8 *)ptr - pool->data;
	if (relative_addr % smallest_block_size > 0)
		return (0);
	size = pool->sizes[relative_addr / smallest_block_size];
	if (size == 0)
		return (0);
	return (size);
}

t_uint8		get_block_level(t_uint64 size, t_uint32 smallest_block_size)
{
	t_uint8		level;
	t_uint64	block_size;

	level = 0;
	block_size = smallest_block_size;
	while (block_size < size)
	{
		block_size <<= 1;
		level++;
	}
	return (level);
}
