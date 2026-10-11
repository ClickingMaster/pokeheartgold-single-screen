#include "to56_r2_w13_private.h"
#pragma force_active on

void ov07_0222D3BC(void *owner, void *arg1, int direction, void *sprite) {
    R2W13Spin *w = BattleAnimUtil_AllocWork(owner, 0x58);
    w->owner = owner;
    w->arg1 = arg1;
    w->direction = direction;
    int mode = BattleAnimSystem_GetAttacker(w->owner);
    int delta = ov07_02222004(w->owner, mode);
    int signedDelta;
    if (ov07_0221FB78(w->owner, 0) == 1) {
        w->sign = -1;
        signedDelta = -delta;
    } else {
        w->sign = 1;
        signedDelta = delta;
    }
    int x = BattleAnimSystem_GetBattlerScreenCoord(owner, BattleAnimSystem_GetAttacker(w->owner), 0);
    int y = BattleAnimSystem_GetBattlerScreenCoord(owner, BattleAnimSystem_GetAttacker(w->owner), 1);
    w->sprite = sprite;
    ManagedSprite_SetAffineOverwriteMode(sprite, 2);
    ManagedSprite_SetAffineScale(w->sprite, 1.0f, 1.0f);
    ManagedSprite_SetPositionXY(w->sprite, x, y);
    ManagedSprite_OffsetPositionXY(w->sprite, (s16)(delta * 40), 0);
    ManagedSprite_SetDrawPriority(w->sprite, 100);
    ManagedSprite_SetPriority(w->sprite, 1);
    ManagedSprite_SetAffineTranslation(w->sprite, (s16)(signedDelta * -12), 12);
    if (BattleAnimUtil_GetBattlerSide(owner, BattleAnimSystem_GetAttacker(owner)) == 4) {
        ManagedSprite_SetAnim(w->sprite, 1);
    } else {
        ManagedSprite_SetAnim(w->sprite, 0);
    }
    BattleAnimUtil_InitScaleMotion(w->scaleAnim, 1, 10, 10, 8);
    if (signedDelta >= 0) {
        BattleAnimUtil_InitValueMotion(w->translationAnim, 0xFF48, 0xE38D, 4);
        w->x = 0xFF48;
    } else {
        BattleAnimUtil_InitValueMotion(w->translationAnim, 0, 0x1C71, 4);
        w->x = 0;
    }
    BattleAnimSystem_CreateTaskDefaultPriority(w->owner, ov07_0222D2B0, w);
}
