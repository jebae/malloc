#include "malloc.h"

t_mem_pool	*find_mem_pool(void *ptr, t_mem_zone *zone)
{
	t_mem_pool	*cur;

	cur = zone->head;
	while (cur)
	{
		if (cur->data <= (t_uint8 *)ptr
			&& (t_uint8 *)ptr < cur->data + zone->pool_size)
			return (cur);
		cur = cur->next;
	}
	return (0x0);
}

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

t_uint32	get_pool_count(t_mem_zone *zone)
{
	t_mem_pool	*pool;
	t_uint32	count;

	count = 0;
	pool = zone->head;
	while (pool)
	{
		count++;
		pool = pool->next;
	}
	return (count);
}

t_uint32	get_page_count(t_mem_page *page)
{
	t_uint32	count;

	count = 0;
	while (page)
	{
		count++;
		page = page->next;
	}
	return (count);
}

t_uint64	get_data_limit(void)
{
	struct rlimit	rl;

	if (getrlimit(RLIMIT_DATA, &rl) == -1)
		return (0);
	return (rl.rlim_max);
}
