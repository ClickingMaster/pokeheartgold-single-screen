#pragma force_active on
#include "global.h"
#include <string.h>
#pragma force_active on
typedef struct A{u8 pad[0x29];u8 side;}A;typedef struct Slot{u16 valid;u8 gap[22];}Slot;typedef struct W{A*args;u8 pad[8];void*bg;u8 gap[0x218-16];Slot slots[6];}W;
extern void*GetBgTilemapBuffer(void*,int);void ov05_0221DD08(W*);void ov05_0221DDEC(W*,u16*,int);extern const u8 ov05_0221EA60[6][2];extern void CopyToBgTilemapRect(void*,int,u8,u8,u8,u8,const void*,u8,u8,u8,u8),BgTilemapRectChangePalette(void*,int,u8,u8,u8,u8,u8),BG_LoadPlttData(int,const void*,u32,u32),ScheduleBgTilemapBufferTransfer(void*,int);
void ov05_0221DD08(W*w){u32 i;for(i=0;i<6;i++){void*map;if(i==0||i==3)map=(u8*)w+0x2a4;else if(w->slots[i].valid==0)map=(u8*)w+0x424;else map=(u8*)w+0x364;CopyToBgTilemapRect(w->bg,2,ov05_0221EA60[i][0],ov05_0221EA60[i][1],16,6,map,0,0,16,6);if(w->slots[i].valid){BgTilemapRectChangePalette(w->bg,2,ov05_0221EA60[i][0],ov05_0221EA60[i][1],16,6,(u8)(i+3));if((i<3&&w->args->side==1)||(i>=3&&w->args->side==0))BG_LoadPlttData(2,(u8*)w+0x504,32,(u16)((i+3)*32));else BG_LoadPlttData(2,(u8*)w+0x4e4,32,(u16)((i+3)*32));}}ScheduleBgTilemapBufferTransfer(w->bg,2);}
void ov05_0221DDEC(W*w,u16*out,int bg){u16*tiles=GetBgTilemapBuffer(w->bg,bg);int x;if(w->args->side==0)x=16;else x=0;u16 i;for(i=0;i<24;i++){int offset=x+(i<<5);memcpy(out+16*i,tiles+offset,32);memset(tiles+offset,0,32);}}
