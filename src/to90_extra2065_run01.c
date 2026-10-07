#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct {u32 data[4];} Modes;
typedef struct {u32 data[7];} Template;
extern const Modes ov52_021E94CC;
extern const Template ov52_021E9524,ov52_021E9540,ov52_021E955C,ov52_021E94EC,ov52_021E9508;
extern void SetBothScreensModesAndDisable(const Modes *),InitBgFromTemplate(void *,int,const Template *,int),BgClearTilemapBufferAndCommit(void *,int),BG_ClearCharDataRange(int,int,int,int);
void ov52_021E83C4(void *);
void ov52_021E83C4(void *bg){
 Modes modes;Template t0,t1,t2,t4,t5;
 modes=ov52_021E94CC;SetBothScreensModesAndDisable(&modes);
 t0=ov52_021E9524;InitBgFromTemplate(bg,0,&t0,0);BgClearTilemapBufferAndCommit(bg,0);
 t1=ov52_021E9540;InitBgFromTemplate(bg,1,&t1,0);BgClearTilemapBufferAndCommit(bg,1);
 t2=ov52_021E955C;InitBgFromTemplate(bg,2,&t2,0);
 t4=ov52_021E94EC;InitBgFromTemplate(bg,4,&t4,0);BgClearTilemapBufferAndCommit(bg,4);
 t5=ov52_021E9508;InitBgFromTemplate(bg,5,&t5,0);
 BG_ClearCharDataRange(0,32,0,39);BG_ClearCharDataRange(4,32,0,39);
}
