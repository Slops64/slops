#include <types.h>
#include <string.h>
#include <console.h>
#include <common.h>

term_write_t *term_write;

void init_console(term_write_t *term_hook)
{
	term_write = term_hook;
}

int vsnprintk(char *buffer, u32 buffer_size, const char *fmt, __builtin_va_list vl)
{
	char c;
	char buf[20];
	char *p = NULL;

	u32 buffer_index = 0;

	while ((c = *fmt++) != 0)
	{
		if (c == '%')
		{
			c = *fmt++;
			switch (c)
			{
			case 'p':
			case 'x':
				buf[0] = '0';
				buf[1] = 'x';
				itoa(buf + 2, c, __builtin_va_arg(vl, int));
				p = buf;
				goto string;
				break;
			//case 'l':
			//	*fmt++;
			//	c = 'd';
			//	// fallthrough
			case 'd':
			case 'u':
				itoa(buf, c, __builtin_va_arg(vl, int));
				p = buf;
				goto string;
				break;

			case 's':
				p = __builtin_va_arg(vl, char *);
				if (!p)
					p = "(null)";

			string:
				while (*p)
				{
					buffer[buffer_index++] = (*p++);
				}
				break;

			default:
				buffer[buffer_index++] =
					__builtin_va_arg(vl, int);
				break;
			}
		}
		else
		{
			buffer[buffer_index++] = c;
		}

		if (buffer_index >= buffer_size - 1)
		{
			break;
		}
	}

	buffer[buffer_index] = '\0';

	return buffer_index;
}

void printk(const char *fmt, ...)
{
	char buffer[1024];

	__builtin_va_list vl;
	__builtin_va_start(vl, fmt);

	vsnprintk(buffer, sizeof(buffer), fmt, vl);

	__builtin_va_end(vl);

	term_write(buffer, strlen(buffer));
}
