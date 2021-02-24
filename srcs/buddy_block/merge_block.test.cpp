#include <gtest/gtest.h>
#include <stdlib.h>
#include "malloc.h"

static t_uint8	STATS[4][128];

class MergeBlockTest: public ::testing::Test {
protected:
	t_uint8	*stats[4];
	int		sizes[4];

	virtual void	SetUp() {
		int		i = 0;

		for (int count=886; count >= 100; count >>= 1) {
			int size = count / 8;

			if (count % 8 > 0)
				size++;
			stats[i] = STATS[i];
			memset(stats[i], 0, size);

			sizes[i] = size;
			i++;
		}
	}

	void	isLevelAllZero(t_uint8 level) {
		for (int i=0; i < sizes[level]; i++) {
			ASSERT_EQ(stats[level][i], 0);
		}
	}
};

TEST_F(MergeBlockTest, buddy_not_exist_odd_idx)
{
	t_uint16	idx = 55;
	t_uint8		level = 0;

	set_block_stat(idx, 1, stats[level]);
	set_block_stat(idx + 1, 1, stats[level]);
	merge_block(idx, level, stats);

	ASSERT_EQ(get_block_stat(idx, stats[level]), (t_uint32)1);
	ASSERT_EQ(get_block_stat(idx + 1, stats[level]), (t_uint32)1);
}

TEST_F(MergeBlockTest, buddy_not_exist_even_idx)
{
	t_uint16	idx = 54;
	t_uint8		level = 0;

	set_block_stat(idx, 1, stats[level]);
	set_block_stat(idx - 1, 1, stats[level]);
	merge_block(idx, level, stats);

	ASSERT_EQ(get_block_stat(idx, stats[level]), (t_uint32)1);
	ASSERT_EQ(get_block_stat(idx - 1, stats[level]), (t_uint32)1);
}

TEST_F(MergeBlockTest, _0_to_1)
{
	t_uint16	idx = 55;
	t_uint8		level = 0;

	set_block_stat(idx, 1, stats[level]);
	set_block_stat(idx - 1, 1, stats[level]);
	merge_block(idx, level, stats);

	// me
	ASSERT_EQ(get_block_stat(idx, stats[level]), (t_uint32)0);

	// buddy
	ASSERT_EQ(get_block_stat(idx - 1, stats[level]), (t_uint32)0);

	// level 1
	ASSERT_EQ(get_block_stat(idx >> 1, stats[level + 1]), (t_uint32)1);
}

TEST_F(MergeBlockTest, _0_to_2)
{
	t_uint16	idx = 55;
	t_uint8		level = 0;

	set_block_stat(idx, 1, stats[level]);
	set_block_stat(idx - 1, 1, stats[level]);
	set_block_stat((idx >> 1) - 1, 1, stats[level + 1]);
	merge_block(idx, level, stats);

	// me
	ASSERT_EQ(get_block_stat(idx, stats[level]), (t_uint32)0);

	// buddy
	ASSERT_EQ(get_block_stat(idx - 1, stats[level]), (t_uint32)0);

	// level 1
	ASSERT_EQ(get_block_stat(idx >> 1, stats[level + 1]), (t_uint32)0);
	ASSERT_EQ(get_block_stat((idx >> 1) - 1, stats[level + 1]), (t_uint32)0);

	// level 2
	ASSERT_EQ(get_block_stat(idx >> 2, stats[level + 2]), (t_uint32)1);
}

