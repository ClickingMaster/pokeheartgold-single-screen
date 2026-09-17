#ifndef TO50_OV65_R14_PRIVATE_H
#define TO50_OV65_R14_PRIVATE_H
#include "global.h"
typedef struct Ov65StateR14 {
    u8 pad0[0x36B8];
    void *field36B8;
    s32 field36BC;
} Ov65StateR14;
extern void sub_02039AD8(s32);
void ov65_0221F864(void *);
void ov65_0221F890(void);
#endif
