#ifndef OV08_BANK20_PRIVATE_H
#define OV08_BANK20_PRIVATE_H
#include "global.h"
typedef struct Ov08Bank20Work {
    u8 pad_0000[0x2077];
    u8 mode : 4;
    u8 upper : 4;
    u8 pad_2078[0x10];
    void *cursor;
} Ov08Bank20Work;
extern void *BattleDpadCursor_GetBattleCursor(const void *state);
extern void BattleDpadCursor_SetActive(void *state, u8 value);
extern void BattleDpadCursor_ResetPosition(void *state);
extern void BattleDpadCursor_SetEnabledMask(void *state, s32 value);
extern void BattleCursor_Disable(void *cursor);
void BattlePartyMenu_SetLearnMoveCursorMask(u8 *work);
void BattlePartyMenu_SetForgetMoveCursorMask(u8 *work);
void BattlePartyMenu_DeactivateCursor(u8 *work);
#endif
