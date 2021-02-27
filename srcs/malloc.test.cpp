#include <gtest/gtest.h>
#include <string.h>
#include "malloc.h"

extern t_dynamic_mem g_dym;

class MallocTest: public ::testing::Test {
	virtual void	TearDown() {
		init_dynamic_memory(&g_dym);
	}
};

TEST_F(MallocTest, allocate_100)
{
	char 		*mem = (char *)malloc(100);
	const char *str = "hello malloc";

	strcpy(mem, str);
	ASSERT_STREQ(mem, str);
}

TEST_F(MallocTest, allocate_1)
{
	char *mem = (char *)malloc(1);

	*mem = 42;
	ASSERT_EQ(*mem, 42);
}

TEST_F(MallocTest, allocate_0)
{
	char *mem = (char *)malloc(0);

	ASSERT_EQ((long)mem, 0x0);
}

TEST_F(MallocTest, allocate_limit)
{
	struct rlimit	rl;

	getrlimit(RLIMIT_DATA, &rl);
	char 	*mem = (char *)malloc(rl.rlim_max + 1);

	ASSERT_EQ((long)mem, 0x0);
}

TEST_F(MallocTest, allocate_tiny)
{
	t_uint32	smallest_block_size = g_dym.tiny_zone.smallest_block_size;
	t_uint32	smallest_block_count = g_dym.tiny_zone.smallest_block_count;

	for (t_uint32 i=0; i < 4; i++) {
		for (t_uint16 j=0; j < (smallest_block_count >> i); j++) {
			unsigned char *mem =
				(unsigned char *)malloc(smallest_block_size << i);

			ASSERT_NE((long)mem, NULL);
			memset(mem, (t_uint8)j, smallest_block_size << i);
			for (t_uint32 k=0; k < (smallest_block_size << i); k++) {
				ASSERT_EQ(mem[k], (t_uint8)j);
			}
		}
		ASSERT_EQ(get_pool_count(&g_dym.tiny_zone), i + 1);
	}
}

TEST_F(MallocTest, allocate_small)
{
	t_uint32	smallest_block_size = g_dym.small_zone.smallest_block_size;
	t_uint32	smallest_block_count = g_dym.small_zone.smallest_block_count;

	for (t_uint32 i=0; i < 4; i++) {
		for (t_uint16 j=0; j < (smallest_block_count >> i); j++) {
			unsigned char *mem =
				(unsigned char *)malloc(smallest_block_size << i);

			ASSERT_NE((long)mem, NULL);
			memset(mem, (t_uint8)j, smallest_block_size << i);
			for (t_uint32 k=0; k < (smallest_block_size << i); k++) {
				ASSERT_EQ(mem[k], (t_uint8)j);
			}
		}
		ASSERT_EQ(get_pool_count(&g_dym.small_zone), i + 1);
	}
}

TEST_F(MallocTest, allocate_large)
{
	char		*ptr;
	t_uint64	size;

	size = getpagesize() + 1;
	ptr = (char *)malloc(size);
	ASSERT_NE((long)ptr, NULL);
	ASSERT_EQ(get_page_count(g_dym.page), (t_uint32)1);

	memset(ptr, 0x12, size);

	// test if page struct is overwritten
	ASSERT_EQ(get_page_count(g_dym.page), (t_uint32)1);
	for (t_uint64 i=0; i < size; i++) {
		ASSERT_EQ(ptr[i], 0x12);
	}

	size = getpagesize() * 2;
	ptr = (char *)malloc(size);
	ASSERT_NE((long)ptr, NULL);
	ASSERT_EQ(get_page_count(g_dym.page), (t_uint32)2);

	memset(ptr, 0x12, size);
	ASSERT_EQ(get_page_count(g_dym.page), (t_uint32)2);
	for (t_uint64 i=0; i < size; i++) {
		ASSERT_EQ(ptr[i], 0x12);
	}

	size = getpagesize() * 300;
	ptr = (char *)malloc(size);
	ASSERT_NE((long)ptr, NULL);
	ASSERT_EQ(get_page_count(g_dym.page), (t_uint32)3);

	memset(ptr, 0x12, size);
	ASSERT_EQ(get_page_count(g_dym.page), (t_uint32)3);
	for (t_uint64 i=0; i < size; i++) {
		ASSERT_EQ(ptr[i], 0x12);
	}
}
