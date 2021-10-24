#ifndef FB_H
#define FB_H
#include <types.h>
#include <stivale2.h>

struct framebuffer_pixel
{
    uint8_t blue;
    uint8_t green;
    uint8_t red;
    uint8_t __unused;
}__attribute__((packed));


void init_fb(struct stivale2_struct_tag_framebuffer *fb_tag);
void set_bg(u8 r, u8 g, u8 b);
void plot_pixel(struct framebuffer_pixel, int x, int y, int color);
void draw_char(const char, int x, int y, u32 fg, u32 bg);

#endif
