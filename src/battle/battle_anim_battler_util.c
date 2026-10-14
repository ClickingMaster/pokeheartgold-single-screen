#include "battle/battle_anim_battler_util.h"

#include "global.h"

#include "constants/battle.h"

#include "heap.h"
#include "particle_system_internal.h"

typedef struct BattleAnimBattlerTypePositions {
    BattleAnimPosition positions[6];
} BattleAnimBattlerTypePositions;

// [battler type][kind * 2 + particle perspective type]
typedef struct BattleAnimEmitterPositions {
    VecFx32 positions[8][24];
} BattleAnimEmitterPositions;

extern const VecFx32 sBattleAnimFlippedCameraUp;
extern const BattleAnimBattlerTypePositions sBattleAnimBattlerTypePositions;
extern const BattleAnimEmitterPositions sBattleAnimEmitterPositions;

int BattleAnimUtil_GetBattlerType(BattleAnimSystem *system, int battler) {
    return BattleAnimSystem_GetBattlerType(system, battler);
}

int BattleAnimUtil_GetBattlerSide(BattleAnimSystem *system, int battler) {
    int side;
    switch (BattleAnimSystem_GetBattlerType(system, battler)) {
    case BATTLER_TYPE_SOLO_PLAYER:
    case BATTLER_TYPE_PLAYER_SIDE_SLOT_1:
    case BATTLER_TYPE_PLAYER_SIDE_SLOT_2:
        side = BATTLE_ANIM_SIDE_PLAYER;
        break;
    case BATTLER_TYPE_SOLO_ENEMY:
    case BATTLER_TYPE_ENEMY_SIDE_SLOT_1:
    case BATTLER_TYPE_ENEMY_SIDE_SLOT_2:
        side = BATTLE_ANIM_SIDE_ENEMY;
        break;
    }
    return side;
}

int BattleAnimUtil_FindBattlerByType(BattleAnimSystem *system, int battlerType) {
    int i;
    for (i = 0; i < 4; i++) {
        int type = BattleAnimSystem_GetBattlerType(system, i);
        if (type != battlerType) {
            continue;
        }
        return i;
    }
    return 0;
}

int BattleAnimUtil_GetPartnerBattler(BattleAnimSystem *system, int battler) {
    int i;
    u32 battlerType = BattleAnimSystem_GetBattlerType(system, battler);
    if (battlerType <= BATTLER_TYPE_SOLO_ENEMY) {
        return battlerType;
    }
    for (i = 0; i < 4; i++) {
        int type = BattleAnimSystem_GetBattlerType(system, i);
        switch (battlerType) {
        case BATTLER_TYPE_PLAYER_SIDE_SLOT_1:
            if (type == BATTLER_TYPE_PLAYER_SIDE_SLOT_2) {
                return i;
            }
            break;
        case BATTLER_TYPE_PLAYER_SIDE_SLOT_2:
            if (type == BATTLER_TYPE_PLAYER_SIDE_SLOT_1) {
                return i;
            }
            break;
        case BATTLER_TYPE_ENEMY_SIDE_SLOT_1:
            if (type == BATTLER_TYPE_ENEMY_SIDE_SLOT_2) {
                return i;
            }
            break;
        case BATTLER_TYPE_ENEMY_SIDE_SLOT_2:
            if (type == BATTLER_TYPE_ENEMY_SIDE_SLOT_1) {
                return i;
            }
            break;
        }
    }
    GF_AssertFail();
    return battlerType;
}

int BattleAnimUtil_GetOpposingBattlerType(int battlerType) {
    switch (battlerType) {
    case BATTLER_TYPE_SOLO_PLAYER:
        return BATTLER_TYPE_SOLO_ENEMY;
    case BATTLER_TYPE_SOLO_ENEMY:
        return BATTLER_TYPE_SOLO_PLAYER;
    case BATTLER_TYPE_PLAYER_SIDE_SLOT_1:
        return BATTLER_TYPE_ENEMY_SIDE_SLOT_1;
    case BATTLER_TYPE_ENEMY_SIDE_SLOT_1:
        return BATTLER_TYPE_PLAYER_SIDE_SLOT_1;
    case BATTLER_TYPE_PLAYER_SIDE_SLOT_2:
        return BATTLER_TYPE_ENEMY_SIDE_SLOT_2;
    case BATTLER_TYPE_ENEMY_SIDE_SLOT_2:
        return BATTLER_TYPE_PLAYER_SIDE_SLOT_2;
    default:
        GF_AssertFail();
        return BATTLER_TYPE_SOLO_PLAYER;
    }
}

