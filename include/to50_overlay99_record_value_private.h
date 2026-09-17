#ifndef TO50_OV99_R6_PRIVATE_H
#define TO50_OV99_R6_PRIVATE_H
#include "global.h"
typedef struct Ov99R6Work {
    u8 pad00[0x14];
    u16 values[493];
    u8 pad3ee[6];
    u32 packed;
} Ov99R6Work;
u16 ov99_021E71B0(const Ov99R6Work *work);
#endif
