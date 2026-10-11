#pragma force_active on
#include "global.h"
#pragma force_active on
extern void SpriteSystem_FreeResourcesAndManager(void*,void*),SpriteSystem_Free(void*);
void PokedexApp_DestroySpriteSystem(void*);
void PokedexApp_DestroySpriteSystem(void*s){SpriteSystem_FreeResourcesAndManager(*(void**)((u8*)s+0x668),*(void**)((u8*)s+0x66c));SpriteSystem_Free(*(void**)((u8*)s+0x668));}
