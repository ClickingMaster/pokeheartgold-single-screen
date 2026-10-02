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
extern Caps ov68_021E7C2C;
extern Setup ov68_021E7CD0;
extern Limits ov68_021E7C18;
extern void GF_CreateVramTransferManager(int,int);
typedef struct {u8 pad[0x11c];void*system,*manager;} Work;
void ov68_021E6DDC(Work*);
void ov68_021E6DDC(Work*w){Caps caps=ov68_021E7C2C;Setup setup;Limits limits;GF_CreateVramTransferManager(64,66);w->system=SpriteSystem_Alloc(66);w->manager=SpriteManager_New(w->system);setup=ov68_021E7CD0;limits=ov68_021E7C18;SpriteSystem_Init(w->system,&setup,&limits,32);SpriteSystem_InitSprites(w->system,w->manager,12);SpriteSystem_InitManagerWithCapacities(w->system,w->manager,&caps);GfGfx_EngineATogglePlanes(16,1);GfGfx_EngineBTogglePlanes(16,1);}
