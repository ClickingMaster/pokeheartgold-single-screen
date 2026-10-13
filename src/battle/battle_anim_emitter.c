#include "battle/battle_anim_emitter.h"

#include <string.h>

#include "global.h"

#include "library/spl_emitter.h"

#include "assert.h"
#include "camera.h"
#include "geometry_math.h"
#include "heap.h"
#include "obj_pltt_transfer.h"
#include "particle_system.h"
#include "particle_system_internal.h"
#include "pokepic.h"
#include "sprite.h"
#include "sprite_system.h"

// Same layouts as in battle/battle_anim_internal.h (switch to it when it lands)
struct BattleAnimLinearMotion {
    s16 x;
    s16 y;
    int duration;
    fx32 stepX;
    fx32 stepY;
    fx32 curX;
    fx32 curY;
    u8 unk_18[0xC];
};

struct BattleAnimSinMotion {
    s16 x;
    s16 y;
    int duration;
    int angleX;
    int ampX;
    int angleY;
    int ampY;
    int stepX;
    int stepY;
    int unk_24;
};

// Head of the motion structs: the current offset
typedef struct BattleAnimMotionOffset {
    s16 x;
    s16 y;
} BattleAnimMotionOffset;

// Emitter offsets by attacker type and defender type (units of 172)
typedef struct BattleAnimEmitterOffsetTable {
    s16 values[6][6];
} BattleAnimEmitterOffsetTable;

// Script arguments: a vector and the components BattleAnimUtil_ScaleVecComponents scales
typedef struct BattleAnimEmitterTargetArgs {
    VecFx32 pos;
    int scaleKind;
} BattleAnimEmitterTargetArgs;

// Script arguments: an offset, mirrored by BattleAnimEmitterConfig.sideSign unless ignoreSide
typedef struct BattleAnimEmitterOffsetArgs {
    BOOL ignoreSide;
    VecFx32 offset;
} BattleAnimEmitterOffsetArgs;

// Screen positions (x, y) per battler type
typedef struct BattleAnimScreenPosTable6 {
    struct {
        int x, y;
    } pos[6];
} BattleAnimScreenPosTable6;

typedef struct BattleAnimScreenPosTable7 {
    struct {
        int x, y;
    } pos[7];
} BattleAnimScreenPosTable7;

typedef struct BattleAnimEmitterAxisRow {
    int v[4]; // x, y, z, [3] == 1: not negated
} BattleAnimEmitterAxisRow;

typedef struct BattleAnimEmitterAxisRows {
    BattleAnimEmitterAxisRow rows[7];
} BattleAnimEmitterAxisRows;

typedef struct BattleAnimEmitterAxisPositions {
    int pos[6][2];
} BattleAnimEmitterAxisPositions;

// One VecFx32 per (attacker type, defender type); each table is referenced through three labels (x, y, z)
typedef struct BattleAnimEmitterAxisEntry {
    int v[3];
} BattleAnimEmitterAxisEntry;

typedef struct BattleAnimEmitterAxis {
    u16 x, y, z;
} BattleAnimEmitterAxis;

typedef struct BattleAnimEmitterFlagMasks {
    int masks[15];
} BattleAnimEmitterFlagMasks;

extern const BattleAnimEmitterOffsetTable ov07_02235440;
extern const BattleAnimEmitterOffsetTable ov07_02235488;
extern const BattleAnimEmitterAxisRow ov07_022354D0;
extern const BattleAnimScreenPosTable6 ov07_022354E0;
extern const BattleAnimEmitterAxisPositions ov07_02235510;
extern const BattleAnimScreenPosTable7 ov07_02235540;
extern const BattleAnimScreenPosTable7 ov07_02235578;
extern const BattleAnimEmitterAxisRows ov07_022355B0;
extern const BattleAnimEmitterAxisEntry ov07_02235CE0[6][6];
extern const BattleAnimEmitterAxisEntry ov07_02235CE4[6][6];
extern const BattleAnimEmitterAxisEntry ov07_02235CE8[6][6];
extern const BattleAnimEmitterAxisEntry ov07_02235620[6][6];
extern const BattleAnimEmitterAxisEntry ov07_02235624[6][6];
extern const BattleAnimEmitterAxisEntry ov07_02235628[6][6];
extern const BattleAnimEmitterAxisEntry ov07_022357D0[6][6];
extern const BattleAnimEmitterAxisEntry ov07_02235B30[6][6];
extern const BattleAnimEmitterAxisEntry ov07_02235B34[6][6];
extern const BattleAnimEmitterAxisEntry ov07_02235B38[6][6];
extern const BattleAnimEmitterAxisEntry ov07_02235E90[6][6];
extern const BattleAnimEmitterAxisEntry ov07_02235E94[6][6];
extern const BattleAnimEmitterAxisEntry ov07_02235E98[6][6];
extern const BattleAnimEmitterAxisEntry ov07_02235980[6][6];
extern const BattleAnimEmitterAxisEntry ov07_02235984[6][6];
extern const BattleAnimEmitterAxisEntry ov07_02235988[6][6];
extern const BattleAnimEmitterAxisEntry ov07_02236040[6][6];
extern const BattleAnimEmitterAxisEntry ov07_02236044[6][6];
extern const BattleAnimEmitterAxisEntry ov07_02236048[6][6];
extern const BattleAnimEmitterAxisEntry ov07_022361F0[6][6];
extern const BattleAnimEmitterAxisEntry ov07_022361F4[6][6];
extern const BattleAnimEmitterAxisEntry ov07_022361F8[6][6];
extern BattleAnimEmitterConfigCb sBattleAnimEmitterFlagCallbacks[];
extern const BattleAnimEmitterFlagMasks sBattleAnimEmitterFlagMasks; // 0xFEFE-terminated
extern CameraAngle sBattleAnimEmitterCameraAngles[];
extern BattleAnimEmitterConfigCb sBattleAnimEmitterCameraCallbacks[];
// s16 [2][6][2]: (x, y) per battler slot, set 0 singles (+2: the 0221BFC0 mode), set 1 doubles; y read via its own label
extern const s16 ov07_02236468[][12];
extern const s16 ov07_0223646A[][12];

void BattleAnimEmitterCb_SetPositionAtFirstOpponent(SPLEmitter *emitter) {
    CameraAngle angle;
    VecFx32 pos;
    VecFx32 unusedPos;
    VecFx32 up;
    BattleAnimSystem *system;
    SPLEmitter *particleSystem;
    Camera *camera;

    system = ParticleSystem_GetEmitterParam();
    BattleAnimSystem_GetAttacker(system);
    BattleAnimSystem_GetDefender(system);
    particleSystem = BattleAnimSystem_GetCurrentParticleSystem(system);
    if (BattleAnimUtil_GetBattlerSide(system, 1) == 3) {
        BattleAnimUtil_GetBattlerEmitterPositionKind0(system, 1, &pos);
    } else {
        int type = BattleAnimUtil_GetBattlerType(system, 1);
        int opposingType = BattleAnimUtil_GetOpposingBattlerType(type);
        int perspective = ParticleSystem_GetPerspectiveType((ParticleSystem *)particleSystem);
        int mode = ov07_0221BFC0(system);
        BattleAnimUtil_GetEmitterPositionKind0(opposingType, &pos, mode, perspective);
        BattleAnimUtil_GetEmitterPositionKind0(type, &unusedPos, mode, perspective);
    }
    SPL_SetEmitterPosition(emitter, &pos);
    BattleAnimUtil_GetCameraUp(1, &up);
    camera = ParticleSystem_GetCamera(particleSystem);
    angle = Camera_GetAngle(camera);
    Camera_SetAnglePos(&angle, camera);
}

void BattleAnimEmitterCb_SetPositionAtBattler0(SPLEmitter *emitter) {
    VecFx32 pos;
    VecFx32 up;
    BattleAnimSystem *system = ParticleSystem_GetEmitterParam();
    int defender = BattleAnimSystem_GetDefender(system);
    SPLEmitter *particleSystem = BattleAnimSystem_GetCurrentParticleSystem(system);
    BattleAnimUtil_GetBattlerEmitterPositionKind0(system, 0, &pos);
    SPL_SetEmitterPosition(emitter, &pos);
    BattleAnimUtil_GetCameraUp(defender, &up);
    ParticleSystem_SetCameraUp((ParticleSystem *)particleSystem, &up);
}

