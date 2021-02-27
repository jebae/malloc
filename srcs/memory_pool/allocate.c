#include "malloc.h"

t_uint16		get_smallest_block_count(
	t_uint32 biggest_block_size, t_uint32 mem_size)
{
	t_uint32	stats_size;
	int			i;

	stats_size = 0;
	i = 0;
	while (i < 4)
	{
		stats_size += (128 / 8) << i;
		i++;
	}
	mem_size -= sizeof(t_mem_pool);
	mem_size -= stats_size;
	mem_size -= 1024 * sizeof(t_uint16);
	return (mem_size / (biggest_block_size >> 3));
}

static void		init_level3_stat(t_uint16 count, t_uint8 *stats)
{
	t_uint16	count_by_8;
	t_uint16	i;
	t_uint16	j;

	count_by_8 = count / 8;
	i = 0;
	while (i < count_by_8)
	{
		stats[i] = 0xFF;
		i++;
	}
	count %= 8;
	j = 0;
	while (j < count)
	{
		stats[i] |= (1 << j);
		j++;
	}
}

static t_uint8	*init_stats(
	t_uint8 *ptr, t_uint16 smallest_block_count, t_uint8 **stats)
{
	int			i;
	t_uint32	size;
	t_uint16	count;

	i = 0;
	while (i < 4)
	{
		stats[i] = ptr;
		size = (1024 / 8) >> i;
		count = smallest_block_count >> i;
		ft_memset(stats[i], 0, size);
		if (i == 3)
			init_level3_stat(count, stats[3]);
		else if (count % 2 == 1)
			set_block_stat(count - 1, 1, stats[i]);
		ptr += size;
		i++;
	}
	return (ptr);
}

t_mem_pool		*init_mem_pool(void *mem, t_uint16 smallest_block_count)
{
	t_uint8		*ptr;
	t_mem_pool	*pool;

	pool = (t_mem_pool *)mem;
	pool->allocated = 0;
	ptr = (t_uint8 *)mem + sizeof(t_mem_pool);
	ptr = init_stats(ptr, smallest_block_count, pool->stats);
	pool->sizes = (t_uint16 *)ptr;
	ft_memset(pool->sizes, 0, sizeof(t_uint16) * 1024);
	pool->data = ptr + sizeof(t_uint16) * 1024;
	pool->next = 0x0;
	return (pool);
}

t_mem_pool		*alloc_mem_pool(t_uint32 mem_size, t_uint32 biggest_block_size)
{
	void		*mem;
	t_uint16	smallest_block_count;

	mem = mmap(0, mem_size,
		PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANON, -1, 0);
	if (mem == MAP_FAILED)
		return (0x0);
	smallest_block_count =
		get_smallest_block_count(biggest_block_size, mem_size);
	return (init_mem_pool(mem, smallest_block_count));
}
