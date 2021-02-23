#include <gtest/gtest.h>
#include <stdlib.h>
#include "malloc.h"

class AllocMemPoolTinyTest: public ::testing::Test {
protected:
	t_mem_pool		*pool;
	t_uint32		biggest_block_size;
	t_uint32		smallest_block_size;
	t_uint32		mem_size;
	t_uint16		smallest_block_count;
	unsigned char	*mem;

	virtual void	SetUp() {
		biggest_block_size = 4096 / 16;
		smallest_block_size = biggest_block_size >> 3;
		mem_size = 8 * 4096;

		// smallest_block_count == 950
		smallest_block_count =
			get_smallest_block_count(biggest_block_size, mem_size);
	}
};

TEST_F(AllocMemPoolTinyTest, level_0)
{
	pool = alloc_mem_pool(mem_size, biggest_block_size);

	ASSERT_NE((long)pool, NULL);

	t_uint32 block_size = smallest_block_size;

	// alloc and write value
	for (t_uint16 i=0; i < smallest_block_count; i++) {
		mem = (unsigned char *)alloc_block(
			block_size, smallest_block_size, smallest_block_count, pool);

		memset(mem, 0x12, block_size);
		for (t_uint32 j=0; j < block_size; j++) {
			ASSERT_EQ(mem[j], 0x12);
		}
	}

	// check no more alloc available
	mem = (unsigned char *)alloc_block(
		block_size, smallest_block_size, smallest_block_count, pool);

	ASSERT_EQ((long)mem, NULL);

	// free all block
	for (t_uint16 i=0; i < smallest_block_count; i++) {
		free_block(pool->data + i * block_size, smallest_block_size, pool);
	}

	// check all block is freed
	ASSERT_EQ(get_avail_block(pool->stats[0], smallest_block_count),
		NO_AVAIL_BLOCK);

	ASSERT_EQ(get_avail_block(pool->stats[1], smallest_block_count >> 1),
		(smallest_block_count >> 1) - 1);

	ASSERT_EQ(get_avail_block(pool->stats[2], smallest_block_count >> 2),
		(smallest_block_count >> 2) - 1);

	for (t_uint16 i=0; i < (smallest_block_count >> 3); i++) {
		ASSERT_EQ(get_block_stat(i, pool->stats[3]), (t_uint32)1);
	}

	ASSERT_NE(munmap(pool, mem_size), -1);
}

TEST_F(AllocMemPoolTinyTest, level_1)
{
	pool = alloc_mem_pool(mem_size, biggest_block_size);

	ASSERT_NE((long)pool, NULL);

	t_uint32 block_size = smallest_block_size << 1;

	// alloc and write value
	for (t_uint16 i=0; i < smallest_block_count >> 1; i++) {
		mem = (unsigned char *)alloc_block(
			block_size, smallest_block_size, smallest_block_count, pool);

		memset(mem, 0x12, block_size);
		for (t_uint32 j=0; j < block_size; j++) {
			ASSERT_EQ(mem[j], 0x12);
		}
	}

	// check no more alloc available
	mem = (unsigned char *)alloc_block(
		block_size, smallest_block_size, smallest_block_count, pool);

	ASSERT_EQ((long)mem, NULL);

	// free all block
	for (t_uint16 i=0; i < smallest_block_count >> 1; i++) {
		free_block(pool->data + i * block_size, smallest_block_size, pool);
	}

	// check all block is freed
	ASSERT_EQ(get_avail_block(pool->stats[1], smallest_block_count >> 1),
		(smallest_block_count >> 1) - 1);

	ASSERT_EQ(get_avail_block(pool->stats[2], smallest_block_count >> 2),
		(smallest_block_count >> 2) - 1);

	for (t_uint16 i=0; i < (smallest_block_count >> 3); i++) {
		ASSERT_EQ(get_block_stat(i, pool->stats[3]), (t_uint32)1);
	}

	ASSERT_NE(munmap(pool, mem_size), -1);
}

TEST_F(AllocMemPoolTinyTest, level_2)
{
	pool = alloc_mem_pool(mem_size, biggest_block_size);

	ASSERT_NE((long)pool, NULL);

	t_uint32 block_size = smallest_block_size << 2;

	// alloc and write value
	for (t_uint16 i=0; i < smallest_block_count >> 2; i++) {
		mem = (unsigned char *)alloc_block(
			block_size, smallest_block_size, smallest_block_count, pool);

		memset(mem, 0x12, block_size);
		for (t_uint32 j=0; j < block_size; j++) {
			ASSERT_EQ(mem[j], 0x12);
		}
	}

	// check no more alloc available
	mem = (unsigned char *)alloc_block(
		block_size, smallest_block_size, smallest_block_count, pool);

	ASSERT_EQ((long)mem, NULL);

	// free all block
	for (t_uint16 i=0; i < smallest_block_count >> 2; i++) {
		free_block(pool->data + i * block_size, smallest_block_size, pool);
	}

	// check all block is freed
	ASSERT_EQ(get_avail_block(pool->stats[2], smallest_block_count >> 2),
		(smallest_block_count >> 2) - 1);

	for (t_uint16 i=0; i < (smallest_block_count >> 3); i++) {
		ASSERT_EQ(get_block_stat(i, pool->stats[3]), (t_uint32)1);
	}

	ASSERT_NE(munmap(pool, mem_size), -1);
}

TEST_F(AllocMemPoolTinyTest, level_3)
{
	pool = alloc_mem_pool(mem_size, biggest_block_size);

	ASSERT_NE((long)pool, NULL);

	t_uint32 block_size = smallest_block_size << 3;

	// alloc and write value
	for (t_uint16 i=0; i < smallest_block_count >> 3; i++) {
		mem = (unsigned char *)alloc_block(
			block_size, smallest_block_size, smallest_block_count, pool);

		memset(mem, 0x12, block_size);
		for (t_uint32 j=0; j < block_size; j++) {
			ASSERT_EQ(mem[j], 0x12);
		}
	}

	// check no more alloc available
	mem = (unsigned char *)alloc_block(
		block_size, smallest_block_size, smallest_block_count, pool);

	ASSERT_EQ((long)mem, NULL);

	// free all block
	for (t_uint16 i=0; i < smallest_block_count >> 3; i++) {
		free_block(pool->data + i * block_size, smallest_block_size, pool);
	}

	// check all block is freed
	for (t_uint16 i=0; i < (smallest_block_count >> 3); i++) {
		ASSERT_EQ(get_block_stat(i, pool->stats[3]), (t_uint32)1);
	}

	ASSERT_NE(munmap(pool, mem_size), -1);
}
