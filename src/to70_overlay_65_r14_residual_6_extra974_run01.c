#pragma force_active on
#include "global.h"
typedef struct {u8 pad[8];u32 size;u8 data[1];} Screen;
extern void *GfGfxLoader_LoadFromOpenNarc(void*,int,int,int,int);extern void *GetBgTilemapBuffer(void*,u8);extern void BG_LoadScreenTilemapData(void*,u8,const void*,u32);extern void Heap_Free(void*);

void ov65_0221D204(void*narc,int member,void*bg,int layer,int unused,u32 size,int a,int b);

void ov65_0221D204(void*narc,int member,void*bg,int layer,int unused,u32 size,int a,int b){Screen*screen;void*raw=GfGfxLoader_LoadFromOpenNarc(narc,member,a,b,1);if(raw){if(NNS_G2dGetUnpackedScreenData(raw,(NNSG2dScreenData**)&screen)){if(size==0)size=screen->size;if(GetBgTilemapBuffer(bg,(u8)layer))BG_LoadScreenTilemapData(bg,(u8)layer,screen->data,size);}Heap_Free(raw);}}
