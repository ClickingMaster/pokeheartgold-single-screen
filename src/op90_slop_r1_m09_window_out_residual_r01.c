#pragma force_active on
#include "global.h"
#include <nitro/gx/g2.h>
static inline GXWndPlane GetPlane(volatile GXWndPlane *r) { return *r; }
s8 sub_0201333C(BOOL engine);
// @fn sub_0201333C
s8 sub_0201333C(BOOL engine) {GXWndPlane plane; if (engine == 0) { plane = GetPlane((volatile GXWndPlane *)0x0400004A); } else { plane = GetPlane((volatile GXWndPlane *)0x0400104A); } return *(s8 *)&plane;}
