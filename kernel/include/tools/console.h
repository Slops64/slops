#ifndef CONSOLE_H
#define CONSOLE_H

#include <misc/types.h>
#include <boot/stivale2.h>

// printk helpers
#define KERN_INFO "INFO: "
#define KERN_WARN "WARN: "
#define KERN_ERR "ERR: "
#define KERN_EMERG "EMERG: "
#define pr_info(fmt, ...) printk(KERN_INFO fmt "\n", ##__VA_ARGS__)
#define pr_warn(fmt, ...) printk(KERN_WARN fmt "\n", ##__VA_ARGS__)
#define pr_err(fmt, ...) printk(KERN_ERR fmt "\n", ##__VA_ARGS__)
#define pr_emerg(fmt, ...) printk(KERN_EMERG fmt "\n", ##__VA_ARGS__)

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
void puts(char *str);
void clear_screen(void);
void printk(const char *fmt, ...);

#endif // CONSOLE_H
