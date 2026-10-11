#pragma force_active on
#include "global.h"
#pragma force_active on
extern void PokedexApp_UpdateBodyTypeSprite(void*),ManagedSprite_SetPositionXY(void*,int,int),PokedexApp_SetSpriteVisible(void*,int,int);
void ov18_021F34EC(u8*,int);
void ov18_021F34EC(u8 *s,int mode){u32 i;PokedexApp_UpdateBodyTypeSprite(s);if(mode==1){ManagedSprite_SetPositionXY(*(void**)(s+0x6e0),224,72);for(i=29;i<=42;i++)PokedexApp_SetSpriteVisible(s,i,0);}else{ManagedSprite_SetPositionXY(*(void**)(s+0x6e0),152,20);for(i=29;i<=42;i++)PokedexApp_SetSpriteVisible(s,i,1);}}
