#include <types.h>
#include <string.h>
#include <console.h>

term_write_t *term_write;

void init_console(term_write_t *term_hook)
{
	term_write = term_hook;
}

void printk(char *string)
{
	term_write(string, strlen(string));
}
