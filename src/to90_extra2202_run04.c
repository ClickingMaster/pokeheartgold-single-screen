#pragma force_active on
#include "global.h"
#pragma force_active on
extern const u8 ov18_021FB620[];extern void*GridInputHandler_GetDpadBox(void*);extern void DpadMenuBox_GetPosition(void*,u8*,u8*),ManagedSprite_SetPositionXY(void*,s16,s16),ov18_021F118C(void*,int,int);void ov18_021F76E0(void*,int);
void ov18_021F76E0(void*w,int index){u8 x,y;void*box=GridInputHandler_GetDpadBox(*(void**)((u8*)w+0x1864));DpadMenuBox_GetPosition(box,&x,&y);ManagedSprite_SetPositionXY(*(void**)((u8*)w+0x670),x,y);ov18_021F118C(w,0,ov18_021FB620[index]);}
