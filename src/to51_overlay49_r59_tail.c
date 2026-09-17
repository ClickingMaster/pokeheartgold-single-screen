#include "to51_overlay49_r59_tail_private.h"

BOOL ov49_022613AC(Ov49R59Small *work) {
    VecFx32 position;
    BOOL finished;
    fx32 offset;

    work->counter++;
    if (work->counter < 16) {
        finished = FALSE;
    } else {
        work->counter = 16;
        finished = TRUE;
    }

    offset = FX_Mul(FX_SinIdx((u16)(((work->counter % 8) * 0x7FFF) / 8)), FX32_CONST(3));
    ov49_02259154(work->object, &position);
    position.y += offset;
    ov49_02259148(work->object, &position);
    return finished;
}
