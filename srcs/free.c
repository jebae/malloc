#include "malloc.h"

extern t_dynamic_mem	g_dym;

static int	free_blocked(void *ptr, t_mem_zone *zone)
{
	t_mem_pool	*cur;
	t_mem_pool	*prev;

	prev = 0x0;
	cur = zone->head;
	while (cur)
	{
		if (cur->data <= (t_uint8 *)ptr
			&& (t_uint8 *)ptr < cur->data + zone->pool_size)
		{
			free_block(ptr, zone->smallest_block_size, cur);
			if (cur->allocated == 0 && cur != zone->head)
			{
				prev->next = cur->next;
				munmap(cur, zone->mem_size);
			}
			return (1);
		}
		prev = cur;
		cur = cur->next;
	}
	return (0);
}

static void	free_paged(void *ptr)
{
	t_mem_page	*cur;
	t_mem_page	*prev;
	t_uint32	page_count;

	prev = 0x0;
	cur = g_dym.page;
	while (cur)
	{
		if (ptr == cur->data)
		{
			if (prev)
				prev->next = cur->next;
			else
				g_dym.page = cur->next;
			page_count = get_required_page_count(
				get_adjusted_page_size(cur->size));
			munmap(cur, page_count * getpagesize());
			return ;
		}
		prev = cur;
		cur = cur->next;
	}
}

void		free(void *ptr)
{
	if (!g_dym.is_initialized && init_dynamic_memory(&g_dym) == -1)
		return ;
	if (free_blocked(ptr, &g_dym.tiny_zone))
		return ;
	if (free_blocked(ptr, &g_dym.small_zone))
		return ;
	free_paged(ptr);
}
