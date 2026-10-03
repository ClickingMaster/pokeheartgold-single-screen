#pragma force_active on
#include "global.h"
extern void GfGfx_EngineATogglePlanes(int,int),GfGfx_EngineBTogglePlanes(int,int),GF_CreateVramTransferManager(int,int),ov15_021FFA40(void *),ov15_021FFAD0(void *),ov15_021FFDD8(void *),G2dRenderer_SetSubSurfaceCoords(void *,int,int);extern void *SpriteSystem_GetRenderer(void *);void ov15_021FF850(void *);
void ov15_021FF850(void *w){GfGfx_EngineATogglePlanes(16,1);GfGfx_EngineBTogglePlanes(16,1);GF_CreateVramTransferManager(32,6);ov15_021FFA40(w);ov15_021FFAD0(w);ov15_021FFDD8(w);G2dRenderer_SetSubSurfaceCoords(SpriteSystem_GetRenderer(*(void**)((u8*)w+0x248)),0,0x100000);}
