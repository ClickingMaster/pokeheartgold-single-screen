#pragma force_active on
#include "global.h"
#include "system.h"
#pragma force_active on
typedef struct W{u8 pad[0x3fc];void*grid;}W;
extern int GridInputHandler_HandleInput_AllowHold(void*),GridInputHandler_GetNextInput(void*),TouchscreenHitbox_FindHitboxAtTouchNew(const void*);extern const u8 ov99_021E9F54[];extern void ov99_021E7AD4(W*,int,int);int ov99_021E7BC8(W*);
int ov99_021E7BC8(W*w){int input=GridInputHandler_HandleInput_AllowHold(w->grid);u32 next=GridInputHandler_GetNextInput(w->grid);int touch=TouchscreenHitbox_FindHitboxAtTouchNew(ov99_021E9F54);if(touch!=-1){if(touch==0)ov99_021E7AD4(w,0,1);else if(touch==1)ov99_021E7AD4(w,1,1);}else{if(input==-1&&next<30){if(gSystem.newAndRepeatedKeys==32)ov99_021E7AD4(w,0,0);else if(gSystem.newAndRepeatedKeys==16)ov99_021E7AD4(w,1,0);}else if(next==30){if(gSystem.newKeys&1)return 1;}}return 0;}