void BattleAnimEmitterCb_SetPositionAtDefender(SPLEmitter *emitter) {
    VecFx32 pos;
    BattleAnimSystem *system = ParticleSystem_GetEmitterParam();
    int defender = BattleAnimSystem_GetDefender(system);
    BattleAnimUtil_GetBattlerEmitterPositionKind0(system, defender, &pos);
    SPL_SetEmitterPosition(emitter, &pos);
}

void BattleAnimEmitterCb_SetPositionAtAttacker(SPLEmitter *emitter) {
    VecFx32 pos;
    BattleAnimSystem *system = ParticleSystem_GetEmitterParam();
    int attacker = BattleAnimSystem_GetAttacker(system);
    BattleAnimUtil_GetBattlerEmitterPositionKind0(system, attacker, &pos);
    SPL_SetEmitterPosition(emitter, &pos);
}

void BattleAnimEmitterCb_SetPositionAtAttacker2(SPLEmitter *emitter) {
    VecFx32 pos;
    BattleAnimSystem *system = ParticleSystem_GetEmitterParam();
    int attacker = BattleAnimSystem_GetAttacker(system);
    BattleAnimSystem_GetDefender(system);
    BattleAnimUtil_GetBattlerEmitterPositionKind0(system, attacker, &pos);
    SPL_SetEmitterPosition(emitter, &pos);
}

void BattleAnimEmitterCb_SetPositionAtAttackerSide(SPLEmitter *emitter) {
    VecFx32 pos;
    BattleAnimSystem *system = ParticleSystem_GetEmitterParam();
    SPLEmitter *particleSystem = BattleAnimSystem_GetCurrentParticleSystem(system);
    int perspective = ParticleSystem_GetPerspectiveType((ParticleSystem *)particleSystem);
    int attacker = BattleAnimSystem_GetAttacker(system);
    int type = BattleAnimUtil_GetBattlerSide(system, attacker) == 3 ? 0 : 1;
    int mode = ov07_0221BFC0(system);
    BattleAnimUtil_GetEmitterPositionKind0(type, &pos, mode, perspective);
    SPL_SetEmitterPosition(emitter, &pos);
}

void BattleAnimEmitterCb_SetPositionAtDefenderSide(SPLEmitter *emitter) {
    VecFx32 pos;
    BattleAnimSystem *system = ParticleSystem_GetEmitterParam();
    SPLEmitter *particleSystem = BattleAnimSystem_GetCurrentParticleSystem(system);
    int perspective = ParticleSystem_GetPerspectiveType((ParticleSystem *)particleSystem);
    int defender = BattleAnimSystem_GetDefender(system);
    int type = BattleAnimUtil_GetBattlerSide(system, defender) == 3 ? 0 : 1;
    int mode = ov07_0221BFC0(system);
    BattleAnimUtil_GetEmitterPositionKind0(type, &pos, mode, perspective);
    SPL_SetEmitterPosition(emitter, &pos);
}

#pragma force_active on

void BattleAnimEmitterCb_SetPositionByBattlerTypes(SPLEmitter *emitter) {
    s16 unused[6] = { 0 }; // unused (kept: stack layout)
    BattleAnimEmitterOffsetTable offsetsX;
    BattleAnimEmitterOffsetTable offsetsY;
    BattleAnimSystem *system;
    u32 attacker, defender;
    int attackerType, defenderType;
    u32 offsetX, offsetY;

    offsetsX = ov07_02235440;
    offsetsY = ov07_02235488;
    system = ParticleSystem_GetEmitterParam();
    attacker = BattleAnimSystem_GetAttacker(system);
    defender = BattleAnimSystem_GetDefender(system);
    attackerType = BattleAnimUtil_GetBattlerType(system, attacker);
    defenderType = BattleAnimUtil_GetBattlerType(system, defender);
    offsetX = offsetsX.values[attackerType][defenderType] * 172u;
    emitter->emtr_pos.x = offsetX + emitter->p_res->p_base->pos.x;
    offsetY = offsetsY.values[attackerType][defenderType] * 172u;
    emitter->emtr_pos.y = offsetY + emitter->p_res->p_base->pos.y;
    emitter->emtr_pos.z = emitter->p_res->p_base->pos.z;
}

#pragma force_active reset

// kind: 1 x, 2 y, 3 z, 4 xy, 5 xz, 6 yz, 7 xyz
void BattleAnimUtil_ScaleVecComponents(int kind, int scale, VecFx32 *vec) {
    fx32 *z = &vec->z;

    switch (kind) {
    case 1:
        vec->x *= scale;
        break;
    case 2:
        vec->y *= scale;
        break;
    case 3:
        *z *= scale;
        break;
    case 4:
        vec->x *= scale;
        vec->y *= scale;
        break;
    case 5:
        vec->x *= scale;
        *z *= scale;
        break;
    case 6:
        vec->y *= scale;
        *z *= scale;
        break;
    case 7:
        vec->x *= scale;
        vec->y *= scale;
        *z *= scale;
        break;
    }
}

#pragma force_active on

// kind: 0 origin, 1 current convergence position mirrored by scale, 2 up, 3 battlerB, 4 battlerA, 5 script arguments;
// relative to the emitter position origin
void BattleAnimEmitter_SetConvergenceTarget(BattleAnimSystem *system, SPLEmitter *emitter, int battlerA, int battlerB, int kind, s8 scale, VecFx32 *origin) {
    VecFx32 pos = { 0, 0, 0 };

    switch (kind) {
    case 0:
        break;
    case 1:
        ParticleEmitter_GetConvergencePos(emitter, &pos);
        pos.x *= scale;
        pos.y *= scale;
        pos.z *= scale;
        break;
    case 2:
        pos.x = 0;
        pos.y = 3200 * scale;
        pos.z = 0;
        break;
    case 3:
        BattleAnimUtil_GetBattlerEmitterPositionKind0(system, battlerB, &pos);
        break;
    case 4:
        BattleAnimUtil_GetBattlerEmitterPositionKind0(system, battlerA, &pos);
        break;
    case 5: {
        BattleAnimEmitterTargetArgs args = { 0 };
        BattleAnimSystem_ReadScriptArgs(system, (u32 *)&args, 4);
        pos.x = args.pos.x;
        pos.y = args.pos.y;
        pos.z = args.pos.z;
        BattleAnimUtil_ScaleVecComponents(args.scaleKind, scale, &pos);
        break;
    }
    }
    pos.x -= origin->x;
    pos.y -= origin->y;
    pos.z -= origin->z;
    ParticleEmitter_SetConvergencePos(emitter, &pos);
}

// kind as in BattleAnimEmitter_SetConvergenceTarget (no origin kind)
void BattleAnimEmitter_SetMagnetTarget(BattleAnimSystem *system, SPLEmitter *emitter, u32 battlerA, u32 battlerB, u32 kind, s8 scale, const VecFx32 *origin) {
    VecFx32 pos = { 0, 0, 0 };

    switch (kind) {
    case 1:
        ParticleEmitter_GetMagnetPos(emitter, &pos);
        pos.x *= scale;
        pos.y *= scale;
        pos.z *= scale;
        break;
    case 2:
        pos.x = 0;
        pos.y = 3200 * scale;
        pos.z = 0;
        break;
    case 3:
        BattleAnimUtil_GetBattlerEmitterPositionKind0(system, battlerB, &pos);
        break;
    case 4:
        BattleAnimUtil_GetBattlerEmitterPositionKind0(system, battlerA, &pos);
        break;
    case 5: {
        s32 args[4] = { 0, 0, 0, 0 }; // BattleAnimEmitterTargetArgs
        BattleAnimSystem_ReadScriptArgs(system, (u32 *)args, 4);
        pos.x = args[0];
        pos.y = args[1];
        pos.z = args[2];
        BattleAnimUtil_ScaleVecComponents(args[3], scale, &pos);
    } break;
    }
    pos.x -= origin->x;
    pos.y -= origin->y;
    pos.z -= origin->z;
    ParticleEmitter_SetMagnetPos(emitter, &pos);
}

static inline void SetAxis(volatile BattleAnimEmitterAxis *axis, VecFx32 *src) {
    axis->x = src->x;
    axis->y = src->y;
    axis->z = src->z;
}

static inline void SetAxisNegated(volatile BattleAnimEmitterAxis *axis, VecFx32 *src) {
    axis->x = -src->x;
    axis->y = -src->y;
    axis->z = -src->z;
}

