#ifndef CONSOLE_H
#define CONSOLE_H

#include <misc/types.h>
#include <boot/stivale2.h>

// printk helpers
#define INFO "INFO: "
#define WARN "WARN: "
#define ERR "ERR: "
#define EMERG "EMERG: "
enum log_level {
	KERN_INFO,
	KERN_WARN,
	KERN_ERR,
	KERN_EMERG,
	KERN_DEBUG
};
#define pr_info(fmt, ...) printk(INFO fmt "\n", ##__VA_ARGS__)
#define pr_warn(fmt, ...) printk(WARN fmt "\n", ##__VA_ARGS__)
#define pr_err(fmt, ...) printk(ERR fmt "\n", ##__VA_ARGS__)
#define pr_emerg(fmt, ...) printk(EMERG fmt "\n", ##__VA_ARGS__)

// Default colors:
#define DEFAULT_FG_COLOR 0xffffff
#define DEFAULT_BG_COLOR 0x000000

void init_console(u64 fb_addr, u16 fb_width, u16 fb_height, u16 fb_pitch);
void putc(char c);
void puts(const char *str); void clear_screen(void);
void printk(const char *fmt, ...);
void klog(enum log_level level, const char *module, const char *fmt, ...);

#endif // CONSOLE_H
