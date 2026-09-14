#include "libft.h"

static int	ft_error(char c)
{
	char	*formats;

	formats = "csiduxXp%";
	while (*formats)
	{
		if (c == *formats)
			return (0);
		formats++;
	}
	return (1);
}

int	ft_printf(const char *format, ...)
{
	int		print_len;
	va_list	arg_list;

	print_len = 0;
	va_start(arg_list, format);
	if (!format)
		return (-1);
	while (*format)
	{
		if (*format == '%')
		{
			if (ft_error(*(++format)))
			{
				ft_putstr_a("Error: Invalid Format\n");
				return (-2);
			}
			print_len += ft_print_format(arg_list, *format);
		}
		else
			print_len += ft_putchar_a(*format);
		format++;
	}
	va_end(arg_list);
	return (print_len);
}
