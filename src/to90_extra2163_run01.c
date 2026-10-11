#pragma force_active on
#include "global.h"
#pragma force_active on
#include "system.h"
#include "touchscreen.h"
typedef struct{u8 pad[0x114b];u8 selected;} Work;
u32 BattleBagMenu_WaitMessageInput(const Work*);
u32 BattleBagMenu_WaitMessageInput(const Work *w){if((gSystem.newKeys&3)||System_GetTouchNew()==1)return w->selected;return 10;}
