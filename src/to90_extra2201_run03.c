#pragma force_active on
#include "global.h"
#define W(p,o) (*(int*)((u8*)(p)+(o)))
#define U(p,o) (*(u32*)((u8*)(p)+(o)))
#define P(p,o) (*(void**)((u8*)(p)+(o)))
#define H(p,o) (*(u16*)((u8*)(p)+(o)))
#define B(p,o) (*(u8*)((u8*)(p)+(o)))
#define C(p,o) (*(s8*)((u8*)(p)+(o)))
extern const u8 ov18_021FB61C[];extern void* GridInputHandler_GetDpadBox(void*);extern void DpadMenuBox_GetPosition(void*,u8*,u8*),ManagedSprite_SetPositionXY(void*,int,int),ov18_021F118C(void*,int,u8);extern int ov18_021F3AD0(void*,int);
void ov18_021F75C8(void*,int);
#pragma force_active on
void ov18_021F75C8(void*w,int which){u8 x,y;DpadMenuBox_GetPosition(GridInputHandler_GetDpadBox(P(w,0x1864)),&x,&y);
 if(which==0)x=ov18_021F3AD0(w,W(w,6276));else if(which==1)x=ov18_021F3AD0(w,W(w,6272));
 ManagedSprite_SetPositionXY(P(w,0x670),x,y);ov18_021F118C(w,0,ov18_021FB61C[which]);}
