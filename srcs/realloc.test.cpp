#include <gtest/gtest.h>
#include "malloc.h"

extern t_dynamic_mem	g_dym;

class ReallocTest: public ::testing::Test {
	virtual void	TearDown() {
		init_dynamic_memory(&g_dym);
	}
};

TEST_F(ReallocTest, tiny_expected_same_address)
{
	char		*origin;
	char		*realloced;
	t_uint32	size;

	size = g_dym.tiny_zone.smallest_block_size;

	origin = (char *)malloc(size - 10);
	realloced = (char *)realloc(origin, size);
	ASSERT_EQ(origin, realloced);

	origin = (char *)malloc((size << 1) - 10);
	realloced = (char *)realloc(origin, size << 1);
	ASSERT_EQ(origin, realloced);

	origin = (char *)malloc((size << 2) - 10);
	realloced = (char *)realloc(origin, size << 2);
	ASSERT_EQ(origin, realloced);

	origin = (char *)malloc((size << 3) - 10);
	realloced = (char *)realloc(origin, size << 3);
	ASSERT_EQ(origin, realloced);
}

TEST_F(ReallocTest, tiny_expected_different_address)
{
	char		*origin;
	char		*realloced;
	t_uint32	size;

	size = g_dym.tiny_zone.smallest_block_size;

	origin = (char *)malloc(size - 10);
	strcpy(origin, "hello world");
	realloced = (char *)realloc(origin, size + 10);
	ASSERT_NE(origin, realloced);
	ASSERT_STREQ(origin, realloced);

	origin = (char *)malloc((size << 1) - 10);
	strcpy(origin, "bye a bientot");
	realloced = (char *)realloc(origin, (size << 1) + 10);
	ASSERT_NE(origin, realloced);
	ASSERT_STREQ(origin, realloced);

	origin = (char *)malloc((size << 2) - 10);
	strcpy(origin, "go rust js");
	realloced = (char *)realloc(origin, (size << 2) + 10);
	ASSERT_NE(origin, realloced);
	ASSERT_STREQ(origin, realloced);

	origin = (char *)malloc((size << 3) - 10);
	strcpy(origin, "qwer wasf wer");
	realloced = (char *)realloc(origin, (size << 3) + 10);
	ASSERT_NE(origin, realloced);
	ASSERT_STREQ(origin, realloced);
}

TEST_F(ReallocTest, small_expected_same_address)
{
	char		*origin;
	char		*realloced;
	t_uint32	size;

	size = g_dym.small_zone.smallest_block_size;

	origin = (char *)malloc(size - 10);
	realloced = (char *)realloc(origin, size);
	ASSERT_EQ(origin, realloced);

	origin = (char *)malloc((size << 1) - 10);
	realloced = (char *)realloc(origin, size << 1);
	ASSERT_EQ(origin, realloced);

	origin = (char *)malloc((size << 2) - 10);
	realloced = (char *)realloc(origin, size << 2);
	ASSERT_EQ(origin, realloced);

	origin = (char *)malloc((size << 3) - 10);
	realloced = (char *)realloc(origin, size << 3);
	ASSERT_EQ(origin, realloced);
}

TEST_F(ReallocTest, small_expected_different_address)
{
	char		*origin;
	char		*realloced;
	t_uint32	size;

	size = g_dym.small_zone.smallest_block_size;

	origin = (char *)malloc(size - 10);
	strcpy(origin, "hello world");
	realloced = (char *)realloc(origin, size + 10);
	ASSERT_NE(origin, realloced);
	ASSERT_STREQ(origin, realloced);

	origin = (char *)malloc((size << 1) - 10);
	strcpy(origin, "bye a bientot");
	realloced = (char *)realloc(origin, (size << 1) + 10);
	ASSERT_NE(origin, realloced);
	ASSERT_STREQ(origin, realloced);

	origin = (char *)malloc((size << 2) - 10);
	strcpy(origin, "hogwart");
	realloced = (char *)realloc(origin, (size << 2) + 10);
	ASSERT_NE(origin, realloced);
	ASSERT_STREQ(origin, realloced);

	origin = (char *)malloc((size << 3) - 10);
	strcpy(origin, "hogwart");
	realloced = (char *)realloc(origin, (size << 3) + 10);
	ASSERT_NE(origin, realloced);
	ASSERT_STREQ(origin, realloced);
}