void BattleAnimUtil_GetBattlerTypePosition(int battlerType, int unused, BattleAnimPosition *pos) {
    BattleAnimBattlerTypePositions table = sBattleAnimBattlerTypePositions;
    *pos = table.positions[battlerType];
}

void BattleAnimUtil_GetBattlerPosition(BattleAnimSystem *system, int battler, BattleAnimPosition *pos) {
    int battlerType = BattleAnimUtil_GetBattlerType(system, battler);
    int unk = BattleAnimSystem_GetField08(system);
    BattleAnimUtil_GetBattlerTypePosition(battlerType, unk, pos);
}

void BattleAnimUtil_GetEmitterPosition(int battlerType, VecFx32 *pos, int unused, int perspective, int kind) {
    BattleAnimEmitterPositions table = sBattleAnimEmitterPositions;
    VecFx32 *positions = table.positions[battlerType];
    int index = perspective + kind * 2;
    fx32 z = positions[index].z;
    fx32 y = positions[index].y;
    fx32 x = positions[index].x;
    pos->x = x;
    pos->y = y;
    pos->z = z;
}

// BattleAnimUtil_GetEmitterPosition with a fixed kind
#define EMITTER_POSITION_FUNC(name, kind)                                               \
    void name(int battlerType, VecFx32 *pos, int unused, int perspective) {             \
        BattleAnimUtil_GetEmitterPosition(battlerType, pos, unused, perspective, kind); \
    }

// ... of a battler, for the current particle system's perspective type
#define BATTLER_EMITTER_POSITION_FUNC(name, func)                                                     \
    void name(BattleAnimSystem *system, int battler, VecFx32 *pos) {                                  \
        int perspective = ParticleSystem_GetPerspectiveType((ParticleSystem *)BattleAnimSystem_GetCurrentParticleSystem(system)); \
        int battlerType = BattleAnimUtil_GetBattlerType(system, battler);                             \
        int unk = BattleAnimSystem_GetField08(system);                                                              \
        func(battlerType, pos, unk, perspective);                                                     \
    }

EMITTER_POSITION_FUNC(BattleAnimUtil_GetEmitterPositionKind0, 0)
EMITTER_POSITION_FUNC(BattleAnimUtil_GetEmitterPositionKind1, 1)
EMITTER_POSITION_FUNC(BattleAnimUtil_GetEmitterPositionKind2, 2)
EMITTER_POSITION_FUNC(BattleAnimUtil_GetEmitterPositionKind3, 3)
EMITTER_POSITION_FUNC(BattleAnimUtil_GetEmitterPositionKind4, 4)
EMITTER_POSITION_FUNC(BattleAnimUtil_GetEmitterPositionKind5, 5)
EMITTER_POSITION_FUNC(BattleAnimUtil_GetEmitterPositionKind6, 6)
EMITTER_POSITION_FUNC(BattleAnimUtil_GetEmitterPositionKind7, 7)
EMITTER_POSITION_FUNC(BattleAnimUtil_GetEmitterPositionKind8, 8)
EMITTER_POSITION_FUNC(BattleAnimUtil_GetEmitterPositionKind9, 9)
EMITTER_POSITION_FUNC(BattleAnimUtil_GetEmitterPositionKind10, 10)
EMITTER_POSITION_FUNC(BattleAnimUtil_GetEmitterPositionKind11, 11)

BATTLER_EMITTER_POSITION_FUNC(BattleAnimUtil_GetBattlerEmitterPositionKind0, BattleAnimUtil_GetEmitterPositionKind0)
BATTLER_EMITTER_POSITION_FUNC(BattleAnimUtil_GetBattlerEmitterPositionKind1, BattleAnimUtil_GetEmitterPositionKind1)
BATTLER_EMITTER_POSITION_FUNC(BattleAnimUtil_GetBattlerEmitterPositionKind2, BattleAnimUtil_GetEmitterPositionKind2)
BATTLER_EMITTER_POSITION_FUNC(BattleAnimUtil_GetBattlerEmitterPositionKind3, BattleAnimUtil_GetEmitterPositionKind3)
BATTLER_EMITTER_POSITION_FUNC(BattleAnimUtil_GetBattlerEmitterPositionKind4, BattleAnimUtil_GetEmitterPositionKind4)
BATTLER_EMITTER_POSITION_FUNC(BattleAnimUtil_GetBattlerEmitterPositionKind5, BattleAnimUtil_GetEmitterPositionKind5)
BATTLER_EMITTER_POSITION_FUNC(BattleAnimUtil_GetBattlerEmitterPositionKind6, BattleAnimUtil_GetEmitterPositionKind6)
BATTLER_EMITTER_POSITION_FUNC(BattleAnimUtil_GetBattlerEmitterPositionKind7, BattleAnimUtil_GetEmitterPositionKind7)
BATTLER_EMITTER_POSITION_FUNC(BattleAnimUtil_GetBattlerEmitterPositionKind8, BattleAnimUtil_GetEmitterPositionKind8)
BATTLER_EMITTER_POSITION_FUNC(BattleAnimUtil_GetBattlerEmitterPositionKind9, BattleAnimUtil_GetEmitterPositionKind9)
BATTLER_EMITTER_POSITION_FUNC(BattleAnimUtil_GetBattlerEmitterPositionKind10, BattleAnimUtil_GetEmitterPositionKind10)
BATTLER_EMITTER_POSITION_FUNC(BattleAnimUtil_GetBattlerEmitterPositionKind11, BattleAnimUtil_GetEmitterPositionKind11)

