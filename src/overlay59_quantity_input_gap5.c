#include "overlay59_quantity_input_gap5_private.h"
#define U8(w, off) (*(u8 *)((u8 *)(w) + (off)))
int ov59_0223A48C(Ov59Gap5Work *work, int input) {
    if (input == 0) {
        PlaySE(0x5DC);
        ov59_02238834(work, 0, 1);
        return 0;
    }
    if (input == 1) {
        PlaySE(0x5DC);
        ov59_02238834(work, 1, 1);
        return 1;
    }
    switch (input) {
    case 2:
        U8(work, 0x4E) = ov59_0223A564(U8(work, 0x4E), U8(work, 0x4F), 1);
        break;
    case 4:
        U8(work, 0x4E) = ov59_0223A564(U8(work, 0x4E), U8(work, 0x4F), 10);
        break;
    case 3:
        U8(work, 0x4E) = ov59_0223A564(U8(work, 0x4E), U8(work, 0x4F), -1);
        break;
    case 5:
        U8(work, 0x4E) = ov59_0223A564(U8(work, 0x4E), U8(work, 0x4F), -10);
        break;
    default:
        return 6;
    }
    PlaySE(0x5DC);
    ov59_02238A04(work, U8(work, 0x4E));
    return 6;
}
