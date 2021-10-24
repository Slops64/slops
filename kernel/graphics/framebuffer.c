#include <framebuffer.h>
#include <common.h>
#include <types.h>

struct framebuffer_pixel *fb;
u16 fb_height;
u16 fb_width;
u16 fb_pitch; // Pitch or "scanline"

void init_fb(struct stivale2_struct_tag_framebuffer *fb_tag)
{
	fb = (void *)fb_tag->framebuffer_addr;
	fb_height = fb_tag->framebuffer_height;
	fb_width = fb_tag->framebuffer_width;
	fb_pitch = fb_tag->framebuffer_pitch;
}

void set_bg(u8 r, u8 g, u8 b)
{
	for (size_t x = 0; x < fb_width; x++)
	{
		for (size_t y = 0; y < fb_height; y++)
		{
			size_t raw_position = x + y * fb_width;
			fb[raw_position].red = r;
			fb[raw_position].green = g;
			fb[raw_position].blue = b;
		}
	}
}

// TODO: Write these functions
// void plot_pixel(struct framebuffer_pixel, int x, int y, int color)
//{

//}

// void draw_char(const char, int x, int y, u32 fg, u32 bg)
//{

//}
