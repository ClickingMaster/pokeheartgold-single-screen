#pragma force_active on
#include "global.h"
#pragma force_active on
extern const u8 sSearchAreaCursorAnims[];extern void*GridInputHandler_GetDpadBox(void*);extern void DpadMenuBox_GetPosition(void*,u8*,u8*),ManagedSprite_SetPositionXY(void*,s16,s16),PokedexApp_SetSpriteAnim(void*,int,int);void PokedexApp_UpdateSearchAreaCursor(void*,int);
void PokedexApp_UpdateSearchAreaCursor(void*w,int index){u8 x,y;void*box=GridInputHandler_GetDpadBox(*(void**)((u8*)w+0x1864));DpadMenuBox_GetPosition(box,&x,&y);ManagedSprite_SetPositionXY(*(void**)((u8*)w+0x670),x,y);PokedexApp_SetSpriteAnim(w,0,sSearchAreaCursorAnims[index]);}
