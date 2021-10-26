#include <misc/types.h>

size_t strlen(char *str)
{
	size_t len = 0;

	while (*str++)
		len++;

	return len;
}

int strcmp(char *str1, char *str2)
{
	int i = 0;
	int failed = 0;
	while (str1[i] != '\0' && str2[i] != '\0')
	{
		if (str1[i] != str2[i])
		{
			failed = 1;
			break;
		}
		i++;
	}

	if ((str1[i] == '\0' && str2[i] != '\0') || (str1[i] != '\0' && str2[i] == '\0'))
		failed = 1;

	return failed;
}

char *strcpy(char *dest, const char *src)
{
	do
	{
		*dest++ = *src++;
	} while (*src != 0);
	return dest;
}

char *strcat(char *dest, const char *src)
{
	while (*dest != 0)
	{
		dest++;
	}

	do
	{
		*dest++ = *src++;
	} while (*src != 0);
	return dest;
}

void memset(void *dest, u8 val, u32 len)
{
	u8 *temp = (u8 *)dest;
	for (; len != 0; len--)
		*temp++ = val;
}

void memcpy(void *dest, const void *src, u32 len)
{
	const u8 *sp = (const u8 *)src;
	u8 *dp = (u8 *)dest;
	for (; len != 0; len--)
		*dp++ = *sp++;
}

void itoa(char *buf, int base, int d)
{
	char *p = buf;
	char *p1, *p2;
	unsigned long ud = d;
	int divisor = 10;

	if (base == 'd' && d < 0)
	{
		*p++ = '-';
		buf++;
		ud = -d;
	}
	else if (base == 'x')
	{
		divisor = 16;
	}

	do
	{
		int remainder = ud % divisor;

		*p++ =
			(remainder < 10) ? remainder + '0' : remainder + 'A' - 10;
	} while (ud /= divisor);

	*p = 0;

	//Reverse BUF.
	p1 = buf;
	p2 = p - 1;
	while (p1 < p2)
	{
		char tmp = *p1;
		*p1 = *p2;
		*p2 = tmp;
		p1++;
		p2--;
	}
}
