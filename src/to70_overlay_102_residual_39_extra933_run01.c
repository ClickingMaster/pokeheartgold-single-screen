#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct{u8 p[12];void *bg;}Work;
extern void GfGfxLoader_LoadScrnDataFromOpenNarc(void*,int,void*,int,int,int,int,int),GfGfxLoader_LoadCharDataFromOpenNarc(void*,int,void*,int,int,int,int,int),BgCommitTilemapBufferToVram(void*,int);

void ov102_021EC248(Work*w,void*narc);

void ov102_021EC248(Work*w,void*narc){GfGfxLoader_LoadScrnDataFromOpenNarc(narc,11,w->bg,5,0,0,1,35);GfGfxLoader_LoadCharDataFromOpenNarc(narc,12,w->bg,5,0,0,1,35);BgCommitTilemapBufferToVram(w->bg,5);}
