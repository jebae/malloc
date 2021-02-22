#include <gtest/gtest.h>
#include "malloc.h"

TEST(get_block_level, fit_to_block_size_by_level)
{
	t_uint64	sizes[] = {32, 64, 128, 256, 512};

	for (t_uint32 i=0; i < sizeof(sizes) / sizeof(t_uint64); i++) {
		ASSERT_EQ(get_block_level(sizes[i], 32), i);
	}
}

TEST(get_block_level, less_than_smallest)
{
	ASSERT_EQ(get_block_level(20, 32), 0);
}

TEST(get_block_level, bigger_than_block_size_by_level)
{
	t_uint64	sizes[] = {32, 64, 128, 256, 512};

	for (t_uint32 i=0; i < sizeof(sizes) / sizeof(t_uint64); i++) {
		ASSERT_EQ(get_block_level(sizes[i] + 1, 32), i + 1);
	}
}
