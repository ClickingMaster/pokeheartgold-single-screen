#ifndef TO50_OV15_R20_PRIVATE_H
#define TO50_OV15_R20_PRIVATE_H
#include "global.h"
#define U16(p, o) (*(u16 *)((u8 *)(p) + (o)))
#define U32(p, o) (*(u32 *)((u8 *)(p) + (o)))
#define S32(p, o) (*(s32 *)((u8 *)(p) + (o)))
typedef struct Ov15InterpRec {
    u16 a;
    u16 b;
    u32 pad4;
    s32 c;
    s32 d;
} Ov15InterpRec;
extern const u8 ov15_02200790[];
void ov15_021FDAF4(void *, s32, s32);
void ov15_021FDB2C(void *, s32);
#endif
