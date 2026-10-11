#include "to56_w04_overlay08_bank06_private.h"

#pragma force_active on

BOOL ov08_0221D438(void *work) {
    int result = ov08_0221D5D0(work, ov08_02224F1C);
    if (result == -1) {
        result = BattleDpadCursor_HandleInput(*(void **)((u8 *)work + 0x2088));
        if (result == -2) {
            result = 6;
        } else if (result == -1) {
            return FALSE;
        }
        if (result != 6 && !ov08_0221D5DC(work, result)) {
            goto fail;
        }
        (*(u8 **)work)[0x11] = result;
        return TRUE;
    }
    if (result != 6 && !ov08_0221D5DC(work, result)) {
        goto fail;
    }
    (*(u8 **)work)[0x11] = result;
    ov08_022217C8(work);
    return TRUE;
fail:
    return FALSE;
}

#define DEFINE_TOUCH_FN(name, table, fallback)                       \
    u8 name(void *work) {                                            \
        int result = ov08_0221D5D0(work, table);                     \
        if (result == -1) {                                          \
            result = BattleDpadCursor_HandleInput(*(void **)((u8 *)work + 0x2088)); \
            if (result == -2) {                                      \
                result = fallback;                                   \
            } else if (result == -1) {                               \
                return 0xFF;                                         \
            }                                                        \
        } else {                                                     \
            ov08_022217C8(work);                                     \
        }                                                            \
        return result;                                               \
    }

DEFINE_TOUCH_FN(ov08_0221D4B0, ov08_02224E54, 3)
DEFINE_TOUCH_FN(ov08_0221D4F8, ov08_02224E68, 3)
DEFINE_TOUCH_FN(ov08_0221D540, ov08_02224F5C, 7)
DEFINE_TOUCH_FN(ov08_0221D588, ov08_02224E7C, 4)
