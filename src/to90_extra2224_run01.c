#pragma force_active on
#include "global.h"
#pragma force_active on
#define H(p,n) (*(u16 *)((u8 *)(p)+(n)))
typedef struct{u8 pad[19];u8 low:5,hide:1,high:2;}Header;
extern int ov80_02236DD4(u8);extern void ClearWindowTilemapAndScheduleTransfer(void *),GfGfx_EngineATogglePlanes(int,int),FillWindowPixelBuffer(void *,u8),ScheduleWindowCopyToVram(void *),ov81_0224093C(void *,void *,int,int,int),ov81_02240AD8(void *,void *,int,int,int,int,int,int,u16,u8);
void ov81_02241144(u8 *);
void ov81_02241144(u8 *p){int count=ov80_02236DD4(p[9]);if(((Header *)p)->hide==1){ClearWindowTilemapAndScheduleTransfer(p+0x60);ClearWindowTilemapAndScheduleTransfer(p+0xa0);ClearWindowTilemapAndScheduleTransfer(p+0xb0);GfGfx_EngineATogglePlanes(4,0);return;}if(p[0x463]==1){int i=0;if(count>0){u8 *base=p,*cur=p;base+=0x50;do{void *w=base+(i+5)*16;FillWindowPixelBuffer(w,0);if(i<p[24])ov81_02240AD8(p,w,0,0,15,2,0,0,H(cur,0x45a),(u8)H(cur,0x45e));ScheduleWindowCopyToVram(w);i++;cur+=2;}while(i<count);}ov81_0224093C(p,p+0x60,0,0,0);GfGfx_EngineATogglePlanes(4,1);}p[0x463]=0;}
