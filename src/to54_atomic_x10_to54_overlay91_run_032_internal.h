#ifndef TO54_X10_PRIVATE_H
#define TO54_X10_PRIVATE_H
#include "global.h"
#pragma require_prototypes off
#define U8(p, o)  (*(u8 *)((u8 *)(p) + (o)))
#define S16(p, o) (*(s16 *)((u8 *)(p) + (o)))
#define S32(p, o) (*(s32 *)((u8 *)(p) + (o)))
void ov91_02261208(void *, void *, void *, void *, void *, void *);
#endif
