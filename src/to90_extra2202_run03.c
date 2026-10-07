#pragma force_active on
#include "global.h"
#pragma force_active on
#define P(p,o) (*(void**)((u8*)(p)+(o)))
#include "system.h"
extern void MenuInputStateMgr_SetState(void*,int);extern int GridInputHandler_HandleInput_AllowHold(void*);int ov18_021F76B0(void*);
int ov18_021F76B0(void*w){MenuInputStateMgr_SetState(P(P(w,0),12),0);if(gSystem.newKeys&8)return 4;return GridInputHandler_HandleInput_AllowHold(P(w,0x1864));}
