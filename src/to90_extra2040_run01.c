#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct {u32 x[10];} Banks;
typedef struct {u32 x[4];} Modes;
typedef struct {u32 x[7];} Bg;
typedef struct {Bg a,b,c,d;} Four;
typedef struct {Bg a,b;} Two;
extern const Banks ov75_022499FC;
extern const Modes ov75_0224997C;
extern const Bg ov75_02249A5C[4],ov75_02249A24[2];
void GfGfx_DisableEngineAPlanes(void),GfGfx_DisableEngineBPlanes(void),GfGfx_SetBanks(const Banks*),SetBothScreensModesAndDisable(const Modes*);
void InitBgFromTemplate(void*,int,const Bg*,int),BgClearTilemapBufferAndCommit(void*,int),BgSetPosTextAndCommit(void*,int,int,int),BG_ClearCharDataRange(int,int,int,int);
void ov75_0224725C(void*);
void ov75_0224725C(void *w) {
 GfGfx_DisableEngineAPlanes();GfGfx_DisableEngineBPlanes();
 {Banks b=ov75_022499FC;GfGfx_SetBanks(&b);}
 MIi_CpuClear32(0,(void*)0x06000000,0x80000);MIi_CpuClear32(0,(void*)0x06200000,0x20000);MIi_CpuClear32(0,(void*)0x06400000,0x40000);MIi_CpuClear32(0,(void*)0x06600000,0x20000);
 {Modes m=ov75_0224997C;SetBothScreensModesAndDisable(&m);}
 {Four b=*(const Four*)ov75_02249A5C;
InitBgFromTemplate(w,0,&b.a,0);BgClearTilemapBufferAndCommit(w,0);BgSetPosTextAndCommit(w,0,0,0);BgSetPosTextAndCommit(w,0,3,0);
InitBgFromTemplate(w,1,&b.b,0);BgClearTilemapBufferAndCommit(w,1);BgSetPosTextAndCommit(w,1,0,0);BgSetPosTextAndCommit(w,1,3,0);
InitBgFromTemplate(w,2,&b.c,0);BgClearTilemapBufferAndCommit(w,2);BgSetPosTextAndCommit(w,2,0,0);BgSetPosTextAndCommit(w,2,3,0);
InitBgFromTemplate(w,3,&b.d,0);BgClearTilemapBufferAndCommit(w,3);BgSetPosTextAndCommit(w,3,0,0);BgSetPosTextAndCommit(w,3,3,0);
}
 {Two b=*(const Two*)ov75_02249A24;
InitBgFromTemplate(w,4,&b.a,0);BgClearTilemapBufferAndCommit(w,4);BgSetPosTextAndCommit(w,4,0,0);BgSetPosTextAndCommit(w,4,3,0);
InitBgFromTemplate(w,5,&b.b,0);BgClearTilemapBufferAndCommit(w,5);BgSetPosTextAndCommit(w,5,0,0);BgSetPosTextAndCommit(w,5,3,0);
}
 BG_ClearCharDataRange(0,32,0,116);BG_ClearCharDataRange(4,32,0,116);
}
