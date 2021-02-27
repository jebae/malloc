#include "malloc.h"

extern t_dynamic_mem	g_dym;

static void	put_hex(t_uint64 hex)
{
	static const char *digits = "0123456789ABCDEF";

	if (hex / 16)
		put_hex(hex / 16);
	else
		ft_putstr((char *)"0x");
	ft_putchar(digits[hex % 16]);
}

static void	put_decimal(t_uint64 num)
{
	if (num / 10)
		put_decimal(num / 10);
	ft_putchar('0' + num % 10);
}

static void	show_blocked(t_mem_zone *zone, t_uint64 *total)
{
	t_mem_pool	*pool;
	t_uint8		*ptr;
	int			i;

	pool = zone->head;
	while (pool)
	{
		i = -1;
		while (++i < zone->smallest_block_count)
		{
			if (pool->sizes[i] != 0)
			{
				ptr = pool->data + i * zone->smallest_block_size;
				put_hex((t_uint64)ptr);
				ft_putstr((char *)" ~ ");
				put_hex((t_uint64)(ptr + pool->sizes[i]));
				ft_putstr((char *)" : ");
				put_decimal(pool->sizes[i]);
				ft_putstr((char *)" bytes");
				ft_putchar('\n');
				*total += pool->sizes[i];
			}
		}
		pool = pool->next;
	}
}

static void	show_paged(t_mem_page *page, t_uint64 *total)
{
	t_uint8	*ptr;

	while (page)
	{
		ptr = page->data;
		put_hex((t_uint64)ptr);
		ft_putstr((char *)" ~ ");
		put_hex((t_uint64)(ptr + page->size));
		ft_putstr((char *)" : ");
		put_decimal(page->size);
		ft_putstr((char *)" bytes");
		ft_putchar('\n');
		*total += page->size;
		page = page->next;
	}
}

void	show_alloc_mem(void)
{
	t_uint64	total;

	total = 0;
	ft_putstr((char *)"[ TINY ]\n");
	show_blocked(&g_dym.tiny_zone, &total);
	ft_putstr((char *)"[ SMALL ]\n");
	show_blocked(&g_dym.small_zone, &total);
	ft_putstr((char *)"[ LARGE ]\n");
	show_paged(g_dym.page, &total);
	ft_putstr((char *)"Total : ");
	put_decimal(total);
	ft_putstr((char *)" bytes");
}
