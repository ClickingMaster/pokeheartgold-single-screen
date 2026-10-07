#include "global.h"
#pragma force_active on
#include "bg_window.h"

typedef struct{u8 prefix[0x340];BgConfig*bg;u8 gap[0x190];NNSG2dScreenData*tilemap;u8 gap2[0x184e2-0x4d8];u8 flag0:1,bit1:1,flagB:1,mode:5;u8 active;s16 x,y;u32 tailState;} TileWork;
void ov108_021E79A8(void*,u8,int,int);
void ov108_021E79A8(void*w,u8 layer,int index,int show){TileWork*work=w;struct{u8 x,y;}selection;if(!show){BgClearTilemapBufferAndCommit(work->bg,layer);ScheduleBgTilemapBufferTransfer(work->bg,layer);return;}{NNSG2dScreenData*t=work->tilemap;CopyToBgTilemapRect(work->bg,layer,0,(u8)(24*work->mode),32,24,t->rawData,0,0,t->screenWidth>>3,t->screenHeight>>3);}if(work->mode==0){selection.x=(u8)((u32)(index%3)*9+3);selection.y=(u8)((u32)(index/3)*9+3);if(layer==4){NNSG2dScreenData*t=work->tilemap;CopyToBgTilemapRect(work->bg,layer,0,21,32,3,t->rawData,0,24,t->screenWidth>>3,t->screenHeight>>3);}}else{selection.x=(u8)((u32)(index%3)*10+2);selection.y=(u8)((u32)(index/3)*9+27);}FillBgTilemapRect(work->bg,layer,0,selection.x,selection.y,8,8,17);ScheduleBgTilemapBufferTransfer(work->bg,layer);}
