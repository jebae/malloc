#include <gtest/gtest.h>
#include <stdlib.h>
#include "malloc.h"

static t_uint8	STATS[4][128];

class SplitBlockTest: public ::testing::Test {
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

	void	is_level_all_zero(t_uint8 level) {
		for (int i=0; i < sizes[level]; i++) {
			ASSERT_EQ(stats[level][i], 0);
		}
	}
};

TEST_F(SplitBlockTest, _1_to_0)
{
	t_uint8		from = 1;
	t_uint8		to = 0;
	t_uint16	idx = 14;

	set_block_stat(idx, 1, stats[from]);

	ASSERT_EQ(split_block(from, to, idx, stats), idx * 2);
	is_level_all_zero(3);
	is_level_all_zero(2);
	is_level_all_zero(1);

	for (int i=0; i < sizes[0]; i++) {
		if ((8 * i) <= (idx * 2) && (idx * 2) < (8 * (i + 1))) {
			ASSERT_EQ(stats[0][i], 0x30);
		} else {
			ASSERT_EQ(stats[0][i], 0);
		}
	}
}

TEST_F(SplitBlockTest, _2_to_0)
{
	t_uint8		from = 2;
	t_uint8		to = 0;
	t_uint16	idx = 221;

	set_block_stat(idx, 1, stats[from]);

	ASSERT_EQ(split_block(from, to, idx, stats), idx * 4);
	is_level_all_zero(3);
	is_level_all_zero(2);

	for (int i=0; i < sizes[1]; i++) {
		if ((8 * i) <= (idx * 2) && (idx * 2) < (8 * (i + 1))) {
			ASSERT_EQ(stats[1][i], 0x08);
		} else {
			ASSERT_EQ(stats[1][i], 0);
		}
	}

	for (int i=0; i < sizes[0]; i++) {
		if ((8 * i) <= (idx * 4) && (idx * 4) < (8 * (i + 1))) {
			ASSERT_EQ(stats[0][i], 0x30);
		} else {
			ASSERT_EQ(stats[0][i], 0);
		}
	}
}

TEST_F(SplitBlockTest, _3_to_0)
{
	t_uint8		from = 3;
	t_uint8		to = 0;
	t_uint16	idx = 96;

	set_block_stat(idx, 1, stats[from]);

	ASSERT_EQ(split_block(from, to, idx, stats), idx * 8);
	is_level_all_zero(3);

	for (int i=0; i < sizes[2]; i++) {
		if ((8 * i) <= (idx * 2) && (idx * 2) < (8 * (i + 1))) {
			ASSERT_EQ(stats[2][i], 0x02);
		} else {
			ASSERT_EQ(stats[2][i], 0);
		}
	}

	for (int i=0; i < sizes[1]; i++) {
		if ((8 * i) <= (idx * 4) && (idx * 4) < (8 * (i + 1))) {
			ASSERT_EQ(stats[1][i], 0x02);
		} else {
			ASSERT_EQ(stats[1][i], 0);
		}
	}

	for (int i=0; i < sizes[0]; i++) {
		if ((8 * i) <= (idx * 8) && (idx * 8) < (8 * (i + 1))) {
			ASSERT_EQ(stats[0][i], 0x03);
		} else {
			ASSERT_EQ(stats[0][i], 0);
		}
	}
}

TEST_F(SplitBlockTest, _2_to_1)
{
	t_uint8		from = 2;
	t_uint8		to = 1;
	t_uint16	idx = 129;

	set_block_stat(idx, 1, stats[from]);

	ASSERT_EQ(split_block(from, to, idx, stats), idx * 2);
	is_level_all_zero(3);
	is_level_all_zero(2);
	is_level_all_zero(0);

	for (int i=0; i < sizes[1]; i++) {
		if ((8 * i) <= (idx * 2) && (idx * 2) < (8 * (i + 1))) {
			ASSERT_EQ(stats[1][i], 0x0c);
		} else {
			ASSERT_EQ(stats[1][i], 0);
		}
	}
}

TEST_F(SplitBlockTest, _3_to_1)
{
	t_uint8		from = 3;
	t_uint8		to = 1;
	t_uint16	idx = 101;

	set_block_stat(idx, 1, stats[from]);

	ASSERT_EQ(split_block(from, to, idx, stats), idx * 4);
	is_level_all_zero(3);
	is_level_all_zero(0);

	for (int i=0; i < sizes[2]; i++) {
		if ((8 * i) <= (idx * 2) && (idx * 2) < (8 * (i + 1))) {
			ASSERT_EQ(stats[2][i], 0x08);
		} else {
			ASSERT_EQ(stats[2][i], 0);
		}
	}

	for (int i=0; i < sizes[1]; i++) {
		if ((8 * i) <= (idx * 4) && (idx * 4) < (8 * (i + 1))) {
			ASSERT_EQ(stats[1][i], 0x30);
		} else {
			ASSERT_EQ(stats[1][i], 0);
		}
	}
}

TEST_F(SplitBlockTest, _3_to_2)
{
	t_uint8		from = 3;
	t_uint8		to = 2;
	t_uint16	idx = 1;

	set_block_stat(idx, 1, stats[from]);

	ASSERT_EQ(split_block(from, to, idx, stats), idx * 2);
	is_level_all_zero(3);
	is_level_all_zero(1);
	is_level_all_zero(0);

	for (int i=0; i < sizes[2]; i++) {
		if ((8 * i) <= (idx * 2) && (idx * 2) < (8 * (i + 1))) {
			ASSERT_EQ(stats[2][i], 0x0c);
		} else {
			ASSERT_EQ(stats[2][i], 0);
		}
	}
}
