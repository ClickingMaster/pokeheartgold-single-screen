#include "to56_overlay08_bank16_private.h"

void ov08_02220AAC(Ov08Work16 *work) {
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

void ov08_02220AEC(Ov08Work16 *work, ManagedSprite *sprite, int resId, u32 type) {
    SpriteSystem *sys = BattleSystem_GetSpriteSystem(work->root->battleSystem);
    int narc = sub_020776B4();
    int file = sub_02077678(type);
    SpriteSystem_ReplaceCharResObj(sys, work->spriteManager, narc, file, TRUE, resId);
    ManagedSprite_SetPaletteOverride(sprite, sub_0207769C(type) + 4);
}

void ov08_02220B3C(Ov08Work16 *work, ManagedSprite *sprite, u32 value) {
    SpriteSystem *sys = BattleSystem_GetSpriteSystem(work->root->battleSystem);
    int narc = sub_02077830();
    int file = sub_02077800(value);
    SpriteSystem_ReplaceCharResObj(sys, work->spriteManager, narc, file, TRUE, 0xB015);
    ManagedSprite_SetPaletteOverride(sprite, sub_02077818(value) + 4);
}

void ov08_02220B90(int anim, ManagedSprite *sprite, int x, int y) {
    if (anim != 7) {
        ManagedSprite_SetAnim(sprite, anim);
        ov08_02220A8C(sprite, x, y);
    }
}

void ov08_02220BB4(Ov08Work16 *work, Ov08RecBB4 *rec, const Ov08Pair16 *pair) {
    ov08_02220AEC(work, work->sprite2004, 0xB00E, rec->first);
    ov08_02220A8C(work->sprite2004, pair->a, pair->b);
    if (rec->first != rec->second) {
        ov08_02220AEC(work, work->sprite2008, 0xB00F, rec->second);
        ov08_02220A8C(work->sprite2008, pair->c, pair->d);
    }
}

void ov08_02220C08(u16 item, ManagedSprite *sprite, int x, int y) {
    if (item != 0) {
        if (ItemIdIsMail(item) == TRUE) {
            ManagedSprite_SetAnim(sprite, 1);
        } else {
            ManagedSprite_SetAnim(sprite, 0);
        }
        ov08_02220A8C(sprite, x, y);
    }
}
