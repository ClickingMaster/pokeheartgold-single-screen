#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct{u32 v[8];}Info;typedef struct{u32 v[5];}Settings;typedef struct{u32 v[6];}Capacities;
extern const Info ov18_021FA3C8;extern const Settings ov18_021FA36C;extern const Capacities ov18_021FA380;extern void*SpriteSystem_Alloc(int),*SpriteManager_New(void*),*SpriteSystem_GetRenderer(void*);extern void SpriteSystem_Init(void*,Info*,Settings*,int),SpriteSystem_InitSprites(void*,void*,int),SpriteSystem_InitManagerWithCapacities(void*,void*,Capacities*),G2dRenderer_SetSubSurfaceCoords(void*,int,int);
void ov18_021F1024(void*);
void ov18_021F1024(void*s){Info info;Settings settings;Capacities caps;*(void**)((u8*)s+0x668)=SpriteSystem_Alloc(37);*(void**)((u8*)s+0x66c)=SpriteManager_New(*(void**)((u8*)s+0x668));info=ov18_021FA3C8;settings=ov18_021FA36C;SpriteSystem_Init(*(void**)((u8*)s+0x668),&info,&settings,32);caps=ov18_021FA380;SpriteSystem_InitSprites(*(void**)((u8*)s+0x668),*(void**)((u8*)s+0x66c),120);SpriteSystem_InitManagerWithCapacities(*(void**)((u8*)s+0x668),*(void**)((u8*)s+0x66c),&caps);G2dRenderer_SetSubSurfaceCoords(SpriteSystem_GetRenderer(*(void**)((u8*)s+0x668)),0,2097152);}
