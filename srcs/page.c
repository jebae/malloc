#include "malloc.h"

t_uint64	get_adjusted_page_size(t_uint64 size)
{
	return (size + sizeof(t_mem_page));
}

t_uint32	get_required_page_count(t_uint64 size)
{
	t_uint32		page_count;

	page_count = size / getpagesize();
	if (size % getpagesize() > 0)
		page_count++;
	return (page_count);
}

t_mem_page	*alloc_mem_page(t_uint64 size)
{
	t_mem_page		*page;
	t_uint32		page_count;
	t_uint64		size_adjusted;

	size_adjusted = get_adjusted_page_size(size);
	page_count = get_required_page_count(size_adjusted);
	page = (t_mem_page *)mmap(0, page_count * getpagesize(),
		PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANON, -1, 0);
	if (page == MAP_FAILED)
		return (0x0);
	page->next = 0x0;
	page->size = size;
	page->data = (t_uint8 *)page + sizeof(t_mem_page);
	return (page);
}

t_mem_page	*find_mem_page(void *ptr, t_mem_page *page)
{
	while (page)
	{
		if (ptr == page->data)
			return (page);
		page = page->next;
	}
	return (0x0);
}
