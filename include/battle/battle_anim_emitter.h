#ifndef POKEHEARTGOLD_BATTLE_BATTLE_ANIM_EMITTER_H
#define POKEHEARTGOLD_BATTLE_BATTLE_ANIM_EMITTER_H

// Overlay 7: the SPL emitter callbacks of the battle animation system (sBattleAnimEmitterCallbackTable), the emitter
// configured from script arguments with its flag and camera callbacks, battler screen positions and motion helpers.

#include "battle/battle_anim_battler_util.h"

#include "camera.h"
#include "pokepic.h"
#include "spl.h"
#include "sprite_system.h"

typedef struct BattleAnimLinearMotion BattleAnimLinearMotion;
typedef struct BattleAnimSinMotion BattleAnimSinMotion;

// Emitter configured from six script arguments (BattleAnimEmitterCb_ConfigureFromScript)
typedef struct BattleAnimEmitterConfig {
    BattleAnimSystem *system;
    SPLEmitter *particleSystem;
    s8 sideSign; // +1 / -1 by the side of battlerA, see BattleAnimEmitter_GetConfiguredSideSign
    u32 zMode;   // 0x0C, first script argument
    u32 battlerOrder;
    int positionMode;
    int axisMode;
    u32 flags; // a flag callback runs for every mask of sBattleAnimEmitterFlagMasks set here
    int cameraMode;
    int battlerA; // 0x24
    int battlerB;
    VecFx32 position;
} BattleAnimEmitterConfig; // size: 0x38

// Script arguments of the flag callbacks (BattleAnimEmitter_ReadFlagCbParams)
typedef struct BattleAnimEmitterFlagParams {
    int mode;
    BOOL ignoreSide; // TRUE: the value is not mirrored by BattleAnimEmitterConfig.sideSign
    VecFx32 value;
} BattleAnimEmitterFlagParams;

typedef BOOL (*BattleAnimEmitterConfigCb)(SPLEmitter *emitter, BattleAnimEmitterConfig *config);

