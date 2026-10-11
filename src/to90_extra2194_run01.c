#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct{void*root;s32 offset;}Work;typedef struct{s16 y,x;}Pos;void PokedexApp_GetSpritePos(void*,int,s16*,s16*,int),PokedexApp_SetSpritePos(void*,int,s16,s16,int);int ov18_021F50C0(Work*);
int ov18_021F50C0(Work*w){Pos a,b;PokedexApp_GetSpritePos(w->root,2,&a.x,&a.y,1);PokedexApp_GetSpritePos(w->root,4,&b.x,&b.y,1);w->offset+=0x400;a.y+=w->offset>>12;b.y+=w->offset>>12;if(a.y>=88){PokedexApp_SetSpritePos(w->root,2,a.x,88,1);PokedexApp_SetSpritePos(w->root,4,b.x,88,1);return 0;}PokedexApp_SetSpritePos(w->root,2,a.x,a.y,1);PokedexApp_SetSpritePos(w->root,4,b.x,b.y,1);return 1;}
