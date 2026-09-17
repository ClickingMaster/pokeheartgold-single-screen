#ifndef TO50_OV15_R13_PRIVATE_H
#define TO50_OV15_R13_PRIVATE_H
#include "global.h"
#define PTR(p, o) (*(void **)((u8 *)(p) + (o)))
extern void ov15_021FED3C(void *);
extern void ov15_021FB518(void *);
extern void ov15_02200428(void *);
extern void ov15_021FF29C(void *, s32);
extern void ManagedSprite_SetAnimationFrame(void *, s32);
extern void ManagedSprite_SetAnim(void *, s32);
s32 ov15_021FBFC0(void *);
#endif
