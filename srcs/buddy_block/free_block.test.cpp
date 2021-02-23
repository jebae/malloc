#include <gtest/gtest.h>
#include "malloc.h"

class FreeBlockTest: public ::testing::Test {
protected:
	t_mem_pool	pool;
	t_uint16	smallest_block_count;
	t_uint32	smallest_block_size;
	int			sizes[4];

	virtual void	SetUp() {
		smallest_block_count = 886;
		smallest_block_size = 32;

		int	i = 0;

		for (int count=smallest_block_count; count >= 100; count >>= 1) {
			int size = count / 8;

			if (count % 8 > 0)
				size++;
			pool.stats[i] = (t_uint8 *)malloc(size);
			memset(pool.stats[i], 0, size);

			sizes[i] = size;
			i++;
		}
		pool.sizes = (t_uint16 *)malloc(sizeof(t_uint16) * smallest_block_count);
		memset(pool.sizes, 0, sizeof(t_uint16) * smallest_block_count);
		pool.data = (t_uint8 *)0x1234;
	}

	virtual	void	TearDown() {
		for (int i=0; i < 4; i++) {
			free(pool.stats[i]);
		}
		free(pool.sizes);
	}

	void	is_level_all_zero(t_uint8 level) {
		for (int i=0; i < sizes[level]; i++) {
			ASSERT_EQ(pool.stats[level][i], 0);
		}
	}

	void	test_by_level(t_uint8 level) {
		t_uint16	count = smallest_block_count >> level;

		for (t_uint16 i=0; i < count; i++) {
			pool.sizes[i << level] = smallest_block_size << level;
		}

		for (t_uint16 i=0; i < count; i++) {
			free_block(pool.data + (smallest_block_size << level) * i,
				smallest_block_size, &pool);
		}

		for (t_uint8 l=level; l < 3; l++) {
			count = smallest_block_count >> l;

			if (count % 2 == 1) {
				t_uint16 avail = get_avail_block(pool.stats[l], count);

				ASSERT_EQ(avail, count - 1);
			} else {
				is_level_all_zero(l);
			}
		}

		for (t_uint16 i=0; i < smallest_block_count >> 3; i++) {
			ASSERT_EQ(get_block_stat(i, pool.stats[3]), (t_uint32)1);
		}
	}
};

TEST_F(FreeBlockTest, level_0)
{
	test_by_level(0);
}

TEST_F(FreeBlockTest, level_1)
{
	test_by_level(1);
}

TEST_F(FreeBlockTest, level_2)
{
	test_by_level(2);
}

TEST_F(FreeBlockTest, level_3)
{
	test_by_level(3);
}

TEST_F(FreeBlockTest, non_allocated)
{
	// not expected seg fault
	free_block(pool.data, smallest_block_size, &pool);
	free_block(pool.data + smallest_block_size,
		smallest_block_size, &pool);
}
