#include "to56_w10_overlay07_private.h"

void ov07_02223EB4(void *owner) {
    FadeWork *w = BattleAnimUtil_AllocWork(owner, 8);
    w->owner = owner;
    w->palette = ov07_0221FA78(owner);
    u16 opaque;
    switch (((s32)ov07_0221C4A8(owner, 0))) {
    case 0:
        opaque = ov07_0221E6C8(owner);
        break;
    case 1:
        opaque = 0x100;
        break;
    case 2:
        opaque = 0x200;
        break;
    default:
        GF_AssertFail();
        break;
    }
    int wait = ((s32)ov07_0221C4A8(owner, 1));
    int cur = ((s32)ov07_0221C4A8(owner, 2));
    int end = ((s32)ov07_0221C4A8(owner, 3));
    int color = ((s32)ov07_0221C4A8(owner, 4));
    PaletteData_BeginPaletteFade(w->palette, 1, opaque, (s8)wait, (u8)cur, (u8)end, (u16)color);
    ov07_0221C410(w->owner, ov07_02223E94, w);
}

void ov07_02223F5C(void *task, FadeWork *w) {
    switch (w->state) {
    case 0:
        Pokepic_StartPaletteFade(w->palette, 0, w->unk18, w->unk10, w->unk14);
        w->state++;
        break;
    case 1:
        if (!Pokepic_ResumePaletteFade(w->palette)) {
            w->state++;
        }
        break;
    case 2:
        if (w->delay == 0) {
            w->state++;
        } else {
            w->delay--;
            break;
        }
        /* fallthrough */
    case 3:
        Pokepic_StartPaletteFade(w->palette, w->unk18, 0, w->unk10, w->unk14);
        w->state++;
        break;
    case 4:
        if (!Pokepic_ResumePaletteFade(w->palette)) {
            if (--w->count <= 0) {
                w->state++;
            } else {
                w->state = 0;
            }
        }
        break;
    case 5:
        Heap_Free(w);
        ov07_0221C448(w->owner, task);
        break;
    }
}

void ov07_02224008(void *owner) {
    FadeWork *w = BattleAnimUtil_AllocWork(owner, 0x20);
    w->owner = owner;
    w->palette = NULL;
    int kind = ((s32)ov07_0221C4A8(owner, 0));
    int i, slot;
    switch (kind) {
    case 2:
        w->palette = ov07_0221FA48(w->owner, ov07_0221C468(owner));
        break;
    case 4:
        if (ov07_0221FAB0(owner) == 1) {
            w->palette = ov07_0221FA48(w->owner, BattleAnimUtil_GetPartnerBattler(w->owner, ov07_0221C468(w->owner)));
        }
        break;
    case 8:
        w->palette = ov07_0221FA48(w->owner, ov07_0221C470(owner));
        break;
    case 16:
        if (ov07_0221FAB0(owner) == 1) {
            w->palette = ov07_0221FA48(w->owner, BattleAnimUtil_GetPartnerBattler(w->owner, ov07_0221C470(w->owner)));
        }
        break;
    case 0x802:
        slot = 0xFF;
        for (i = 0; i < 4; i++) {
            int x = ov07_0221FA04(owner, i);
            if (x == 0 || x == 2) {
                slot = i;
                break;
            }
        }
        if (slot == 0xFF) {
            slot = 0;
        }
        w->palette = ov07_0221FA48(w->owner, slot);
        break;
    case 0x804:
        slot = 0xFF;
        for (i = 0; i < 4; i++) {
            if (ov07_0221FA04(owner, i) == 4) {
                slot = i;
                break;
            }
        }
        if (slot == 0xFF) {
            slot = 0;
        }
        w->palette = ov07_0221FA48(w->owner, slot);
        break;
    case 0x808:
        slot = 0xFF;
        for (i = 0; i < 4; i++) {
            int x = ov07_0221FA04(owner, i);
            if (x == 1 || x == 3) {
                slot = i;
                break;
            }
        }
        if (slot == 0xFF) {
            slot = 0;
        }
        w->palette = ov07_0221FA48(w->owner, slot);
        break;
    case 0x810:
        slot = 0xFF;
        for (i = 0; i < 4; i++) {
            if (ov07_0221FA04(owner, i) == 5) {
                slot = i;
                break;
            }
        }
        if (slot == 0xFF) {
            slot = 0;
        }
        w->palette = ov07_0221FA48(w->owner, slot);
        break;
    default:
        GF_AssertFail();
        break;
    }
    if (w->palette == NULL) {
        Heap_Free(w);
        return;
    }
    w->unk10 = ((s32)ov07_0221C4A8(owner, 1));
    w->count = ((s32)ov07_0221C4A8(owner, 2));
    w->unk14 = ((s32)ov07_0221C4A8(owner, 3));
    w->unk18 = ((s32)ov07_0221C4A8(owner, 4));
    w->delay = ((s32)ov07_0221C4A8(owner, 5));
    ov07_0221C410(w->owner, ov07_02223F5C, w);
}

