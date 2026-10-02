#pragma force_active on
#include "global.h"
#pragma force_active on
void ManagedSprite_SetPositionXY(void *,s16,s16),ManagedSprite_SetAnimationFrame(void *,int),ManagedSprite_SetPaletteOverride(void *,int);
void ov68_021E73A4(u8 *,u32,int);
void ManagedSprite_ResetSpriteAnimCtrlState(void *),ManagedSprite_SetAnim(void *,int),ManagedSprite_SetAnimateFlag(void *,int);void ov68_021E7424(u8 *,int,int);
void ov68_021E73A4(u8 *w,u32 n,int pal){if(n<4){ManagedSprite_SetPositionXY(*(void **)(w+300),0,(s16)(n*32+32));ManagedSprite_SetAnimationFrame(*(void **)(w+300),0);}else if(n<6){ManagedSprite_SetPositionXY(*(void **)(w+300),(s16)((n-4)*40),168);ManagedSprite_SetAnimationFrame(*(void **)(w+300),1);}else{ManagedSprite_SetPositionXY(*(void **)(w+300),192,160);ManagedSprite_SetAnimationFrame(*(void **)(w+300),2);}ManagedSprite_SetPaletteOverride(*(void **)(w+300),pal);}
void ov68_021E7424(u8 *w,int which,int reset){int animate;if(which==0&&reset){animate=1;ManagedSprite_ResetSpriteAnimCtrlState(*(void **)(w+292));}else animate=0;if(*(u16 *)((u8 *)*(void **)w+22)+4<w[440])ManagedSprite_SetAnim(*(void **)(w+292),animate);else ManagedSprite_SetAnim(*(void **)(w+292),animate+2);ManagedSprite_SetAnimateFlag(*(void **)(w+292),animate);if(which==1&&reset){animate=1;ManagedSprite_ResetSpriteAnimCtrlState(*(void **)(w+296));}else animate=0;if(*(u16 *)((u8 *)*(void **)w+22))ManagedSprite_SetAnim(*(void **)(w+296),animate+4);else ManagedSprite_SetAnim(*(void **)(w+296),animate+6);ManagedSprite_SetAnimateFlag(*(void **)(w+296),animate);}
