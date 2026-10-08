#pragma force_active on
#include "global.h"
#include "bg_window.h"
typedef struct{u16 width,height;u8 pad[8];}Image;
typedef struct{u8 pad[0x14];BgConfig*bg;u8 q[0xbc-0x18];Image*image;u8 r[5];u8 count;u8 z[0xd0-0xc6];void*items[24];}State;
extern u8 _021E7884[];
void ov109_021E6FD4(State*,int,int,int);void ov109_021E7030(State*,int,int,int);
// @fn ov109_021E6FD4
void ov109_021E6FD4(State *s, int bg, int mode, int index) {
    Image *image;
    u8 x;
    int row;
    if (mode == 1) {
        x = 3;
        row = (u8)(index * 2 + 1);
    } else {
        x = 28;
        row = (u8)(index * 2 + 2);
    }
    image = s->image;
    CopyToBgTilemapRect(s->bg, (u8)bg, x, 15, 1, 2, (u8 *)image + 12, (u8)row, 24, image->width >> 3, image->height >> 3);
}
