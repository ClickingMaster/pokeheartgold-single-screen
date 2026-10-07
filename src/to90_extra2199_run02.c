#pragma force_active on
#include "global.h"
#pragma force_active on
#define W(p,o) (*(int*)((u8*)(p)+(o)))
#define P(p,o) (*(void**)((u8*)(p)+(o)))
#define H(p,o) (*(u16*)((u8*)(p)+(o)))
#define B(p,o) (*(u8*)((u8*)(p)+(o)))
#include "system.h"
#include "touchscreen.h"
extern const TouchscreenHitbox ov18_021FB718[];extern int GridInputHandler_GetNextInput(void*),GridInputHandler_HandleInput_AllowHold(void*);extern void GridInputHandler_SetNextInput(void*,u8),MenuInputStateMgr_SetState(void*,int),ov18_021F7444(void*,unsigned);int ov18_021F739C(void*);
#pragma opt_common_subs off
int ov18_021F739C(void*w){int next=GridInputHandler_GetNextInput(P(w,0x1864));unsigned hit,index;MenuInputStateMgr_SetState(P(P(w,0),12),0);if(gSystem.newKeys&8)return 2;if(next==0){if(gSystem.newAndRepeatedKeys&32)return 5;if(gSystem.newAndRepeatedKeys&16)return 4;}if(next==1){if(gSystem.newAndRepeatedKeys&32)return 7;if(gSystem.newAndRepeatedKeys&16)return 6;}hit=TouchscreenHitbox_FindRectAtTouchHeld(ov18_021FB718);if(hit!=(unsigned)-1){MenuInputStateMgr_SetState(P(P(w,0),12),1);index=hit>>1;GridInputHandler_SetNextInput(P(w,0x1864),(u8)index);ov18_021F7444(w,index);return hit+4;}return GridInputHandler_HandleInput_AllowHold(P(w,0x1864));}