// Reads 8 VecFx32 script arguments: the axis for the pair of battler types (7), the position (0: at battlerA,
// mirrored by side). mode 1 / 2: then sets the convergence / magnet target of the given kind.
void BattleAnimEmitter_SetTransformFromScript(BattleAnimSystem *system, SPLEmitter *emitter, int battlerA, int battlerB, int mode, int kind) {
    VecFx32 final, axis0, axis1, axis3, axis2, axis6, axis4, axis5;
    volatile VecFx32 posArg;
    VecFx32 pos;
    volatile BattleAnimEmitterAxis axis;
    int typeA, typeB;
    s8 sign;

    BattleAnimSystem_GetCurrentParticleSystem(system);
    typeA = BattleAnimUtil_GetBattlerType(system, battlerA);
    typeB = BattleAnimUtil_GetBattlerType(system, battlerB);
    BattleAnimSystem_ReadScriptArgs(system, (u32 *)&axis0, 3);
    BattleAnimSystem_ReadScriptArgs(system, (u32 *)&axis1, 3);
    BattleAnimSystem_ReadScriptArgs(system, (u32 *)&axis2, 3);
    BattleAnimSystem_ReadScriptArgs(system, (u32 *)&axis3, 3);
    BattleAnimSystem_ReadScriptArgs(system, (u32 *)&axis4, 3);
    BattleAnimSystem_ReadScriptArgs(system, (u32 *)&axis5, 3);
    BattleAnimSystem_ReadScriptArgs(system, (u32 *)&axis6, 3);
    switch (typeA) {
    default:
    case 0:
        SetAxis(&axis, &axis0);
        break;
    case 1:
        SetAxisNegated(&axis, &axis0);
        break;
    case 2:
        if (typeB == 3) {
            SetAxis(&axis, &axis1);
        } else if (typeB == 5) {
            SetAxis(&axis, &axis2);
        } else {
            SetAxis(&axis, &axis3);
        }
        break;
    case 3:
        if (typeB == 2) {
            SetAxisNegated(&axis, &axis1);
        } else if (typeB == 5) {
            SetAxisNegated(&axis, &axis3);
        } else {
            SetAxisNegated(&axis, &axis4);
        }
        break;
    case 4:
        if (typeB == 3) {
            SetAxis(&axis, &axis4);
        } else if (typeB == 5) {
            SetAxis(&axis, &axis5);
        } else {
            SetAxis(&axis, &axis6);
        }
        break;
    case 5:
        if (typeB == 3) {
            SetAxisNegated(&axis, &axis6);
        } else if (typeB == 2) {
            SetAxisNegated(&axis, &axis2);
        } else {
            SetAxisNegated(&axis, &axis5);
        }
        break;
    }
    // called as returning s8 (the result is not truncated again)
    sign = ((s8(*)(BattleAnimSystem *, int, int))BattleAnimSystem_GetSideSign)(system, battlerA, battlerB);
    memset((void *)&posArg, 0, sizeof(posArg));
    BattleAnimSystem_ReadScriptArgs(system, (u32 *)&posArg, 3);
    BattleAnimUtil_GetBattlerEmitterPositionKind0(system, battlerA, &pos);
    if (posArg.x == 0 && posArg.y == 0 && posArg.z == 0) {
        final = pos;
        SPL_SetEmitterPosition(emitter, &pos);
    } else {
        final.x = posArg.x;
        final.y = posArg.y;
        final.z = posArg.z;
        if (final.x == 0 && final.y == 3200) {
            sign = 1;
        }
        final.x *= sign;
        final.y *= sign;
        final.z = pos.z * sign;
        SPL_SetEmitterPosition(emitter, &final);
    }
    switch (mode) {
    case 0:
        break;
    case 1:
        BattleAnimEmitter_SetConvergenceTarget(system, emitter, battlerA, battlerB, kind, sign, &final);
        break;
    case 2:
        BattleAnimEmitter_SetMagnetTarget(system, emitter, battlerA, battlerB, kind, sign, &final);
        break;
    }
    emitter->axis.x = axis.x;
    emitter->axis.y = axis.y;
    emitter->axis.z = axis.z;
}

#pragma force_active reset

void BattleAnimEmitter_SetTransformAttackerToDefender(SPLEmitter *emitter, int mode, int kind) {
    BattleAnimSystem *system = ParticleSystem_GetEmitterParam();
    int attacker = BattleAnimSystem_GetAttacker(system);
    int defender = BattleAnimSystem_GetDefender(system);
    BattleAnimEmitter_SetTransformFromScript(system, emitter, attacker, defender, mode, kind);
}

void BattleAnimEmitterCb_AttackerToDefender(SPLEmitter *emitter) {
    BattleAnimEmitter_SetTransformAttackerToDefender(emitter, 0, 0);
}

// Attacker to defender, then the convergence target (BattleAnimEmitter_SetConvergenceTarget kinds 1-5)
void BattleAnimEmitterCb_ConvergeBySide(SPLEmitter *emitter) {
    BattleAnimEmitter_SetTransformAttackerToDefender(emitter, 1, 1);
}

void BattleAnimEmitterCb_ConvergeUp(SPLEmitter *emitter) {
    BattleAnimEmitter_SetTransformAttackerToDefender(emitter, 1, 2);
}

void BattleAnimEmitterCb_ConvergeToDefender(SPLEmitter *emitter) {
    BattleAnimEmitter_SetTransformAttackerToDefender(emitter, 1, 3);
}

void BattleAnimEmitterCb_ConvergeToAttacker(SPLEmitter *emitter) {
    BattleAnimEmitter_SetTransformAttackerToDefender(emitter, 1, 4);
}

void BattleAnimEmitterCb_ConvergeFromScript(SPLEmitter *emitter) {
    BattleAnimEmitter_SetTransformAttackerToDefender(emitter, 1, 5);
}

// Attacker to defender, then the magnet target (same kinds)
void BattleAnimEmitterCb_MagnetBySide(SPLEmitter *emitter) {
    BattleAnimEmitter_SetTransformAttackerToDefender(emitter, 2, 1);
}

void BattleAnimEmitterCb_MagnetUp(SPLEmitter *emitter) {
    BattleAnimEmitter_SetTransformAttackerToDefender(emitter, 2, 2);
}

void BattleAnimEmitterCb_MagnetToDefender(SPLEmitter *emitter) {
    BattleAnimEmitter_SetTransformAttackerToDefender(emitter, 2, 3);
}

void BattleAnimEmitterCb_MagnetToAttacker(SPLEmitter *emitter) {
    BattleAnimEmitter_SetTransformAttackerToDefender(emitter, 2, 4);
}

void BattleAnimEmitterCb_MagnetFromScript(SPLEmitter *emitter) {
    BattleAnimEmitter_SetTransformAttackerToDefender(emitter, 2, 5);
}

void BattleAnimEmitterCb_DefenderToAttacker(SPLEmitter *emitter) {
    BattleAnimSystem *system = ParticleSystem_GetEmitterParam();
    int attacker = BattleAnimSystem_GetAttacker(system);
    int defender = BattleAnimSystem_GetDefender(system);
    BattleAnimEmitter_SetTransformFromScript(system, emitter, defender, attacker, 0, 0);
}

void BattleAnimEmitter_UpdateConfigured(SPLEmitter *emitter, BattleAnimEmitterConfig *config) {
    if (config->positionMode != 0) {
        BattleAnimEmitter_ApplyConfiguredPosition(emitter, config);
    }
    if (config->axisMode != 0) {
        BattleAnimEmitter_ApplyConfiguredAxis(emitter, config);
    }
    if (config->flags != 0) {
        BattleAnimEmitter_ApplyConfiguredFlags(emitter, config);
    }
    BattleAnimEmitter_ApplyConfiguredCamera(emitter, config);
    if (config->zMode != 0) {
        BattleAnimEmitter_ApplyConfiguredZ(emitter, config);
    }
}

void BattleAnimEmitter_GetDefenderSidePosition(SPLEmitter *emitter, BattleAnimEmitterConfig *config, VecFx32 *pos) {
    BattleAnimSystem *system = config->system;
    int perspective = ParticleSystem_GetPerspectiveType((ParticleSystem *)BattleAnimSystem_GetCurrentParticleSystem(system));
    int defender = BattleAnimSystem_GetDefender(system);
    int type = BattleAnimUtil_GetBattlerSide(system, defender) != 3;
    int mode = ov07_0221BFC0(system);
    BattleAnimUtil_GetEmitterPositionKind0(type, pos, mode, perspective);
}

