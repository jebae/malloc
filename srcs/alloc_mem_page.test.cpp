#include <gtest/gtest.h>
#include "malloc.h"

static void test_by_size(t_uint64 size) {
	t_mem_page	*page = alloc_mem_page(size);

	ASSERT_EQ((long)page->next, NULL);
	ASSERT_EQ(page->size, size);
	ASSERT_EQ(page->data, (t_uint8 *)page + sizeof(t_mem_page));

	memset(page->data, 0x12, size);
	for (t_uint64 i=0; i < size; i++) {
		ASSERT_EQ(page->data[i], 0x12);
	}
}

TEST(alloc_mem_page, one_page_plus_1)
{
	test_by_size(getpagesize() + 1);
}

TEST(alloc_mem_page, just_two_page)
{
	test_by_size(getpagesize() * 2 - sizeof(t_mem_page));
}

TEST(alloc_mem_page, two_page)
{
	test_by_size(getpagesize() * 2);
}

TEST(alloc_mem_page, more_than_two_page)
{
	test_by_size(getpagesize() * 300);
}
