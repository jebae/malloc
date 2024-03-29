#include "malloc.h"

static void	init_tiny_zone(t_uint32 page_size, void *mem, t_mem_zone *zone)
{
	t_mem_pool	*pool;
	t_uint32	biggest_block_size;

	biggest_block_size = page_size / 16;
	zone->mem_size = page_size * 8;
	zone->smallest_block_size = page_size / 128;
	zone->smallest_block_count = get_smallest_block_count(
		biggest_block_size, 8 * page_size);
	pool = init_mem_pool(mem, zone->smallest_block_count);
	zone->head = pool;
	zone->tail = pool;
	zone->pool_size = zone->smallest_block_count * zone->smallest_block_size;
}

static void	init_small_zone(t_uint32 page_size, void *mem, t_mem_zone *zone)
{
	t_mem_pool	*pool;

	zone->mem_size = page_size * 128;
	zone->smallest_block_size = page_size / 8;
	zone->smallest_block_count = get_smallest_block_count(
		page_size, 128 * page_size);
	pool = init_mem_pool(mem, zone->smallest_block_count);
	zone->head = pool;
	zone->tail = pool;
	zone->pool_size = zone->smallest_block_count * zone->smallest_block_size;
}

int			init_dynamic_memory(t_dynamic_mem *dym)
{
	void		*mem;
	t_uint32	page_size;

	page_size = (t_uint32)getpagesize();
	if (page_size < 2048)
		return (-1);
	mem = mmap(0, (8 + 128) * page_size,
		PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANON, -1, 0);
	if (mem == MAP_FAILED)
		return (-1);
	init_tiny_zone(page_size, mem, &dym->tiny_zone);
	init_small_zone(page_size,
		(t_uint8 *)mem + 8 * page_size, &dym->small_zone);
	dym->page = 0x0;
	dym->is_initialized = 1;
	return (0);
}