void BattleAnimEmitter_GetAttackerSidePosition(SPLEmitter *emitter, BattleAnimEmitterConfig *config, VecFx32 *pos) {
    BattleAnimSystem *system = config->system;
    int perspective = ParticleSystem_GetPerspectiveType((ParticleSystem *)BattleAnimSystem_GetCurrentParticleSystem(system));
    int attacker = BattleAnimSystem_GetAttacker(system);
    int type = BattleAnimUtil_GetBattlerSide(system, attacker) != 3;
    int mode = ov07_0221BFC0(system);
    BattleAnimUtil_GetEmitterPositionKind0(type, pos, mode, perspective);
}

void BattleAnimEmitter_ApplyConfiguredPosition(SPLEmitter *emitter, BattleAnimEmitterConfig *config) {
    VecFx32 pos;
    BattleAnimEmitterOffsetArgs args;
    BattleAnimScreenPosTable6 positions6;
    BattleAnimScreenPosTable7 positions7;
    BattleAnimScreenPosTable7 positions7b;

    switch (config->positionMode) {
    case 1:
        BattleAnimUtil_GetBattlerEmitterPositionKind0(config->system, config->battlerA, &pos);
        break;
    case 2:
        BattleAnimUtil_GetBattlerEmitterPositionKind0(config->system, config->battlerB, &pos);
        break;
    case 6:
        BattleAnimUtil_GetBattlerEmitterPositionKind1(config->system, config->battlerA, &pos);
        break;
    case 7:
        BattleAnimUtil_GetBattlerEmitterPositionKind1(config->system, config->battlerB, &pos);
        break;
    case 10:
        BattleAnimUtil_GetBattlerEmitterPositionKind2(config->system, config->battlerA, &pos);
        break;
    case 11:
        BattleAnimUtil_GetBattlerEmitterPositionKind2(config->system, config->battlerB, &pos);
        break;
    case 14:
        BattleAnimUtil_GetBattlerEmitterPositionKind3(config->system, config->battlerA, &pos);
        break;
    case 15:
        BattleAnimUtil_GetBattlerEmitterPositionKind3(config->system, config->battlerB, &pos);
        break;
    case 16:
        BattleAnimUtil_GetBattlerEmitterPositionKind4(config->system, config->battlerA, &pos);
        break;
    case 17:
        BattleAnimUtil_GetBattlerEmitterPositionKind4(config->system, config->battlerB, &pos);
        break;
    case 18:
        BattleAnimUtil_GetBattlerEmitterPositionKind5(config->system, config->battlerA, &pos);
        break;
    case 19:
        BattleAnimUtil_GetBattlerEmitterPositionKind5(config->system, config->battlerB, &pos);
        break;
    case 20:
        BattleAnimUtil_GetBattlerEmitterPositionKind6(config->system, config->battlerA, &pos);
        break;
    case 21:
        BattleAnimUtil_GetBattlerEmitterPositionKind6(config->system, config->battlerB, &pos);
        break;
    case 22:
        BattleAnimUtil_GetBattlerEmitterPositionKind7(config->system, config->battlerA, &pos);
        break;
    case 23:
        BattleAnimUtil_GetBattlerEmitterPositionKind7(config->system, config->battlerB, &pos);
        break;
    case 24:
        BattleAnimUtil_GetBattlerEmitterPositionKind8(config->system, config->battlerA, &pos);
        break;
    case 25:
        BattleAnimUtil_GetBattlerEmitterPositionKind8(config->system, config->battlerB, &pos);
        break;
    case 26:
        BattleAnimUtil_GetBattlerEmitterPositionKind9(config->system, config->battlerA, &pos);
        break;
    case 27:
        BattleAnimUtil_GetBattlerEmitterPositionKind9(config->system, config->battlerB, &pos);
        break;
    case 8:
        BattleAnimUtil_GetBattlerEmitterPositionKind11(config->system, config->battlerA, &pos);
        break;
    case 9:
        BattleAnimUtil_GetBattlerEmitterPositionKind11(config->system, config->battlerB, &pos);
        break;
    case 3:
        BattleAnimSystem_ReadScriptArgs(config->system, (u32 *)&args, 4);
        config->sideSign = BattleAnimEmitter_GetConfiguredSideSign(config);
        pos.x = args.offset.x;
        pos.y = args.offset.y;
        pos.z = args.offset.z;
        if (args.ignoreSide == TRUE) {
            config->sideSign = 1;
        }
        pos.x *= config->sideSign;
        pos.y *= config->sideSign;
        break;
    case 4:
        BattleAnimUtil_GetBattlerEmitterPositionKind0(config->system, config->battlerA, &pos);
        BattleAnimEmitter_AddOffsetFromScript(config, &pos);
        break;
    case 5:
        BattleAnimUtil_GetBattlerEmitterPositionKind0(config->system, config->battlerB, &pos);
        BattleAnimEmitter_AddOffsetFromScript(config, &pos);
        break;
    case 100:
        BattleAnimUtil_GetBattlerEmitterPositionKind0(config->system, config->battlerA, &pos);
        BattleAnimEmitter_AddOffsetForBattlerType(config, config->battlerA, &pos);
        break;
    case 101:
        BattleAnimUtil_GetBattlerEmitterPositionKind0(config->system, config->battlerB, &pos);
        BattleAnimEmitter_AddOffsetForBattlerType(config, config->battlerB, &pos);
        break;
    case 12:
        BattleAnimEmitter_GetAttackerSidePosition(emitter, config, &pos);
        BattleAnimEmitter_AddOffsetFromScript(config, &pos);
        break;
    case 13:
        BattleAnimEmitter_GetDefenderSidePosition(emitter, config, &pos);
        BattleAnimEmitter_AddOffsetFromScript(config, &pos);
        break;
    case 28:
        pos.x = 0x2CE0;
        pos.y = 0;
        pos.z = 0;
        break;
    case 33:
        pos.x = -5000;
        pos.y = -6000;
        pos.z = 0;
        break;
    case 34:
        BattleAnimUtil_GetBattlerEmitterPositionKind0(config->system, config->battlerA, &pos);
        break;
    case 31:
        positions6 = ov07_022354E0;
        {
            int type = BattleAnimUtil_GetBattlerType(config->system, config->battlerA);
            pos.x = positions6.pos[type].x;
            pos.y = positions6.pos[type].y;
        }
        pos.z = 0;
        break;
    case 30:
        positions7 = ov07_02235578;
        {
            int type;
            if (ov07_0221BFC0(config->system) == 1) {
                type = 6;
            } else {
                type = BattleAnimUtil_GetBattlerType(config->system, config->battlerA);
            }
            pos.x = positions7.pos[type].x;
            pos.y = positions7.pos[type].y;
        }
        pos.z = 0;
        break;
    case 32:
        positions7b = ov07_02235540;
        {
            int type;
            if (ov07_0221BFC0(config->system) == 1) {
                type = 6;
            } else {
                type = BattleAnimUtil_GetBattlerType(config->system, config->battlerA);
            }
            pos.x = positions7b.pos[type].x;
            pos.y = positions7b.pos[type].y;
        }
        pos.z = 0;
        break;
    default:
        break;
    }
    config->position = pos;
    SPL_SetEmitterPosition(emitter, &pos);
}

#define SET_AXIS(x_, y_, z_) \
    axis.x = (x_);           \
    axis.y = (y_);           \
    axis.z = (z_)
#define SET_AXIS_FROM_TABLES(tableX, tableY, tableZ, divZ, signX) \
    SET_AXIS((signX) * tableX[typeA][typeB].v[0], tableY[typeA][typeB].v[0], tableZ[typeA][typeB].v[0] / (divZ))
#define SET_AXIS_FROM_ROW(row)         SET_AXIS((row).v[0], (row).v[1], (row).v[2])
#define SET_AXIS_FROM_ROW_NEGATED(row) SET_AXIS(-(row).v[0], -(row).v[1], -(row).v[2])

#pragma force_active on

