#include <gtest/gtest.h>
#include <stdlib.h>
#include "malloc.h"

static t_uint8	STATS[4][128];
static t_uint16	SIZES[1024];

class AllocBlockTest: public ::testing::Test {
protected:
	t_mem_pool	pool;
	int			sizes[4];
	t_uint16	smallest_block_count;
	t_uint32	smallest_block_size;

	virtual void	SetUp() {
		smallest_block_count = 886;
		smallest_block_size = 32;

		int	i = 0;

		for (int count=smallest_block_count; count >= 100; count >>= 1) {
			int size = count / 8;

			if (count % 8 > 0)
				size++;
			pool.stats[i] = STATS[i];
			memset(pool.stats[i], 0, size);

			sizes[i] = size;
			i++;
		}
		pool.sizes = SIZES;
		memset(pool.sizes, 0, 1024);
		pool.data = (t_uint8 *)0x1234;
	}

	void			test_by_level(t_uint64 size) {
		t_uint8	level = get_block_level(size, smallest_block_size);

		for (t_uint8 l=level; l < 4; l++) {
			for (t_uint16 i=0; i < (smallest_block_count >> l); i++) {
				set_block_stat(i, 1, pool.stats[l]);

				void *mem = alloc_block(size, smallest_block_size, smallest_block_count, &pool);

				// check returned address
				ASSERT_EQ(mem, pool.data + (smallest_block_size << l) * i);

				// check allocated size
				ASSERT_EQ(pool.sizes[i << l], size);

				// check avail block is still available
				ASSERT_EQ(get_block_stat(i, pool.stats[l]), (t_uint32)0);

				// check under level is splited
				if (l == 0)
					continue ;
				for (int j = l-1; j >= (int)level; j--) {
					ASSERT_EQ(get_block_stat((i << (l - j)), pool.stats[j]), (t_uint32)0);
					ASSERT_EQ(get_block_stat((i << (l - j)) + 1, pool.stats[j]), (t_uint32)1);
					set_block_stat((i << (l - j)) + 1, 0, pool.stats[j]);
				}
			}
		}
	}

	void	is_level_all_zero(t_uint8 level) {
		for (int i=0; i < sizes[level]; i++) {
			ASSERT_EQ(pool.stats[level][i], 0);
		}
	}

	void	init_pool() {
		t_uint16	count = smallest_block_count;

		for (t_uint8 level=0; level < 4; level++) {
			if (count % 2 == 1) {
				set_block_stat(count - 1, 1, pool.stats[level]);
			}
			count >>= 1;
		}

		for (t_uint16 i=0; i < smallest_block_count >> 3; i++) {
			set_block_stat(i, 1, pool.stats[3]);
		}
	}
};

TEST_F(AllocBlockTest, less_than_smallest)
{
	test_by_level(smallest_block_size - 10);
}

TEST_F(AllocBlockTest, smallest)
{
	test_by_level(smallest_block_size);
}

TEST_F(AllocBlockTest, less_than_level_1)
{
	test_by_level((smallest_block_size << 1) - 10);
}

TEST_F(AllocBlockTest, level_1)
{
	test_by_level(smallest_block_size << 1);
}

TEST_F(AllocBlockTest, less_than_level_2)
{
	test_by_level((smallest_block_size << 2) - 10);
}

TEST_F(AllocBlockTest, level_2)
{
	test_by_level(smallest_block_size << 2);
}

TEST_F(AllocBlockTest, less_than_level_3)
{
	test_by_level((smallest_block_size << 3) - 10);
}

TEST_F(AllocBlockTest, level_3)
{
	test_by_level(smallest_block_size << 3);
}

TEST_F(AllocBlockTest, no_available_block)
{
	ASSERT_EQ((long)alloc_block(
		10, smallest_block_size, smallest_block_count, &pool), NULL);
}

TEST_F(AllocBlockTest, size_0)
{
	set_block_stat(0, 1, pool.stats[0]);
	ASSERT_EQ((long)alloc_block(
		0, smallest_block_size, smallest_block_count, &pool), NULL);
}

TEST_F(AllocBlockTest, alloc_all_level_0)
{
	init_pool();

	for (t_uint16 i=0; i < smallest_block_count; i++) {
		void *mem = alloc_block(
			smallest_block_size, smallest_block_size,
			smallest_block_count, &pool);

		ASSERT_GE(mem, pool.data);
	}

	for (t_uint16 i=0; i < smallest_block_count; i++) {
		ASSERT_EQ(pool.sizes[i], smallest_block_size);
	}

	for (int i=0; i < 4; i++) {
		is_level_all_zero(i);
	}
}

TEST_F(AllocBlockTest, alloc_all_level_1)
{
	init_pool();

	for (t_uint16 i=0; i < smallest_block_count >> 1; i++) {
		void *mem = alloc_block(
			smallest_block_size << 1, smallest_block_size,
			smallest_block_count, &pool);

		ASSERT_GE(mem, pool.data);
	}

	for (t_uint16 i=0; i < smallest_block_count >> 1; i++) {
		ASSERT_EQ(pool.sizes[i << 1], smallest_block_size << 1);
	}

	for (int i=1; i < 4; i++) {
		is_level_all_zero(i);
	}
}

TEST_F(AllocBlockTest, alloc_all_level_2)
{
	init_pool();

	for (t_uint16 i=0; i < smallest_block_count >> 2; i++) {
		void *mem = alloc_block(
			smallest_block_size << 2, smallest_block_size,
			smallest_block_count, &pool);

		ASSERT_GE(mem, pool.data);
	}

	for (t_uint16 i=0; i < smallest_block_count >> 2; i++) {
		ASSERT_EQ(pool.sizes[i << 2], smallest_block_size << 2);
	}

	for (int i=2; i < 4; i++) {
		is_level_all_zero(i);
	}
}

TEST_F(AllocBlockTest, alloc_all_level_3)
{
	init_pool();

	for (t_uint16 i=0; i < smallest_block_count >> 3; i++) {
		void *mem = alloc_block(
			smallest_block_size << 3, smallest_block_size,
			smallest_block_count, &pool);

		ASSERT_GE(mem, pool.data);
	}

	for (t_uint16 i=0; i < smallest_block_count >> 3; i++) {
		ASSERT_EQ(pool.sizes[i << 3], smallest_block_size << 3);
	}

	for (int i=3; i < 4; i++) {
		is_level_all_zero(i);
	}
}
