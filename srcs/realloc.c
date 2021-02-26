#include "malloc.h"

extern t_dynamic_mem	g_dym;

static t_uint64		get_origin_size(void *ptr)
{
	t_mem_pool	*pool;

	if ((pool = find_mem_pool(ptr, &g_dym.tiny_zone)) != 0x0)
		return (get_block_size(
			ptr, g_dym.tiny_zone.smallest_block_size, pool));
	else if ((pool = find_mem_pool(ptr, &g_dym.small_zone)) != 0x0)
		return (get_block_size(
			ptr, g_dym.small_zone.smallest_block_size, pool));
	// large
	return (0);
}

static void			*realloc_on_place(
	void *ptr, t_uint16 size, t_mem_zone *zone)
{
	t_mem_pool	*pool;

	pool = find_mem_pool(ptr, zone);
	set_block_size(ptr, size, zone->smallest_block_size, pool);
	return (ptr);
}

static void			*realloc_new(
	void *ptr, t_uint64 size, t_uint32 origin_size)
{
	void	*mem;

	mem = malloc(size);
	if (mem == 0x0)
		return (0x0);
	ft_memcpy(mem, ptr, (size > origin_size) ? origin_size : size);
	free(ptr);
	return (mem);
}

void				*realloc(void *ptr, t_uint64 size)
{
	t_uint32	origin_size;
	t_zone_type	origin_zone_type;
	t_zone_type	type;
	t_mem_zone	*zone;

	if (size == 0)
	{
		free(ptr);
		return (0x0);
	}
	if (ptr == 0x0)
		return (malloc(size));
	origin_size = get_origin_size(ptr);
	if (origin_size == 0)
		return (0x0);
	origin_zone_type = get_zone_type(origin_size);
	type = get_zone_type(size);
	if (origin_zone_type == type)
	{
		zone = (type == TINY) ? &g_dym.tiny_zone : &g_dym.small_zone;
		if (get_block_level(origin_size, zone->smallest_block_size)
			== get_block_level(size, zone->smallest_block_size))
			return (realloc_on_place(ptr, size, zone));
	}
	return (realloc_new(ptr, size, origin_size));
}