void BattleAnimEmitter_ApplyConfiguredAxis(SPLEmitter *emitter, BattleAnimEmitterConfig *config) {
    BattleAnimEmitterAxisRows table;
    BattleAnimEmitterAxisRow a, b, c, d, e, f, g, base;
    BattleAnimEmitterAxisPositions positions;
    volatile VecFx16 axis;
    int typeA = BattleAnimUtil_GetBattlerType(config->system, config->battlerA);
    int typeB = BattleAnimUtil_GetBattlerType(config->system, config->battlerB);
    int i, type;

    switch (config->axisMode) {
    case 0:
        SET_AXIS(0, 0, 0);
        break;
    case 1:
    case 2:
        if (ov07_0221BFC0(config->system) == 1) {
            if (config->battlerOrder == 2) {
                SET_AXIS(-3336, 1840, 736);
            } else {
                SET_AXIS(2336, -1440, 736);
            }
        } else {
            SET_AXIS_FROM_TABLES(ov07_02235CE0, ov07_02235CE4, ov07_02235CE8, 2, 1);
        }
        break;
    case 4:
        if (ov07_0221BFC0(config->system) == 1) {
            SET_AXIS_FROM_TABLES(ov07_02235CE0, ov07_02235CE4, ov07_02235CE8, 2, -1);
        } else if (BattleAnimUtil_GetBattlerSide(config->system, config->battlerA) == BattleAnimUtil_GetBattlerSide(config->system, config->battlerB)) {
            SET_AXIS_FROM_TABLES(ov07_02235CE0, ov07_02235CE4, ov07_02235CE8, 2, 1);
        } else if (BattleAnimUtil_GetBattlerSide(config->system, config->battlerA) == 3) {
            SET_AXIS(3776, 2112, 1532);
        } else {
            SET_AXIS(-4228, -2728, 1532);
        }
        break;
    case 5:
        if (ov07_0221BFC0(config->system) == 1) {
            SET_AXIS_FROM_TABLES(ov07_02235CE0, ov07_02235CE4, ov07_02235CE8, 2, -1);
        } else if (BattleAnimUtil_GetBattlerSide(config->system, config->battlerA) == BattleAnimUtil_GetBattlerSide(config->system, config->battlerB)) {
            SET_AXIS_FROM_TABLES(ov07_02235CE0, ov07_02235CE4, ov07_02235CE8, 2, 1);
        } else if (BattleAnimUtil_GetBattlerSide(config->system, config->battlerA) == 3) {
            SET_AXIS(3776, 2112, 1532);
        } else {
            SET_AXIS(-4228, -2728, 1532);
        }
        break;
    case 8:
    case 9:
        if (ov07_0221BFC0(config->system) == 1) {
            if (config->battlerOrder == 2) {
                SET_AXIS(-2336, 1440, 736);
            } else {
                SET_AXIS(2336, -1440, 736);
            }
        } else {
            SET_AXIS_FROM_TABLES(ov07_02235620, ov07_02235624, ov07_02235628, 2, 1);
        }
        break;
    case 10:
    case 11:
        if (ov07_0221BFC0(config->system) == 1) {
            SET_AXIS(-2336, 1440, 736);
        } else {
            SET_AXIS_FROM_TABLES(ov07_022357D0, ov07_02235624, ov07_02235628, 2, 1);
        }
        break;
    case 12:
    case 13:
        if (ov07_0221BFC0(config->system) == 1) {
            SET_AXIS(-2936, 1440, 736);
        } else {
            SET_AXIS_FROM_TABLES(ov07_02235B30, ov07_02235B34, ov07_02235B38, 1, 1);
        }
        break;
    case 14:
    case 15:
        if (ov07_0221BFC0(config->system) == 1) {
            SET_AXIS(-2336, 1440, 736);
        } else {
            SET_AXIS_FROM_TABLES(ov07_02235E90, ov07_02235E94, ov07_02235E98, 1, 1);
        }
        break;
    case 16:
    case 17:
        if (ov07_0221BFC0(config->system) == 1) {
            SET_AXIS(-2336, 1440, 736);
        } else {
            SET_AXIS_FROM_TABLES(ov07_02235980, ov07_02235984, ov07_02235988, 1, 1);
        }
        break;
    case 18:
    case 19:
        if (ov07_0221BFC0(config->system) == 1) {
            SET_AXIS(-2336, 1440, 736);
        } else {
            SET_AXIS_FROM_TABLES(ov07_02236040, ov07_02236044, ov07_02236048, 1, 1);
        }
        break;
    case 20:
    case 21:
        if (ov07_0221BFC0(config->system) == 1) {
            SET_AXIS(-4336, 1440, 736);
        } else {
            SET_AXIS_FROM_TABLES(ov07_022361F0, ov07_022361F4, ov07_022361F8, 1, 1);
        }
        break;
    case 6:
    case 7:
        table = ov07_022355B0;
        base = ov07_022354D0;
        for (i = 0; i < 4; i++) {
            a.v[i] = table.rows[0].v[i];
            b.v[i] = table.rows[1].v[i];
            c.v[i] = table.rows[2].v[i];
            d.v[i] = table.rows[3].v[i];
            e.v[i] = table.rows[4].v[i];
            f.v[i] = table.rows[5].v[i];
            g.v[i] = table.rows[6].v[i];
        }
        if (ov07_0221BFC0(config->system) == 1) {
            for (i = 0; i < 4; i++) {
                a.v[i] = base.v[i];
                b.v[i] = base.v[i];
                c.v[i] = base.v[i];
                d.v[i] = base.v[i];
                e.v[i] = base.v[i];
                f.v[i] = base.v[i];
                g.v[i] = base.v[i];
            }
        }
        switch (typeA) {
        default:
        case 0:
            SET_AXIS_FROM_ROW(a);
            break;
        case 1:
            if (a.v[3] == 1) {
                SET_AXIS_FROM_ROW(a);
            } else {
                SET_AXIS_FROM_ROW_NEGATED(a);
            }
            break;
        case 2:
            if (typeB == 3) {
                SET_AXIS_FROM_ROW(b);
            } else if (typeB == 5) {
                SET_AXIS_FROM_ROW(d);
            } else {
                SET_AXIS_FROM_ROW(c);
            }
            break;
        case 3:
            if (typeB == 2) {
                if (b.v[3] == 1) {
                    SET_AXIS_FROM_ROW(b);
                } else {
                    SET_AXIS_FROM_ROW_NEGATED(b);
                }
            } else if (typeB == 5) {
                if (c.v[3] == 1) {
                    SET_AXIS_FROM_ROW_NEGATED(e);
                } else {
                    SET_AXIS_FROM_ROW(e);
                }
            } else {
                if (f.v[3] == 1) {
                    SET_AXIS_FROM_ROW(f);
                } else {
                    SET_AXIS_FROM_ROW_NEGATED(f);
                }
            }
            break;
        case 4:
            if (typeB == 3) {
                SET_AXIS_FROM_ROW(f);
            } else if (typeB == 5) {
                SET_AXIS_FROM_ROW(g);
            } else {
                SET_AXIS_FROM_ROW(e);
            }
            break;
        case 5:
            if (typeB == 3) {
                if (b.v[3] == 1) {
                    SET_AXIS_FROM_ROW(e);
                } else {
                    SET_AXIS_FROM_ROW_NEGATED(e);
                }
            } else if (typeB == 2) {
                if (b.v[3] == 1) {
                    SET_AXIS_FROM_ROW(d);
                } else {
                    SET_AXIS_FROM_ROW_NEGATED(d);
                }
            } else {
                if (b.v[3] == 1) {
                    SET_AXIS_FROM_ROW(g);
                } else {
                    SET_AXIS_FROM_ROW_NEGATED(g);
                }
            }
            break;
        }
        break;
    case 3:
        SET_AXIS(-800, 1200, 500);
        break;
    case 22:
        SET_AXIS(-3410, -2644, 0);
        break;
    case 25:
        SET_AXIS(-3440, 1952, 0);
        break;
    case 26:
        if (BattleAnimUtil_GetBattlerSide(config->system, config->battlerA) == 3) {
            SET_AXIS(3776, 2112, 0);
        } else {
            SET_AXIS(-6000, -2200, 0);
        }
        break;
    case 24:
        positions = ov07_02235510;
        type = BattleAnimUtil_GetBattlerType(config->system, config->battlerA);
        SET_AXIS(positions.pos[type][0], positions.pos[type][1], 0);
        break;
    }
    // read back as unsigned halfwords
    emitter->axis.x = ((volatile BattleAnimEmitterAxis *)&axis)->x;
    emitter->axis.y = ((volatile BattleAnimEmitterAxis *)&axis)->y;
    emitter->axis.z = ((volatile BattleAnimEmitterAxis *)&axis)->z;
}

#pragma force_active reset

#undef SET_AXIS
#undef SET_AXIS_FROM_TABLES
#undef SET_AXIS_FROM_ROW
#undef SET_AXIS_FROM_ROW_NEGATED

void BattleAnimEmitter_ApplyConfiguredFlags(SPLEmitter *emitter, BattleAnimEmitterConfig *config) {
    int count = BattleAnimEmitter_CountFlagMasks();
    int i;

    for (i = 0; i < count; i++) {
        int mask = BattleAnimEmitter_GetFlagMask(i);
        if ((config->flags & mask) == mask) {
            BattleAnimEmitter_RunFlagCallback(i, emitter, config);
        }
    }
}

