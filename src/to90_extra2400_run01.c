#pragma force_active on
#include "global.h"
#pragma force_active on
#define U32(p,o) (*(u32 *)((u8 *)(p)+(o)))
#define U8(p,o) (*(u8 *)((u8 *)(p)+(o)))
#include "sprite.h"
extern s32 _s32_div_f(s32,s32);
static inline void Split(u16 frames,u8 *seconds,u8 *fraction) { *seconds=(u8)(frames/30); *fraction=(u8)(_s32_div_f((u8)(frames%30)*100,30)); }
void ov96_021FB994(void *);
void ov96_021FB994(void *state) { u16 frames=(u16)U32(state,0x22c); u8 seconds,fraction; Split(frames,&seconds,&fraction); Sprite_SetAnimCtrlSeq((Sprite *)U32(state,0x3ac),(u8)(seconds/60)+1); seconds=(u8)(seconds%60); Sprite_SetAnimCtrlSeq((Sprite *)U32(state,0x3b4),(u8)(seconds/10)+1); Sprite_SetAnimCtrlSeq((Sprite *)U32(state,0x3b8),(u8)(seconds%10)+1); Sprite_SetAnimCtrlSeq((Sprite *)U32(state,0x3c0),(u8)(fraction/10)+1); }
