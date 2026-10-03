#pragma force_active on
#include "global.h"
typedef struct{u32 a,b,c,d;}Mode;typedef struct{u32 a,b,c,d,e,f,g;}Bg;
typedef struct{u32 heap;u8 pad[0x3c];void*bg;}Work;
extern Mode ov113_021E6B94;extern Bg ov113_021E6C3C,ov113_021E6C74,ov113_021E6BE8,ov113_021E6C04,ov113_021E6C20,ov113_021E6C58;
extern void ov113_021E62E0(Work*),SetBothScreensModesAndDisable(Mode*),InitBgFromTemplate(void*,int,Bg*,int),BgClearTilemapBufferAndCommit(void*,int),BG_ClearCharDataRange(int,int,int,u32);extern void*BgConfig_Alloc(u32);
void ov113_021E6300(Work*);
void ov113_021E6300(Work*w){Mode mode;Bg a,b,c,d,e,f;ov113_021E62E0(w);w->bg=BgConfig_Alloc(w->heap);mode=ov113_021E6B94;SetBothScreensModesAndDisable(&mode);*(volatile u16*)0x04000304&=0xffff7fff;a=ov113_021E6C3C;InitBgFromTemplate(w->bg,5,&a,0);BgClearTilemapBufferAndCommit(w->bg,5);b=ov113_021E6C74;InitBgFromTemplate(w->bg,6,&b,0);BgClearTilemapBufferAndCommit(w->bg,6);c=ov113_021E6BE8;InitBgFromTemplate(w->bg,7,&c,0);BgClearTilemapBufferAndCommit(w->bg,7);d=ov113_021E6C04;InitBgFromTemplate(w->bg,1,&d,0);BgClearTilemapBufferAndCommit(w->bg,1);e=ov113_021E6C20;InitBgFromTemplate(w->bg,2,&e,0);BgClearTilemapBufferAndCommit(w->bg,2);f=ov113_021E6C58;InitBgFromTemplate(w->bg,3,&f,0);BgClearTilemapBufferAndCommit(w->bg,3);BG_ClearCharDataRange(5,32,0,w->heap);BG_ClearCharDataRange(6,32,0,w->heap);BG_ClearCharDataRange(7,32,0,w->heap);BG_ClearCharDataRange(1,32,0,w->heap);BG_ClearCharDataRange(2,32,0,w->heap);BG_ClearCharDataRange(3,32,0,w->heap);}
