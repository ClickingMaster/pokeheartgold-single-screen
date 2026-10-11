#include "global.h"
extern const u8 *const sBattlePartyButtonWindowLists[];
extern const u8 sBattlePartySlotButtonScrollAmounts[];
extern const u8 sBattlePartyButtonScrollAmounts[];
extern void ScrollWindow(void *window, int direction, int amount, int unused);
extern void ScheduleWindowCopyToVram(void *window);
void BattlePartyMenu_ScrollButtonWindows(u8 *work, unsigned int type, int mode);
void BattlePartyMenu_ScrollButtonWindows(u8 *work, unsigned int type, int mode) {
    const u8 *indices = sBattlePartyButtonWindowLists[type];
    u16 i;
    int amount;
    int direction;
    if (indices != 0) {
        if (type <= 5) {
            amount = sBattlePartySlotButtonScrollAmounts[mode];
        } else {
            amount = sBattlePartyButtonScrollAmounts[mode];
        }
        switch (mode) {
        case 0: direction = 1; break;
        case 1: direction = 0; break;
        case 2: direction = 1; break;
        }
        if (type >= 14 && type <= 17) {
            ScrollWindow((void *)(*(u32 *)(work + 0x2070) + (indices[work[0x2075]] << 4)), direction, amount, 0);
            ScheduleWindowCopyToVram((void *)(*(u32 *)(work + 0x2070) + (indices[work[0x2075]] << 4)));
            return;
        }
        for (i = 0; i < 8; i++) {
            if (indices[i] == 0xFF) break;
            ScrollWindow((void *)(*(u32 *)(work + 0x2070) + (indices[i] << 4)), direction, amount, 0);
            ScheduleWindowCopyToVram((void *)(*(u32 *)(work + 0x2070) + (indices[i] << 4)));
        }
    }
}

extern const s16 sBattlePartyButtonSpriteOffsets[];
extern const s16 sBattlePartySlotButtonSpriteOffsets[];
extern void ManagedSprite_OffsetPositionXY(void *sprite, int x, int y);
void BattlePartyMenu_OffsetButtonSprites(u8 *work, unsigned int type, int mode);
void BattlePartyMenu_OffsetButtonSprites(u8 *work, unsigned int type, int mode) {
    int delta;
    switch (type) {
    case 0: case 1: case 2: case 3: case 4: case 5:
        delta = sBattlePartySlotButtonSpriteOffsets[mode];
        ManagedSprite_OffsetPositionXY(*(void **)(work + 0x1FB8 + (type + 13) * 4), 0, delta);
        ManagedSprite_OffsetPositionXY(*(void **)(work + 0x1FB8 + type * 4), 0, delta);
        ManagedSprite_OffsetPositionXY(*(void **)(work + 0x1FB8 + (type + 7) * 4), 0, delta);
        break;
    case 7:
        delta = sBattlePartyButtonSpriteOffsets[mode];
        ManagedSprite_OffsetPositionXY(*(void **)(work + 0x1FB8 + (*(u8 **)work)[0x11] * 4), 0, delta);
        ManagedSprite_OffsetPositionXY(*(void **)(work + 0x1FB8 + ((*(u8 **)work)[0x11] + 7) * 4), 0, delta);
        break;
    case 14: case 15: case 16: case 17:
        ManagedSprite_OffsetPositionXY(*(void **)(work + 0x1FB8 + (type + 7) * 4), 0, sBattlePartyButtonSpriteOffsets[mode]);
        break;
    case 19: case 20: case 21: case 22:
        ManagedSprite_OffsetPositionXY(*(void **)(work + 0x1FB8 + (type + 2) * 4), 0, sBattlePartyButtonSpriteOffsets[mode]);
        break;
    case 23: case 24: case 25: case 26: case 27:
        ManagedSprite_OffsetPositionXY(*(void **)(work + 0x1FB8 + (type - 2) * 4), 0, sBattlePartyButtonSpriteOffsets[mode]);
        break;
    }
}
