#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct Tiles{u16 width,height;u8 pad[8];u16 tiles[1];}Tiles;typedef struct Sprite{void*image;u32 flags;}Sprite;typedef struct W{u8 pad[20];void*bg;u8 pad18[0xa4];Tiles*tiles;u8 padc0[5];u8 count;u8 padc6[10];Sprite sprites[64];}W;
extern void BgClearTilemapBufferAndCommit(void*,int),CopyToBgTilemapRect(void*,u8,u8,u8,u8,u8,void*,u8,u8,u8,u8),ov109_021E7178(W*,void*,int,u8,u8);extern const u8 _021E7884[3][4];void ov109_021E6F7C(W*,int,int),ov109_021E6FD4(W*,int,int,int),ov109_021E7030(W*,int,int,int);
void ov109_021E6F7C(W*w,int position,int draw){BgClearTilemapBufferAndCommit(w->bg,2);if(draw)CopyToBgTilemapRect(w->bg,2,(u8)(16*(position/2)),0,16,24,w->tiles->tiles,(u8)(16*(position/2)),0,(u8)(w->tiles->width>>3),(u8)(w->tiles->height>>3));}
