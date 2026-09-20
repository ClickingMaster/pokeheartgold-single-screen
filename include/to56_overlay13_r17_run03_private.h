#ifndef TO56_W24_RUN1_H
#define TO56_W24_RUN1_H
#include "global.h"
#define Swap16(value) ((u16)((u8)((value) >> 8) | (((value) << 8) & 0xff00)))
u8 *ov13_02224694(u8 **, u8 *, u32 *, u32 *);
u8 *ov13_022246E4(u8 *, u32 *, u32 *);
#endif
