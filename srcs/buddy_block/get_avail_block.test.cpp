#include <gtest/gtest.h>
#include <stdlib.h>
#include "malloc.h"

class GetAvailBlockTest: public ::testing::Test {
protected:
	t_uint8	stats[128];

	virtual void	SetUp() {
		memset(stats, 0, 128);
	}
};

TEST_F(GetAvailBlockTest, count_1024)
{
	t_uint16	idxs[] = {1, 32, 48, 68, 90, 1023, 776, 864};
	t_uint16	count = 1024;

	for (t_uint32 i=0; i < sizeof(idxs) / sizeof(t_uint16); i++) {
		set_block_stat(idxs[i], 1, stats);
		ASSERT_EQ(get_avail_block(stats, count), idxs[i]);
		set_block_stat(idxs[i], 0, stats);
	}
}

TEST_F(GetAvailBlockTest, count_886)
{
	t_uint16	idxs[] = {1, 32, 48, 68, 90, 864};
	t_uint16	count = 886;

	for (t_uint32 i=0; i < sizeof(idxs) / sizeof(t_uint16); i++) {
		set_block_stat(idxs[i], 1, stats);
		ASSERT_EQ(get_avail_block(stats, count), idxs[i]);
		set_block_stat(idxs[i], 0, stats);
	}
}

TEST_F(GetAvailBlockTest, count_128)
{
	t_uint16	idxs[] = {1, 32, 48, 68, 90};
	t_uint16	count = 128;

	for (t_uint32 i=0; i < sizeof(idxs) / sizeof(t_uint16); i++) {
		set_block_stat(idxs[i], 1, stats);
		ASSERT_EQ(get_avail_block(stats, count), idxs[i]);
		set_block_stat(idxs[i], 0, stats);
	}
}

TEST_F(GetAvailBlockTest, multiple_available)
{
	t_uint16	idxs[] = {1, 32, 48, 68, 90, 776, 1023};
	t_uint16	count = 1024;

	for (t_uint32 i=0; i < sizeof(idxs) / sizeof(t_uint16); i++) {
		set_block_stat(idxs[i], 1, stats);
	}

	for (t_uint32 i=0; i < sizeof(idxs) / sizeof(t_uint16); i++) {
		ASSERT_EQ(get_avail_block(stats, count), idxs[i]);
		set_block_stat(idxs[i], 0, stats);
	}
}

TEST_F(GetAvailBlockTest, not_available)
{
	ASSERT_EQ(get_avail_block(stats, 1024), NO_AVAIL_BLOCK);
}