void BattleAnimUtil_GetEmitterHighPosition(VecFx32 *pos) {
    pos->x = 0;
    pos->y = 0x68D0;
    pos->z = 0;
}

void BattleAnimUtil_GetEmitterPositionKind0Alt(int battlerType, VecFx32 *pos, int unused, int perspective) {
    BattleAnimUtil_GetEmitterPositionKind0(battlerType, pos, unused, perspective);
}

void BattleAnimUtil_GetCameraUp(BOOL flipped, VecFx32 *up) {
    if (flipped == FALSE) {
        ParticleSystem_GetDefaultCameraUp(up);
    } else {
        *up = sBattleAnimFlippedCameraUp;
    }
}

void BattleAnimUtil_SetBlendWithBattleBg(BattleAnimSystem *system, int eva, int evb) {
    if (eva == -1) {
        eva = 26;
    }
    if (evb == -1) {
        evb = 5;
    }
    int plane2 = (1 << BattleAnimSystem_GetBgMaskLayer(system, 2)) | GX_BLEND_PLANEMASK_BG0;
    G2_SetBlendAlpha(GX_BLEND_PLANEMASK_NONE, plane2, eva, evb);
}

void BattleAnimUtil_SetBlendWithMask(BattleAnimSystem *system, int plane2, int eva, int evb) {
    if (eva == -1) {
        eva = 26;
    }
    if (evb == -1) {
        evb = 5;
    }
    G2_SetBlendAlpha(GX_BLEND_PLANEMASK_NONE, plane2, eva, evb);
}

void BattleAnimUtil_SetBlendBgOverBattleBg(BattleAnimSystem *system, int eva, int evb) {
    if (eva == -1) {
        eva = 26;
    }
    if (evb == -1) {
        evb = 5;
    }
    int plane2 = (1 << BattleAnimSystem_GetBgMaskLayer(system, 2)) | GX_BLEND_PLANEMASK_BG0 | GX_BLEND_PLANEMASK_OBJ;
    int plane1 = 1 << BattleAnimSystem_GetBgMaskLayer(system, 1);
    G2_SetBlendAlpha(plane1, plane2, eva, evb);
}

#pragma force_active on

void BattleAnimUtil_SetWindowInsidePlanes(BattleAnimSystem *system, int window, BOOL effect) {
    if (window == 0) {
        G2_SetWnd0InsidePlane((1 << BattleAnimSystem_GetBgMaskLayer(system, 2)) | (1 << BattleAnimSystem_GetBgMaskLayer(system, 0)) | (1 << BattleAnimSystem_GetBgMaskLayer(system, 1)) | GX_WND_PLANEMASK_BG0 | GX_WND_PLANEMASK_OBJ, effect);
    } else {
        G2_SetWnd1InsidePlane((1 << BattleAnimSystem_GetBgMaskLayer(system, 2)) | (1 << BattleAnimSystem_GetBgMaskLayer(system, 0)) | (1 << BattleAnimSystem_GetBgMaskLayer(system, 1)) | GX_WND_PLANEMASK_BG0 | GX_WND_PLANEMASK_OBJ, effect);
    }
    G2_SetWndOutsidePlane((1 << BattleAnimSystem_GetBgMaskLayer(system, 0)) | (1 << BattleAnimSystem_GetBgMaskLayer(system, 1)) | GX_WND_PLANEMASK_BG0 | GX_WND_PLANEMASK_OBJ, TRUE);
}

#pragma force_active reset

