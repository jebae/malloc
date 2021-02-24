#include "malloc.h"

t_dynamic_mem	g_dym;

t_zone_type	get_zone_type(t_uint64 size)
{
	t_uint64	page_size;

	page_size = (t_uint32)getpagesize();
	if (size <= (page_size >> 4))
		return (TINY);
	if (size <= page_size)
		return (SMALL);
	return (LARGE);
}

// static void *alloc_paged_memory -> large

/*
** try allocation zone->tail first which is recent allocated
** if not available search from head to tail
** if not available allocate new memory pool
*/

static void	*alloc_blocked_memory(t_uint32 size, t_mem_zone *zone)
{
	t_mem_pool	*pool;
	void		*mem;

	mem = alloc_block(size, zone->smallest_block_size,
		zone->smallest_block_count, zone->tail);
	if (mem != 0x0)
		return (mem);
	pool = zone->head;
	while (pool != zone->tail)
	{
		mem = alloc_block(size, zone->smallest_block_size,
			zone->smallest_block_count, pool);
		if (mem != 0x0)
			return (mem);
		pool = pool->next;
	}
	pool = alloc_mem_pool(zone->mem_size, zone->smallest_block_size << 3);
	if (pool == 0x0)
		return (0x0);
	zone->tail->next = pool;
	zone->tail = pool;
	return (alloc_block(size, zone->smallest_block_size,
		zone->smallest_block_count, zone->tail));
}

void		*malloc(t_uint64 size)
{
	static int	is_initialized = 0;
	t_zone_type	type;
	t_mem_zone	*zone;

	if (!is_initialized)
	{
		init_dynamic_memory(&g_dym);
		is_initialized = 1;
	}
	type = get_zone_type(size);
	if (type == TINY || type == SMALL)
	{
		zone = (type == TINY) ? &g_dym.tiny_zone : &g_dym.small_zone;
		return (alloc_blocked_memory(size, zone));
	}
	return (0x0);
}
