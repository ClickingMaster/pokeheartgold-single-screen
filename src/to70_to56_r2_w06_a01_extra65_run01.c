#pragma force_active on
#include "global.h"
#pragma force_active on
#define U32(p,o) (*(u32 *)((u8 *)(p)+(o)))
#define U8(p,o) (*(u8 *)((u8 *)(p)+(o)))
typedef struct Ov07R2Work Ov07R2Work;
extern void ov07_02226CB0(Ov07R2Work *);
extern void ov07_0221C448(void *,void *);
extern void ManagedSprite_SetDrawFlag(void *,BOOL);
extern void ManagedSprite_TickFrame(void *);
extern void SpriteSystem_DrawSprites(void *);
extern void Heap_Free(void *);
void ov07_02226F04(void *,Ov07R2Work *);
void ov07_02226F04(void *task,Ov07R2Work *state) { if(U32(state,0x30))ov07_02226CB0(state); U8(state,0)++; if(U8(state,0)>=(s32)U32(state,0x24)) { GX_SetVisibleWnd(0); u32 tmp=*(volatile u16 *)0x04000048 &~0x3f; *(volatile u16 *)0x04000048=(u16)tmp; tmp=*(volatile u16 *)0x0400004a &~0x3f; *(volatile u16 *)0x0400004a=(u16)tmp; *(volatile u16 *)0x04000040=0; *(volatile u16 *)0x04000044=0; ManagedSprite_SetDrawFlag((void *)U32(state,0x1c),FALSE); SpriteSystem_DrawSprites((void *)U32(state,12)); ov07_0221C448((void *)U32(state,4),task); Heap_Free(state); }else { ManagedSprite_TickFrame((void *)U32(state,0x1c)); SpriteSystem_DrawSprites((void *)U32(state,12)); } }