#pragma force_active on

void BattleAnimEmitter_ApplyConfiguredCamera(SPLEmitter *emitter, BattleAnimEmitterConfig *config) {
    BattleAnimEmitter_RunCameraCallback(config->cameraMode, emitter, config);
}

void BattleAnimEmitter_ApplyConfiguredZ(SPLEmitter *emitter, BattleAnimEmitterConfig *config) {
    int typeB;
    fx32 z;

    BattleAnimUtil_GetBattlerType(config->system, config->battlerA);
    typeB = BattleAnimUtil_GetBattlerType(config->system, config->battlerB);
    switch (config->zMode) {
    case 0:
        break;
    case 1:
        z = typeB == 0 ? 256 : -5120;
        break;
    case 2:
        z = typeB == 0 ? 0 : -5376;
        break;
    case 3:
        break;
    }
    ParticleSystem_GetPerspectiveType((ParticleSystem *)config->particleSystem);
    SPL_SetEmitterPositionZ(emitter, z);
}

void BattleAnimEmitterCb_ConfigureFromScript(SPLEmitter *emitter) {
    BattleAnimSystem *system = ParticleSystem_GetEmitterParam();
    BattleAnimEmitterConfig *config = Heap_Alloc(BattleAnimSystem_GetHeapID(system), sizeof(BattleAnimEmitterConfig));

    config->system = system;
    config->particleSystem = BattleAnimSystem_GetCurrentParticleSystem(config->system);
    BattleAnimSystem_ReadScriptArgs(config->system, &config->zMode, 6);
    switch (config->battlerOrder) {
    default:
    case 0:
        config->battlerA = BattleAnimSystem_GetAttacker(config->system);
        config->battlerB = BattleAnimSystem_GetDefender(config->system);
        break;
    case 1:
        config->battlerA = BattleAnimSystem_GetDefender(config->system);
        config->battlerB = BattleAnimSystem_GetAttacker(config->system);
        break;
    case 2:
        config->battlerA = BattleAnimSystem_GetAttacker(config->system);
        config->battlerB = BattleAnimSystem_GetDefender(config->system);
        break;
    case 3:
        config->battlerA = BattleAnimSystem_GetDefender(config->system);
        config->battlerB = BattleAnimSystem_GetAttacker(config->system);
        break;
    case 4:
        config->battlerA = BattleAnimSystem_GetAttacker(config->system);
        config->battlerB = BattleAnimSystem_GetDefender(config->system);
        break;
    }
    BattleAnimEmitter_UpdateConfigured(emitter, config);
    Heap_Free(config);
}

#pragma force_active reset

// -1 when battlerA's type is 1, 3 or 5, else +1
int BattleAnimEmitter_GetConfiguredSideSign(BattleAnimEmitterConfig *config) {
    int sign = 1;
    int typeA = BattleAnimUtil_GetBattlerType(config->system, config->battlerA);
    BattleAnimUtil_GetBattlerType(config->system, config->battlerB);

    switch (typeA) {
    case 0:
        break;
    case 1:
        sign -= 2;
        break;
    case 2:
        break;
    case 3:
        sign -= 2;
        break;
    case 4:
        break;
    case 5:
        sign -= 2;
        break;
    }
    return sign;
}

// Reads one BattleAnimEmitterOffsetArgs per battler type (6) and adds the one of battler's type to pos
void BattleAnimEmitter_AddOffsetForBattlerType(BattleAnimEmitterConfig *config, int battler, VecFx32 *pos) {
    VecFx32 offset = { 0 };
    BattleAnimEmitterOffsetArgs args;
    BattleAnimEmitterOffsetArgs skipped;
    int i;

    for (i = 0; i < 6; i++) {
        if (i == BattleAnimUtil_GetBattlerType(config->system, battler)) {
            BattleAnimSystem_ReadScriptArgs(config->system, (u32 *)&args, 4);
        } else {
            BattleAnimSystem_ReadScriptArgs(config->system, (u32 *)&skipped, 4);
        }
    }
    config->sideSign = BattleAnimEmitter_GetConfiguredSideSign(config);
    offset.x = args.offset.x;
    offset.y = args.offset.y;
    offset.z = args.offset.z;
    if (args.ignoreSide == TRUE) {
        config->sideSign = 1;
    }
    offset.x *= config->sideSign;
    offset.y *= config->sideSign;
    offset.z *= config->sideSign;
    pos->x += offset.x;
    pos->y += offset.y;
    pos->z += offset.z;
}

void BattleAnimEmitter_AddOffsetFromScript(BattleAnimEmitterConfig *config, VecFx32 *pos) {
    BattleAnimEmitterOffsetArgs args;
    VecFx32 offset = { 0 };

    BattleAnimSystem_ReadScriptArgs(config->system, (u32 *)&args, 4);
    config->sideSign = BattleAnimEmitter_GetConfiguredSideSign(config);
    offset.x = args.offset.x;
    offset.y = args.offset.y;
    offset.z = args.offset.z;
    if (args.ignoreSide == TRUE) {
        config->sideSign = 1;
    }
    offset.x *= config->sideSign;
    offset.y *= config->sideSign;
    offset.z *= config->sideSign;
    pos->x += offset.x;
    pos->y += offset.y;
    pos->z += offset.z;
}

void BattleAnimEmitter_ReadFlagCbParams(BattleAnimEmitterConfig *config, BattleAnimEmitterFlagParams *params) {
    BattleAnimSystem_ReadScriptArgs(config->system, (u32 *)params, 5);
}

BOOL BattleAnimEmitterFlagCb_Nop0(SPLEmitter *emitter, BattleAnimEmitterConfig *config) {
    return TRUE;
}

#pragma force_active on

// mode 0: none (mirrored zero), 1: script value
BOOL BattleAnimEmitterFlagCb_SetGravity(SPLEmitter *emitter, BattleAnimEmitterConfig *config) {
    BattleAnimEmitterFlagParams params;
    VecFx16 value = { 0, 0, 0 };

    BattleAnimEmitter_ReadFlagCbParams(config, &params);
    config->sideSign = BattleAnimEmitter_GetConfiguredSideSign(config);
    if (params.ignoreSide == TRUE) {
        config->sideSign = 1;
    }
    switch (params.mode) {
    case 0:
        ParticleEmitter_SetGravity(emitter, &value);
        value.x *= config->sideSign;
        value.y *= config->sideSign;
        value.z *= config->sideSign;
        break;
    case 1:
        value.x = params.value.x;
        value.y = params.value.y;
        value.z = params.value.z;
        value.x *= config->sideSign;
        value.y *= config->sideSign;
        value.z *= config->sideSign;
        ParticleEmitter_SetGravity(emitter, &value);
        return TRUE;
    case 2:
    case 3:
    case 255:
        break;
    }
    value.x -= config->position.x;
    value.y -= config->position.y;
    value.z -= config->position.z;
    ParticleEmitter_SetGravity(emitter, &value);
    return TRUE;
}

#pragma force_active reset

BOOL BattleAnimEmitterFlagCb_Nop2(SPLEmitter *emitter, BattleAnimEmitterConfig *config) {
    return TRUE;
}

BOOL BattleAnimEmitterFlagCb_Nop3(SPLEmitter *emitter, BattleAnimEmitterConfig *config) {
    return TRUE;
}

// mode 0: current position mirrored, 1: script value, 2: battlerA, 3: battlerB; relative to the configured position
BOOL BattleAnimEmitterFlagCb_SetMagnetPos(SPLEmitter *emitter, BattleAnimEmitterConfig *config) {
    BattleAnimEmitterFlagParams params;
    VecFx32 value = { 0 };

    BattleAnimEmitter_ReadFlagCbParams(config, &params);
    config->sideSign = BattleAnimEmitter_GetConfiguredSideSign(config);
    if (params.ignoreSide == TRUE) {
        config->sideSign = 1;
    }
    switch (params.mode) {
    case 0:
        ParticleEmitter_GetMagnetPos(emitter, &value);
        value.x *= config->sideSign;
        value.y *= config->sideSign;
        value.z *= config->sideSign;
        break;
    case 1:
        value.x = params.value.x;
        value.y = params.value.y;
        value.z = params.value.z;
        value.x *= config->sideSign;
        value.y *= config->sideSign;
        value.z *= config->sideSign;
        break;
    case 2:
        BattleAnimUtil_GetBattlerEmitterPositionKind0(config->system, config->battlerA, &value);
        break;
    case 3:
        BattleAnimUtil_GetBattlerEmitterPositionKind0(config->system, config->battlerB, &value);
        break;
    case 0xFF:
        break;
    }
    value.x -= config->position.x;
    value.y -= config->position.y;
    ParticleEmitter_SetMagnetPos(emitter, &value);
    return TRUE;
}

