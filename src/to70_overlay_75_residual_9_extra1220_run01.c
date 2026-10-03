#pragma force_active on
#include "global.h"
#include "bg_window.h"
#pragma force_active on
typedef struct {u32 pad;BgConfig *bg;u8 p[0xac-8];int scroll,selection,total,result,frame,blink;} Context;
void ov75_02247D24(Context *c);
void ov75_02247D24(Context *c){
 c->frame++;
 if(c->frame!=8)return;
 c->blink ^=1;c->frame=0;
 if(c->scroll+6!=c->total){
 FillBgTilemapRect(c->bg,3,(u16)(c->blink*20+1),14,17,1,1,9);
 FillBgTilemapRect(c->bg,3,(u16)(c->blink*20+2),15,17,1,1,9);
 FillBgTilemapRect(c->bg,3,(u16)(c->blink*20+3),16,17,1,1,9);
 FillBgTilemapRect(c->bg,3,(u16)(c->blink*20+4),17,17,1,1,9);
 FillBgTilemapRect(c->bg,3,(u16)(c->blink*20+11),14,18,1,1,9);
 FillBgTilemapRect(c->bg,3,(u16)(c->blink*20+12),15,18,1,1,9);
 FillBgTilemapRect(c->bg,3,(u16)(c->blink*20+13),16,18,1,1,9);
 FillBgTilemapRect(c->bg,3,(u16)(c->blink*20+14),17,18,1,1,9);
 }else FillBgTilemapRect(c->bg,3,0,14,17,4,2,9);
 if(c->scroll!=0){
 FillBgTilemapRect(c->bg,3,(u16)(c->blink*20+5),14,3,1,1,9);
 FillBgTilemapRect(c->bg,3,(u16)(c->blink*20+6),15,3,1,1,9);
 FillBgTilemapRect(c->bg,3,(u16)(c->blink*20+7),16,3,1,1,9);
 FillBgTilemapRect(c->bg,3,(u16)(c->blink*20+8),17,3,1,1,9);
 FillBgTilemapRect(c->bg,3,(u16)(c->blink*20+15),14,4,1,1,9);
 FillBgTilemapRect(c->bg,3,(u16)(c->blink*20+16),15,4,1,1,9);
 FillBgTilemapRect(c->bg,3,(u16)(c->blink*20+17),16,4,1,1,9);
 FillBgTilemapRect(c->bg,3,(u16)(c->blink*20+18),17,4,1,1,9);
 }else FillBgTilemapRect(c->bg,3,0,14,3,4,2,9);
 ScheduleBgTilemapBufferTransfer(c->bg,3);
}
