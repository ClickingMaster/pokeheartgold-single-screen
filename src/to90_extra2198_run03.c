#pragma force_active on
#include "global.h"
#pragma force_active on
extern const u8 sSearchOrderCursorAnims[];
void*GridInputHandler_GetDpadBox(void*);void DpadMenuBox_GetPosition(void*,u8*,u8*);void ManagedSprite_SetPositionXY(void*,s16,s16);void PokedexApp_SetSpriteAnim(void*,int,int);void PokedexApp_UpdateSearchOrderCursor(void*,u32);
void PokedexApp_UpdateSearchOrderCursor(void*p,u32 index){struct{u8 y,x;}pos;DpadMenuBox_GetPosition(GridInputHandler_GetDpadBox(*(void**)((u8*)p+0x1864)),&pos.x,&pos.y);ManagedSprite_SetPositionXY(*(void**)((u8*)p+0x670),pos.x,pos.y);PokedexApp_SetSpriteAnim(p,0,sSearchOrderCursorAnims[index]);}
