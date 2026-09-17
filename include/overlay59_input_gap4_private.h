#ifndef TO50_OV59_GAP4_PRIVATE_H
#define TO50_OV59_GAP4_PRIVATE_H

#include "global.h"

#include "touchscreen.h"

typedef struct Ov59Gap4Work {
    u8 raw[0x254];
} Ov59Gap4Work;

extern const TouchscreenHitbox ov59_0223C924[];

void PlaySE(u32 seq);
int ov59_0223A194(Ov59Gap4Work *work, u32 input);
void ov59_022388C4(Ov59Gap4Work *work);
void ov59_022387D0(Ov59Gap4Work *work, int value);
int ov59_0223A2D0(Ov59Gap4Work *work, int *hit);
int ov59_0223A350(Ov59Gap4Work *work, u32 input);

#endif
