#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct{u8 pad[0x188c];int value;}Work;void PokedexApp_SetSpriteVisible(Work*,int,int),PokedexApp_SetSpriteAnim(Work*,int,int),PokedexApp_UpdateBodyTypeSprite(Work*);
void PokedexApp_UpdateBodyTypeSprite(Work*w){if(w->value==14)PokedexApp_SetSpriteVisible(w,28,0);else{PokedexApp_SetSpriteVisible(w,28,1);PokedexApp_SetSpriteAnim(w,28,w->value);}}
