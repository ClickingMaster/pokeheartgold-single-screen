#include "to56_w04_overlay08_bank06_private.h"

#pragma force_active on

BOOL BattlePartyMenu_HandlePartyListSelection(void *work) {
    int result = BattlePartyMenu_CheckTouchHitbox(work, sBattlePartyPartyListHitboxes);
    if (result == -1) {
        result = BattleDpadCursor_HandleInput(*(void **)((u8 *)work + 0x2088));
        if (result == -2) {
            result = 6;
        } else if (result == -1) {
            return FALSE;
        }
        if (result != 6 && !BattlePartyMenu_GetSlotType(work, result)) {
            goto fail;
        }
        (*(u8 **)work)[0x11] = result;
        return TRUE;
    }
    if (result != 6 && !BattlePartyMenu_GetSlotType(work, result)) {
        goto fail;
    }
    (*(u8 **)work)[0x11] = result;
    BattlePartyMenu_DeactivateCursor(work);
    return TRUE;
fail:
    return FALSE;
}

#define DEFINE_TOUCH_FN(name, table, fallback)                       \
    u8 name(void *work) {                                            \
        int result = BattlePartyMenu_CheckTouchHitbox(work, table);                     \
        if (result == -1) {                                          \
            result = BattleDpadCursor_HandleInput(*(void **)((u8 *)work + 0x2088)); \
            if (result == -2) {                                      \
                result = fallback;                                   \
            } else if (result == -1) {                               \
                return 0xFF;                                         \
            }                                                        \
        } else {                                                     \
            BattlePartyMenu_DeactivateCursor(work);                                     \
        }                                                            \
        return result;                                               \
    }

DEFINE_TOUCH_FN(BattlePartyMenu_GetMonMenuInput, sBattlePartyMonMenuHitboxes, 3)
DEFINE_TOUCH_FN(BattlePartyMenu_GetSummaryInput, sBattlePartySummaryHitboxes, 3)
DEFINE_TOUCH_FN(BattlePartyMenu_GetCheckMovesInput, sBattlePartyCheckMovesHitboxes, 7)
DEFINE_TOUCH_FN(BattlePartyMenu_GetMoveDetailInput, sBattlePartyMoveDetailHitboxes, 4)
