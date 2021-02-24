#include "malloc.h"

t_mem_pool	*alloc_mem_pool(t_uint32 mem_size, t_uint32 biggest_block_size)
{
	void		*mem;
	t_uint16	smallest_block_count;

	mem = mmap(0, mem_size,
		PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANON, -1, 0);
	if (mem == MAP_FAILED)
		return (0x0);
	smallest_block_count =
		get_smallest_block_count(biggest_block_size, mem_size);
	return (init_mem_pool(mem, smallest_block_count));
}
