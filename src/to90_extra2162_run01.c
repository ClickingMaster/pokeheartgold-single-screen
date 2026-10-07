#pragma force_active on
#include "global.h"
#pragma force_active on
#include "system.h"
#include "touchscreen.h"
typedef struct{u8 pad[0x2079];u8 selected;} Work;
u32 ov08_0221CA50(const Work*);
u32 ov08_0221CA50(const Work *w){if((gSystem.newKeys&3)||System_GetTouchNew()==1)return w->selected;return 18;}
