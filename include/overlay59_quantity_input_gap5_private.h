#ifndef TO50_OV59_GAP5_PRIVATE_H
#define TO50_OV59_GAP5_PRIVATE_H
#include "global.h"
typedef struct Ov59Gap5Work {
    u8 raw[0x254];
} Ov59Gap5Work;
void PlaySE(u32 seq);
void ov59_02238834(Ov59Gap5Work *, int, int);
void ov59_02238A04(Ov59Gap5Work *, int);
int ov59_0223A564(int value, int max, int delta);
int ov59_0223A48C(Ov59Gap5Work *, int input);
#endif
