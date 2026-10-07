#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct{u32 words[4];} Modes;
typedef struct{u32 words[7];} Bg;
extern const Modes ov96_0221CAF0;
extern const Bg ov96_0221CB64,ov96_0221CB80,ov96_0221CB10,ov96_0221CB2C,ov96_0221CB48;
extern void SetBothScreensModesAndDisable(Modes*),InitBgFromTemplate(void*,int,Bg*,int),BgClearTilemapBufferAndCommit(void*,int),GfGfx_EngineATogglePlanes(int,int);
void ov96_02204F40(void*);
void ov96_02204F40(void*w){Modes modes=ov96_0221CAF0;Bg bg0,bg3,bg4,bg5,bg6;SetBothScreensModesAndDisable(&modes);bg0=ov96_0221CB64;InitBgFromTemplate(w,0,&bg0,0);BgClearTilemapBufferAndCommit(w,0);bg3=ov96_0221CB80;InitBgFromTemplate(w,3,&bg3,0);BgClearTilemapBufferAndCommit(w,3);bg4=ov96_0221CB10;InitBgFromTemplate(w,4,&bg4,0);BgClearTilemapBufferAndCommit(w,4);bg5=ov96_0221CB2C;InitBgFromTemplate(w,5,&bg5,0);BgClearTilemapBufferAndCommit(w,5);bg6=ov96_0221CB48;InitBgFromTemplate(w,6,&bg6,0);BgClearTilemapBufferAndCommit(w,6);GfGfx_EngineATogglePlanes(8,0);}
