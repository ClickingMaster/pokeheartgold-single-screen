#pragma force_active on
#include "global.h"
typedef struct {u32 heap;u8 pad[0x54];void *touch;u8 pad2[0x1f0];void *system,*manager;} W;
extern u8 ov59_0223C750[],ov59_0223C678[],ov59_0223C648[];
extern void GF_CreateVramTransferManager(int,u32),GF_DestroyVramTransferManager(void),thunk_ClearMainOAM(u32),thunk_ClearSubOAM(u32);
extern void *SpriteSystem_Alloc(u32),*SpriteManager_New(void *),*TouchscreenListMenuSpawner_Create(u32,int);
extern void SpriteSystem_Init(void *,void *,void *,int),SpriteSystem_InitSprites(void *,void *,int),sub_0200D2A4(void *,void *,void *,int,int),TouchscreenListMenuSpawner_Destroy(void *),SpriteSystem_DestroySpriteManager(void *,void *),SpriteSystem_Free(void *);
void ov59_02239938(W *); void ov59_022399B4(W *);
void ov59_02239938(W *w){GF_CreateVramTransferManager(32,w->heap);w->system=SpriteSystem_Alloc(w->heap);SpriteSystem_Init(w->system,ov59_0223C750,ov59_0223C678,3);thunk_ClearMainOAM(w->heap);thunk_ClearSubOAM(w->heap);w->manager=SpriteManager_New(w->system);SpriteSystem_InitSprites(w->system,w->manager,15);sub_0200D2A4(w->system,w->manager,ov59_0223C648,0,0);w->touch=TouchscreenListMenuSpawner_Create(w->heap,0);}
void ov59_022399B4(W *w){TouchscreenListMenuSpawner_Destroy(w->touch);SpriteSystem_DestroySpriteManager(w->system,w->manager);w->manager=0;SpriteSystem_Free(w->system);w->system=0;GF_DestroyVramTransferManager();thunk_ClearMainOAM(w->heap);}
