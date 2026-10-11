#pragma force_active on
#include "global.h"
#pragma force_active on
extern void PokedexApp_GetSpritePos(void*,int,s16*,s16*,int),PokedexApp_SetSpritePos(void*,int,s16,s16,int);void ov18_021F4FC8(void*,int,int);
void ov18_021F4FC8(void*w,int id,int from){s16 x,y;PokedexApp_GetSpritePos(w,from,&x,&y,1);y-=16;PokedexApp_SetSpritePos(w,id,x,y,1);}