TEST_F(ReallocTest, large_expected_same_address)
{
	char		*origin;
	char		*realloced;

	origin = (char *)malloc(getpagesize() + 1);
	realloced = (char *)realloc(origin, getpagesize() * 2 - sizeof(t_mem_page));
	ASSERT_EQ(origin, realloced);
}

TEST_F(ReallocTest, large_expected_different_address)
{
	char		*origin;
	char		*realloced;
	const char	*str = "hogwart";

	origin = (char *)malloc(getpagesize() + 1);
	strcpy(origin, str);
	realloced = (char *)realloc(origin, getpagesize() * 2);
	ASSERT_NE(origin, realloced);
	ASSERT_STREQ(str, realloced);
}

TEST_F(ReallocTest, large_expected_different_address_to_small_zone)
{
	char		*origin;
	char		*realloced;
	const char	*str = "hogwart";

	origin = (char *)malloc(getpagesize() + 1);
	strcpy(origin, str);
	realloced = (char *)realloc(origin, getpagesize());
	ASSERT_NE(origin, realloced);
	ASSERT_STREQ(str, realloced);
	ASSERT_EQ((long)g_dym.page, NULL);
}

TEST_F(ReallocTest, large_expected_different_address_to_tiny_zone)
{
	char		*origin;
	char		*realloced;
	const char	*str = "hogwart";

	origin = (char *)malloc(getpagesize() + 1);
	strcpy(origin, str);
	realloced = (char *)realloc(origin, getpagesize() / 128);
	ASSERT_NE(origin, realloced);
	ASSERT_STREQ(str, realloced);
	ASSERT_EQ((long)g_dym.page, NULL);
}

TEST_F(ReallocTest, realloc_bigger)
{
	char		*origin;
	char		*realloced;

	origin = (char *)malloc(12);
	strcpy(origin, "hello world");
	realloced = (char *)realloc(origin, 100);
	ASSERT_NE(origin, realloced);
	ASSERT_STREQ(origin, realloced);
}

TEST_F(ReallocTest, realloc_smaller)
{
	char		*origin;
	char		*realloced;

	origin = (char *)malloc(100);
	strcpy(origin, "hello world");
	realloced = (char *)realloc(origin, 5);
	ASSERT_NE(origin, realloced);
	ASSERT_EQ(memcmp(realloced, "hello", 5), 0);
}

TEST_F(ReallocTest, not_allocated_address)
{
	char *ptr = (char *)realloc(
		(void *)g_dym.tiny_zone.head->data, 10);

	ASSERT_EQ((long)ptr, 0x0);
}

TEST_F(ReallocTest, invalid_address)
{
	char		*origin;
	char		*realloced;

	origin = (char *)malloc(100);
	realloced = (char *)realloc(origin + 1, 10);
	ASSERT_EQ((long)realloced, 0x0);
}

TEST_F(ReallocTest, size_0)
{
	char		*origin;
	char		*realloced;

	origin = (char *)malloc(100);
	realloced = (char *)realloc(origin, 0);

	ASSERT_EQ((long)realloced, 0x0);
	ASSERT_EQ(get_block_size(
		origin, g_dym.tiny_zone.smallest_block_size, g_dym.tiny_zone.head),
		0);
}

TEST_F(ReallocTest, null_ptr)
{
	char *realloced = (char *)realloc(NULL, 100);

	ASSERT_NE((long)realloced, 0x0);
	ASSERT_EQ(get_block_size(
		realloced, g_dym.tiny_zone.smallest_block_size, g_dym.tiny_zone.head),
		100);
}
