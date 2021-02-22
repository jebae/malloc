#include "malloc.h"

void	merge_block(t_uint16 idx, t_uint8 level, t_uint8 **stats)
{
	t_uint16	buddy_idx;
	t_uint32	buddy_stat;

	while (level < 3 && get_block_stat(idx, stats[level]) == 1)
	{
		buddy_idx = (idx % 2 == 1) ? idx - 1 : idx + 1;
		buddy_stat = get_block_stat(buddy_idx, stats[level]);
		if (buddy_stat == 1)
		{
			set_block_stat(idx, 0, stats[level]);
			set_block_stat(buddy_idx, 0, stats[level]);
			set_block_stat(idx >> 1, 1, stats[level + 1]);
		}
		else
			break ;
		idx >>= 1;
		level++;
	}
}

void	free_block(
	void *ptr, t_uint32 smallest_block_size, t_mem_pool *pool)
{
	t_uint64	relative_addr;
	t_uint16	size;
	t_uint8		level;
	t_uint16	idx;

	relative_addr = (t_uint8 *)ptr - pool->data;
	size = pool->sizes[relative_addr / smallest_block_size];
	if (size == 0)
		return ;
	pool->sizes[relative_addr / smallest_block_size] = 0;
	level = get_block_level(size, smallest_block_size);
	size = smallest_block_size << level;
	idx = relative_addr / size;
	set_block_stat(idx, 1, pool->stats[level]);
	merge_block(idx, level, pool->stats);
}
