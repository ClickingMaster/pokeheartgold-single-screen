#ifndef TO52_OVERLAY82_R9_H
#define TO52_OVERLAY82_R9_H
#include "global.h"

#include "overlay_82_helpers_internal.h"
#include "system.h"

u8 ov80_02237920(u8);
void PlaySE(u16);
u16 ov82_0223F558(Ov82Work *);
u16 ov82_0223F570(Ov82Work *);
void ov82_0223FCBC(Ov82Spr *, s32, s32);
void ov82_0223FCFC(Ov82Spr *, u32);
void ov82_0223F300(Ov82Work *);
BOOL ov82_0223F488(Ov82Work *);
#endif
