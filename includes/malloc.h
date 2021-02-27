#ifndef MALLOC_H
# define MALLOC_H

# include <stdio.h>
# include <sys/mman.h>
# include <unistd.h>
# include <sys/resource.h>
# include <stdlib.h>

# define NO_AVAIL_BLOCK	0xFFFF

typedef unsigned long	t_uint64;
typedef unsigned int	t_uint32;
typedef unsigned short	t_uint16;
typedef unsigned char	t_uint8;
typedef long			t_int64;
typedef int				t_int32;
typedef short			t_int16;
typedef char			t_int8;

typedef enum			e_zone_type
{
	TINY,
	SMALL,
	LARGE,
}						t_zone_type;

typedef struct			s_mem_pool
{
	struct s_mem_pool	*next;
	t_uint16			allocated;
	t_uint8				*stats[4];
	t_uint16			*sizes;
	t_uint8				*data;
}						t_mem_pool;

typedef struct			s_mem_page
{
	struct s_mem_page	*next;
	t_uint64			size;
	t_uint8				*data;
}						t_mem_page;

typedef struct			s_mem_zone
{
	t_mem_pool	*head;
	t_mem_pool	*tail;
	t_uint64	mem_size;
	t_uint64	pool_size;
	t_uint32	smallest_block_size;
	t_uint16	smallest_block_count;
}						t_mem_zone;

typedef struct			s_dynamic_mem
{
	t_mem_zone	tiny_zone;
	t_mem_zone	small_zone;
	t_mem_page	*page;
	int			is_initialized;
}						t_dynamic_mem;

/*
** buddy block
*/

t_uint8					get_block_level(
	t_uint64 size, t_uint32 smallest_block_size);

void					set_block_stat(
	t_uint16 idx, t_int32 flag, t_uint8 *stats);

t_uint32				get_block_stat(t_uint16 idx, t_uint8 *stats);

t_uint16				get_block_size(
	void *ptr, t_uint32 smallest_block_size, t_mem_pool *pool);

void					set_block_size(
	void *ptr, t_uint16 size, t_uint32 smallest_block_size, t_mem_pool *pool);

t_uint16				get_avail_block(t_uint8 *stats, t_uint16 count);

t_uint16				split_block(
	t_uint8 from, t_uint8 to, t_uint16 idx, t_uint8 **stats);

void					*alloc_block(
	t_uint64 size, t_uint32 smallest_block_size,
	t_uint16 smallest_block_count, t_mem_pool *pool);

void					merge_block(
	t_uint16 idx, t_uint8 level, t_uint8 **stats);

void					free_block(
	void *ptr, t_uint32 smallest_block_size, t_mem_pool *pool);

/*
** memory pool
*/

t_uint16				get_smallest_block_count(
	t_uint32 biggest_block_size, t_uint32 mem_size);

t_mem_pool				*init_mem_pool(
	void *mem, t_uint16 smallest_block_count);

t_mem_pool				*alloc_mem_pool(
	t_uint32 mem_size, t_uint32 biggest_block_size);

/*
** memory page
*/

t_uint64				get_adjusted_page_size(t_uint64 size);
t_uint32				get_required_page_count(t_uint64 size);
t_mem_page				*alloc_mem_page(t_uint64 size);
t_mem_page				*find_mem_page(void *ptr, t_mem_page *page);

/*
** init dynamic memory
*/

int						init_dynamic_memory(t_dynamic_mem *dym);

/*
** utils
*/

t_mem_pool				*find_mem_pool(void *ptr, t_mem_zone *zone);
t_uint32				get_pool_count(t_mem_zone *zone);
t_uint32				get_page_count(t_mem_page *page);
t_zone_type				get_zone_type(t_uint64 size);
t_uint64				get_data_limit(void);

/*
** libc
*/

void					*ft_memset(void *b, int c, size_t len);
void					*ft_memcpy(void *dst, const void *src, size_t n);
void					ft_putstr(char *s);
void					ft_putchar(char c);

/*
** main functions
*/

void					*malloc(t_uint64 size);
void					free(void *ptr);
void					*realloc(void *ptr, t_uint64 size);
void					show_alloc_mem(void);

#endif
