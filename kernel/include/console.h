#ifndef CONSOLE_H
#define CONSOLE_H

#include <types.h>

typedef void term_write_t(const char *string, size_t length);
void init_console(term_write_t *term_hook);
void printk(const char *fmt, ...);

#define PUTV(var) monitor_putv(#var, (u32)var)
#define KERN_INFO "INFO: "
#define KERN_WARN "WARN: "
#define KERN_ERR "ERR: "
#define KERN_EMERG "EMERG: "

#define pr_info(fmt, ...) printk(KERN_INFO fmt "\n", ##__VA_ARGS__)
#define pr_warn(fmt, ...) printk(KERN_WARN fmt "\n", ##__VA_ARGS__)
#define pr_err(fmt, ...) printk(KERN_ERR fmt "\n", ##__VA_ARGS__)
#define pr_emerg(fmt, ...) printk(KERN_EMERG fmt "\n", ##__VA_ARGS__)

#endif // CONSOLE_H
