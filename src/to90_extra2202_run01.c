#pragma force_active on
#include "global.h"
#pragma force_active on
extern void System_GetTouchHeldCoords(u32*,u32*);
void PokedexApp_GetTouchHeldX(u32*);
void PokedexApp_GetTouchHeldX(u32*x){u32 y;System_GetTouchHeldCoords(x,&y);}