void BattleAnimUtil_GetPicPosition(Pokepic *pokepic, s16 *pos) {
    if (pokepic == NULL) {
        pos[0] = 0;
        pos[1] = 0;
        return;
    }
    if (!Pokepic_IsActive(pokepic)) {
        pos[0] = 0;
        pos[1] = 0;
        return;
    }
    pos[0] = Pokepic_GetAttr(pokepic, POKEPIC_X);
    pos[1] = Pokepic_GetAttr(pokepic, POKEPIC_Y);
}

void BattleAnimUtil_GetSpritePosition(ManagedSprite *sprite, s16 *pos) {
    ManagedSprite_GetPositionXY(sprite, &pos[0], &pos[1]);
}

void BattleAnimUtil_InitCommonWork(BattleAnimSystem *system, BattleAnimCommonWork *work) {
    work->state[0] = 0;
    work->state[1] = 0;
    work->state[2] = 0;
    work->state[3] = 0;
    work->system = system;
    work->spriteSystem = BattleAnimSystem_GetSpriteSystem(system);
    work->battlerSpriteManager = BattleAnimSystem_GetBattlerSpriteManager(system);
    work->spriteManager = BattleAnimSystem_GetFirstSpriteManager(system);
    work->bgConfig = BattleAnimSystem_GetBgConfig(system);
    work->paletteData = BattleAnimSystem_GetPaletteData(system);
}

// flags: 0x40 all battlers, 0x20 all but the attacker, otherwise 0x02 attacker, 0x04 its partner, 0x08 defender,
// 0x10 its partner (partners in double battles only)
void BattleAnimUtil_CollectPicsByFlags(BattleAnimSystem *system, u32 flags, BattleAnimTarget *targets, int *count) {
    u32 attacker;
    u32 partner;
    u32 opponent;
    u32 opponentPartner;

    *count = 0;
    if ((flags & 0x40) == 0x40) {
        attacker = BattleAnimSystem_GetAttacker(system);
        targets[*count].pokepic = BattleAnimSystem_GetBattlerPokepic(system, attacker);
        if (targets[*count].pokepic != NULL) {
            targets[*count].battler = attacker;
            BattleAnimUtil_GetPicPosition(targets[*count].pokepic, targets[*count].pos);
            (*count)++;
        }

        partner = BattleAnimUtil_GetPartnerBattler(system, attacker);
        targets[*count].pokepic = BattleAnimSystem_GetBattlerPokepic(system, partner);
        if (targets[*count].pokepic != NULL) {
            targets[*count].battler = partner;
            BattleAnimUtil_GetPicPosition(targets[*count].pokepic, targets[*count].pos);
            (*count)++;
        }

        opponent = BattleAnimUtil_FindBattlerByType(system, BattleAnimUtil_GetOpposingBattlerType(BattleAnimSystem_GetBattlerType(system, attacker)));
        targets[*count].pokepic = BattleAnimSystem_GetBattlerPokepic(system, opponent);
        if (targets[*count].pokepic != NULL) {
            targets[*count].battler = opponent;
            BattleAnimUtil_GetPicPosition(targets[*count].pokepic, targets[*count].pos);
            (*count)++;
        }

        opponentPartner = BattleAnimUtil_GetPartnerBattler(system, opponent);
        targets[*count].pokepic = BattleAnimSystem_GetBattlerPokepic(system, opponentPartner);
        if (targets[*count].pokepic != NULL) {
            targets[*count].battler = opponentPartner;
            BattleAnimUtil_GetPicPosition(targets[*count].pokepic, targets[*count].pos);
            (*count)++;
        }
        return;
    }

    if ((flags & 0x20) == 0x20) {
        attacker = BattleAnimSystem_GetAttacker(system);
        partner = BattleAnimUtil_GetPartnerBattler(system, attacker);
        if (partner != attacker) {
            targets[*count].pokepic = BattleAnimSystem_GetBattlerPokepic(system, partner);
            if (targets[*count].pokepic != NULL) {
                targets[*count].battler = partner;
                BattleAnimUtil_GetPicPosition(targets[*count].pokepic, targets[*count].pos);
                (*count)++;
            }
        }

        opponent = BattleAnimUtil_FindBattlerByType(system, BattleAnimUtil_GetOpposingBattlerType(BattleAnimSystem_GetBattlerType(system, attacker)));
        if (opponent != attacker) {
            targets[*count].pokepic = BattleAnimSystem_GetBattlerPokepic(system, opponent);
            if (targets[*count].pokepic != NULL) {
                targets[*count].battler = opponent;
                BattleAnimUtil_GetPicPosition(targets[*count].pokepic, targets[*count].pos);
                (*count)++;
            }
        }

        opponentPartner = BattleAnimUtil_GetPartnerBattler(system, opponent);
        if (opponentPartner != attacker) {
            targets[*count].pokepic = BattleAnimSystem_GetBattlerPokepic(system, opponentPartner);
            if (targets[*count].pokepic != NULL) {
                targets[*count].battler = opponentPartner;
                BattleAnimUtil_GetPicPosition(targets[*count].pokepic, targets[*count].pos);
                (*count)++;
            }
        }
        return;
    }

    if ((flags & 0x02) == 0x02) {
        targets[*count].pokepic = BattleAnimSystem_GetBattlerPokepic(system, BattleAnimSystem_GetAttacker(system));
        if (targets[*count].pokepic != NULL) {
            targets[*count].battler = BattleAnimSystem_GetAttacker(system);
            BattleAnimUtil_GetPicPosition(targets[*count].pokepic, targets[*count].pos);
            (*count)++;
        }
    }

    if (BattleAnimSystem_IsDoubleBattle(system) == TRUE && (flags & 0x04) == 0x04) {
        targets[*count].pokepic = BattleAnimSystem_GetBattlerPokepic(system, BattleAnimUtil_GetPartnerBattler(system, BattleAnimSystem_GetAttacker(system)));
        if (targets[*count].pokepic != NULL) {
            targets[*count].battler = BattleAnimUtil_GetPartnerBattler(system, BattleAnimSystem_GetAttacker(system));
            BattleAnimUtil_GetPicPosition(targets[*count].pokepic, targets[*count].pos);
            (*count)++;
        }
    }

    if ((flags & 0x08) == 0x08) {
        targets[*count].pokepic = BattleAnimSystem_GetBattlerPokepic(system, BattleAnimSystem_GetDefender(system));
        if (targets[*count].pokepic != NULL) {
            targets[*count].battler = BattleAnimSystem_GetDefender(system);
            BattleAnimUtil_GetPicPosition(targets[*count].pokepic, targets[*count].pos);
            (*count)++;
        }
    }

    if (BattleAnimSystem_IsDoubleBattle(system) == TRUE && (flags & 0x10) == 0x10) {
        targets[*count].pokepic = BattleAnimSystem_GetBattlerPokepic(system, BattleAnimUtil_GetPartnerBattler(system, BattleAnimSystem_GetDefender(system)));
        if (targets[*count].pokepic != NULL) {
            targets[*count].battler = BattleAnimUtil_GetPartnerBattler(system, BattleAnimSystem_GetDefender(system));
            BattleAnimUtil_GetPicPosition(targets[*count].pokepic, targets[*count].pos);
            (*count)++;
        }
    }
}

