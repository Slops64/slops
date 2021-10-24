#include <types.h>
#include <string.h>
#include <console.h>
#include <common.h>
#include <stivale2.h>

static u16 width;
static u16 height;
static u16 x;
static u16 y;
static u16 byte_per_char;
static u16 *vidmem;

void init_console(struct stivale2_struct_tag_textmode *console)
{
	width = console->cols;
	height = console->rows;
	byte_per_char = console->bytes_per_char;
	vidmem = (u16 *)console->address;
}

int vsnprintk(char *buffer, u32 buffer_size, const char *fmt, __builtin_va_list vl)
{
	char c;
	char buf[20];
	char *p = NULL;

	u32 buffer_index = 0;

	while ((c = *fmt++) != 0)
	{
		if (c == '%')
		{
			c = *fmt++;
			switch (c)
			{
			case 'p':
			case 'x':
				buf[0] = '0';
				buf[1] = 'x';
				itoa(buf + 2, c, __builtin_va_arg(vl, int));
				p = buf;
				goto string;
				break;
			// case 'l':
			//	*fmt++;
			//	c = 'd';
			//	// fallthrough
			case 'd':
			case 'u':
				itoa(buf, c, __builtin_va_arg(vl, int));
				p = buf;
				goto string;
				break;

			case 's':
				p = __builtin_va_arg(vl, char *);
				if (!p)
					p = "(null)";

			string:
				while (*p)
				{
					buffer[buffer_index++] = (*p++);
				}
				break;

			default:
				buffer[buffer_index++] =
					__builtin_va_arg(vl, int);
				break;
			}
		}
		else
		{
			buffer[buffer_index++] = c;
		}

		if (buffer_index >= buffer_size - 1)
		{
			break;
		}
	}

	buffer[buffer_index] = '\0';

	return buffer_index;
}

void printk(const char *fmt, ...)
{
	char buffer[1024];

	__builtin_va_list vl;
	__builtin_va_start(vl, fmt);

	vsnprintk(buffer, sizeof(buffer), fmt, vl);

	__builtin_va_end(vl);

	puts(buffer);
}

void puts(char *str)
{
	if (!vidmem)
		return;

	while (*str)
		putc(*str++);
}

// Updates the hardware cursor.
static void update_cursor(void)
{
	// The screen is width characters wide...
	u16 cursor = y * width + x;
	outb(PORT_VGA_CMD, CMD_VGA_HIGH); // Tell the VGA board we are setting the high cursor byte.
	outb(PORT_VGA_DATA, cursor >> 8); // Send the high cursor byte.
	outb(PORT_VGA_CMD, CMD_VGA_LOW);  // Tell the VGA board we are setting the low cursor byte.
	outb(PORT_VGA_DATA, cursor);	  // Send the low cursor byte.
}

// Scrolls the text on the screen up by one line.
static void scroll(void)
{
	// Get a space character with the default colour attributes.
	u8 attributeByte = (0 /*black */ << 4) | (15 /*white */ & 0x0F);
	u16 blank = 0x20 /* space */ | (attributeByte << 8);

	// Row height is the end, this means we need to scroll up
	if (y >= height)
	{
		// Move the current text chunk that makes up the screen
		// back in the buffer by a line
		int i;
		for (i = 0 * width; i < 24 * width; i++)
		{
			vidmem[i] = vidmem[i + width];
		}

		// The last line should now be blank. Do this by writing
		// width spaces to it.
		for (i = 24 * width; i < height * width; i++)
		{
			vidmem[i] = blank;
		}
		// The cursor should now be on the last line.
		y = 24;
	}
}

// Writes a single character out to the screen.
void putc(char c)
{
	switch (c)
	{
	// Handle a backspace, by moving the cursor back one space
	case '\b':
		if (x)
			x--;
		break;

	// Handle a tab by increasing the cursor's X, but only to a point
	// where it is divisible by 8.
	case '\t':
		x = (x + 8) & ~(8 - 1);
		break;

	// Handle carriage return
	case '\r':
		x = 0;
		break;

	// Handle newline by moving cursor back to left and increasing the row
	case '\n':
		y++;
		x = 0;
		break;

	// Handle any other printable character.
	default:
		if (c >= ' ')
			vidmem[y * width + x++] = VGA_CHAR(c);
		break;
	}

	// Check if we need to insert a new line because we have reached the end
	// of the screen.
	if (x >= width)
	{
		x = 0;
		y++;
	}
	// Scroll the screen if needed.
	scroll();
	// Move the hardware cursor.
	update_cursor();
}

// Clears the screen, by copying lots of spaces to the framebuffer.
void clear_screen(void)
{
	u16 blank = VGA_CHAR(' ');

	int i;
	for (i = 0; i < width * height; i++)
	{
		vidmem[i] = blank;
	}

	// Move the hardware cursor back to the start.
	x = 0;
	y = 0;
	update_cursor();
}
