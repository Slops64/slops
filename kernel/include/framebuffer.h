#ifndef FB_H
#define FB_H
#include <types.h>
#include <common.h>
#include <stivale2.h>

struct framebuffer_pixel
{
	u8 blue;
	u8 green;
	u8 red;
	u8 reserved;
} __packed;

void init_fb(struct stivale2_struct_tag_framebuffer *fb_tag);
void set_bg(u8 r, u8 g, u8 b);
void plot_pixel(struct framebuffer_pixel, int x, int y, int color);
void draw_char(const char, int x, int y, u32 fg, u32 bg);

#endif