// flags 0x02, 0x04, 0x08, 0x10: one more of the effect sprites 0..3
void BattleAnimUtil_CollectSpritesByFlags(BattleAnimSystem *system, u32 flags, BattleAnimTarget *targets, int *count) {
    *count = 0;
    if ((flags & 0x02) == 0x02) {
        targets[*count].sprite = BattleAnimSystem_GetBattlerSprite(system, *count);
        BattleAnimUtil_GetSpritePosition(targets[*count].sprite, targets[*count].pos);
        (*count)++;
    }
    if ((flags & 0x04) == 0x04) {
        targets[*count].sprite = BattleAnimSystem_GetBattlerSprite(system, *count);
        BattleAnimUtil_GetSpritePosition(targets[*count].sprite, targets[*count].pos);
        (*count)++;
    }
    if ((flags & 0x08) == 0x08) {
        targets[*count].sprite = BattleAnimSystem_GetBattlerSprite(system, *count);
        BattleAnimUtil_GetSpritePosition(targets[*count].sprite, targets[*count].pos);
        (*count)++;
    }
    if ((flags & 0x10) == 0x10) {
        targets[*count].sprite = BattleAnimSystem_GetBattlerSprite(system, *count);
        BattleAnimUtil_GetSpritePosition(targets[*count].sprite, targets[*count].pos);
        (*count)++;
    }
}

void *BattleAnimUtil_AllocWork(BattleAnimSystem *system, u32 size) {
    void *work;
    GF_ASSERT(system != NULL);
    work = Heap_Alloc(BattleAnimSystem_GetHeapID(system), size);
    GF_ASSERT(work != NULL);
    memset(work, 0, size);
    return work;
}

void BattleAnimUtil_FreeWork(void *work) {
    GF_ASSERT(work != NULL);
    Heap_Free(work);
}
