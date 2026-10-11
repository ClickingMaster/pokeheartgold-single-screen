#include "to57_new35_overlay07_transition_private.h"

void ov07_0222CBFC(int task, W35 *w) {
    f32 x, y;
    switch (w->state) {
    case 0: {
        BOOL active = BattleAnimUtil_UpdateValueMotionFx(&w->scalar);
        *(vu16 *)0x04000052 = w->scalar.value | ((31 - w->scalar.value) << 8);
        if (active == 0) {
            w->state++;
            w->counter = 0;
        }
    } break;
    case 1:
        w->counter++;
        if (w->counter > 32) {
            w->state++;
        }
        break;
    case 2: {
        BOOL active = BattleAnimUtil_UpdateScaleMotion(&w->anim);
        BattleAnimUtil_GetScaleFloat(&w->anim, &x, &y);
        if (w->mirror == 1) {
            x = 0.0f - x;
        }
        ManagedSprite_SetAffineScale(w->sprite, x, y);
        if (active == 0) {
            ManagedSprite_SetDrawFlag(w->sprite, 0);
            w->state++;
        }
        break;
    }
    case 3:
        BattleAnimSystem_DestroyTask(w->owner, task);
        Heap_Free(w);
        return;
    }
    SpriteSystem_DrawSprites(w->spriteSystem);
}

void ov07_0222CCBC(int task, W35Fade *w) {
    switch (w->state) {
    case 0:
        BattleAnimUtil_UpdateShake(&w->motion);
        BattleAnimUtil_SetPokepicPosWithOffset(&w->motion, w->pic, w->x, w->y);
        if (w->special == 1) {
            BattleAnimUtil_SetSpritePosWithOffset(&w->motion, w->sprite, w->x, (s16)(w->y - w->delta));
        }
        if (Pokepic_ResumePaletteFade(w->pic) == 0) {
            Pokepic_StartPaletteFade(w->pic, 16, 0, 0, 0);
            if (w->special == 1) {
                u32 mask = 1 << BattleAnimUtil_GetSpritePaletteSlot(w->sprite);
                PaletteData_BeginPaletteFade(BattleAnimSystem_GetPaletteData(w->owner), 4, (u16)mask, 0, 16, 0, 0);
            }
            w->state++;
        }
        break;
    case 1:
        BattleAnimUtil_UpdateShake(&w->motion);
        BattleAnimUtil_SetPokepicPosWithOffset(&w->motion, w->pic, w->x, w->y);
        if (w->special == 1) {
            BattleAnimUtil_SetSpritePosWithOffset(&w->motion, w->sprite, w->x, (s16)(w->y - w->delta));
        }
        if (Pokepic_ResumePaletteFade(w->pic) == 0) {
            Pokepic_SetAttr(w->pic, 0, w->x);
            Pokepic_SetAttr(w->pic, 1, w->y);
            if (w->special == 1) {
                if (BattleAnimSystem_IsBattlerSemiInvulnerable(w->owner, BattleAnimSystem_GetDefender(w->owner)) == 0) {
                    Pokepic_SetAttr(w->pic, 6, 0);
                }
                ManagedSprite_SetPositionXY(w->sprite, w->x, (s16)(w->y - w->delta));
            }
            w->state++;
        }
        break;
    case 2:
        BattleAnimSystem_DestroyTask(w->owner, task);
        Heap_Free(w);
        break;
    }
}

void ov07_0222CE10(void *owner) {
    W35 *w;
    w = Heap_Alloc((enum HeapID)BattleAnimSystem_GetHeapID(owner), sizeof(W35));
    memset(w, 0, sizeof(W35));
    w->owner = owner;
    w->spriteSystem = BattleAnimSystem_GetBattlerSpriteManager(owner);
    w->mirror = ov07_0221BFC0(owner);
    w->sprite = BattleAnimSystem_GetBattlerSprite(w->owner, 0);
    BattleAnimUtil_InitScaleMotion(&w->anim, 14, 10, 10, 8);
    BattleAnimUtil_InitValueMotionFx(&w->scalar, 0, 16, 8);
    ManagedSprite_SetOamMode(w->sprite, 1);
    BattleAnimUtil_SetBlendWithBattleBg(w->owner, 0, 31);
    ManagedSprite_SetAffineOverwriteMode(w->sprite, 2);
    if (w->mirror == 1) {
        ManagedSprite_SetAffineScale(w->sprite, -1.4f, 1.4f);
    } else {
        ManagedSprite_SetAffineScale(w->sprite, 1.4f, 1.4f);
    }
    ManagedSprite_SetDrawPriority(w->sprite, 100);
    ManagedSprite_SetPriority(w->sprite, 1);
    BattleAnimSystem_CreateTaskDefaultPriority(w->owner, ov07_0222CBFC, w);
}
