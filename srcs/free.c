#include "malloc.h"

extern t_dynamic_mem	g_dym;

static int	try_blocked_zone(void *mem, t_mem_zone *zone)
{
	t_mem_pool	*cur;
	t_mem_pool	*prev;

	prev = 0x0;
	cur = zone->head;
	while (cur)
	{
		if (cur->data <= mem && mem < cur->data + zone->pool_size)
		{
			free_block(mem, zone->smallest_block_size, cur);
			if (cur->allocated == 0 && cur != zone->head)
			{
				prev->next = cur->next;
				free_mem_pool(cur, zone->mem_size);
			}
			return (1);
		}
		prev = cur;
		cur = cur->next;
	}
	return (0);
}

void		free(void *mem)
{
	if (try_blocked_zone(mem, &g_dym.tiny_zone))
		return ;
	if (try_blocked_zone(mem, &g_dym.small_zone))
		return ;
}
