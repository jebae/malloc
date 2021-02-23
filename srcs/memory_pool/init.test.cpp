#include <gtest/gtest.h>
#include <stdlib.h>
#include "malloc.h"

TEST(get_smallest_block_count, tiny)
{
	t_uint32	biggest_block_size = 4096 / 16;
	t_uint32	mem_size = 8 * 4096;

	ASSERT_EQ(
		get_smallest_block_count(biggest_block_size, mem_size),
		950);
}

TEST(get_smallest_block_count, small)
{
	t_uint32	biggest_block_size = 4096;
	t_uint32	mem_size = 128 * 4096;

	ASSERT_EQ(
		get_smallest_block_count(biggest_block_size, mem_size),
		1019);
}


TEST(init_mem_pool, tiny)
{
	unsigned char	mem[8 * 4096];
	t_uint16		smallest_block_count = 950;
	t_mem_pool		*pool;

	memset(mem, 0xFF, 8 * 4096);
	pool = init_mem_pool(mem, smallest_block_count);

	ASSERT_EQ((long)pool, (long)mem);
	ASSERT_EQ((long)pool->next, NULL);
	ASSERT_EQ(pool->stats[0], mem + sizeof(t_mem_pool));
	ASSERT_EQ(pool->stats[1], mem + sizeof(t_mem_pool) + 128);
	ASSERT_EQ(pool->stats[2], mem + sizeof(t_mem_pool) + 128 + 64);
	ASSERT_EQ(pool->stats[3], mem + sizeof(t_mem_pool) + 128 + 64 + 32);
	ASSERT_EQ((t_uint8 *)pool->sizes, mem + sizeof(t_mem_pool) + 240);
	ASSERT_EQ(pool->data, mem + sizeof(t_mem_pool) + 240
		+ sizeof(t_uint16) * 1024);

	ASSERT_EQ(get_avail_block(pool->stats[0], smallest_block_count),
		NO_AVAIL_BLOCK);

	ASSERT_EQ(get_avail_block(pool->stats[1], smallest_block_count >> 1),
		(smallest_block_count >> 1) - 1);

	ASSERT_EQ(get_avail_block(pool->stats[2], smallest_block_count >> 2),
		(smallest_block_count >> 2) - 1);

	for (t_uint16 i=0; i < (smallest_block_count >> 3); i++) {
		ASSERT_EQ(get_block_stat(i, pool->stats[3]), (t_uint32)1);
	}

	for (t_uint16 i=0; i < smallest_block_count; i++) {
		ASSERT_EQ(pool->sizes[i], 0);
	}
}

TEST(init_mem_pool, small)
{
	unsigned char	mem[128 * 4096];
	t_uint16		smallest_block_count = 1019;
	t_mem_pool		*pool;

	memset(mem, 0xFF, 128 * 4096);
	pool = init_mem_pool(mem, smallest_block_count);

	ASSERT_EQ((long)pool, (long)mem);
	ASSERT_EQ((long)pool->next, NULL);
	ASSERT_EQ(pool->stats[0], mem + sizeof(t_mem_pool));
	ASSERT_EQ(pool->stats[1], mem + sizeof(t_mem_pool) + 128);
	ASSERT_EQ(pool->stats[2], mem + sizeof(t_mem_pool) + 128 + 64);
	ASSERT_EQ(pool->stats[3], mem + sizeof(t_mem_pool) + 128 + 64 + 32);
	ASSERT_EQ((t_uint8 *)pool->sizes, mem + sizeof(t_mem_pool) + 240);
	ASSERT_EQ(pool->data, mem + sizeof(t_mem_pool) + 240
		+ sizeof(t_uint16) * 1024);

	ASSERT_EQ(get_avail_block(pool->stats[0], smallest_block_count),
		smallest_block_count - 1);

	ASSERT_EQ(get_avail_block(pool->stats[1], smallest_block_count >> 1),
		(smallest_block_count >> 1) - 1);

	ASSERT_EQ(get_avail_block(pool->stats[2], smallest_block_count >> 2),
		NO_AVAIL_BLOCK);

	for (t_uint16 i=0; i < (smallest_block_count >> 3); i++) {
		ASSERT_EQ(get_block_stat(i, pool->stats[3]), (t_uint32)1);
	}

	for (t_uint16 i=0; i < smallest_block_count; i++) {
		ASSERT_EQ(pool->sizes[i], 0);
	}
}
