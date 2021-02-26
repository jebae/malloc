#include "malloc.h"

void		free_mem_pool(t_mem_pool *pool, t_uint64 mem_size)
{
	munmap(pool, mem_size);
}
