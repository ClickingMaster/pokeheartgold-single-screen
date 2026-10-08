#pragma force_active on
#include "global.h"
#include "sprite_system.h"
#include "item.h"
#include "pokemon_icon_idx.h"
typedef struct MonRecord05 {
    u16 species;
    u8 unk02[4];
    u16 item;
    u8 unk08[3];
    u8 isEgg;
    u8 hasCapsule;
    u8 form;
    u8 unk0E[2];
    int status;
    u8 unk14[4];
} MonRecord05;
typedef struct Work05 {
    u8 unk000[0x198];
    ManagedSprite *sprites[32];
    MonRecord05 records[6];
} Work05;
typedef struct SpriteSrc05 {
    u8 data[0x1C];
} SpriteSrc05;
extern const SpriteSrc05 ov05_0221EF2C[];
extern const u8 ov05_0221EC5C[][5][2];
ManagedSprite *ov05_0221CDC4(Work05 *work, const SpriteSrc05 *template);
int ov05_0221E0A8(Work05 *work, u32 slot);
void ov05_0221E110(Work05 *work, u32 slot);
// @fn ov05_0221E110
void ov05_0221E110(Work05 *work, u32 slot) {
    u32 i;
    for (i = 0; i < 5; i++) {
        work->sprites[slot * 5 + i] = ov05_0221CDC4(work, &ov05_0221EF2C[slot * 5 + i]);
        ManagedSprite_SetPositionXY(work->sprites[slot * 5 + i], ov05_0221EC5C[slot][i][0], ov05_0221EC5C[slot][i][1]);
    }
    if (work->records[slot].species == 0) {
        for (i = 0; i < 5; i++) {
            ManagedSprite_SetDrawFlag(work->sprites[slot * 5 + i], FALSE);
        }
        return;
    }
    ManagedSprite_SetPaletteOverride(work->sprites[slot * 5], GetMonIconPaletteEx(work->records[slot].species, work->records[slot].form, work->records[slot].isEgg));
    ManagedSprite_SetAnim(work->sprites[slot * 5], ov05_0221E0A8(work, slot));
    if (work->records[slot].item == 0) {
        ManagedSprite_SetDrawFlag(work->sprites[slot * 5 + 1], FALSE);
    } else if (ItemIdIsMail(work->records[slot].item) == TRUE) {
        ManagedSprite_SetAnim(work->sprites[slot * 5 + 1], 1);
    } else {
        ManagedSprite_SetAnim(work->sprites[slot * 5 + 1], 0);
    }
    if (work->records[slot].hasCapsule == 0) {
        ManagedSprite_SetDrawFlag(work->sprites[slot * 5 + 2], FALSE);
    } else {
        ManagedSprite_SetAnim(work->sprites[slot * 5 + 2], 2);
    }
    if (work->records[slot].status == 7) {
        ManagedSprite_SetDrawFlag(work->sprites[slot * 5 + 3], FALSE);
    } else {
        ManagedSprite_SetAnim(work->sprites[slot * 5 + 3], work->records[i].status);
    }
}
