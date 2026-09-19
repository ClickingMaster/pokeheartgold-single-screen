#ifndef TO50_OV59_R10_GAP3_PRIVATE_H
#define TO50_OV59_R10_GAP3_PRIVATE_H
#include "global.h"

#include "system.h"
typedef struct Ov59R10Gap3Work {
    u8 raw[0x254];
} Ov59R10Gap3Work;
int ov59_0223B2B4(Ov59R10Gap3Work *, int);
int ov59_0223B358(Ov59R10Gap3Work *);
#endif
