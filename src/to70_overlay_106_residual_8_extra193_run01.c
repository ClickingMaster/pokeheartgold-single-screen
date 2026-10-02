#pragma force_active on
#include "global.h"
#pragma force_active on
void Main_SetVBlankIntrCB(void (*)(void *),void *);
void HBlankInterruptDisable(void),GfGfx_EngineASetPlanes(int),GfGfx_EngineBSetPlanes(int);
void *Heap_Alloc(int,u32),*NARC_New(int,int);
void ov106_021E70E0(void *),ov106_021E6408(void),ov106_021E6418(void *),ov106_021E6520(void *),ov106_021E63E0(void *);
int ov106_021E6814(u8 *);
#define P(p,o) (*(void **)((p)+(o)))
int ov106_021E6814(u8 *w){
 u8 *data;
 Main_SetVBlankIntrCB(NULL,NULL);HBlankInterruptDisable();GfGfx_EngineASetPlanes(0);GfGfx_EngineBSetPlanes(0);
 *(volatile u16 *)0x04000050=0;*(volatile u16 *)0x04001050=0;*(volatile u16 *)0x04000304|=0x8000;
 P(w,0x418)=Heap_Alloc(153,0x6040);MI_CpuFill8(P(w,0x418),0,0x6040);
 data=P(w,0x418);*(void (**)(void *))(data+20)=ov106_021E70E0;P(data,4)=NARC_New(247,153);
 ov106_021E6408();ov106_021E6418(data);ov106_021E6520(w);
 Main_SetVBlankIntrCB(ov106_021E63E0,P(w,0x418));return 4;
}
