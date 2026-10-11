#pragma force_active on
#include "global.h"
#define W(p,o) (*(int*)((u8*)(p)+(o)))
#define U(p,o) (*(u32*)((u8*)(p)+(o)))
#define P(p,o) (*(void**)((u8*)(p)+(o)))
#define H(p,o) (*(u16*)((u8*)(p)+(o)))
#define B(p,o) (*(u8*)((u8*)(p)+(o)))
#define C(p,o) (*(s8*)((u8*)(p)+(o)))
extern const u8 sSearchHeightCursorAnims[];extern void* GridInputHandler_GetDpadBox(void*);extern void DpadMenuBox_GetPosition(void*,u8*,u8*),ManagedSprite_SetPositionXY(void*,int,int),PokedexApp_SetSpriteAnim(void*,int,u8);extern int PokedexApp_SliderValueToX(void*,int);
void PokedexApp_UpdateSearchHeightCursor(void*,int);
#pragma force_active on
void PokedexApp_UpdateSearchHeightCursor(void*w,int which){u8 x,y;DpadMenuBox_GetPosition(GridInputHandler_GetDpadBox(P(w,0x1864)),&x,&y);
 if(which==0)x=PokedexApp_SliderValueToX(w,W(w,6268));else if(which==1)x=PokedexApp_SliderValueToX(w,W(w,6264));
 ManagedSprite_SetPositionXY(P(w,0x670),x,y);PokedexApp_SetSpriteAnim(w,0,sSearchHeightCursorAnims[which]);}
