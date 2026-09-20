#include "overlay08_exact_bank_private.h"

#pragma force_active on

BOOL ov08_0221DAE4(void *work) {
    u32 type = BattleSystem_GetBattleType(*(void **)(*(u8 **)work + 8));
    if (type != 0x4A && type != 0x4B && (type & 0x12) != 0) {
        return TRUE;
    }
    return FALSE;
}

BOOL ov08_0221DB04(void *work) {
    u32 type = BattleSystem_GetBattleType(*(void **)(*(u8 **)work + 8));
    if (type != 0x4A && type != 0x4B && (type & 8) != 0) {
        return TRUE;
    }
    return FALSE;
}
