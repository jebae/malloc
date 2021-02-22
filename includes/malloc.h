#ifndef MALLOC_H
# define MALLOC_H

#include <stdio.h>

# define NO_AVAIL_BLOCK	0xFFFF

typedef unsigned long	t_uint64;
typedef unsigned int	t_uint32;
typedef unsigned short	t_uint16;
typedef unsigned char	t_uint8;
typedef long			t_int64;
typedef int				t_int32;
typedef short			t_int16;
typedef char			t_int8;

typedef struct			s_mem_pool
{
	struct s_mem_pool	*next;
	t_uint8				*stats[4];
	t_uint16			*sizes;
	t_uint8				*data;
}						t_mem_pool;

/* buddy block */
t_uint8		get_block_level(t_uint64 size, t_uint32 smallest_block_size);
void		set_block_stat(t_uint16 idx, t_int32 flag, t_uint8 *stats);
t_uint32	get_block_stat(t_uint16 idx, t_uint8 *stats);
t_uint16	get_avail_block(t_uint8 *stats, t_uint16 count);

t_uint16	split_block(
	t_uint8 from, t_uint8 to, t_uint16 idx, t_uint8 **stats);

void		*alloc_block(
	t_uint64 size, t_uint32 smallest_block_size,
	t_uint16 smallest_block_count, t_mem_pool *pool);

void		merge_block(t_uint16 idx, t_uint8 level, t_uint8 **stats);

void		free_block(
	void *ptr, t_uint32 smallest_block_size, t_mem_pool *pool);

#endif
