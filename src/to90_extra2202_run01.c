#pragma force_active on
#include "global.h"
#pragma force_active on
extern void System_GetTouchHeldCoords(u32*,u32*);
void ov18_021F765C(u32*);
void ov18_021F765C(u32*x){u32 y;System_GetTouchHeldCoords(x,&y);}
