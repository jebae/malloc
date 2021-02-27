#include "malloc.h"

static void	copy_by_1byte(
		unsigned char *dst, unsigned char *src, size_t len)
{
	size_t	i;

	i = 0;
	while (i < len)
	{
		dst[i] = src[i];
		i++;
	}
}

void	*ft_memcpy(void *dst, const void *src, size_t n)
{
	unsigned long	*p_dst;
	unsigned long	*p_src;
	size_t			quotient;
	size_t			i;

	if (dst == NULL || src == NULL)
		return (dst);
	p_dst = (unsigned long *)dst;
	p_src = (unsigned long *)src;
	quotient = n / sizeof(unsigned long);
	n %= sizeof(unsigned long);
	if (quotient > 0)
	{
		i = 0;
		while (i < quotient)
		{
			p_dst[i] = p_src[i];
			i++;
		}
	}
	copy_by_1byte((unsigned char *)(p_dst + quotient),
		(unsigned char *)(p_src + quotient), n);
	return (dst);
}
