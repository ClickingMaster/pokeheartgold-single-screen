#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct A{u32 words[8];}A;typedef struct B{u32 words[5];}B;typedef struct C{u16 words[7];}C;typedef struct W{u8 p[0x3fc];void*system,*manager;}W;extern const A _02103A50;extern const B _02103A3C;extern const C _02103A2C;extern void GfGfx_EngineATogglePlanes(u32,int),GfGfx_EngineBTogglePlanes(u32,int),GF_CreateVramTransferManager(int,int),SpriteSystem_Init(void*,A*,B*,int),SpriteSystem_InitSprites(void*,void*,int),sub_0200D294(void*,void*,C*);extern void*SpriteSystem_Alloc(int),*SpriteManager_New(void*);void sub_0208B1AC(W*);
void sub_0208B1AC(W*w){A a;B b;C c;GfGfx_EngineATogglePlanes(16,1);GfGfx_EngineBTogglePlanes(16,1);GF_CreateVramTransferManager(32,19);w->system=SpriteSystem_Alloc(19);w->manager=SpriteManager_New(w->system);a=_02103A50;b=_02103A3C;SpriteSystem_Init(w->system,&a,&b,32);SpriteSystem_InitSprites(w->system,w->manager,115);c=_02103A2C;sub_0200D294(w->system,w->manager,&c);}