void BattleAnimEmitterCb_SetPositionAtFirstOpponent(SPLEmitter *emitter);
void BattleAnimEmitterCb_SetPositionAtBattler0(SPLEmitter *emitter);
void BattleAnimEmitterCb_SetPositionAtDefender(SPLEmitter *emitter);
void BattleAnimEmitterCb_SetPositionAtAttacker(SPLEmitter *emitter);
void BattleAnimEmitterCb_SetPositionAtAttacker2(SPLEmitter *emitter);
void BattleAnimEmitterCb_SetPositionAtAttackerSide(SPLEmitter *emitter);
void BattleAnimEmitterCb_SetPositionAtDefenderSide(SPLEmitter *emitter);
void BattleAnimEmitterCb_SetPositionByBattlerTypes(SPLEmitter *emitter);
void BattleAnimUtil_ScaleVecComponents(int kind, int scale, VecFx32 *vec);
void BattleAnimEmitter_SetConvergenceTarget(BattleAnimSystem *system, SPLEmitter *emitter, int battlerA, int battlerB, int kind, s8 scale, VecFx32 *origin);
void BattleAnimEmitter_SetMagnetTarget(BattleAnimSystem *system, SPLEmitter *emitter, u32 battlerA, u32 battlerB, u32 kind, s8 scale, const VecFx32 *origin);
void BattleAnimEmitter_SetTransformFromScript(BattleAnimSystem *system, SPLEmitter *emitter, int battlerA, int battlerB, int mode, int kind);
void BattleAnimEmitter_SetTransformAttackerToDefender(SPLEmitter *emitter, int mode, int kind);
void BattleAnimEmitterCb_AttackerToDefender(SPLEmitter *emitter);
void BattleAnimEmitterCb_ConvergeBySide(SPLEmitter *emitter);
void BattleAnimEmitterCb_ConvergeUp(SPLEmitter *emitter);
void BattleAnimEmitterCb_ConvergeToDefender(SPLEmitter *emitter);
void BattleAnimEmitterCb_ConvergeToAttacker(SPLEmitter *emitter);
void BattleAnimEmitterCb_ConvergeFromScript(SPLEmitter *emitter);
void BattleAnimEmitterCb_MagnetBySide(SPLEmitter *emitter);
void BattleAnimEmitterCb_MagnetUp(SPLEmitter *emitter);
void BattleAnimEmitterCb_MagnetToDefender(SPLEmitter *emitter);
void BattleAnimEmitterCb_MagnetToAttacker(SPLEmitter *emitter);
void BattleAnimEmitterCb_MagnetFromScript(SPLEmitter *emitter);
void BattleAnimEmitterCb_DefenderToAttacker(SPLEmitter *emitter);
void BattleAnimEmitter_UpdateConfigured(SPLEmitter *emitter, BattleAnimEmitterConfig *config);
void BattleAnimEmitter_GetDefenderSidePosition(SPLEmitter *emitter, BattleAnimEmitterConfig *config, VecFx32 *pos);
void BattleAnimEmitter_GetAttackerSidePosition(SPLEmitter *emitter, BattleAnimEmitterConfig *config, VecFx32 *pos);
void BattleAnimEmitter_ApplyConfiguredPosition(SPLEmitter *emitter, BattleAnimEmitterConfig *config);
void BattleAnimEmitter_ApplyConfiguredAxis(SPLEmitter *emitter, BattleAnimEmitterConfig *config);
void BattleAnimEmitter_ApplyConfiguredFlags(SPLEmitter *emitter, BattleAnimEmitterConfig *config);
void BattleAnimEmitter_ApplyConfiguredCamera(SPLEmitter *emitter, BattleAnimEmitterConfig *config);
void BattleAnimEmitter_ApplyConfiguredZ(SPLEmitter *emitter, BattleAnimEmitterConfig *config);
void BattleAnimEmitterCb_ConfigureFromScript(SPLEmitter *emitter);
int BattleAnimEmitter_GetConfiguredSideSign(BattleAnimEmitterConfig *config);
void BattleAnimEmitter_AddOffsetForBattlerType(BattleAnimEmitterConfig *config, int battler, VecFx32 *pos);
void BattleAnimEmitter_AddOffsetFromScript(BattleAnimEmitterConfig *config, VecFx32 *pos);
void BattleAnimEmitter_ReadFlagCbParams(BattleAnimEmitterConfig *config, BattleAnimEmitterFlagParams *params);
BOOL BattleAnimEmitterFlagCb_Nop0(SPLEmitter *emitter, BattleAnimEmitterConfig *config);
BOOL BattleAnimEmitterFlagCb_SetGravity(SPLEmitter *emitter, BattleAnimEmitterConfig *config);
BOOL BattleAnimEmitterFlagCb_Nop2(SPLEmitter *emitter, BattleAnimEmitterConfig *config);
BOOL BattleAnimEmitterFlagCb_Nop3(SPLEmitter *emitter, BattleAnimEmitterConfig *config);
BOOL BattleAnimEmitterFlagCb_SetMagnetPos(SPLEmitter *emitter, BattleAnimEmitterConfig *config);
BOOL BattleAnimEmitterFlagCb_SetMagnetForce(SPLEmitter *emitter, BattleAnimEmitterConfig *config);
BOOL BattleAnimEmitterFlagCb_SetSpinAngle(SPLEmitter *emitter, BattleAnimEmitterConfig *config);
BOOL BattleAnimEmitterFlagCb_SetSpinAxis(SPLEmitter *emitter, BattleAnimEmitterConfig *config);
BOOL BattleAnimEmitterFlagCb_Nop8(SPLEmitter *emitter, BattleAnimEmitterConfig *config);
BOOL BattleAnimEmitterFlagCb_Nop9(SPLEmitter *emitter, BattleAnimEmitterConfig *config);
BOOL BattleAnimEmitterFlagCb_Nop10(SPLEmitter *emitter, BattleAnimEmitterConfig *config);
BOOL BattleAnimEmitterFlagCb_Nop11(SPLEmitter *emitter, BattleAnimEmitterConfig *config);
BOOL BattleAnimEmitterFlagCb_SetConvergencePos(SPLEmitter *emitter, BattleAnimEmitterConfig *config);
BOOL BattleAnimEmitterFlagCb_SetConvergenceForce(SPLEmitter *emitter, BattleAnimEmitterConfig *config);
void BattleAnimEmitter_RunFlagCallback(int index, SPLEmitter *emitter, BattleAnimEmitterConfig *config);
int BattleAnimEmitter_GetFlagMask(int index);
int BattleAnimEmitter_CountFlagMasks(void);
void BattleAnimEmitter_ReadCameraCbParams(BattleAnimEmitterConfig *config, u32 *params);
BOOL BattleAnimEmitterCameraCb_ResetAngle(SPLEmitter *emitter, BattleAnimEmitterConfig *config);
BOOL BattleAnimEmitterCameraCb_SetPresetAngle4(SPLEmitter *emitter, BattleAnimEmitterConfig *config);
BOOL BattleAnimEmitterCameraCb_SetAngleFromScript(SPLEmitter *emitter, BattleAnimEmitterConfig *config);
BOOL BattleAnimEmitterCameraCb_SetPresetAngle3(SPLEmitter *emitter, BattleAnimEmitterConfig *config);
BOOL ov07_02221DC4(SPLEmitter *emitter, BattleAnimEmitterConfig *config);
BOOL BattleAnimEmitterCameraCb_SetPresetAngle0(SPLEmitter *emitter, BattleAnimEmitterConfig *config);
BOOL BattleAnimEmitterCameraCb_SetPresetAngle1(SPLEmitter *emitter, BattleAnimEmitterConfig *config);
BOOL BattleAnimEmitterCameraCb_LookAtBattlerA(SPLEmitter *emitter, BattleAnimEmitterConfig *config);
BOOL BattleAnimEmitterCameraCb_LookAtBattlerB(SPLEmitter *emitter, BattleAnimEmitterConfig *config);
void BattleAnimEmitter_RunCameraCallback(int index, SPLEmitter *emitter, BattleAnimEmitterConfig *config);
void BattleAnimUtil_LookupScreenPos(int set, int index, s16 *x, s16 *y);
void BattleAnimSystem_GetBattlerScreenPos(BattleAnimSystem *system, int index, s16 *x, s16 *y);
s16 BattleAnimSystem_GetBattlerScreenCoord(BattleAnimSystem *system, int battler, u32 coord);
u8 BattleAnimUtil_GetSpritePaletteSlot(ManagedSprite *sprite);
int ov07_02222004(BattleAnimSystem *system, int battler);
int BattleAnimUtil_GetBattlerSideSign(BattleAnimSystem *system, int battler);
fx32 BattleAnimUtil_CalcStep(fx32 start, fx32 end, int duration);
int BattleAnimUtil_CalcStepCount(fx32 start, fx32 end, fx32 step);
void BattleAnimUtil_SetSpritePosWithOffset(void *position, ManagedSprite *sprite, s16 x, s16 y);
void BattleAnimUtil_SetSpriteScale(BattleAnimLinearMotion *scale, ManagedSprite *sprite);
void BattleAnimUtil_SetPokepicPosWithOffset(void *position, Pokepic *pokepic, int x, int y);
void BattleAnimUtil_SetPokepicScale(BattleAnimLinearMotion *scale, Pokepic *pokepic);
void BattleAnimUtil_InitSinMotion(BattleAnimSinMotion *motion, int startX, int endX, int startY, u16 endY, fx32 ampX, fx32 ampY, int duration);

