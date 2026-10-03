#pragma force_active on
#include "global.h"
extern void BG_LoadCharTilesData(void*,u8,void*,int,int),ScheduleBgTilemapBufferTransfer(void*,u8);extern void*GetBgTilemapBuffer(void*,u8);
void ov51_021E6D44(void*w,int bg,void*data);
void ov51_021E6D44(void*w,int bg,void*data){u16*map;u8 x,y;u16 tile;BG_LoadCharTilesData(w,(u8)bg,data,12288,1);map=GetBgTilemapBuffer(w,(u8)bg);tile=0;y=0;do{x=0;do{tile++;map[(y+12)*32+x+4]=tile;x++;}while(x<24);y++;}while(y<8);ScheduleBgTilemapBufferTransfer(w,(u8)bg);}
