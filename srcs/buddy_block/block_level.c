#include "malloc.h"

t_uint8		get_block_level(t_uint64 size, t_uint32 smallest_block_size)
{
	t_uint8		level;
	t_uint64	block_size;

	level = 0;
	block_size = smallest_block_size;
	while (block_size < size)
	{
		block_size <<= 1;
		level++;
	}
	return (level);
}
