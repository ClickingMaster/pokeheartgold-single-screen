#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct Modes{u32 words[4];}Modes;
typedef struct Template{u32 words[7];}Template;
extern const Modes ov96_0221C7DC;
extern const Template ov96_0221C83C,ov96_0221C858,ov96_0221C890,ov96_0221C8AC,ov96_0221C804,ov96_0221C820,ov96_0221C874;
void SetBothScreensModesAndDisable(const Modes *);void InitBgFromTemplate(void *,int,const Template *,int);
void BgClearTilemapBufferAndCommit(void *,int);void GfGfx_EngineATogglePlanes(int,int);
void ov96_02201CB0(void *);
void ov96_02201CB0(void *w){Modes m;Template t0,t1,t2,t3,t4,t5,t6;m=ov96_0221C7DC;SetBothScreensModesAndDisable(&m);
t0=ov96_0221C83C;InitBgFromTemplate(w,0,&t0,0);BgClearTilemapBufferAndCommit(w,0);
t1=ov96_0221C858;InitBgFromTemplate(w,1,&t1,0);BgClearTilemapBufferAndCommit(w,1);
t2=ov96_0221C890;InitBgFromTemplate(w,3,&t2,0);BgClearTilemapBufferAndCommit(w,3);
t3=ov96_0221C8AC;InitBgFromTemplate(w,4,&t3,0);BgClearTilemapBufferAndCommit(w,4);
t4=ov96_0221C804;InitBgFromTemplate(w,5,&t4,0);BgClearTilemapBufferAndCommit(w,5);
t5=ov96_0221C820;InitBgFromTemplate(w,6,&t5,0);BgClearTilemapBufferAndCommit(w,6);
t6=ov96_0221C874;InitBgFromTemplate(w,7,&t6,0);BgClearTilemapBufferAndCommit(w,7);
GfGfx_EngineATogglePlanes(8,0);}
