#ifndef TO54_W14_PRIVATE_H
#define TO54_W14_PRIVATE_H
#include "global.h"

typedef struct SysTask SysTask;
void SysTask_Destroy(SysTask *task);
void SpriteSystem_FreeResourcesAndManager(void *renderer, void *manager);
void Sprite_DeleteAndFreeResources(void *sprite);
void SpriteSystem_DrawSprites(void *manager);
void ov07_0221D55C(void *p, u32 value);

void ov07_0221DA74(void *p);
void ov07_0221DAA0(void *p);
struct DrawTaskData;
void ov07_0221DAD0(SysTask *task, struct DrawTaskData *data);
void ov07_0221DCA8(void *p);
void ov07_0221DCC0(void *p);
void ov07_0221DCD4(void *p);
void ov07_0221DCF0(void *p);

void *BgGetCharPtr(u8 bgId);
void MI_CpuFill8(void *dest, u8 value, u32 size);
void *SpriteManager_New(void *system);
void SpriteSystem_InitSprites(void *system, void *manager, int maxSprites);
void *SpriteSystem_GetRenderer(void *system);
void G2dRenderer_SetSubSurfaceCoords(void *renderer, int x, int y);
void SpriteSystem_InitManagerWithCapacities(void *system, void *manager, const void *counts);
void ov07_0221D718(void *p);
void ov07_0221D740(void *p);

void SpriteSystem_LoadCharResObjFromOpenNarc(void *, void *, void *, int, int, int, int);
void SpriteSystem_LoadPaletteBufferFromOpenNarc(void *, int, void *, void *, void *, int, int, int, int, int);
void SpriteSystem_LoadCellResObjFromOpenNarc(void *, void *, void *, int, int, int);
void SpriteSystem_LoadAnimResObjFromOpenNarc(void *, void *, void *, int, int, int);
void ov07_0221D7B8(void *p);

#endif
