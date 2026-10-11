#include "unk020932e0_r13_private.h"

void sub_020955EC(Residual13Work *work) {
    VecFx32 matrix;
    u8 value;
    u8 remainder;

    work->selection++;
    if (work->selection == work->selectionCount) {
        work->selection = 0;
    }
    Sprite_SetAnimCtrlSeq(work->sprite8B8, 3);
    sub_020943EC(work);
    PlaySE(0x5DC);
    if (work->selection == 0x12) {
        if (GridCursor_GetPanel(work->cursor) == 2) {
            remainder = GridCursor_GetIndex(work->cursor) % 6;
            GridCursor_SetPosition(work->cursor, 6, remainder);
            matrix.x = (remainder * 24 + 20) << FX32_SHIFT;
            matrix.y = 0x28000;
            matrix.z = 0;
            Sprite_SetMatrix(work->sprite8C0, &matrix);
        }
    } else if (GridCursor_GetPanel(work->cursor) == 6) {
        value = GridCursor_GetIndex(work->cursor);
        GridCursor_SetPosition(work->cursor, 2, value);
    }
}
