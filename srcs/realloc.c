#include "malloc.h"

extern t_dynamic_mem	g_dym;

static t_uint64		get_origin_size(void *ptr)
{
	t_mem_pool	*pool;
	t_mem_page	*page;

	if ((pool = find_mem_pool(ptr, &g_dym.tiny_zone)) != 0x0)
		return (get_block_size(
			ptr, g_dym.tiny_zone.smallest_block_size, pool));
	else if ((pool = find_mem_pool(ptr, &g_dym.small_zone)) != 0x0)
		return (get_block_size(
			ptr, g_dym.small_zone.smallest_block_size, pool));
	else if ((page = find_mem_page(ptr, g_dym.page)) != 0x0)
		return (page->size);
	return (0);
}

static void			*realloc_new(
	void *ptr, t_uint64 size, t_uint64 origin_size)
{
	void		*mem;
	t_uint64	smaller_size;

	mem = malloc(size);
	if (mem == 0x0)
		return (0x0);
	smaller_size = (size > origin_size) ? origin_size : size;
	ft_memcpy(mem, ptr, smaller_size);
	free(ptr);
	return (mem);
}

static void			*realloc_blocked(
	void *ptr, t_uint64 size,
	t_uint16 origin_size, t_zone_type origin_zone_type)
{
	t_zone_type	type;
	t_mem_zone	*zone;
	t_uint8		origin_level;
	t_uint8		level;
	t_mem_pool	*pool;

	type = get_zone_type(size);
	if (origin_zone_type == type)
	{
		zone = (type == TINY) ? &g_dym.tiny_zone : &g_dym.small_zone;
		origin_level = get_block_level(origin_size, zone->smallest_block_size);
		level = get_block_level(size, zone->smallest_block_size);
		if (origin_level == level)
		{
			pool = find_mem_pool(ptr, zone);
			set_block_size(ptr, size, zone->smallest_block_size, pool);
			return (ptr);
		}
	}
	return (realloc_new(ptr, size, origin_size));
}

static void			*realloc_paged(
	void *ptr, t_uint64 size, t_uint64 origin_size)
{
	t_uint32	origin_page_count;
	t_uint32	page_count;
	t_mem_page	*page;

	if (size <= (t_uint64)getpagesize())
		return (realloc_new(ptr, size, origin_size));
	origin_page_count = get_required_page_count(
		get_adjusted_page_size(origin_size));
	page_count = get_required_page_count(
		get_adjusted_page_size(size));
	if (origin_page_count == page_count)
	{
		page = find_mem_page(ptr, g_dym.page);
		page->size = size;
		return (ptr);
	}
	return (realloc_new(ptr, size, origin_size));
}

void				*realloc(void *ptr, t_uint64 size)
{
	t_uint32	origin_size;
	t_zone_type	origin_zone_type;

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
	if (origin_zone_type == TINY || origin_zone_type == SMALL)
		return (realloc_blocked(ptr, size, origin_size, origin_zone_type));
	return (realloc_paged(ptr, size, origin_size));
}
