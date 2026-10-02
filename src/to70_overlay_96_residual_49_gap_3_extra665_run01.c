#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct {u32 x[6];} Caps;
typedef struct {u32 x[8];} Setup;
typedef struct {u32 x[5];} Limits;
extern void* SpriteSystem_Alloc(int);
extern void* SpriteManager_New(void*);
extern void SpriteSystem_Init(void*,Setup*,Limits*,int);
extern void SpriteSystem_InitSprites(void*,void*,int);
extern void SpriteSystem_InitManagerWithCapacities(void*,void*,Caps*);
extern void GfGfx_EngineATogglePlanes(int,int);
extern void GfGfx_EngineBTogglePlanes(int,int);
extern Caps ov96_0221BA68;
extern Setup ov96_0221BAA0;
extern Limits ov96_0221BA54;
extern void* SpriteSystem_GetRenderer(void*);
extern void G2dRenderer_SetSubSurfaceCoords(void*,int,int);
typedef struct {u32 pad[3];int heap;void*system,*manager;} Work;
extern void ov96_021EF8C0(Work*);
extern void ov96_021EF99C(Work*);
void ov96_021EF818(Work*);
void ov96_021EF818(Work*w){Caps caps=ov96_0221BA68;Setup setup=ov96_0221BAA0;Limits limits=ov96_0221BA54;limits.x[0]=4;w->system=SpriteSystem_Alloc(w->heap);w->manager=SpriteManager_New(w->system);SpriteSystem_Init(w->system,&setup,&limits,32);SpriteSystem_InitSprites(w->system,w->manager,4);SpriteSystem_InitManagerWithCapacities(w->system,w->manager,&caps);GfGfx_EngineATogglePlanes(16,1);GfGfx_EngineBTogglePlanes(16,1);G2dRenderer_SetSubSurfaceCoords(SpriteSystem_GetRenderer(w->system),0,2<<20);ov96_021EF8C0(w);ov96_021EF99C(w);}
