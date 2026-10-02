#pragma force_active on
#include "global.h"
typedef struct {u32 v[8];} Config;typedef struct {u32 v[5];} Config2;typedef struct {u32 v[6];} Caps;
extern Config ov112_021FF330;extern Config2 ov112_021FF304;extern Caps ov112_021FF318;
typedef struct {u32 x,heap;u8 p[0x68-8];void *sys,*manager;} Work;
extern void *SpriteSystem_Alloc(u32);extern void *SpriteManager_New(void*);extern void SpriteSystem_Init(void*,Config*,Config2*,u32);extern void SpriteSystem_InitSprites(void*,void*,u32);extern void SpriteSystem_InitManagerWithCapacities(void*,void*,Caps*);extern void *SpriteSystem_GetRenderer(void*);extern void G2dRenderer_SetSubSurfaceCoords(void*,int,int);extern void GfGfx_EngineATogglePlanes(int,int),GfGfx_EngineBTogglePlanes(int,int);void ov112_021F196C(Work*);
void ov112_021F196C(Work*w){Config config;Config2 config2;Caps caps;w->sys=SpriteSystem_Alloc(w->heap);w->manager=SpriteManager_New(w->sys);config=ov112_021FF330;config2=ov112_021FF304;config2.v[0]=32;SpriteSystem_Init(w->sys,&config,&config2,32);SpriteSystem_InitSprites(w->sys,w->manager,32);caps=ov112_021FF318;SpriteSystem_InitManagerWithCapacities(w->sys,w->manager,&caps);G2dRenderer_SetSubSurfaceCoords(SpriteSystem_GetRenderer(w->sys),0,0x20c000);GfGfx_EngineATogglePlanes(16,1);GfGfx_EngineBTogglePlanes(16,1);}
