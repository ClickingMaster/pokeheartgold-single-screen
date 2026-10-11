#pragma force_active on
#include "global.h"
#pragma force_active on
extern void ManagedSprite_SetOamMode(void*,int);
void PokedexApp_SetSpriteTranslucent(void*,int,int);
void PokedexApp_SetSpriteTranslucent(void*c,int index,int enabled){if(enabled==1)ManagedSprite_SetOamMode(*(void**)((u8*)c+0x670+index*4),1);else ManagedSprite_SetOamMode(*(void**)((u8*)c+0x670+index*4),0);}
