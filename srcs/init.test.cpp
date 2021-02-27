#include <gtest/gtest.h>
#include "malloc.h"

TEST(init_dynamic_memory, _)
{
	t_dynamic_mem	mem;
	t_uint32		page_size = (t_uint32)getpagesize();

	init_dynamic_memory(&mem);

	// tiny
	ASSERT_NE((long)mem.tiny_zone.head, NULL);
	ASSERT_EQ(mem.tiny_zone.head, mem.tiny_zone.tail);
	ASSERT_EQ(mem.tiny_zone.smallest_block_size, page_size / 128);
	ASSERT_EQ(mem.tiny_zone.smallest_block_count, 950);
	ASSERT_EQ(mem.tiny_zone.pool_size, page_size / 128 * 950);
	ASSERT_EQ(mem.tiny_zone.mem_size, page_size * 8);

	ASSERT_LT(mem.tiny_zone.head, mem.small_zone.head);

	// small
	ASSERT_NE((long)mem.small_zone.head, NULL);
	ASSERT_EQ(mem.small_zone.head, mem.small_zone.tail);
	ASSERT_EQ(mem.small_zone.smallest_block_size, page_size / 8);
	ASSERT_EQ(mem.small_zone.smallest_block_count, 1019);
	ASSERT_EQ(mem.small_zone.pool_size, page_size / 8 * 1019);
	ASSERT_EQ(mem.small_zone.mem_size, page_size * 128);

	// large
	ASSERT_EQ((long)mem.page, NULL);
}
