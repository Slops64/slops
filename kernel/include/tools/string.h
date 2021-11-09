#ifndef STRING_H
#define STRING_H

#include <misc/types.h>

size_t strlen(char *str);
int strcmp(char *str1, char *str2);
char *strcpy(char *dest, const char *src);
char *strcat(char *dest, const char *src);
void memset(void *dest, u8 val, u32 len);
void memcpy(void *dest, const void *src, u32 len);
void itoa(char *buf, int base, int d);

#endif // STRING_H
