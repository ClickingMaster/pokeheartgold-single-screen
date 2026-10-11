#pragma force_active on
#include "global.h"
#pragma force_active on
u32 PokedexApp_GetScrollbarLength(void*,int,const u8*);int PokedexApp_GetScrollbarMinY(void*,int,const u8*,int,u32);
int PokedexApp_GetScrollbarMinY(void*p,int idx,const u8*table,int x,u32 width){u32 text=PokedexApp_GetScrollbarLength(p,idx,table);return x-(width>>1)+(text>>1);}
