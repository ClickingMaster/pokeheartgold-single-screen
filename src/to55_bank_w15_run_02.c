#include "to55_bank_w15_private.h"

void ov07_0222D4F4(W06ScaleSprite *work, void *sprite, s16 x, s16 y, BOOL flip) {
    work->sprite = sprite;
    BattleAnimUtil_InitScaleMotion(work->scaleAnim, 10, 10, 8, 8);
    work->delay = 3;
    work->start = 8;
    work->end = 10;
    work->flip = flip;
    ManagedSprite_SetPositionXY(work->sprite, x, y);
    ManagedSprite_SetPositionXY(work->sprite, x, y);
    ManagedSprite_SetAffineOverwriteMode(work->sprite, 2);
    ManagedSprite_SetDrawPriority(work->sprite, 100);
    ManagedSprite_SetPriority(work->sprite, 1);
    if (flip) {
        ManagedSprite_SetAffineScale(work->sprite, -1.0f, 1.0f);
    }
    ManagedSprite_SetDrawFlag(work->sprite, FALSE);
}
