#ifndef TO50_OV15_R19_PRIVATE_H
#define TO50_OV15_R19_PRIVATE_H
#include "global.h"
void ov15_022004DC(void *, u32);
void ov15_021FF29C(void *, u32);
void ScheduleWindowCopyToVram(void *);
void ManagedSprite_SetAnimationFrame(void *, u32);
void ManagedSprite_SetAnim(void *, u32);
void ov15_021FF844(void *);
void ov15_021FD788(void *, u32);
int ov15_021FD7D0(void *, u8, u8, u8, void *);
int ov15_021FD810(void *, u8, u32, void *);
BOOL ManagedSprite_IsAnimated(void *);
void ManagedSprite_SetPaletteOverride(void *, u32);
int ov15_021FD850(void *);
#endif
