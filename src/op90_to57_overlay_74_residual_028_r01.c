#pragma force_active on
#include "global.h"

extern u32 ov74_0223D450;
extern u32 ov74_0223D454[];

// ov74_0223D454 (0xEA8-byte survey work area) directly follows ov74_0223D450. The original file owned the work area,
// so its literal pool holds the address twice; the first entry is reached here through the preceding label.
#define SURVEY_WORK (&ov74_0223D450 + 1)

void ov74_022352A0(u32 value);

// @fn ov74_022352A0
void ov74_022352A0(u32 value)
{
    memset(SURVEY_WORK, 0, 0xEA8);
    ov74_0223D454[1] = 1;
    ov74_0223D454[2] = value;
}
