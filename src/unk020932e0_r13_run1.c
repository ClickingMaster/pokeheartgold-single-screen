#include "unk020932e0_r13_private.h"

int sub_020954CC(Residual13Work *work) {
    VecFx32 matrix;
    int result = 0;
    switch (GridCursor_GetPanel(work->cursor)) {
    case 4:
        PlaySE(0x5DC);
        sub_02094A90(work, result);
        break;
    case 1:
        sub_0209569C(work);
        result = 3;
        break;
    default:
        if (work->enabled != 0) {
            GridCursor_SetPosition(work->cursor, 1, result);
            matrix.x = 0xE0000;
            matrix.y = 0xB0000;
            matrix.z = 0;
            Sprite_SetMatrix(work->sprite8C0, &matrix);
            PlaySE(0x5DC);
        }
        break;
    }
    return result;
}
