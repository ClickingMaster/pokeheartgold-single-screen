#pragma force_active on
#include "global.h"
#pragma force_active on
struct Image{u16 x,y;u8 pad[8];u8 data[1];};struct A{u8 pad[16];struct Image*images[2];u16 enabled,index;};void CopyToBgTilemapRect(void*,u8,u8,u8,u8,u8,const void*,u8,u8,u8,u8);void ScheduleBgTilemapBufferTransfer(void*,int);void ov48_0225AA5C(struct A*,int,void**);
void ov48_0225AA5C(struct A*a,int v,void**bg){if(a->enabled){struct Image*i=a->images[a->index];CopyToBgTilemapRect(*bg,6,0,(u8)v,32,1,i->data,0,(u8)v,i->x>>3,i->y>>3);ScheduleBgTilemapBufferTransfer(*bg,6);}}
