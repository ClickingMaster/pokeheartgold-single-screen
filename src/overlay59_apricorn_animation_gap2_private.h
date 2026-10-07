#ifndef TO50_OV59_R10_GAP2_PRIVATE_H
#define TO50_OV59_R10_GAP2_PRIVATE_H
#include "global.h"
typedef struct Ov59R10Gap2Work {
    u8 raw[0x254];
} Ov59R10Gap2Work;
extern const u32 ov59_0223C99C[];
void PlaySE(u32);
void ov59_0223BE70(Ov59R10Gap2Work *, u8, int);
void ov59_0223BD4C(void *, void *, u8);
void ov59_0223BBB0(Ov59R10Gap2Work *);
void ov59_0223BBD4(Ov59R10Gap2Work *, int);
BOOL TextPrinterCheckActive(u8);
BOOL ov59_0223B164(Ov59R10Gap2Work *);
#endif
