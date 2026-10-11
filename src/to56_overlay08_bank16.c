#include "to56_overlay08_bank16_private.h"

void BattlePartyMenu_SetMonIconPalettes(Ov08Work16 *work) {
    Ov08Record16 *mon;
    u8 *flags;
    u8 *spriteOff;
    int i;
    mon = (Ov08Record16 *)work;
    flags = (u8 *)mon;
    i = 0;
    flags += 0x1B;
    spriteOff = (u8 *)mon;
    for (; i < 6; i++) {
        if (mon->species != 0) {
            ManagedSprite_SetPaletteOverride(*(ManagedSprite **)(spriteOff + 0x1FD4), GetMonIconPaletteEx(mon->species, mon->form_value, ((u32)*flags << 24) >> 31));
        }
        mon++;
        flags += 0x50;
        spriteOff += 4;
    }
}

void BattlePartyMenu_SetTypeIcon(Ov08Work16 *work, ManagedSprite *sprite, int resId, u32 type) {
    SpriteSystem *sys = BattleSystem_GetSpriteSystem(work->root->battleSystem);
    int narc = TypeIcon_GetNarc();
    int file = TypeIcon_GetCharFile(type);
    SpriteSystem_ReplaceCharResObj(sys, work->spriteManager, narc, file, TRUE, resId);
    ManagedSprite_SetPaletteOverride(sprite, TypeIcon_GetPalette(type) + 4);
}

void BattlePartyMenu_SetMoveCategoryIcon(Ov08Work16 *work, ManagedSprite *sprite, u32 value) {
    SpriteSystem *sys = BattleSystem_GetSpriteSystem(work->root->battleSystem);
    int narc = MoveCategoryIcon_GetNarc();
    int file = MoveCategoryIcon_GetCharFile(value);
    SpriteSystem_ReplaceCharResObj(sys, work->spriteManager, narc, file, TRUE, 0xB015);
    ManagedSprite_SetPaletteOverride(sprite, MoveCategoryIcon_GetPalette(value) + 4);
}

void BattlePartyMenu_ShowStatusIcon(int anim, ManagedSprite *sprite, int x, int y) {
    if (anim != 7) {
        ManagedSprite_SetAnim(sprite, anim);
        BattlePartyMenu_ShowSpriteAt(sprite, x, y);
    }
}

void BattlePartyMenu_ShowMonTypeIcons(Ov08Work16 *work, Ov08RecBB4 *rec, const Ov08Pair16 *pair) {
    BattlePartyMenu_SetTypeIcon(work, work->sprite2004, 0xB00E, rec->first);
    BattlePartyMenu_ShowSpriteAt(work->sprite2004, pair->a, pair->b);
    if (rec->first != rec->second) {
        BattlePartyMenu_SetTypeIcon(work, work->sprite2008, 0xB00F, rec->second);
        BattlePartyMenu_ShowSpriteAt(work->sprite2008, pair->c, pair->d);
    }
}

void BattlePartyMenu_ShowHeldItemIcon(u16 item, ManagedSprite *sprite, int x, int y) {
    if (item != 0) {
        if (ItemIdIsMail(item) == TRUE) {
            ManagedSprite_SetAnim(sprite, 1);
        } else {
            ManagedSprite_SetAnim(sprite, 0);
        }
        BattlePartyMenu_ShowSpriteAt(sprite, x, y);
    }
}