BOOL BattleAnimEmitterFlagCb_SetMagnetForce(SPLEmitter *emitter, BattleAnimEmitterConfig *config) {
    s16 value = 0;
    BattleAnimEmitterFlagParams params;

    BattleAnimEmitter_ReadFlagCbParams(config, &params);
    config->sideSign = BattleAnimEmitter_GetConfiguredSideSign(config);
    if (params.ignoreSide == TRUE) {
        config->sideSign = 1;
    }
    switch (params.mode) {
    case 0:
        ParticleEmitter_GetMagnetForce(emitter, &value);
        value *= config->sideSign;
        break;
    case 1:
        value = params.value.x;
        value *= config->sideSign;
        break;
    case 2:
    case 3:
    case 0xFF:
        break;
    }
    ParticleEmitter_SetMagnetForce(emitter, &value);
    return TRUE;
}

BOOL BattleAnimEmitterFlagCb_SetSpinAngle(SPLEmitter *emitter, BattleAnimEmitterConfig *config) {
    u16 value = 0;
    BattleAnimEmitterFlagParams params;

    BattleAnimEmitter_ReadFlagCbParams(config, &params);
    config->sideSign = BattleAnimEmitter_GetConfiguredSideSign(config);
    if (params.ignoreSide == TRUE) {
        config->sideSign = 1;
    }
    switch (params.mode) {
    case 0:
        ParticleEmitter_GetSpinAngle(emitter, &value);
        value *= config->sideSign;
        break;
    case 1:
        value = params.value.x;
        value *= config->sideSign;
        break;
    case 2:
    case 3:
    case 0xFF:
        break;
    }
    ParticleEmitter_SetSpinAngle(emitter, &value);
    return TRUE;
}

BOOL BattleAnimEmitterFlagCb_SetSpinAxis(SPLEmitter *emitter, BattleAnimEmitterConfig *config) {
    u16 value = 0;
    BattleAnimEmitterFlagParams params;

    BattleAnimEmitter_ReadFlagCbParams(config, &params);
    config->sideSign = BattleAnimEmitter_GetConfiguredSideSign(config);
    if (params.ignoreSide == TRUE) {
        config->sideSign = 1;
    }
    switch (params.mode) {
    case 0:
        ParticleEmitter_GetSpinAxis(emitter, &value);
        break;
    case 1:
        value = params.value.x;
        break;
    case 2:
    case 3:
    case 0xFF:
        break;
    }
    ParticleEmitter_SetSpinAxis(emitter, &value);
    return TRUE;
}

BOOL BattleAnimEmitterFlagCb_Nop8(SPLEmitter *emitter, BattleAnimEmitterConfig *config) {
    return TRUE;
}

BOOL BattleAnimEmitterFlagCb_Nop9(SPLEmitter *emitter, BattleAnimEmitterConfig *config) {
    return TRUE;
}

BOOL BattleAnimEmitterFlagCb_Nop10(SPLEmitter *emitter, BattleAnimEmitterConfig *config) {
    return TRUE;
}

BOOL BattleAnimEmitterFlagCb_Nop11(SPLEmitter *emitter, BattleAnimEmitterConfig *config) {
    return TRUE;
}

#pragma force_active on

// mode as in SetMagnetPos; 4: battlerB position scaled by value.x / value.y
BOOL BattleAnimEmitterFlagCb_SetConvergencePos(SPLEmitter *emitter, BattleAnimEmitterConfig *config) {
    BattleAnimEmitterFlagParams params;
    VecFx32 value = { 0, 0, 0 };

    BattleAnimEmitter_ReadFlagCbParams(config, &params);
    config->sideSign = BattleAnimEmitter_GetConfiguredSideSign(config);
    if (params.ignoreSide == TRUE) {
        config->sideSign = 1;
    }
    switch (params.mode) {
    case 0:
        ParticleEmitter_GetConvergencePos(emitter, &value);
        value.x *= config->sideSign;
        value.y *= config->sideSign;
        value.z *= config->sideSign;
        break;
    case 1:
        value.x = params.value.x;
        value.y = params.value.y;
        value.z = params.value.z;
        value.x *= config->sideSign;
        value.y *= config->sideSign;
        value.z *= config->sideSign;
        break;
    case 4:
        BattleAnimUtil_GetBattlerEmitterPositionKind0(config->system, config->battlerB, &value);
        {
            int num = params.value.x, den = params.value.y;
            value.x = value.x * num / den;
            value.y = value.y * num / den;
            value.z = value.z * num / den;
        }
        break;
    case 2:
        BattleAnimUtil_GetBattlerEmitterPositionKind0(config->system, config->battlerA, &value);
        break;
    case 3:
        BattleAnimUtil_GetBattlerEmitterPositionKind0(config->system, config->battlerB, &value);
        break;
    case 255:
        break;
    }
    value.x -= config->position.x;
    value.y -= config->position.y;
    value.z -= config->position.z;
    ParticleEmitter_SetConvergencePos(emitter, &value);
    return TRUE;
}

BOOL BattleAnimEmitterFlagCb_SetConvergenceForce(SPLEmitter *emitter, BattleAnimEmitterConfig *config) {
    BattleAnimEmitterFlagParams params;
    s16 value = 0;

    BattleAnimEmitter_ReadFlagCbParams(config, &params);
    config->sideSign = BattleAnimEmitter_GetConfiguredSideSign(config);
    if (params.ignoreSide == TRUE) {
        config->sideSign = 1;
    }
    switch (params.mode) {
    case 0:
        ParticleEmitter_GetConvergenceForce(emitter, &value);
        value *= config->sideSign;
        break;
    case 1:
        value = params.value.x;
        value *= config->sideSign;
        break;
    case 2:
    case 3:
    case 255:
        break;
    }
    ParticleEmitter_SetConvergenceForce(emitter, &value);
    return TRUE;
}

#pragma force_active reset

void BattleAnimEmitter_RunFlagCallback(int index, SPLEmitter *emitter, BattleAnimEmitterConfig *config) {
    sBattleAnimEmitterFlagCallbacks[index](emitter, config);
}

int BattleAnimEmitter_GetFlagMask(int index) {
    BattleAnimEmitterFlagMasks table = sBattleAnimEmitterFlagMasks;
    return table.masks[index];
}

// Entries up to and including the 0xFEFE terminator (0 if there is none in the first 255)
int BattleAnimEmitter_CountFlagMasks(void) {
    int index = 0;
    int count = 0;

    while (1) {
        int mask = BattleAnimEmitter_GetFlagMask(index);
        index++;
        if (mask == 0xFEFE) {
            break;
        }
        count++;
        if (count >= 0xFF) {
            break;
        }
    }
    if (count >= 0xFF) {
        return 0;
    }
    return index;
}

void BattleAnimEmitter_ReadCameraCbParams(BattleAnimEmitterConfig *config, u32 *params) {
    BattleAnimSystem_ReadScriptArgs(config->system, params, 4);
}

BOOL BattleAnimEmitterCameraCb_ResetAngle(SPLEmitter *emitter, BattleAnimEmitterConfig *config) {
    CameraAngle angle = { 0, 0, 0, 0 };
    Camera_SetAnglePos(&angle, ParticleSystem_GetCamera(config->particleSystem));
    return TRUE;
}

// Field by field through volatile pointers
static inline void CopyCameraAngle(volatile CameraAngle *dst, const volatile CameraAngle *src) {
    u16 z, unused;

    dst->x = src->x;
    dst->y = src->y;
    z = src->z;
    unused = src->unused;
    dst->z = z;
    dst->unused = unused;
}

#pragma force_active on

BOOL BattleAnimEmitterCameraCb_SetPresetAngle4(SPLEmitter *emitter, BattleAnimEmitterConfig *config) {
    CameraAngle angle;
    Camera *camera;

    CopyCameraAngle(&angle, &sBattleAnimEmitterCameraAngles[4]);
    camera = ParticleSystem_GetCamera(config->particleSystem);
    ParticleSystem_SetPerspectiveType(config->particleSystem, 1);
    Camera_SetAnglePos(&angle, camera);
    return TRUE;
}

#pragma force_active reset

