#pragma force_active on
#include "global.h"
typedef struct{u32 data[4];} Modes;typedef struct{u32 data[7];} BgTemplate;typedef struct{BgTemplate bg[7];} Templates;
extern Modes ov96_0221D6C4;extern Templates ov96_0221D834;extern int ov96_0221D738[];
typedef struct{int heap;void*bg;int a,b;void*format,*msg;} Work;
extern void SetBothScreensModesAndDisable(Modes*);extern void InitBgFromTemplate(void*,u8,BgTemplate*,int);extern void BgClearTilemapBufferAndCommit(void*,u8);extern void BG_ClearCharDataRange(u8,int,int,int);extern void*NewMsgDataFromNarc(int,int,int,int);extern void*MessageFormat_New(int);extern void FontID_Alloc(int,int);extern void GfGfx_EngineATogglePlanes(int,int);

void ov96_0221654C(Work*w);

void ov96_0221654C(Work*w){Templates templates;Modes modes;void*bg=w->bg;int heap=w->heap;int i;modes=ov96_0221D6C4;SetBothScreensModesAndDisable(&modes);templates=ov96_0221D834;for(i=0;i<7;i++){InitBgFromTemplate(bg,(u8)ov96_0221D738[i],&templates.bg[i],0);BgClearTilemapBufferAndCommit(bg,(u8)ov96_0221D738[i]);BG_ClearCharDataRange((u8)ov96_0221D738[i],32,0,heap);}w->msg=NewMsgDataFromNarc(1,27,309,w->heap);w->format=MessageFormat_New(w->heap);FontID_Alloc(4,w->heap);GfGfx_EngineATogglePlanes(8,0);}
