#include "to56_r3_w01_private.h"
void BattlePartyMenu_SetLearnMoveCursorMask(u8 *raw) {
    Ov08Bank20Work *work = (Ov08Bank20Work *)raw;
    if (work->upper == 0) {
        BattleDpadCursor_SetEnabledMask(work->cursor, 0x5F);
    } else {
        BattleDpadCursor_SetEnabledMask(work->cursor, 0x7F);
    }
}
void BattlePartyMenu_SetForgetMoveCursorMask(u8 *raw) {
    Ov08Bank20Work *work = (Ov08Bank20Work *)raw;
    if (work->upper == 0) {
        BattleDpadCursor_SetEnabledMask(work->cursor, 5);
    } else {
        BattleDpadCursor_SetEnabledMask(work->cursor, 7);
    }
}