BOOL BattleAnimEmitterCameraCb_SetAngleFromScript(SPLEmitter *emitter, BattleAnimEmitterConfig *config) {
    CameraAngle angle = { 0, 0, 0, 0 };
    u32 params[4];

    BattleAnimEmitter_ReadCameraCbParams(config, params);
    angle.x = params[1];
    angle.y = params[2];
    angle.z = params[3];
    Camera_SetAnglePos(&angle, ParticleSystem_GetCamera(config->particleSystem));
    return TRUE;
}

#pragma force_active on

BOOL BattleAnimEmitterCameraCb_SetPresetAngle3(SPLEmitter *emitter, BattleAnimEmitterConfig *config) {
    CameraAngle angle;
    Camera *camera;

    CopyCameraAngle(&angle, &sBattleAnimEmitterCameraAngles[3]);
    camera = ParticleSystem_GetCamera(config->particleSystem);
    ParticleSystem_SetPerspectiveType(config->particleSystem, 1);
    Camera_SetAnglePos(&angle, camera);
    return TRUE;
}

#pragma force_active reset

// Preset 2 in the 0221BFC0 mode, else preset 5
BOOL BattleAnimEmitterCameraCb_SetAngleByMode(SPLEmitter *emitter, BattleAnimEmitterConfig *config) {
    CameraAngle angle1 = sBattleAnimEmitterCameraAngles[2];
    CameraAngle angle2 = sBattleAnimEmitterCameraAngles[5];
    Camera *camera = ParticleSystem_GetCamera(config->particleSystem);

    ParticleSystem_SetPerspectiveType(config->particleSystem, 1);
    if (ov07_0221BFC0(config->system) == 1) {
        Camera_SetAnglePos(&angle1, camera);
    } else {
        Camera_SetAnglePos(&angle2, camera);
    }
    return TRUE;
}

#pragma force_active on

BOOL BattleAnimEmitterCameraCb_SetPresetAngle0(SPLEmitter *emitter, BattleAnimEmitterConfig *config) {
    CameraAngle angle;
    Camera *camera;

    CopyCameraAngle(&angle, &sBattleAnimEmitterCameraAngles[0]);
    camera = ParticleSystem_GetCamera(config->particleSystem);
    ParticleSystem_SetPerspectiveType(config->particleSystem, 1);
    Camera_SetAnglePos(&angle, camera);
    return TRUE;
}

BOOL BattleAnimEmitterCameraCb_SetPresetAngle1(SPLEmitter *emitter, BattleAnimEmitterConfig *config) {
    CameraAngle angle;
    Camera *camera;

    CopyCameraAngle(&angle, &sBattleAnimEmitterCameraAngles[1]);
    camera = ParticleSystem_GetCamera(config->particleSystem);
    ParticleSystem_SetPerspectiveType(config->particleSystem, 1);
    Camera_SetAnglePos(&angle, camera);
    return TRUE;
}

#pragma force_active reset

BOOL BattleAnimEmitterCameraCb_LookAtBattlerA(SPLEmitter *emitter, BattleAnimEmitterConfig *config) {
    VecFx32 target;
    Camera *camera = ParticleSystem_GetCamera(config->particleSystem);
    BattleAnimUtil_GetBattlerEmitterPositionKind10(config->system, config->battlerA, &target);
    Camera_SetLookAtCamTarget(&target, camera);
    return TRUE;
}

BOOL BattleAnimEmitterCameraCb_LookAtBattlerB(SPLEmitter *emitter, BattleAnimEmitterConfig *config) {
    VecFx32 target;
    Camera *camera = ParticleSystem_GetCamera(config->particleSystem);
    BattleAnimUtil_GetBattlerEmitterPositionKind10(config->system, config->battlerB, &target);
    Camera_SetLookAtCamTarget(&target, camera);
    return TRUE;
}

void BattleAnimEmitter_RunCameraCallback(int index, SPLEmitter *emitter, BattleAnimEmitterConfig *config) {
    sBattleAnimEmitterCameraCallbacks[index](emitter, config);
}

void BattleAnimUtil_LookupScreenPos(int set, int index, s16 *x, s16 *y) {
    if (x != NULL) {
        *x = ov07_02236468[set][index * 2];
    }
    if (y != NULL) {
        *y = ov07_0223646A[set][index * 2];
    }
}

void BattleAnimSystem_GetBattlerScreenPos(BattleAnimSystem *system, int index, s16 *x, s16 *y) {
    if (ov07_0221BFC0(system) == 1) {
        BattleAnimUtil_LookupScreenPos(0, index + 2, x, y);
    } else if (BattleAnimSystem_IsDoubleBattle(system) == TRUE) {
        BattleAnimUtil_LookupScreenPos(1, index - 2, x, y);
    } else {
        BattleAnimUtil_LookupScreenPos(0, index, x, y);
    }
}

// coord: 0 / 2 x, 1 / 3 y
s16 BattleAnimSystem_GetBattlerScreenCoord(BattleAnimSystem *system, int battler, u32 coord) {
    int index = BattleAnimUtil_GetBattlerType(system, battler);
    int set = BattleAnimSystem_IsDoubleBattle(system) == TRUE;

    if (ov07_0221BFC0(system) == 1) {
        index += 2;
    }
    switch (coord) {
    case 0:
    case 2:
        return ov07_02236468[set][index * 2];
    case 1:
    case 3:
        return ov07_0223646A[set][index * 2];
    default:
        GF_ASSERT(FALSE);
        return 0;
    }
}

u8 BattleAnimUtil_GetSpritePaletteSlot(ManagedSprite *sprite) {
    return ObjPlttTransfer_GetPaletteVramOffset(Sprite_GetPaletteProxy(sprite->sprite), NNS_G2D_VRAM_TYPE_2DMAIN);
}

int BattleAnimUtil_GetBattlerSideSignByMode(BattleAnimSystem *system, int battler) {
    int sign = 1;
    int side = BattleAnimUtil_GetBattlerSide(system, battler);

    if (ov07_0221BFC0(system) != 0) {
        if (side == 3) {
            sign -= 2;
        }
    } else if (side == 4) {
        sign -= 2;
    }
    return sign;
}

// -1 for a battler on side 4, else +1
int BattleAnimUtil_GetBattlerSideSign(BattleAnimSystem *system, int battler) {
    int sign = 1;

    if (BattleAnimUtil_GetBattlerSide(system, battler) == 4) {
        sign -= 2;
    }
    return sign;
}

fx32 BattleAnimUtil_CalcStep(fx32 start, fx32 end, int duration) {
    return FX_Div(end - start, duration << FX32_SHIFT);
}

// Number of steps (rounded up) from start to end
int BattleAnimUtil_CalcStepCount(fx32 start, fx32 end, fx32 step) {
    fx32 count = FX_Div(end - start, step);

    if (FX_Modf(count, &count) != 0) {
        count += FX32_ONE;
    }
    count = count < 0 ? -count : count;
    return count >> FX32_SHIFT;
}

void BattleAnimUtil_SetSpritePosWithOffset(void *position, ManagedSprite *sprite, s16 x, s16 y) {
    BattleAnimMotionOffset *offset = position;
    ManagedSprite_SetPositionXY(sprite, x + offset->x, y + offset->y);
}

void BattleAnimUtil_SetSpriteScale(BattleAnimLinearMotion *scale, ManagedSprite *sprite) {
    f32 x, y;

    BattleAnimUtil_GetScaleFloat(scale, &x, &y);
    ManagedSprite_SetAffineScale(sprite, x, y);
}

void BattleAnimUtil_SetPokepicPosWithOffset(void *position, Pokepic *pokepic, int x, int y) {
    BattleAnimMotionOffset *offset = position;
    Pokepic_SetAttr(pokepic, POKEPIC_X, x + offset->x);
    Pokepic_SetAttr(pokepic, POKEPIC_Y, y + offset->y);
}

void BattleAnimUtil_SetPokepicScale(BattleAnimLinearMotion *scale, Pokepic *pokepic) {
    Pokepic_SetAttr(pokepic, POKEPIC_AFFINEW, scale->x);
    Pokepic_SetAttr(pokepic, POKEPIC_AFFINEH, scale->y);
}

void BattleAnimUtil_InitSinMotion(BattleAnimSinMotion *motion, int startX, int endX, int startY, u16 endY, fx32 ampX, fx32 ampY, int duration) {
    motion->duration = duration;
    motion->angleX = startX;
    motion->ampX = ampX;
    motion->angleY = startY;
    motion->ampY = ampY;
    motion->stepX = (endX - startX) / duration;
    motion->stepY = ((u16)endY - startY) / duration;
}
