#include <tools/common.h>

void reboot(void)
{
    unsigned char good = 0x02;
    while(good & 0x02)
    {
        good = inb(0x64);
    }
    outb(0x64, 0xFE);
    hlt();
}

void shutdown(void)
{
    outw(0x604, 0x2000);
}
