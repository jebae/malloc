#include <gtest/gtest.h>
#include "malloc.h"

extern t_dynamic_mem	g_dym;

class FreeTest: public ::testing::Test {
	virtual void	TearDown() {
		init_dynamic_memory(&g_dym);
	}
};

TEST_F(FreeTest, free_100)
{
	void	*mem = malloc(100);
	void	*ptr = mem;

	free(mem);
	mem = malloc(100);
	ASSERT_EQ(mem, ptr);
}

TEST_F(FreeTest, free_1)
{
	void	*mem = malloc(1);
	void	*ptr = mem;

	free(mem);
	mem = malloc(1);
	ASSERT_EQ(mem, ptr);
}

TEST_F(FreeTest, free_invalid_address)
{
	ASSERT_EQ(g_dym.tiny_zone.head->allocated, 0);
	void	*mem = malloc(100);

	ASSERT_EQ(g_dym.tiny_zone.head->allocated, 1);
	for (t_uint64 i=1 ; i < 100; i++) {
		free((t_uint8 *)mem + i);
		ASSERT_EQ(g_dym.tiny_zone.head->allocated, 1);
	}
}

TEST_F(FreeTest, free_tiny_from_0)
{
	static void	*mems[4][1024];
	t_uint32	smallest_block_size = g_dym.tiny_zone.smallest_block_size;
	t_uint32	smallest_block_count = g_dym.tiny_zone.smallest_block_count;

	// malloc
	for (t_uint32 i=0; i < 4; i++) {
		for (t_uint16 j=0; j < (smallest_block_count >> i); j++) {
			mems[i][j] = (char *)malloc(smallest_block_size << i);

			ASSERT_NE((long)mems[i][j], NULL);
		}
		ASSERT_EQ(get_pool_count(&g_dym.tiny_zone), i + 1);
	}

	// free
	for (t_uint32 i=0; i < 4; i++) {
		for (t_uint16 j=0; j < (smallest_block_count >> i); j++) {
			free(mems[i][j]);
		}
		ASSERT_EQ(get_pool_count(&g_dym.tiny_zone), 4 - i);
	}
}

TEST_F(FreeTest, free_tiny_from_3)
{
	static void	*mems[4][1024];
	t_uint32	smallest_block_size = g_dym.tiny_zone.smallest_block_size;
	t_uint32	smallest_block_count = g_dym.tiny_zone.smallest_block_count;

	// malloc
	for (t_uint32 i=0; i < 4; i++) {
		for (t_uint16 j=0; j < (smallest_block_count >> i); j++) {
			mems[i][j] = (char *)malloc(smallest_block_size << i);

			ASSERT_NE((long)mems[i][j], NULL);
		}
		ASSERT_EQ(get_pool_count(&g_dym.tiny_zone), i + 1);
	}

	// free
	for (t_uint32 i=3; ; i--) {
		for (int j=(smallest_block_count >> i) - 1; j >= 0; j--) {
			free(mems[i][j]);
		}
		if (i == 0) {
			ASSERT_EQ(get_pool_count(&g_dym.tiny_zone), (t_uint32)1);
			return ;
		}
		else
			ASSERT_EQ(get_pool_count(&g_dym.tiny_zone), i);
	}
}

TEST_F(FreeTest, free_small_from_0)
{
	static void	*mems[4][1024];
	t_uint32	smallest_block_size = g_dym.small_zone.smallest_block_size;
	t_uint32	smallest_block_count = g_dym.small_zone.smallest_block_count;

	// malloc
	for (t_uint32 i=0; i < 4; i++) {
		for (t_uint16 j=0; j < (smallest_block_count >> i); j++) {
			mems[i][j] = (char *)malloc(smallest_block_size << i);

			ASSERT_NE((long)mems[i][j], NULL);
		}
		ASSERT_EQ(get_pool_count(&g_dym.small_zone), i + 1);
	}

	// free
	for (t_uint32 i=0; i < 4; i++) {
		for (t_uint16 j=0; j < (smallest_block_count >> i); j++) {
			free(mems[i][j]);
		}
		ASSERT_EQ(get_pool_count(&g_dym.small_zone), 4 - i);
	}
}

TEST_F(FreeTest, free_small_from_3)
{
	static void	*mems[4][1024];
	t_uint32	smallest_block_size = g_dym.small_zone.smallest_block_size;
	t_uint32	smallest_block_count = g_dym.small_zone.smallest_block_count;

	// malloc
	for (t_uint32 i=0; i < 4; i++) {
		for (t_uint16 j=0; j < (smallest_block_count >> i); j++) {
			mems[i][j] = (char *)malloc(smallest_block_size << i);

			ASSERT_NE((long)mems[i][j], NULL);
		}
		ASSERT_EQ(get_pool_count(&g_dym.small_zone), i + 1);
	}

	// free
	for (t_uint32 i=3; i >= 0; i--) {
		for (int j=(smallest_block_count >> i) - 1; j >= 0; j--) {
			free(mems[i][j]);
		}
		if (i == 0) {
			ASSERT_EQ(get_pool_count(&g_dym.small_zone), (t_uint32)1);
			return ;
		}
		else
			ASSERT_EQ(get_pool_count(&g_dym.small_zone), i);
	}
}

TEST_F(FreeTest, free_when_only_one_large)
{
	void	*mem = malloc(getpagesize() + 1);

	free(mem);
	ASSERT_EQ(get_page_count(g_dym.page), (t_uint32)0);
}

TEST_F(FreeTest, free_last_large)
{
	void	*prev = malloc(getpagesize() + 1);
	void	*mem = malloc(getpagesize() + 1);

	ASSERT_EQ(get_page_count(g_dym.page), (t_uint32)2);

	free(mem);
	ASSERT_EQ(get_page_count(g_dym.page), (t_uint32)1);

	free(prev);
	ASSERT_EQ(get_page_count(g_dym.page), (t_uint32)0);
}

TEST_F(FreeTest, free_middle_large)
{
	void	*prev = malloc(getpagesize() + 1);
	void	*mem = malloc(getpagesize() + 1);
	void	*next = malloc(getpagesize() + 1);

	ASSERT_EQ(get_page_count(g_dym.page), (t_uint32)3);

	free(mem);
	ASSERT_EQ(get_page_count(g_dym.page), (t_uint32)2);

	free(prev);
	ASSERT_EQ(get_page_count(g_dym.page), (t_uint32)1);

	free(next);
	ASSERT_EQ(get_page_count(g_dym.page), (t_uint32)0);
}

TEST_F(FreeTest, free_invalid_large_address)
{
	void	*mem = malloc(getpagesize() + 1);

	ASSERT_EQ(get_page_count(g_dym.page), (t_uint32)1);

	free((t_uint8 *)mem + 1);
	ASSERT_EQ(get_page_count(g_dym.page), (t_uint32)1);
}