TEST_F(MergeBlockTest, _0_to_3)
{
	t_uint16	idx = 55;
	t_uint8		level = 0;

	set_block_stat(idx, 1, stats[level]);
	set_block_stat(idx - 1, 1, stats[level]);
	set_block_stat((idx >> 1) - 1, 1, stats[level + 1]);
	set_block_stat((idx >> 2) - 1, 1, stats[level + 2]);
	set_block_stat((idx >> 3) + 1, 1, stats[level + 3]);
	merge_block(idx, level, stats);

	// me
	ASSERT_EQ(get_block_stat(idx, stats[level]), (t_uint32)0);

	// buddy
	ASSERT_EQ(get_block_stat(idx - 1, stats[level]), (t_uint32)0);

	// level 1
	ASSERT_EQ(get_block_stat(idx >> 1, stats[level + 1]), (t_uint32)0);
	ASSERT_EQ(get_block_stat((idx >> 1) - 1, stats[level + 1]), (t_uint32)0);

	// level 2
	ASSERT_EQ(get_block_stat(idx >> 2, stats[level + 2]), (t_uint32)0);
	ASSERT_EQ(get_block_stat((idx >> 2) - 1, stats[level + 2]), (t_uint32)0);

	// level 3 (no more merge)
	ASSERT_EQ(get_block_stat(idx >> 3, stats[level + 3]), (t_uint32)1);
	ASSERT_EQ(get_block_stat((idx >> 3) + 1, stats[level + 3]), (t_uint32)1);
}

TEST_F(MergeBlockTest, _1_to_2)
{
	t_uint16	idx = 402;
	t_uint8		level = 1;

	set_block_stat(idx, 1, stats[level]);
	set_block_stat(idx + 1, 1, stats[level]);
	merge_block(idx, level, stats);

	// me
	ASSERT_EQ(get_block_stat(idx, stats[level]), (t_uint32)0);

	// buddy
	ASSERT_EQ(get_block_stat(idx + 1, stats[level]), (t_uint32)0);

	// level 1
	ASSERT_EQ(get_block_stat(idx >> 1, stats[level + 1]), (t_uint32)1);
}

TEST_F(MergeBlockTest, _1_to_3)
{
	t_uint16	idx = 402;
	t_uint8		level = 1;

	set_block_stat(idx, 1, stats[level]);
	set_block_stat(idx + 1, 1, stats[level]);
	set_block_stat((idx >> 1) - 1, 1, stats[level + 1]);
	set_block_stat((idx >> 2) + 1, 1, stats[level + 2]);
	merge_block(idx, level, stats);

	// me
	ASSERT_EQ(get_block_stat(idx, stats[level]), (t_uint32)0);

	// buddy
	ASSERT_EQ(get_block_stat(idx + 1, stats[level]), (t_uint32)0);

	// level 2
	ASSERT_EQ(get_block_stat(idx >> 1, stats[level + 1]), (t_uint32)0);
	ASSERT_EQ(get_block_stat((idx >> 1) - 1, stats[level + 1]), (t_uint32)0);

	// level 3
	ASSERT_EQ(get_block_stat(idx >> 2, stats[level + 2]), (t_uint32)1);
	ASSERT_EQ(get_block_stat((idx >> 2) + 1, stats[level + 2]), (t_uint32)1);
}

TEST_F(MergeBlockTest, _2_to_3)
{
	t_uint16	idx = 16;
	t_uint8		level = 2;

	set_block_stat(idx, 1, stats[level]);
	set_block_stat(idx + 1, 1, stats[level]);
	merge_block(idx, level, stats);

	// me
	ASSERT_EQ(get_block_stat(idx, stats[level]), (t_uint32)0);

	// buddy
	ASSERT_EQ(get_block_stat(idx + 1, stats[level]), (t_uint32)0);

	// level 1
	ASSERT_EQ(get_block_stat(idx >> 1, stats[level + 1]), (t_uint32)1);
}

TEST_F(MergeBlockTest, _3)
{
	t_uint16	idx = 99;
	t_uint8		level = 3;

	set_block_stat(idx, 1, stats[level]);
	set_block_stat(idx - 1, 1, stats[level]);
	merge_block(idx, level, stats);
	ASSERT_EQ(get_block_stat(idx, stats[level]), (t_uint32)1);
	ASSERT_EQ(get_block_stat(idx - 1, stats[level]), (t_uint32)1);
}
