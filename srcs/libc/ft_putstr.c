#include "malloc.h"

void	ft_putstr(char *s)
{
	if (s == NULL)
		return ;
	while (*s != '\0')
		write(1, s++, 1);
}
