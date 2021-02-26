#include "malloc.h"

t_mem_pool	*find_mem_pool(void *ptr, t_mem_zone *zone)
{
	t_mem_pool	*cur;

	cur = zone->head;
	while (cur)
	{
		if (cur->data <= ptr && ptr < cur->data + zone->pool_size)
			return (cur);
		cur = cur->next;
	}
	return (0x0);
}
