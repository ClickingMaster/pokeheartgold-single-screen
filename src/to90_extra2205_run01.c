#pragma force_active on
#include "global.h"
#pragma force_active on
extern void ManagedSprite_TickFrame(void*);
void PokedexApp_TickSprites(void*);
void PokedexApp_TickSprites(void*s){u32 i;for(i=0;i<120;i++,s=(u8*)s+4){void*p=*(void**)((u8*)s+0x670);if(p)ManagedSprite_TickFrame(p);}}
