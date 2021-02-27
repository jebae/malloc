#include "malloc.h"

t_dynamic_mem	g_dym;

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

static void *alloc_paged_memory(t_uint64 size)
{
	t_mem_page	*prev;

	prev = g_dym.page;
	if (prev == 0x0)
	{
		g_dym.page = alloc_mem_page(size);
		return ((g_dym.page) ? (void *)g_dym.page->data : 0x0);
	}
	while (prev->next)
		prev = prev->next;
	prev->next = alloc_mem_page(size);
	return ((prev->next) ? (void *)prev->next->data : 0x0);
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
	if (size == 0)
		return (0x0);
	type = get_zone_type(size);
	if (type == TINY || type == SMALL)
	{
		zone = (type == TINY) ? &g_dym.tiny_zone : &g_dym.small_zone;
		return (alloc_blocked_memory(size, zone));
	}
	return (alloc_paged_memory(size));
}
