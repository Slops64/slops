#ifndef CONSOLE_H
#define CONSOLE_H

#include <types.h>

typedef void term_write_t(const char *string, size_t length);
void init_console(term_write_t *term_hook);
void printk(char *string);

#endif // CONSOLE_H
