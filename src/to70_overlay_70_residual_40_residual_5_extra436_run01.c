#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct {u32 words[7];} Template;
extern Template ov70_02246058,ov70_0224603C,ov70_02246020,ov70_02246074;
extern void InitBgFromTemplate(void *,int,Template *,int),GfGfx_EngineATogglePlanes(int,int),GfGfx_EngineBTogglePlanes(int,int),BgClearTilemapBufferAndCommit(void *,int),BG_ClearCharDataRange(int,int,int,int);
void ov70_022441A4(void *);
void ov70_022441A4(void *bg){Template a=ov70_02246058;InitBgFromTemplate(bg,0,&a,0);GfGfx_EngineATogglePlanes(1,0);BgClearTilemapBufferAndCommit(bg,0);{Template b=ov70_0224603C;InitBgFromTemplate(bg,1,&b,0);GfGfx_EngineATogglePlanes(2,0);}{Template c=ov70_02246020;InitBgFromTemplate(bg,4,&c,0);GfGfx_EngineBTogglePlanes(1,0);BgClearTilemapBufferAndCommit(bg,4);}{Template d=ov70_02246074;InitBgFromTemplate(bg,5,&d,0);GfGfx_EngineBTogglePlanes(2,0);}BG_ClearCharDataRange(0,32,0,61);BG_ClearCharDataRange(4,32,0,61);GfGfx_EngineBTogglePlanes(16,0);}
