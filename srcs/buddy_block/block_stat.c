#include "malloc.h"

void		set_block_stat(t_uint16 idx, t_int32 flag, t_uint8 *stats)
{
	stats += idx / 8;

	if (flag == 0)
		*stats &= ~(1 << (idx % 8));
	else
		*stats |= 1 << (idx % 8);
}

t_uint32	get_block_stat(t_uint16 idx, t_uint8 *stats)
{
	stats += idx / 8;
	return ((*stats) & (1 << (idx % 8)) ? 1 : 0);
}
