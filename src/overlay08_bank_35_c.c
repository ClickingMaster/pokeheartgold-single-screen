#include "overlay08_exact_bank_private.h"

#pragma force_active on

void ov08_0222417C(u8 *work) {
    Ov08SmallState *state = *(Ov08SmallState **)(work + 0x34);
    ov08_02224B90(state, 0);
    ov08_02224BC0(*(Ov08SmallState **)(work + 0x34));
    BattleCursor_Disable(ov08_02224B84(*(Ov08SmallState **)(work + 0x34)));
}
