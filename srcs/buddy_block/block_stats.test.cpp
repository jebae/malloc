#include <gtest/gtest.h>
#include <stdlib.h>
#include "malloc.h"

class BlockStatTest: public ::testing::Test {
protected:
	t_uint8	stats[1024];

	virtual void	SetUp() {
		memset(stats, 0, 1024);
	}
};

TEST_F(BlockStatTest, set_block_stat_32)
{
	t_uint16	idx = 32;

	set_block_stat(idx, 1, stats);
	ASSERT_NE(stats[4] & (1 << 0), 0);

	set_block_stat(idx, 0, stats);
	ASSERT_EQ(stats[4] & (1 << 0), 0);
}

TEST_F(BlockStatTest, set_block_stat_1024)
{
	t_uint16	idx = 1024;

	set_block_stat(idx, 1, stats);
	ASSERT_NE(stats[128] & (1 << 0), 0);

	set_block_stat(idx, 0, stats);
	ASSERT_EQ(stats[128] & (1 << 0), 0);
}

TEST_F(BlockStatTest, set_block_stat_21)
{
	t_uint16	idx = 21;

	set_block_stat(idx, 1, stats);
	ASSERT_NE(stats[2] & (1 << 5), 0);

	set_block_stat(idx, 0, stats);
	ASSERT_EQ(stats[2] & (1 << 5), 0);
}

TEST_F(BlockStatTest, set_block_stat_7)
{
	t_uint16	idx = 7;

	set_block_stat(idx, 1, stats);
	ASSERT_NE(stats[0] & (1 << 7), 0);

	set_block_stat(idx, 0, stats);
	ASSERT_EQ(stats[0] & (1 << 7), 0);
}

TEST_F(BlockStatTest, set_block_stat_1)
{
	t_uint16	idx = 1;

	set_block_stat(idx, 1, stats);
	ASSERT_NE(stats[0] & (1 << 1), 0);

	set_block_stat(idx, 0, stats);
	ASSERT_EQ(stats[0] & (1 << 1), 0);
}

TEST_F(BlockStatTest, set_block_stat_0)
{
	t_uint16	idx = 0;

	set_block_stat(idx, 1, stats);
	ASSERT_NE(stats[0] & (1 << 0), 0);

	set_block_stat(idx, 0, stats);
	ASSERT_EQ(stats[0] & (1 << 0), 0);
}

TEST_F(BlockStatTest, set_block_stat_31)
{
	t_uint16	idx = 31;

	set_block_stat(idx, 1, stats);
	ASSERT_NE(stats[3] & (1 << 7), 0);

	set_block_stat(idx, 0, stats);
	ASSERT_EQ(stats[4] & (1 << 7), 0);
}

TEST_F(BlockStatTest, set_block_stat_1023)
{
	t_uint16	idx = 1023;

	set_block_stat(idx, 1, stats);
	ASSERT_NE(stats[127] & (1 << 7), 0);

	set_block_stat(idx, 0, stats);
	ASSERT_EQ(stats[127] & (1 << 7), 0);
}

TEST_F(BlockStatTest, set_block_stat_max_idx)
{
	t_uint16	idx = 8 * sizeof(stats) - 1;

	set_block_stat(idx, 1, stats);
	ASSERT_NE(stats[1023] & (1 << 7), 0);

	set_block_stat(idx, 0, stats);
	ASSERT_EQ(stats[1023] & (1 << 7), 0);
}

TEST_F(BlockStatTest, get_block_stat_32)
{
	t_uint16	idx = 32;

	stats[idx / 8] |= 1 << (idx % 8);
	ASSERT_EQ((int)get_block_stat(idx, stats), 1);

	stats[idx / 8] &= ~(1 << (idx % 8));
	ASSERT_EQ((int)get_block_stat(idx, stats), 0);
}

TEST_F(BlockStatTest, get_block_stat_1024)
{
	t_uint16	idx = 1024;

	stats[idx / 8] |= 1 << (idx % 8);
	ASSERT_EQ((int)get_block_stat(idx, stats), 1);

	stats[idx / 8] &= ~(1 << (idx % 8));
	ASSERT_EQ((int)get_block_stat(idx, stats), 0);
}

TEST_F(BlockStatTest, get_block_stat_21)
{
	t_uint16	idx = 21;

	stats[idx / 8] |= 1 << (idx % 8);
	ASSERT_EQ((int)get_block_stat(idx, stats), 1);

	stats[idx / 8] &= ~(1 << (idx % 8));
	ASSERT_EQ((int)get_block_stat(idx, stats), 0);
}

TEST_F(BlockStatTest, get_block_stat_7)
{
	t_uint16	idx = 7;

	stats[idx / 8] |= 1 << (idx % 8);
	ASSERT_EQ((int)get_block_stat(idx, stats), 1);

	stats[idx / 8] &= ~(1 << (idx % 8));
	ASSERT_EQ((int)get_block_stat(idx, stats), 0);
}

TEST_F(BlockStatTest, get_block_stat_1)
{
	t_uint16	idx = 1;

	stats[idx / 8] |= 1 << (idx % 8);
	ASSERT_EQ((int)get_block_stat(idx, stats), 1);

	stats[idx / 8] &= ~(1 << (idx % 8));
	ASSERT_EQ((int)get_block_stat(idx, stats), 0);
}

TEST_F(BlockStatTest, get_block_stat_0)
{
	t_uint16	idx = 0;

	stats[idx / 8] |= 1 << (idx % 8);
	ASSERT_EQ((int)get_block_stat(idx, stats), 1);

	stats[idx / 8] &= ~(1 << (idx % 8));
	ASSERT_EQ((int)get_block_stat(idx, stats), 0);
}

TEST_F(BlockStatTest, get_block_stat_31)
{
	t_uint16	idx = 31;

	stats[idx / 8] |= 1 << (idx % 8);
	ASSERT_EQ((int)get_block_stat(idx, stats), 1);

	stats[idx / 8] &= ~(1 << (idx % 8));
	ASSERT_EQ((int)get_block_stat(idx, stats), 0);
}

TEST_F(BlockStatTest, get_block_stat_1023)
{
	t_uint16	idx = 1023;

	stats[idx / 8] |= 1 << (idx % 8);
	ASSERT_EQ((int)get_block_stat(idx, stats), 1);

	stats[idx / 8] &= ~(1 << (idx % 8));
	ASSERT_EQ((int)get_block_stat(idx, stats), 0);
}

TEST_F(BlockStatTest, get_block_stat_max_idx)
{
	t_uint16	idx = 8 * sizeof(stats) - 1;

	stats[idx / 8] |= 1 << (idx % 8);
	ASSERT_EQ((int)get_block_stat(idx, stats), 1);

	stats[idx / 8] &= ~(1 << (idx % 8));
	ASSERT_EQ((int)get_block_stat(idx, stats), 0);
}
