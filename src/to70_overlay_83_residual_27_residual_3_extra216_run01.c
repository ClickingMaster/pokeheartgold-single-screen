#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct Modes {u32 words[4];} Modes;
typedef struct Template {u32 words[7];} Template;
extern const Modes ov83_02248044;
extern const Template ov83_02248068,ov83_02248084,ov83_022480A0,ov83_022480BC,ov83_022480D8,ov83_022480F4;
void SetBothScreensModesAndDisable(const Modes *);
void InitBgFromTemplate(void *,int,const Template *,int);
void BG_ClearCharDataRange(int,int,int,int);
void BgClearTilemapBufferAndCommit(void *,int);
void GfGfx_EngineATogglePlanes(int,int);
void ov83_022444E0(void *);
void ov83_022444E0(void *w){
 Modes modes;Template t1,t0,t2,t3,t4,t5;
 modes=ov83_02248044;SetBothScreensModesAndDisable(&modes);
 t1=ov83_02248068;InitBgFromTemplate(w,1,&t1,0);BG_ClearCharDataRange(1,32,0,107);BgClearTilemapBufferAndCommit(w,1);
 t0=ov83_02248084;InitBgFromTemplate(w,0,&t0,0);BG_ClearCharDataRange(0,32,0,107);BgClearTilemapBufferAndCommit(w,0);
 t2=ov83_022480A0;InitBgFromTemplate(w,2,&t2,0);BG_ClearCharDataRange(2,32,0,107);BgClearTilemapBufferAndCommit(w,2);
 t3=ov83_022480BC;InitBgFromTemplate(w,3,&t3,0);BG_ClearCharDataRange(3,32,0,107);BgClearTilemapBufferAndCommit(w,3);
 t4=ov83_022480D8;InitBgFromTemplate(w,4,&t4,0);BgClearTilemapBufferAndCommit(w,4);
 t5=ov83_022480F4;InitBgFromTemplate(w,5,&t5,0);BG_ClearCharDataRange(5,32,0,107);BgClearTilemapBufferAndCommit(w,5);
 *(volatile u16 *)0x04000008 = (*(volatile u16 *)0x04000008 & ~3) | 0;GfGfx_EngineATogglePlanes(2,1);
}