void ov07_022241D8(void *task, ScaleWork *w) {
    u32 sx, sy;
    int done;
    switch (w->state) {
    case 0:
        ov07_02222508(w->anim, (s16)w->b, (s16)w->d, (s16)w->c, w->mode >> 16);
        ov07_02222644(w->anim, &sx, &sy);
        if (w->flag == 1) {
            sx = _fsub(0, sx);
        }
        ManagedSprite_SetAffineScale(w->sprite, sx, sy);
        w->state++;
        break;
    case 1:
        done = ov07_02222558(w->anim);
        ov07_02222644(w->anim, &sx, &sy);
        if (w->flag == 1) {
            sx = _fsub(0, sx);
        }
        ManagedSprite_SetAffineScale(w->sprite, sx, sy);
        if (done) {
            ov07_022226FC(w->sprite, w->x, w->y, *(int *)((u8 *)w + 0x28), 0);
        } else {
            w->state++;
        }
        break;
    case 2:
        ov07_02222508(w->anim, (s16)w->c, (s16)w->d, (s16)w->b, (u16)w->mode);
        w->state++;
        break;
    case 3:
        done = ov07_02222558(w->anim);
        ov07_02222644(w->anim, &sx, &sy);
        if (w->flag == 1) {
            sx = _fsub(0, sx);
        }
        ManagedSprite_SetAffineScale(w->sprite, sx, sy);
        if (done) {
            ov07_022226FC(w->sprite, w->x, w->y, *(int *)((u8 *)w + 0x28), 0);
        } else if (--w->count <= 0) {
            w->state++;
        } else {
            w->state = 0;
        }
        break;
    case 4:
        Heap_Free(w);
        ov07_0221C448(w->owner, task);
        return;
    }
    SpriteSystem_DrawSprites(w->spriteSystem);
}

void ov07_02224348(void *owner) {
    ScaleWork *w = BattleAnimUtil_AllocWork(owner, sizeof(ScaleWork));
    w->owner = owner;
    w->spriteSystem = ov07_0221C514(owner);
    void *pic;
    if (((s32)ov07_0221C4A8(owner, 0)) == 0) {
        pic = ov07_0221FA48(w->owner, ov07_0221C468(owner));
        w->y = ov07_0221FAA0(w->owner, ov07_0221C468(w->owner));
    } else {
        pic = ov07_0221FA48(w->owner, ov07_0221C470(w->owner));
        w->y = ov07_0221FAA0(w->owner, ov07_0221C470(w->owner));
    }
    w->x = Pokepic_GetAttr(pic, 1);
    w->x -= Pokepic_GetAttr(pic, 0x29);
    w->a = ((s32)ov07_0221C4A8(owner, 1));
    w->b = ((s32)ov07_0221C4A8(owner, 2));
    w->c = ((s32)ov07_0221C4A8(owner, 3));
    w->d = ((s32)ov07_0221C4A8(owner, 4));
    w->count = ((s32)ov07_0221C4A8(owner, 5));
    w->mode = ((s32)ov07_0221C4A8(owner, 6));
    w->sprite = (void *)ov07_0221C4E8(w->owner, ((s32)ov07_0221C4A8(owner, 7)));
    ManagedSprite_SetDrawPriority(w->sprite, 100);
    ManagedSprite_SetPriority(w->sprite, 1);
    ManagedSprite_SetOamMode(w->sprite, 1);
    ManagedSprite_SetAffineOverwriteMode(w->sprite, 2);
    BattleAnimUtil_SetBlendWithBattleBg(w->owner, w->a, 16 - w->a);
    ov07_0221C410(w->owner, ov07_022241D8, w);
    int flag = ((s32)ov07_0221C4A8(owner, 0)) != 0;
    if (ov07_0221FB78(w->owner, flag) == 1) {
        w->flag = 1;
    }
}

void ov07_0222446C(void *task, MoveWork *w) {
    switch (w->state) {
    case 0:
        ov07_02222590(w->anim, (s16)w->a, (s16)w->b, (s16)w->c, (s16)w->d, (s16)w->e, w->packed >> 16);
        ov07_02222768(w->anim, w->pic);
        w->state++;
        break;
    case 1:
        if (ov07_02222788(w->anim, w->pic)) {
            ov07_022226C4(w->pic, w->x, w->y, *(int *)((u8 *)w + 0x24), 0);
        } else {
            w->state++;
        }
        break;
    case 2:
        if (w->hold == 0) {
            w->state++;
        } else {
            w->hold--;
            break;
        }
        /* fallthrough */
    case 3:
        ov07_02222590(w->anim, (s16)w->b, (s16)w->a, (s16)w->d, (s16)w->c, (s16)w->e, (u16)w->packed);
        ov07_02222788(w->anim, w->pic);
        w->state++;
        break;
    case 4:
        if (ov07_02222788(w->anim, w->pic)) {
            ov07_022226C4(w->pic, w->x, w->y, *(int *)((u8 *)w + 0x24), 0);
        } else if (--w->delay <= 0) {
            w->state++;
        } else {
            w->state = 0;
        }
        break;
    case 5:
        ov07_02223224(w->pic);
        Heap_Free(w);
        ov07_0221C448(w->owner, task);
        break;
    }
}
