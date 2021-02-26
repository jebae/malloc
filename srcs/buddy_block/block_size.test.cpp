#include <gtest/gtest.h>
#include <stdlib.h>
#include "malloc.h"

static t_uint16	SIZES[1024];

class BlockSizeTest: public ::testing::Test {
protected:
	t_mem_pool	pool;

	virtual void	SetUp() {
		memset(SIZES, 0, 1024 * 2);
		pool.sizes = SIZES;
		pool.data = (t_uint8 *)0x1234;
	}

	void	is_size_all_zero() {
		for (int i=0; i < 1024; i++) {
			ASSERT_EQ(SIZES[i], 0);
		}
	}
};

TEST_F(BlockSizeTest, get_block_size_allocated)
{
	t_uint32	smallest_block_size = 32;

	pool.sizes[0] = 13;
	ASSERT_EQ(
		get_block_size(pool.data, smallest_block_size, &pool),
		pool.sizes[0]);

	pool.sizes[1] = 55;
	ASSERT_EQ(
		get_block_size(pool.data + smallest_block_size, smallest_block_size, &pool),
		pool.sizes[1]);

	pool.sizes[253] = 23;
	ASSERT_EQ(
		get_block_size(pool.data + smallest_block_size * 253, smallest_block_size, &pool),
		pool.sizes[253]);
}

TEST_F(BlockSizeTest, get_block_size_not_allocated)
{
	t_uint32	smallest_block_size = 32;

	ASSERT_EQ(
		get_block_size(pool.data, smallest_block_size, &pool),
		0);

	ASSERT_EQ(
		get_block_size(pool.data + smallest_block_size, smallest_block_size, &pool),
		0);

	ASSERT_EQ(
		get_block_size(pool.data + smallest_block_size * 253, smallest_block_size, &pool),
		0);
}

TEST_F(BlockSizeTest, get_block_size_not_arranged)
{
	t_uint32	smallest_block_size = 32;

	ASSERT_EQ(
		get_block_size(pool.data + 1, smallest_block_size, &pool),
		0);

	ASSERT_EQ(
		get_block_size(pool.data + smallest_block_size - 10, smallest_block_size, &pool),
		0);

	ASSERT_EQ(
		get_block_size(pool.data + smallest_block_size * 253 + 4, smallest_block_size, &pool),
		0);
}

TEST_F(BlockSizeTest, set_block_size_valid)
{
	t_uint32	smallest_block_size = 32;

	set_block_size(pool.data, 13, smallest_block_size, &pool);
	ASSERT_EQ(pool.sizes[0], 13);

	set_block_size(
		pool.data + smallest_block_size, 55, smallest_block_size, &pool);
	ASSERT_EQ(pool.sizes[1], 55);

	set_block_size(
		pool.data + smallest_block_size * 253, 23, smallest_block_size, &pool);
	ASSERT_EQ(pool.sizes[253], 23);
}

TEST_F(BlockSizeTest, set_block_size_not_arranged)
{
	t_uint32	smallest_block_size = 32;

	set_block_size(pool.data + 1, 1, smallest_block_size, &pool);
	is_size_all_zero();

	set_block_size(
		pool.data + smallest_block_size - 10, 1, smallest_block_size, &pool);
	is_size_all_zero();

	set_block_size(pool.data + smallest_block_size * 253 + 4, 1, smallest_block_size, &pool);
	is_size_all_zero();
}
