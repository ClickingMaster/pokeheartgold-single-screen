#pragma force_active on
#include "global.h"
#pragma force_active on
extern const u8 ov18_021FB628[];
void*GridInputHandler_GetDpadBox(void*);void DpadMenuBox_GetPosition(void*,u8*,u8*);void ManagedSprite_SetPositionXY(void*,s16,s16);void ov18_021F118C(void*,int,int);void ov18_021F6F38(void*,u32);
void ov18_021F6F38(void*p,u32 index){struct{u8 y,x;}pos;DpadMenuBox_GetPosition(GridInputHandler_GetDpadBox(*(void**)((u8*)p+0x1864)),&pos.x,&pos.y);ManagedSprite_SetPositionXY(*(void**)((u8*)p+0x670),pos.x,pos.y);ov18_021F118C(p,0,ov18_021FB628[index]);}
