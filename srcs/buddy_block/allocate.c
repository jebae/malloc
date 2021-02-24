#include "malloc.h"

static t_uint16	get_avail_block_by_1byte(
	t_uint8 *stats, t_uint16 count, t_uint16 offset)
{
	t_uint32	i;
	t_uint32	j;

	count = (count % 8 > 0) ? count / 8 + 1 : count / 8;
	i = 0;
	while (i < count)
	{
		if (stats[i] != 0)
		{
			j = 0;
			while (j < 8)
			{
				if ((stats[i] & (1 << j)) != 0)
					return (offset + 8 * i + j);
				j++;
			}
		}
		i++;
	}
	return (NO_AVAIL_BLOCK);
}

/*
** use int pointer to check 32bit every loop
*/

t_uint16		get_avail_block(t_uint8 *stats, t_uint16 count)
{
	t_uint32	*ptr;
	t_uint32	i;
	t_uint32	j;
	t_uint32	count_by_32;

	ptr = (t_uint32 *)stats;
	count_by_32 = count / 32;
	i = 0;
	while (i < count_by_32)
	{
		if (ptr[i] != 0)
		{
			j = 0;
			while (j < 32)
			{
				if ((ptr[i] & (1 << j)) != 0)
					return (32 * i + j);
				j++;
			}
		}
		i++;
	}
	return (get_avail_block_by_1byte(stats + 4 * i, count % 32, 32 * i));
}

t_uint16		split_block(
	t_uint8 from, t_uint8 to, t_uint16 idx, t_uint8 **stats)
{
	t_uint8		i;

	i = from;
	while (i > to)
	{
		set_block_stat(idx, 0, stats[i]);
		set_block_stat(idx * 2, 1, stats[i - 1]);
		set_block_stat(idx * 2 + 1, 1, stats[i - 1]);
		idx *= 2;
		i--;
	}
	return (idx);
}

void			*alloc_block(
	t_uint64 size, t_uint32 smallest_block_size,
	t_uint16 smallest_block_count, t_mem_pool *pool)
{
	t_uint8		level;
	t_uint16	idx;
	t_uint8		avail_level;
	t_uint64	relative_addr;

	if (size == 0)
		return (0x0);
	level = get_block_level(size, smallest_block_size);
	idx = NO_AVAIL_BLOCK;
	avail_level = level;
	while (avail_level < 4)
	{
		if ((idx = get_avail_block(pool->stats[avail_level],
			smallest_block_count >> avail_level)) != NO_AVAIL_BLOCK)
			break ;
		avail_level++;
	}
	if (idx == NO_AVAIL_BLOCK)
		return (0x0);
	if (avail_level != level)
		idx = split_block(avail_level, level, idx, pool->stats);
	set_block_stat(idx, 0, pool->stats[level]);
	relative_addr = (smallest_block_size << level) * idx;
	pool->sizes[relative_addr / smallest_block_size] = size;
	return (pool->data + relative_addr);
}
