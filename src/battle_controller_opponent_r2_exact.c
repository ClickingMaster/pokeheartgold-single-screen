#include "global.h"

#include "battle_controller_opponent_r2_exact_private.h"

void ov12_02261B80(BattleSystem *bsys, u8 *ctx, void *arg2, BattleSourceR1 *src) {
    BattleParamsR1 params;
    u32 selected;
    int i;

    if (src->mode4C == 0) {
        params.value68 = 10;
        selected = src->unk2;
    } else {
        params.value68 = 0x3D;
        selected = src->value50;
    }
    params.bgConfig = BattleSystem_GetBgConfig(bsys);
    params.paletteData = BattleSystem_GetPaletteData(bsys);
    params.spriteSystem = BattleSystem_GetSpriteSystem(bsys);
    for (i = 0; i < 4; i++) {
        params.battlerValues[i] = ov12_0223BB88(bsys, i);
        params.values34[i] = src->values18[i];
        params.values3C[i] = src->values20[i];
        params.values40[i] = src->values24[i];
        params.values44[i] = src->values28[i];
        params.values48[i] = src->values2C[i];
        params.values58[i] = src->values3C[i];
    }
    ov12_0223C1C4(bsys, params.mapping);
    ov12_0223C1F4(bsys, params.objects);
    params.battleType = BattleSystem_GetBattleType(bsys);
    params.chatot = BattleSystem_GetChatotVoice(bsys, ctx[0x194]);
    params.bad0 = ov12_0223BAD0(bsys);
    params.bad8 = ov12_0223BAD8(bsys);
    params.value6C = 7;
    params.value70 = BattleSystem_GetBackgroundId(bsys) + 3;
    params.value74 = ov12_0223B52C(bsys) + (BattleSystem_GetBackgroundId(bsys) * 3 + 0xB0);
    params.value78 = 2;
    params.value7C = 0;
    params.value80 = 8;
    ov07_0221C01C(arg2, src, selected, &params);
}

void ov12_02261CA8(BattleSystem *bsys, BattleSourceR2 *src, BattleParamsR2 *dst, u32 value) {
    int i;
    dst->value0 = value;
    dst->value4 = value;
    for (i = 0; i < 4; i++) {
        dst->battlerValues[i] = ov12_0223BB88(bsys, i);
        dst->values28[i] = src->values18[i];
        dst->values30[i] = src->values20[i];
        dst->values34[i] = src->values24[i];
        dst->values38[i] = src->values28[i];
        dst->values3C[i] = src->values2C[i];
    }
    ov12_0223C1C4(bsys, dst->mapping);
    ov12_0223C1F4(bsys, dst->objects);
}

void ov12_02261D30(u8 *outA, u8 *outB, BOOL useKind, int kind, u16 move) {
    if (!useKind) {
        const u16 moveNo = move;
        if (!(GetMoveAttr(moveNo, MOVEATTR_UNK9) & 0x40)) {
            *outA = TRUE;
        } else {
            *outA = FALSE;
        }
        if (GetMoveAttr(moveNo, MOVEATTR_UNK9) & 0x80) {
            *outB = TRUE;
            return;
        }
        *outB = FALSE;
        return;
    }

    switch (kind) {
    case 0x12:
    case 0x13:
    case 0x14:
    case 0x15:
    case 0x16:
    case 0x1F:
    case 0x20:
    case 0x22:
    case 0x23:
    case 0x25:
    case 0x27:
        *outA = TRUE;
        *outB = FALSE;
        break;
    case 0x24:
    case 0x26:
        *outA = TRUE;
        *outB = TRUE;
        break;
    default:
        *outA = FALSE;
        *outB = FALSE;
        break;
    }
}

BOOL ov12_02261DC8(BattleAnimWork *work, ManagedSprite *unused) {
    u32 attr;
    BOOL done = FALSE;

    attr = ManagedSprite_GetUserAttrForCurrentAnimFrame(work->sprite);
    switch (attr) {
    case 1:
        if (work->active == 0) {
            s32 *data;
            work->active = 1;
            data = Heap_Alloc(HEAP_ID_BATTLE, 8);
            data[0] = 0;
            data[1] = 0;
            SysTask_CreateOnMainQueue((SysTaskFunc)ov12_02261E40, data, 0);
        }
        break;
    case 0xFFF:
        done = TRUE;
        break;
    default:
        if ((attr & 0xF00) == 0x100) {
            u8 anim = attr;
            if (anim != 0) {
                ManagedSprite_SetAnimationFrame(work->sprite, 0);
                ManagedSprite_SetAnim(work->sprite, anim - 1);
                done = TRUE;
            }
        }
        break;
    }
    return done;
}