// overlay 7 functions defined in sources not merged yet (switch to battle/battle_anim_internal.h when it lands;
// the other accessors this file uses are declared in battle/battle_anim_battler_util.h)
BOOL BattleAnimSystem_ReadScriptArgs(BattleAnimSystem *system, u32 *out, int count);
int BattleAnimSystem_GetSideSign(BattleAnimSystem *system, int battlerA, int battlerB);
void BattleAnimUtil_GetScaleFloat(BattleAnimLinearMotion *motion, f32 *x, f32 *y);

// src/particle_system.c (missing from particle_system.h)
void ParticleEmitter_SetGravity(SPLEmitter *emitter, VecFx16 *value);
void ParticleEmitter_SetMagnetPos(SPLEmitter *emitter, VecFx32 *value);
void ParticleEmitter_GetMagnetPos(SPLEmitter *emitter, VecFx32 *value);
void ParticleEmitter_SetMagnetForce(SPLEmitter *emitter, s16 *value);
void ParticleEmitter_GetMagnetForce(SPLEmitter *emitter, s16 *value);
void ParticleEmitter_SetSpinAngle(SPLEmitter *emitter, u16 *value);
void ParticleEmitter_GetSpinAngle(SPLEmitter *emitter, u16 *value);
void ParticleEmitter_SetSpinAxis(SPLEmitter *emitter, u16 *value);
void ParticleEmitter_GetSpinAxis(SPLEmitter *emitter, u16 *value);
void ParticleEmitter_SetConvergencePos(SPLEmitter *emitter, VecFx32 *value);
void ParticleEmitter_GetConvergencePos(SPLEmitter *emitter, VecFx32 *value);
void ParticleEmitter_SetConvergenceForce(SPLEmitter *emitter, s16 *value);
void ParticleEmitter_GetConvergenceForce(SPLEmitter *emitter, s16 *value);

#endif // POKEHEARTGOLD_BATTLE_BATTLE_ANIM_EMITTER_H
