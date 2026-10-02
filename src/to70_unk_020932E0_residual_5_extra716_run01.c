#pragma force_active on
#include "global.h"
extern void GfGfxLoader_LoadScrnData(int,int,void *,int,int,int,int,int);
extern void BgTilemapRectChangePalette(void *,int,int,int,int,int,int);
extern void BgCommitTilemapBufferToVram(void *,int);
extern void *GfGfxLoader_GetScrnData(int,int,int,void *,int);
extern void GfGfx_EngineATogglePlanes(int,int);
typedef struct {u8 pad[0x46c8];void *raw[2];void *scr[2];} Work;
void sub_020941CC(Work *,void *,int);
void sub_020941CC(Work *w,void *bg,int heap){GfGfxLoader_LoadScrnData(151,1,bg,2,0,1536,1,heap);BgTilemapRectChangePalette(bg,2,0,0,32,24,1);BgCommitTilemapBufferToVram(bg,2);GfGfxLoader_LoadScrnData(151,4,bg,3,0,1536,1,heap);BgTilemapRectChangePalette(bg,3,0,0,32,24,1);BgCommitTilemapBufferToVram(bg,3);w->raw[0]=GfGfxLoader_GetScrnData(151,3,1,&w->scr[0],heap);w->raw[1]=GfGfxLoader_GetScrnData(151,1,1,&w->scr[1],heap);GfGfx_EngineATogglePlanes(8,0);}
