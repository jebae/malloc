#include <gtest/gtest.h>
#include "malloc.h"

TEST(get_zone_type, tiny)
{
	ASSERT_EQ(get_zone_type(getpagesize() / 16), TINY);
	ASSERT_EQ(get_zone_type(getpagesize() / 32), TINY);
	ASSERT_EQ(get_zone_type(getpagesize() / 64), TINY);
	ASSERT_EQ(get_zone_type(getpagesize() / 128), TINY);
	ASSERT_EQ(get_zone_type(getpagesize() / 128 - 1), TINY);
}

TEST(get_zone_type, small)
{
	ASSERT_EQ(get_zone_type(getpagesize()), SMALL);
	ASSERT_EQ(get_zone_type(getpagesize() / 2), SMALL);
	ASSERT_EQ(get_zone_type(getpagesize() / 4), SMALL);
	ASSERT_EQ(get_zone_type(getpagesize() / 8), SMALL);
	ASSERT_EQ(get_zone_type(getpagesize() / 8 - 1), SMALL);
	ASSERT_EQ(get_zone_type(getpagesize() / 16 + 1), SMALL);
}

TEST(get_zone_type, large)
{
	ASSERT_EQ(get_zone_type(getpagesize() + 1), LARGE);
	ASSERT_EQ(get_zone_type(getpagesize() * 2), LARGE);
}
