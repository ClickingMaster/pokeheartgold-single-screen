#include "to56_overlay08_bank34_private.h"
#pragma force_active on

void ov08_02223F94(u8 *work, u32 mode) {
    u32 i = 0;
    u8 *cursor = work;
    for (; i < 6; i++) {
        ManagedSprite_SetDrawFlag(PTR(cursor, 0x310), FALSE);
        cursor += 4;
    }
    switch (mode) {
    case 0:
        ov08_02223FD8(work);
        break;
    case 1:
        ov08_0222400C(work);
        break;
    case 2:
        ov08_02224064(work);
        break;
    }
}

void ov08_02223FD8(u8 *work) {
    u16 value = U16(PTR(work, 0), 0x20);
    if (value != 0) {
        ov08_02223E3C(work, value, 0xB4B7);
        ov08_02223E74(work, U16(PTR(work, 0), 0x20), 0, 0xB4B7);
        ov08_02223F74(PTR(work, 0x310), 0x18, 0xB2);
    }
}

void ov08_0222400C(u8 *work) {
    const s32(*pos)[2] = ov08_02225D74;
    u32 i = 0;
    u8 *spriteWork = work;
    for (; i < 6; i++) {
        u32 value = ov08_02223CD4(work, i);
        if (value != 0) {
            ov08_02223E3C(work, value, 0xB4B7 + i);
            ov08_02223E74(work, value, (u16)i, 0xB4B7 + i);
            ov08_02223F74(PTR(spriteWork, 0x310), (*pos)[0], (*pos)[1]);
        }
        pos++;
        spriteWork += 4;
    }
}

void ov08_02224064(u8 *work) {
    u8 *entry = (u8 *)PTR(work, 0) + U8(work, 0x114D);
    u32 value = ov08_02223CD4(work, entry[0x27]);
    ov08_02223E3C(work, value, 0xB4B7);
    ov08_02223E74(work, value, 0, 0xB4B7);
    ov08_02223F74(PTR(work, 0x310), 0x28, 0x2C);
}

void ov08_022240A8(u8 *work) {
    void *spriteSystem = BattleSystem_GetSpriteSystem(PTR(PTR(work, 0), 0));
    BattleCursor_LoadResources(spriteSystem, PTR(work, 0x30C), PTR(work, 8), *(u32 *)((u8 *)PTR(work, 0) + 0xC), 0xB4BE, 0xB4BE, 0xB4B9, 0xB4B9);
    BattleDpadCursor_SetBattleCursor(PTR(work, 0x34), BattleCursor_New(spriteSystem, PTR(work, 0x30C), *(u32 *)((u8 *)PTR(work, 0) + 0xC), 0xB4BE, 0xB4BE, 0xB4B9, 0xB4B9, 0, 1));
}

void ov08_02224108(u8 *work) {
    BattleCursor_Delete(BattleDpadCursor_GetBattleCursor(PTR(work, 0x34)));
    BattleCursor_FreeResources(PTR(work, 0x30C), 0xB4BE, 0xB4BE, 0xB4B9, 0xB4B9);
}
