#pragma force_active on
#include "global.h"
#pragma force_active on
void PokedexApp_SetSpriteVisible(void*,int,int);
void ov18_021F34C4(void*,int);
void ov18_021F34C4(void *w,int flag){u32 i;if(flag==1)PokedexApp_SetSpriteVisible(w,28,0);for(i=29;i<=42;i++)PokedexApp_SetSpriteVisible(w,i,0);}
