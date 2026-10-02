#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct Screen {u32 words[4];} Screen;typedef struct Bg {u32 words[7];} Bg;
extern Screen ov96_0221AF64;extern Bg ov96_0221AFA0,ov96_0221AFF4,ov96_0221AFBC,ov96_0221AFD8;
void SetBothScreensModesAndDisable(Screen *),InitBgFromTemplate(void *,int,Bg *,int),BgClearTilemapBufferAndCommit(void *,int),BG_FillCharDataRange(void *,int,int,int,int),ov96_021EC2E8(void *);
void ov96_021EC2E8(void *bg){Screen screens=ov96_0221AF64;Bg a,b,c,d;SetBothScreensModesAndDisable(&screens);a=ov96_0221AFA0;InitBgFromTemplate(bg,0,&a,0);BgClearTilemapBufferAndCommit(bg,0);BG_FillCharDataRange(bg,1,0,1,0);b=ov96_0221AFF4;InitBgFromTemplate(bg,1,&b,0);BgClearTilemapBufferAndCommit(bg,1);BG_FillCharDataRange(bg,1,0,1,0);c=ov96_0221AFBC;InitBgFromTemplate(bg,4,&c,0);BgClearTilemapBufferAndCommit(bg,4);d=ov96_0221AFD8;InitBgFromTemplate(bg,5,&d,0);BgClearTilemapBufferAndCommit(bg,5);}
