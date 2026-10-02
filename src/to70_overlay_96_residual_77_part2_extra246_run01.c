#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct Modes{u32 words[4];}Modes;
typedef struct Template{u32 words[7];}Template;
extern const Modes ov96_0221C61C;
extern const Template ov96_0221C648,ov96_0221C664,ov96_0221C680,ov96_0221C69C,ov96_0221C6B8,ov96_0221C6D4,ov96_0221C62C;
void SetBothScreensModesAndDisable(const Modes *);void InitBgFromTemplate(void *,int,const Template *,int);
void BgClearTilemapBufferAndCommit(void *,int);void GfGfx_EngineATogglePlanes(int,int);
void ov96_021FCF00(void *);
void ov96_021FCF00(void *w){Modes m;Template t0,t1,t2,t3,t4,t5,t6;m=ov96_0221C61C;SetBothScreensModesAndDisable(&m);
t0=ov96_0221C648;InitBgFromTemplate(w,0,&t0,0);BgClearTilemapBufferAndCommit(w,0);
t1=ov96_0221C664;InitBgFromTemplate(w,1,&t1,0);BgClearTilemapBufferAndCommit(w,1);
t2=ov96_0221C680;InitBgFromTemplate(w,2,&t2,0);BgClearTilemapBufferAndCommit(w,2);
t3=ov96_0221C69C;InitBgFromTemplate(w,3,&t3,0);BgClearTilemapBufferAndCommit(w,3);
t4=ov96_0221C6B8;InitBgFromTemplate(w,4,&t4,0);BgClearTilemapBufferAndCommit(w,4);
t5=ov96_0221C6D4;InitBgFromTemplate(w,5,&t5,0);BgClearTilemapBufferAndCommit(w,5);
t6=ov96_0221C62C;InitBgFromTemplate(w,6,&t6,0);BgClearTilemapBufferAndCommit(w,6);
GfGfx_EngineATogglePlanes(8,0);}
