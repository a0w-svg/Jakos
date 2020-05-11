#include "screen.h"

struct Cursor{
    uint8 x;
    uint8 y;
}__attribute__((packed));
uint16 *vram = (uint16*)0xB8000;
// move cursor
struct Cursor cursor;

static void mv_cursor()
{
   
    uint16 cursor_location = cursor.y * 80 + cursor.x;
    out_byte(0x3D4, 0x0F);
    out_byte(0x3D5, (uint8) (cursor_location & 0xFF));
    out_byte(0x3D4, 0x0E);
    out_byte(0x3D5, (uint8) ((cursor_location >> 8) & 0xFF));
}

// scroll text up by one line
static void scroll_text()
{
    uint8 attrib_byte = (0 << 4) | (15 & 0x0F);
    uint8 blank = 0x20 | (attrib_byte << 8);

    if(cursor.y >=25)
    {
        int i;
        for(i = 0*80; i< 24*80; i++)
        {
            vram[i] = vram[i+80];
        }
        for(int i = 24*80; i < 25*80; i++)
        {
            vram[i] = blank;
        }
        cursor.y = 24;
    }
}

void screen_put(char c)
{
    uint8 back_color = 0;
    uint8 fore_color = 15;
    
    uint8 attrib_byte = (back_color << 4) | (fore_color & 0x0F);
    uint16 attrib = attrib_byte << 8;
    uint16 *location;

    if(c == 0x08 && cursor.x)
    {
        cursor.x--;
    }
    else if (c == 0x09)
    {
        cursor.x = (cursor.x+8) & ~(8-1);
    }
    else if(c == '\r')
    {
        cursor.x = 0;
    }
    else if(c == '\n')
    {
        cursor.x = 0;
        cursor.y++;
    }
    else if(c >= ' ')
    {
        location = vram + (cursor.y*80 + cursor.x);
        *location = c | attrib;
        cursor.x++;
    }
    if(cursor.x >= 80)
    {
        cursor.x = 0;
        cursor.y++;
    }
    scroll_text();
    mv_cursor();
}
/* clear screen to black background */
void screen_clean()
{
    uint8 attrib_byte = (0 << 4) | (15 & 0x0F);
    uint16 blank = 0x20 | (attrib_byte << 8);
    int i;
    for(i = 0; i < 80*25; i++)
    {
        vram[i] = blank;
    }
    cursor.x = 0;
    cursor.y = 0;
    mv_cursor();
}

void screen_write(char *c)
{
    int i = 0;
    while(c[i])
    {
        screen_put(c[i++]);
    }
}
/*
void screen_write_hex(uint32 n)
{

}

void screen_write_dec(uint32 n)
{

}
*/