#include <gtest/gtest.h>
#include "malloc.h"

extern t_dynamic_mem	g_dym;

class FreeTest: public ::testing::Test {
	virtual void	TearDown() {
		init_dynamic_memory(&g_dym);
	}
};

int		get_pool_count(t_mem_zone *zone)
{
	t_mem_pool	*pool = zone->head;
	int			count = 0;

	while (pool) {
		count++;
		pool = pool->next;
	}
	return count;
}

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

TEST_F(FreeTest, free_tiny_from_0)
{
	static void	*mems[4][1024];
	t_uint32	smallest_block_size = g_dym.tiny_zone.smallest_block_size;
	t_uint32	smallest_block_count = g_dym.tiny_zone.smallest_block_count;

	// malloc
	for (int i=0; i < 4; i++) {
		for (t_uint16 j=0; j < (smallest_block_count >> i); j++) {
			mems[i][j] = (char *)malloc(smallest_block_size << i);

			ASSERT_NE((long)mems[i][j], NULL);
		}
		ASSERT_EQ(get_pool_count(&g_dym.tiny_zone), i + 1);
	}

	// free
	for (int i=0; i < 4; i++) {
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
	for (int i=0; i < 4; i++) {
		for (t_uint16 j=0; j < (smallest_block_count >> i); j++) {
			mems[i][j] = (char *)malloc(smallest_block_size << i);

			ASSERT_NE((long)mems[i][j], NULL);
		}
		ASSERT_EQ(get_pool_count(&g_dym.tiny_zone), i + 1);
	}

	// free
	for (int i=3; i >= 0; i--) {
		for (t_uint16 j=(smallest_block_count >> i) - 1; j >= 0; j--) {
			free(mems[i][j]);
		}
		if (i == 0)
			ASSERT_EQ(get_pool_count(&g_dym.tiny_zone), 1);
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
	for (int i=0; i < 4; i++) {
		for (t_uint16 j=0; j < (smallest_block_count >> i); j++) {
			mems[i][j] = (char *)malloc(smallest_block_size << i);

			ASSERT_NE((long)mems[i][j], NULL);
		}
		ASSERT_EQ(get_pool_count(&g_dym.small_zone), i + 1);
	}

	// free
	for (int i=0; i < 4; i++) {
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
	for (int i=0; i < 4; i++) {
		for (t_uint16 j=0; j < (smallest_block_count >> i); j++) {
			mems[i][j] = (char *)malloc(smallest_block_size << i);

			ASSERT_NE((long)mems[i][j], NULL);
		}
		ASSERT_EQ(get_pool_count(&g_dym.small_zone), i + 1);
	}

	// free
	for (int i=3; i >= 0; i--) {
		for (t_uint16 j=(smallest_block_count >> i) - 1; j >= 0; j--) {
			free(mems[i][j]);
		}
		if (i == 0)
			ASSERT_EQ(get_pool_count(&g_dym.tiny_zone), 1);
		else
			ASSERT_EQ(get_pool_count(&g_dym.tiny_zone), i);
	}
}
