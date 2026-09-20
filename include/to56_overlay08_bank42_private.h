#ifndef TO56_R2_W02_PRIVATE_H
#define TO56_R2_W02_PRIVATE_H
#include "global.h"

#include "system.h"
typedef struct Ov08MenuEntry {
    u8 values[4];
    u8 flags[4];
} Ov08MenuEntry;
BOOL ov08_02224C48(Ov08MenuEntry *, u32);
#endif
typedef struct Ov08MenuWork {
    void *callbackArg;
    Ov08MenuEntry *entries;
    u8 done;
    u8 index;
    u8 previous;
    u8 pad0B;
    u32 disabled;
} Ov08MenuWork;
BOOL ov08_02224BFC(Ov08MenuWork *);
void ov12_0226BAFC(void *, u8, u8, u8, u8);
void PlaySE(u16);
