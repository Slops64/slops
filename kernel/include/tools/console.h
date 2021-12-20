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

// VGA IO
#define PORT_VGA_CMD	0x3D4
#define PORT_VGA_DATA	0x3D5
#define CMD_VGA_LOW		15
#define CMD_VGA_HIGH	14

#define VGA_COLOR_BLACK	0
#define VGA_COLOR_WHITE	15
#define COLOR(fg, bg) ((fg << 4) | (bg & 0x0f))
#define DEFAULT_COLOR_SCHEME COLOR(VGA_COLOR_BLACK, VGA_COLOR_WHITE)
#define VGA_CHAR(c) ((DEFAULT_COLOR_SCHEME << 8) | c)

void init_console(struct stivale2_struct_tag_textmode *);
void putc(char c);
void puts(const char *str); void clear_screen(void);
void printk(const char *fmt, ...);
void klog(enum log_level level, const char *module, const char *fmt, ...);

#endif // CONSOLE_H
