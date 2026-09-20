#ifndef TO56_R2_W21_OVERLAY120_GRID_STEP_PRIVATE_H
#define TO56_R2_W21_OVERLAY120_GRID_STEP_PRIVATE_H

#define ov120_0225F040 Tracked_ov120_0225F040
#include "overlay120_helpers_internal.h"
#undef ov120_0225F040

typedef struct Ov120GridWork {
    u8 pad00[0xcb];
    u8 flip : 1;
    u8 padFlags : 7;
} Ov120GridWork;

extern void ov120_0225F040(void *, int, int, int, void *, void *, void *, void *, void *, u8);

extern const u8 ov120_022602E4[];
extern const u8 ov120_022602FC[];
extern const u8 ov120_02260314[];
extern const u8 ov120_0226032C[];

int ov120_0225F318(void *work);
int ov120_0225F4DC(void *work);

#endif
