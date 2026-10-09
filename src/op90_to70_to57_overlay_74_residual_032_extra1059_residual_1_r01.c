#pragma force_active on
#include "global.h"

extern u32 ov74_0223D450;
extern int ov74_0223D454[];
extern void BeginNormalPaletteFade(int, int, int, int, int, int, int);

// See ov74_022352A0: second literal-pool entry for the work area, reached through the preceding label.
#define SURVEY_WORK ((int *)(&ov74_0223D450 + 1))

void ov74_0223539C(int mode, int next, int *out, int value);

// @fn ov74_0223539C
void ov74_0223539C(int mode, int next, int *out, int value)
{
    int *work = SURVEY_WORK;
    if (ov74_0223D454[4] == 0) {
        BeginNormalPaletteFade(0, mode, mode, 0, 6, 1, work[2]);
    } else {
        BeginNormalPaletteFade(0, mode, mode, 0x7FFF, 6, 1, work[2]);
    }
    if (out != NULL) {
        *out = value;
    }
    work[3] = next;
}
