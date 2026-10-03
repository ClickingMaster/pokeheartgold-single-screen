#pragma force_active on
#include "global.h"
#include "bg_window.h"
#pragma force_active on
void sub_0200ECBC(BgConfig*,u8,u8,u8,u8,u8,u8,u16);
void sub_0200ECBC(BgConfig *bg,u8 id,u8 x,u8 y,u8 width,u8 height,u8 palette,u16 tile) {
 int top=y-1;
 FillBgTilemapRect(bg,id,tile,x-9,top,1,1,palette);
 FillBgTilemapRect(bg,id,tile+1,x-8,top,1,1,palette);
 FillBgTilemapRect(bg,id,tile+2,x-7,top,width+7,1,palette);
 FillBgTilemapRect(bg,id,tile+3,x+width,top,1,1,palette);
 FillBgTilemapRect(bg,id,tile+4,x+width+1,top,1,1,palette);
 FillBgTilemapRect(bg,id,tile+5,x+width+2,top,1,1,palette);
 FillBgTilemapRect(bg,id,tile+6,x-9,y,1,height,palette);
 FillBgTilemapRect(bg,id,tile+7,x-8,y,1,height,palette);
 FillBgTilemapRect(bg,id,tile+8,x-1,y,1,height,palette);
 FillBgTilemapRect(bg,id,tile+9,x+width,y,1,height,palette);
 FillBgTilemapRect(bg,id,tile+10,x+width+1,y,1,height,palette);
 FillBgTilemapRect(bg,id,tile+11,x+width+2,y,1,height,palette);
 FillBgTilemapRect(bg,id,tile+12,x-9,y+height,1,1,palette);
 FillBgTilemapRect(bg,id,tile+13,x-8,y+height,1,1,palette);
 FillBgTilemapRect(bg,id,tile+14,x-7,y+height,width+7,1,palette);
 FillBgTilemapRect(bg,id,tile+15,x+width,y+height,1,1,palette);
 FillBgTilemapRect(bg,id,tile+16,x+width+1,y+height,1,1,palette);
 FillBgTilemapRect(bg,id,tile+17,x+width+2,y+height,1,1,palette);
}
