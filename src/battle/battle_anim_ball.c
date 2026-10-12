#include "battle/battle_anim_ball.h"

#include "global.h"

#include "constants/battle.h"
#include "constants/gx.h"
#include "constants/pokemon.h"
#include "constants/sndseq.h"

#include "battle/battle.h"

#include "filesystem.h"
#include "heap.h"
#include "math_util.h"
#include "overlay_07.h"
#include "palette.h"
#include "particle_system.h"
#include "pokemon.h"
#include "pokepic.h"
#include "seal_case.h"
#include "sound_playback.h"
#include "sprite_system.h"
#include "sys_task_api.h"
#include "unk_02009D48.h"
#include "unk_02013FDC.h"
#include "unk_02091054.h"

fx32 FX_Sqrt(fx32 x); // not declared by the lib/include fx headers

// UnkBallData (battle/battle.h) as this module uses it:
//   unk0     BALL_ANIM_* animation run by UnkBallData_IsBallAnimationPlaying (sUnkBallDataBallAnimFuncs)
//   unk4     set once that animation has entered its first capture state
//   unk8     step of the current mode / capture state
//   unkC     frame counter of the capture states, unk10 wobble direction, unk18 shake count
//   unk14    capture state (sUnkBallDataCaptureStateFuncs)
//   unk1C    TRUE while the mode handler runs
//   unk20/21 blend alpha of the fade out, unk22 random direction (+1/-1)
//   unk24    the task ticks the sprite animation
//   unk28    set when the thrown ball has opened
//   unkC4    frame counter of the modes, unkC8 height of the throw arc
//   unkCC    the update task, unkD0 the BallBurstEffect, unkD4 the palette flash, unkD8 the battler sprites
//   unkDC    frames the task waits before running the mode
//   unk90    the UnkStruct_134 it was created from: unk0 kind (start / target position, arc), unk8 mode
//            (sUnkBallDataModeFuncs), unkC resource tag offset, unk14 sprite BG priority, unk18 keep the
//            renderer's sub surface
// Members its declaration still keeps as byte arrays:
#define BALL_ROTATION(data)     ((s32 *)&(data)->unk34[0x34 - 0x34])  // 0x34, value motion of the rotation
#define BALL_ARC_MOTION(data)   ((void *)&(data)->unk34[0x48 - 0x34]) // 0x48, linear part of the throw arc
#define BALL_ARC_MOTION2(data)  ((void *)&(data)->unk34[0x6C - 0x34]) // 0x6C, height part of the throw arc
#define BALL_X(data)            (*(s16 *)&(data)->unkB8[0xB8 - 0xB8]) // sprite position at the mode start
#define BALL_Y(data)            (*(s16 *)&(data)->unkB8[0xBA - 0xB8])
#define BALL_TARGET_X(data)     (*(s16 *)&(data)->unkB8[0xBC - 0xB8]) // end of the throw arc
#define BALL_TARGET_Y(data)     (*(s16 *)&(data)->unkB8[0xBE - 0xB8])
#define BALL_ARC_DURATION(data) (*(int *)&(data)->unkB8[0xC0 - 0xB8])

typedef BOOL (*UnkBallDataFunc)(UnkBallData *data);

// Copy of battle/battle_anim_internal.h's definition (follow-up: include that header once it is on master)
typedef struct BattleAnimBattlerPicParams {
    int battler;
    int sourceBattler;
    UnkBattleSystemSub1D0 *picData[4];
    Pokepic *pokepics[4];
    u16 species[4];
    u8 genders[4];
    u8 shiny[4];
    u8 forms[4];
    u32 personalities[4];
    u8 battlerTypes[4];
} BattleAnimBattlerPicParams; // size: 0x50

struct BallSealEffect {
    enum HeapID heapId;
    BallSealEffectTemplate template;
    int count;                      // 0x10, particle systems in use
    SPLEmitter *particleSystems[9]; // 0x14
    SEAL *seals[9];                 // 0x38
    BOOL emitted[9];                // 0x5C
    u8 unk_80[4];
    SysTask *task; // 0x84
    BOOL active;   // 0x88
    NARC *narc;    // 0x8C
    int state;     // 0x90, 0xFF: resources loaded
    BOOL hasCapsule;
    int ball;      // 0x98
    int capsuleId; // 0x9C
    CAPSULE capsule;
};

// One capsule seal's emitters, created after a delay that grows with the seal's distance from the centre
typedef struct BallSealEmitTask {
    int index;
    int delay;
    int battlerType;
    BOOL *emitted;
    SPLEmitter *particleSystem;
    SEAL *seal;
    void (*callback)(SPLEmitter *emitter);
} BallSealEmitTask; // size: 0x1C

struct BallBurstEffect {
    BallBurstEffectTemplate template;
    u32 unk_14;
    SPLEmitter *particleSystem; // 0x18
    int emitterCount;           // 0x1C
    int resource;               // 0x20
}; // size: 0x24

struct BattleAnimEmitter {
    int index;
    enum HeapID heapId;
    BattleAnimEmitterTemplate template;
    SPLEmitter *particleSystem; // 0x10
    u32 unk_14;
    SysTask *task; // 0x18
    int emitterCount;
    BOOL active; // 0x20
};

struct BattleAnimEmitterManager {
    enum HeapID heapId;
    int unk_04;
    BattleAnimEmitter *emitters[16];
};

typedef struct UnkBallDataSpinningKinds {
    u32 kinds[11];
} UnkBallDataSpinningKinds;

typedef struct UnkBallDataFlashingKinds {
    u32 kinds[6];
} UnkBallDataFlashingKinds;

typedef struct UnkBallDataStaticSpriteKinds {
    u32 kinds[3];
} UnkBallDataStaticSpriteKinds;

typedef struct UnkBallDataBounceSound {
    int frame;
    int seqNo;
} UnkBallDataBounceSound;

typedef struct BallSealEmitDelays {
    u8 delays[7];
} BallSealEmitDelays;

typedef struct BattleAnimEmitterCounts {
    u32 counts[22];
} BattleAnimEmitterCounts;

// asm/battle_anim_data.s
extern const UnkBallDataStaticSpriteKinds sUnkBallDataStaticSpriteKinds;
extern const UnkBallDataFlashingKinds sUnkBallDataFlashingKinds;
extern const UnkBallDataFunc sUnkBallDataModeFuncs[];
extern const UnkBallDataFunc sUnkBallDataBallAnimFuncs[];
extern const UnkBallDataBounceSound sUnkBallDataBounceSounds[];
extern const UnkBallDataSpinningKinds sUnkBallDataSpinningKinds;
extern const s16 sUnkBallDataShakeOffsets[];
extern const UnkBallDataFunc sUnkBallDataCaptureStateFuncs[];
extern const s16 ov07_0223730E[]; // bounce steps of 6 bytes: y offset, then (ov07_02237310) the animation frame
extern const u16 ov07_02237310[];
// Per ball (BallAnim_GetBallTableIndex) tables; the inner labels are offsets into the same rows
extern const u32 ov07_0223738C[][2]; // seal particle resource
extern const u32 ov07_02237390[][2]; // seal particle emitter count
extern const u32 ov07_0223746C[][3]; // burst particle resource
extern const u32 ov07_02237470[][3]; // emitter skipped by the burst
extern const u32 ov07_02237474[][3]; // burst particle emitter count
extern const u32 sBallSpriteResources[][4];
extern const BallSealEmitDelays sBallSealEmitDelays;
extern const BattleAnimEmitterCounts sBattleAnimEmitterCounts;
extern const s16 ov07_022377DC[][2];
extern const s16 ov07_022377F4[][3];

int BallAnim_GetBallTableIndex(int ball) {
    if (ball >= 1 && ball <= 24) {
        return ball - 1;
    }
    if (ball >= 1000 && ball <= 1027) {
        return ball - 1000;
    }
    return 3;
}

BOOL UnkBallData_IsSpinningKind(u32 kind) {
    UnkBallDataSpinningKinds list = sUnkBallDataSpinningKinds;
    BOOL result = FALSE;
    u32 i = result;
    u32 *ptr = list.kinds;
    while (i < 11) {
        if (kind == *ptr) {
            result = TRUE;
            break;
        }
        i++;
        ptr++;
    }
    return result;
}

BOOL UnkBallData_IsFlashingKind(u32 kind) {
    UnkBallDataFlashingKinds list = sUnkBallDataFlashingKinds;
    BOOL result = FALSE;
    u32 i = result;
    u32 *ptr = list.kinds;
    while (i < 6) {
        if (kind == *ptr) {
            result = TRUE;
            break;
        }
        i++;
        ptr++;
    }
    return result;
}

BOOL UnkBallData_ShouldAnimateSprite(u32 kind) {
    UnkBallDataStaticSpriteKinds list = sUnkBallDataStaticSpriteKinds;
    BOOL result = TRUE;
    u32 i = 0;
    u32 *ptr = list.kinds;
    while (i < 3) {
        if (kind == *ptr) {
            result = FALSE;
            break;
        }
        i++;
        ptr++;
    }
    return result;
}

u32 BallAnim_GetSealParticleResource(int ball) {
    return ov07_0223738C[BallAnim_GetBallTableIndex(ball)][0];
}

u32 BallAnim_GetSealParticleEmitterCount(int ball) {
    return ov07_02237390[BallAnim_GetBallTableIndex(ball)][0];
}

u32 BallAnim_GetBurstParticleResource(int ball) {
    return ov07_0223746C[BallAnim_GetBallTableIndex(ball)][0];
}

u32 BallAnim_GetBurstParticleEmitterCount(int ball) {
    return ov07_02237474[BallAnim_GetBallTableIndex(ball)][0];
}

u32 BallAnim_GetBurstSkippedEmitter(int ball) {
    return ov07_02237470[BallAnim_GetBallTableIndex(ball)][0];
}

u32 BallAnim_GetSpriteResource(int ball, int index) {
    return sBallSpriteResources[BallAnim_GetBallTableIndex(ball)][index];
}

BallSealEffect *BallSealEffect_Alloc(enum HeapID heapId) {
    BallSealEffect *effect = Heap_Alloc(heapId, sizeof(BallSealEffect));
    if (effect == NULL) {
        GF_AssertFail();
        return NULL;
    }
    effect->heapId = heapId;
    effect->active = FALSE;
    return effect;
}

BallSealEffect *BallSealEffect_New(enum HeapID heapId, const BallSealEffectTemplate *template) {
    BallSealEffect *effect = BallSealEffect_Alloc(heapId);
    int i;
    BallSealEffect_ReadMon(effect, template);
    effect->count = 0;
    for (i = 0; i < 9; i++) {
        effect->particleSystems[i] = NULL;
        effect->seals[i] = NULL;
        effect->emitted[i] = FALSE;
    }
    return effect;
}

void BallSealEffect_ReadMon(BallSealEffect *effect, const BallSealEffectTemplate *template) {
    effect->template = *template;
    effect->capsuleId = GetMonData(effect->template.mon, MON_DATA_BALL_CAPSULE_ID, NULL);
    GF_ASSERT(effect->capsuleId < 13);
    if (effect->capsuleId == 0) {
        effect->hasCapsule = FALSE;
        if (effect->template.ball != 0) {
            effect->ball = effect->template.ball;
        } else {
            effect->ball = GetMonData(effect->template.mon, MON_DATA_POKEBALL, NULL);
        }
    } else {
        effect->hasCapsule = TRUE;
        GetMonData(effect->template.mon, MON_DATA_BALL_CAPSULE, &effect->capsule);
    }
}

// Loads the particle resource of each seal on the capsule, two seal slots per frame
void BallSealEffect_LoadSealParticlesTask(SysTask *task, void *data) {
    BallSealEffect *effect = data;
    u8 resource0, resource3, resource2, resource1;

    switch (effect->state) {
    case 0: {
        int i;
        effect->narc = NARC_New(NARC_a_0_9_5, effect->heapId);
        for (i = 0; i < 2; i++) {
            SEAL *seal = CapsuleGetSealI(&effect->capsule, i);
            if (seal != NULL) {
                s32 sealId = SealOnCapsuleGetID(seal);
                if (sealId != 0 && sealId < 81) {
                    resource0 = sub_02091084((u8)sealId);
                    effect->seals[effect->count] = seal;
                    if (effect->particleSystems[effect->count] != NULL) {
                        GF_AssertFail();
                    }
                    effect->particleSystems[effect->count] = BattleAnimSystem_NewParticleSystemFromOpenNarc(effect->narc, effect->heapId, resource0, FALSE);
                    {
                        SPLEmitter *particleSystem = effect->particleSystems[effect->count];
                        if (particleSystem == NULL) {
                            GF_ASSERT(particleSystem != NULL);
                        } else {
                            effect->count++;
                        }
                    }
                }
            }
        }
        effect->state++;
        return;
    }
    case 1: {
        int i;
        for (i = 2; i < 4; i++) {
            SEAL *seal = CapsuleGetSealI(&effect->capsule, i);
            if (seal != NULL) {
                s32 sealId = SealOnCapsuleGetID(seal);
                if (sealId != 0 && sealId < 81) {
                    resource1 = sub_02091084((u8)sealId);
                    effect->seals[effect->count] = seal;
                    if (effect->particleSystems[effect->count] != NULL) {
                        GF_AssertFail();
                    }
                    effect->particleSystems[effect->count] = BattleAnimSystem_NewParticleSystemFromOpenNarc(effect->narc, effect->heapId, resource1, FALSE);
                    {
                        SPLEmitter *particleSystem = effect->particleSystems[effect->count];
                        if (particleSystem == NULL) {
                            GF_ASSERT(particleSystem != NULL);
                        } else {
                            effect->count++;
                        }
                    }
                }
            }
        }
        effect->state++;
        return;
    }
    case 2: {
        int i;
        for (i = 4; i < 6; i++) {
            SEAL *seal = CapsuleGetSealI(&effect->capsule, i);
            if (seal != NULL) {
                s32 sealId = SealOnCapsuleGetID(seal);
                if (sealId != 0 && sealId < 81) {
                    resource2 = sub_02091084((u8)sealId);
                    effect->seals[effect->count] = seal;
                    if (effect->particleSystems[effect->count] != NULL) {
                        GF_AssertFail();
                    }
                    effect->particleSystems[effect->count] = BattleAnimSystem_NewParticleSystemFromOpenNarc(effect->narc, effect->heapId, resource2, FALSE);
                    {
                        SPLEmitter *particleSystem = effect->particleSystems[effect->count];
                        if (particleSystem == NULL) {
                            GF_ASSERT(particleSystem != NULL);
                        } else {
                            effect->count++;
                        }
                    }
                }
            }
        }
        effect->state++;
        return;
    }
    case 3: {
        int i;
        for (i = 6; i < 8; i++) {
            SEAL *seal = CapsuleGetSealI(&effect->capsule, i);
            if (seal != NULL) {
                s32 sealId = SealOnCapsuleGetID(seal);
                if (sealId != 0 && sealId < 81) {
                    resource3 = sub_02091084((u8)sealId);
                    effect->seals[effect->count] = seal;
                    if (effect->particleSystems[effect->count] != NULL) {
                        GF_AssertFail();
                    }
                    effect->particleSystems[effect->count] = BattleAnimSystem_NewParticleSystemFromOpenNarc(effect->narc, effect->heapId, resource3, FALSE);
                    {
                        SPLEmitter *particleSystem = effect->particleSystems[effect->count];
                        if (particleSystem == NULL) {
                            GF_ASSERT(particleSystem != NULL);
                        } else {
                            effect->count++;
                        }
                    }
                }
            }
        }
        effect->state = 0xFF;
        NARC_Delete(effect->narc);
        SysTask_Destroy(task);
        return;
    }
    }
}

void BallSealEffect_LoadResources(BallSealEffect *effect) {
    if (effect->hasCapsule == FALSE) {
        int resource = BallAnim_GetSealParticleResource(effect->ball);
        effect->count = 1;
        effect->particleSystems[0] = BattleAnimSystem_NewParticleSystemFromNarc(effect->heapId, NARC_a_0_9_5, resource, FALSE);
        effect->state = 0xFF;
    } else {
        SysTask *task;
        effect->count = 0;
        effect->state = 0;
        task = SysTask_CreateOnMainQueue(BallSealEffect_LoadSealParticlesTask, effect, 1000);
        BallSealEffect_LoadSealParticlesTask(task, effect);
    }
}

BOOL BallSealEffect_IsLoaded(BallSealEffect *effect) {
    return effect->state == 0xFF;
}

#pragma force_active on

void BallSealEffect_StartWithCallback(BallSealEffect *effect, void (*callback)(SPLEmitter *)) {
    effect->active = TRUE;
    BattleAnimSystem_InitBlend();
    BallSealEffect_Emit(effect, callback);
    effect->task = SysTask_CreateOnMainQueue(BallSealEffect_WaitTask, effect, 1000);
}

void BallSealEffect_Start(BallSealEffect *effect) {
    BallSealEffect_StartWithCallback(effect, BallSealEffect_SetEmitterPosition);
}

#pragma force_active reset

BOOL BallSealEffect_IsActive(BallSealEffect *effect) {
    return effect->active;
}

#pragma force_active on

void BallSealEffect_WaitTask(SysTask *task, void *data) {
    BallSealEffect *effect = data;
    BOOL busy = FALSE;
    int i;

    GF_ASSERT(effect->count <= 9);
    for (i = 0; i < effect->count; i++) {
        if (effect->particleSystems[i] != NULL) {
            if (ParticleSystem_GetActiveEmitterCount(effect->particleSystems[i]) == FALSE && effect->emitted[i]) {
                BattleAnimSystem_FreeParticleSystem(effect->particleSystems[i]);
                effect->particleSystems[i] = NULL;
                effect->emitted[i] = FALSE;
            } else {
                busy = TRUE;
            }
        }
    }
    if (busy == FALSE) {
        effect->active = FALSE;
        SysTask_Destroy(task);
    }
}

#pragma force_active reset

void BallSealEffect_Free(BallSealEffect *effect) {
    Heap_Free(effect);
}

void BallSealEffect_DelayedEmitTask(SysTask *task, void *data) {
    BallSealEmitTask *emitTask = data;
    if (emitTask->delay == 0) {
        *emitTask->emitted = TRUE;
        ParticleSystem_CreateEmitterWithParam(emitTask->particleSystem, 0, emitTask->callback, emitTask);
        ParticleSystem_SetPerspectiveType(emitTask->particleSystem, 1);
        SysTask_Destroy(task);
        Heap_Free(emitTask);
    } else {
        emitTask->delay--;
    }
}

#pragma force_active on

void BallSealEffect_Emit(BallSealEffect *effect, void (*callback)(SPLEmitter *)) {
    BallSealEmitTask *emitTask;
    int i;

    if (!effect->hasCapsule) {
        int count = BallAnim_GetSealParticleEmitterCount(effect->ball);
        for (i = 0; i < count; i++) {
            ParticleSystem_CreateEmitterWithParam(effect->particleSystems[0], i, callback, effect);
        }
        effect->emitted[0] = TRUE;
        ParticleSystem_SetPerspectiveType(effect->particleSystems[0], 1);
        return;
    }
    for (i = 0; i < effect->count; i++) {
        emitTask = Heap_Alloc(effect->heapId, sizeof(BallSealEmitTask));
        GF_ASSERT(emitTask != NULL);
        emitTask->index = i;
        emitTask->battlerType = effect->template.battlerType;
        emitTask->callback = BallSealEffect_SetEmitterPositionFromSeal;
        effect->emitted[i] = FALSE;
        emitTask->emitted = &effect->emitted[i];
        emitTask->particleSystem = effect->particleSystems[i];
        emitTask->seal = effect->seals[i];
        GF_ASSERT(emitTask->seal != NULL);
        emitTask->delay = BallSealEffect_GetSealEmitDelay(emitTask->seal);
        SysTask_CreateOnMainQueue(BallSealEffect_DelayedEmitTask, emitTask, 1001);
    }
}

void BallSealEffect_GetBasePosition(int battlerType, VecFx32 *pos) {
    int x, y;

    ov07_02231DE0(battlerType, pos, 0, 1);
    switch (battlerType) {
    case BATTLER_TYPE_SOLO_PLAYER:
        x = 0;
        y = -16;
        break;
    case BATTLER_TYPE_SOLO_ENEMY:
        x = -15;
        y = -25;
        break;
    case BATTLER_TYPE_PLAYER_SIDE_SLOT_1:
        x = -8;
        y = -12;
        break;
    case BATTLER_TYPE_ENEMY_SIDE_SLOT_1:
        x = -14;
        y = -25;
        break;
    case BATTLER_TYPE_PLAYER_SIDE_SLOT_2:
        x = -8;
        y = -12;
        break;
    case BATTLER_TYPE_ENEMY_SIDE_SLOT_2:
    default:
        x = -16;
        y = -29;
        break;
    }
    // 172: screen pixels to particle space
    pos->x -= (x + 10) * 172;
    pos->y -= (y + 32) * 172;
}

#pragma force_active reset

void BallSealEffect_SetEmitterPosition(SPLEmitter *emitter) {
    VecFx32 pos;
    BallSealEffect *effect = ParticleSystem_GetEmitterParam();
    BallSealEffect_GetBasePosition(effect->template.battlerType, &pos);
    emitter->emtr_pos.x = pos.x + emitter->p_res->p_base->pos.x;
    emitter->emtr_pos.y = pos.y + emitter->p_res->p_base->pos.y;
    emitter->emtr_pos.z = pos.z + emitter->p_res->p_base->pos.z;
}

#pragma force_active on

void BallSealEffect_SetEmitterPositionFromSeal(SPLEmitter *emitter) {
    VecFx32 pos;
    BallSealEmitTask *emitTask = ParticleSystem_GetEmitterParam();
    int x, y;

    BallSealEffect_GetBasePosition(emitTask->battlerType, &pos);
    sub_0209109C(SealOnCapsuleGetID(emitTask->seal));
    x = SealOnCapsuleGetX(emitTask->seal);
    y = SealOnCapsuleGetY(emitTask->seal);
    pos.x += (s16)(x - 190) * 172;
    pos.y += (s16)(100 - y) * 172;
    emitter->emtr_pos.x = pos.x + emitter->p_res->p_base->pos.x;
    emitter->emtr_pos.y = pos.y + emitter->p_res->p_base->pos.y;
    emitter->emtr_pos.z = pos.z + emitter->p_res->p_base->pos.z;
}

#pragma force_active reset

void BallBurstEffect_SetEmitterPosition(SPLEmitter *emitter) {
    BallBurstEffect *effect = ParticleSystem_GetEmitterParam();
    s32 y = (s16)(100 - effect->template.y) * 172;
    s32 x = (s16)(effect->template.x - 129) * 172;
    emitter->emtr_pos.x = x + emitter->p_res->p_base->pos.x;
    emitter->emtr_pos.y = y + emitter->p_res->p_base->pos.y;
    emitter->emtr_pos.z = emitter->p_res->p_base->pos.z;
}

BallBurstEffect *BallBurstEffect_New(const BallBurstEffectTemplate *template) {
    BallBurstEffect *effect = Heap_Alloc(template->heapId, sizeof(BallBurstEffect));
    if (!effect) {
        GF_AssertFail();
    }
    effect->template = *template;
    if (effect->template.generic) {
        effect->resource = BallAnim_GetBurstParticleResource(1027);
        effect->emitterCount = BallAnim_GetBurstParticleEmitterCount(1027);
    } else {
        effect->resource = BallAnim_GetBurstParticleResource(effect->template.ball);
        effect->emitterCount = BallAnim_GetBurstParticleEmitterCount(effect->template.ball);
    }
    effect->particleSystem = BattleAnimSystem_NewParticleSystemFromNarc(effect->template.heapId, NARC_a_0_9_5, effect->resource, FALSE);
    return effect;
}

void BallBurstEffect_Start(BallBurstEffect *effect) {
    int i;
    u32 emitter = effect->template.emitter;
    if (emitter == 0xFF) {
        if (effect->template.generic) {
            for (i = 0; i < effect->emitterCount; i++) {
                ParticleSystem_CreateEmitterWithParam(effect->particleSystem, i, BallBurstEffect_SetEmitterPosition, effect);
            }
        } else {
            for (i = 0; i < effect->emitterCount; i++) {
                if (i != BallAnim_GetBurstSkippedEmitter(effect->template.ball)) {
                    ParticleSystem_CreateEmitterWithParam(effect->particleSystem, i, BallBurstEffect_SetEmitterPosition, effect);
                }
            }
        }
    } else {
        ParticleSystem_CreateEmitterWithParam(effect->particleSystem, emitter, BallBurstEffect_SetEmitterPosition, effect);
    }
    ParticleSystem_SetPerspectiveType(effect->particleSystem, 1);
}

BOOL BallBurstEffect_IsRunning(BallBurstEffect *effect) {
    if (ParticleSystem_GetActiveEmitterCount(effect->particleSystem) == FALSE) {
        BattleAnimSystem_FreeParticleSystem(effect->particleSystem);
        return FALSE;
    }
    return TRUE;
}

void BallBurstEffect_Free(BallBurstEffect *effect) {
    Heap_Free(effect);
}

BOOL UnkBallData_BallAnimThrow(UnkBallData *data) {
    if (data->unk4 == 0) {
        data->unk4++;
        UnkBallData_SetCaptureState(data, 1);
    }
    if (UnkBallData_IsCaptureState(data, 2) == TRUE) {
        return FALSE;
    }
    return TRUE;
}

BOOL UnkBallData_BallAnimOpen(UnkBallData *data) {
    if (data->unk4 == 0) {
        data->unk4++;
        UnkBallData_SetCaptureState(data, 3);
    }
    if (UnkBallData_IsCaptureState(data, 4) == TRUE) {
        return FALSE;
    }
    return TRUE;
}

BOOL UnkBallData_BallAnimDeflect(UnkBallData *data) {
    if (data->unk4 == 0) {
        data->unk4++;
        UnkBallData_SetCaptureState(data, 5);
    }
    if (UnkBallData_IsCaptureState(data, 6) == TRUE) {
        return FALSE;
    }
    return TRUE;
}

BOOL UnkBallData_BallAnimFall(UnkBallData *data) {
    if (data->unk4 == 0) {
        data->unk4++;
        UnkBallData_SetCaptureState(data, 7);
    }
    if (UnkBallData_IsCaptureState(data, 14) == TRUE) {
        return FALSE;
    }
    return TRUE;
}

BOOL UnkBallData_BallAnimShake(UnkBallData *data) {
    if (data->unk4 == 0) {
        data->unk4++;
        UnkBallData_SetCaptureState(data, 15);
    }
    if (UnkBallData_IsCaptureState(data, 18) == TRUE) {
        return FALSE;
    }
    return TRUE;
}

BOOL ov07_02232EE0(UnkBallData *data) {
    if (data->unk4 == 0) {
        data->unk4++;
        UnkBallData_SetCaptureState(data, 19);
    }
    if (UnkBallData_IsCaptureState(data, 27) == TRUE) {
        return FALSE;
    }
    return TRUE;
}

BOOL UnkBallData_BallAnimClick(UnkBallData *data) {
    if (data->unk4 == 0) {
        data->unk4++;
        UnkBallData_SetCaptureState(data, 21);
    }
    if (UnkBallData_IsCaptureState(data, 22) == TRUE) {
        return FALSE;
    }
    return TRUE;
}

BOOL UnkBallData_BallAnimFade(UnkBallData *data) {
    if (data->unk4 == 0) {
        data->unk4++;
        UnkBallData_SetCaptureState(data, 23);
    }
    if (UnkBallData_IsCaptureState(data, 27) == TRUE) {
        return FALSE;
    }
    return TRUE;
}

void UnkBallData_SetBallAnimation(UnkBallData *data, s32 ballAnim) {
    data->unk0 = ballAnim;
    data->unk4 = 0;
}

BOOL UnkBallData_IsBallAnimationPlaying(UnkBallData *data, s32 ballAnim_Unused) {
    return sUnkBallDataBallAnimFuncs[data->unk0](data);
}

void UnkBallData_GoToCaptureState(UnkBallData *data, int state) {
    data->unk14 = state;
    data->unk8 = 0;
    data->unkC = 0;
    data->unk10 = 0;
}

BOOL ov07_02232F80(UnkBallData *data) {
    return TRUE;
}

BOOL UnkBallData_CaptureStateThrow(UnkBallData *data) {
    if (UnkBallData_RunThrowArc(data) == FALSE) {
        UnkBallData_GoToCaptureState(data, 2);
    }
    return TRUE;
}

BOOL ov07_02232F9C(UnkBallData *data) {
    UnkBallData_GoToCaptureState(data, 3);
    return TRUE;
}

BOOL UnkBallData_CaptureStateOpen(UnkBallData *data) {
    switch (data->unk8) {
    case 0: {
        ManagedSprite_SetAnim(data->managedSprite, 1);
        BallBurstEffectTemplate template;
        template.ball = data->unk90.ball;
        template.heapId = data->unk90.heapID;
        template.emitter = 0xFF;
        template.generic = FALSE;
        ManagedSprite_GetPositionXY(data->managedSprite, &template.x, &template.y);
        data->unkD8 = (int)BattleAnimBattlerSprites_NewAll(data->unk90.battleSystem, data->unk90.heapID);
        data->unkD0 = (int)BallBurstEffect_New(&template);
        data->unk8++;
        break;
    }
    case 1:
        // compared as a signed int, as the original called it
        if (((int (*)(ManagedSprite *))ManagedSprite_GetAnimationFrame)(data->managedSprite) >= 2) {
            data->unk24 = FALSE;
            data->unk8++;
        }
        break;
    case 2:
        BallBurstEffect_Start((BallBurstEffect *)data->unkD0);
        BattleAnimBattlerSprites_Free((struct BattleAnimBattlerSprites *)data->unkD8);
        data->unk8++;
        break;
    case 3:
        if (BallBurstEffect_IsRunning((BallBurstEffect *)data->unkD0) == FALSE) {
            ManagedSprite_SetAnimationFrame(data->managedSprite, 0);
            BallBurstEffect_Free((BallBurstEffect *)data->unkD0);
            data->unk8++;
        }
        break;
    default:
        UnkBallData_GoToCaptureState(data, 4);
        break;
    }
    return TRUE;
}

BOOL ov07_02233088(UnkBallData *data) {
    return TRUE;
}

BOOL UnkBallData_CaptureStateDeflect(UnkBallData *data) {
    if (data->unkC == 0) {
        ManagedSprite_GetPositionXY(data->managedSprite, &BALL_X(data), &BALL_Y(data));
        BALL_TARGET_X(data) = 60;
        BALL_TARGET_Y(data) = 180;
        BALL_ARC_DURATION(data) = 10;
        data->unkC8 = 12;
        data->unkC++;
    } else if (UnkBallData_RunThrowArc(data) == FALSE) {
        UnkBallData_GoToCaptureState(data, 6);
    }
    return TRUE;
}

BOOL ov07_022330E0(UnkBallData *data) {
    return TRUE;
}

BOOL ov07_022330E4(UnkBallData *data) {
    UnkBallData_GoToCaptureState(data, 8);
    return TRUE;
}

BOOL ov07_022330F0(UnkBallData *data) {
    UnkBallData_GoToCaptureState(data, 9);
    return TRUE;
}

BOOL UnkBallData_CaptureStateFlash(UnkBallData *data) {
    switch (data->unk8) {
    case 0: {
        int offset = ManagedSprite_GetPaletteOverrideOffset(data->managedSprite);
        PaletteData_BeginPaletteFade(data->unk90.paletteData, 1 << PLTTBUF_MAIN_OBJ, (u16)(1 << offset), -1, 0, 12, RGB(31, 27, 0));
        data->unk8++;
        break;
    }
    case 1:
        if (PaletteData_GetSelectedBuffersBitmask(data->unk90.paletteData) == 0) {
            int offset = ManagedSprite_GetPaletteOverrideOffset(data->managedSprite);
            PaletteData_BeginPaletteFade(data->unk90.paletteData, 1 << PLTTBUF_MAIN_OBJ, (u16)(1 << offset), -1, 12, 0, RGB(31, 27, 0));
            data->unk8++;
        }
        break;
    default:
        if (PaletteData_GetSelectedBuffersBitmask(data->unk90.paletteData) == 0) {
            UnkBallData_GoToCaptureState(data, 10);
        }
        break;
    }
    return TRUE;
}

BOOL ov07_022331A4(UnkBallData *data) {
    UnkBallData_GoToCaptureState(data, 11);
    return TRUE;
}

BOOL UnkBallData_CaptureStateDrop(UnkBallData *data) {
    switch (data->unk8) {
    case 0:
        data->unkC++;
        if (data->unkC >= 1) {
            s16 x, y;
            ManagedSprite_GetPositionXY(data->managedSprite, &x, &y);
            BattleAnimUtil_InitLinearMotion(BALL_ARC_MOTION(data), x, x, y, (s16)(y + 32), 10);
            data->unk8++;
        }
        break;
    case 1:
        if (BattleAnimUtil_UpdateLinearMotionSprite(BALL_ARC_MOTION(data), data->managedSprite) == FALSE) {
            data->unk8++;
            UnkBallData_GoToCaptureState(data, 13);
        }
        break;
    }
    return TRUE;
}

BOOL ov07_02233224(UnkBallData *data) {
    return TRUE;
}

BOOL UnkBallData_CaptureStateBounce(UnkBallData *data) {
    switch (data->unk8) {
    case 0:
        ManagedSprite_SetAnim(data->managedSprite, 1);
        ManagedSprite_SetAnimationFrame(data->managedSprite, 0);
        data->unk8++;
        // fallthrough
    case 1: {
        int index = data->unkC;
        int offset = index * 6;
        s16 dy = *(const s16 *)((const u8 *)ov07_0223730E + offset);
        int frame = *(const u16 *)((const u8 *)ov07_02237310 + offset);
        ManagedSprite_SetAnimationFrame(data->managedSprite, frame);
        data->unkC++;
        for (int i = 0; i < 5; i++) {
            if (data->unkC == sUnkBallDataBounceSounds[i].frame) {
                PlaySE_SetPan(sUnkBallDataBounceSounds[i].seqNo, 117);
            }
        }
        if (data->unkC > 20 || dy == 0xFF) {
            data->unk8++;
        } else {
            ManagedSprite_OffsetPositionXY(data->managedSprite, 0, dy);
        }
        break;
    }
    default:
        UnkBallData_GoToCaptureState(data, 14);
        break;
    }
    return TRUE;
}

BOOL ov07_022332C4(UnkBallData *data) {
    return TRUE;
}

BOOL UnkBallData_CaptureStateShakeWait(UnkBallData *data) {
    int counter = data->unkC + 1;
    data->unkC = counter;
    if (counter >= 14) {
        data->unk8++;
        data->unkC = 0;
        UnkBallData_GoToCaptureState(data, 16);
    }
    return TRUE;
}

BOOL ov07_022332E8(UnkBallData *data) {
    UnkBallData_GoToCaptureState(data, 17);
    return TRUE;
}

BOOL UnkBallData_CaptureStateShake(UnkBallData *data) {
    switch (data->unk8) {
    case 0:
        if (data->unk18 >= 3) {
            GF_AssertFail();
        }
        data->unk8++;
        // fallthrough
    case 1: {
        int shake = data->unk18;
        const s16 *offsets = (const s16 *)((const u8 *)sUnkBallDataShakeOffsets + shake * 24);
        int index = data->unkC;
        s16 dx = offsets[index];
        data->unkC++;
        if (data->unkC > 11 || dx == 0xFF) {
            data->unk8++;
            data->unk18++;
            ManagedSprite_SetAffineZRotation(data->managedSprite, 0);
            UnkBallData_GoToCaptureState(data, 18);
            return TRUE;
        }
        if (data->unkC == 5) {
            PlaySE_SetPan(SEQ_SE_DP_BOWA, 117);
        }
        ManagedSprite_OffsetPositionXY(data->managedSprite, dx, 0);
        int rotation = 0x1FFFE * dx / 360;
        ManagedSprite_OffsetAffineZRotation(data->managedSprite, rotation);
        break;
    }
    }
    return TRUE;
}

BOOL ov07_0223338C(UnkBallData *data) {
    return TRUE;
}

BOOL ov07_02233390(UnkBallData *data) {
    UnkBallData_GoToCaptureState(data, 20);
    return TRUE;
}

BOOL ov07_0223339C(UnkBallData *data) {
    UnkBallData_GoToCaptureState(data, 27);
    return TRUE;
}

BOOL UnkBallData_CaptureStateClick(UnkBallData *data) {
    switch (data->unk8) {
    case 0: {
        int offset = ManagedSprite_GetPaletteOverrideOffset(data->managedSprite);
        PaletteData_BeginPaletteFade(data->unk90.paletteData, 1 << PLTTBUF_MAIN_OBJ, (u16)(1 << offset), -5, 0, 10, RGB_BLACK);
        data->unk8++;
        break;
    }
    case 1:
        if (PaletteData_GetSelectedBuffersBitmask(data->unk90.paletteData) == 0) {
            BallBurstEffectTemplate template;
            template.ball = data->unk90.ball;
            template.heapId = data->unk90.heapID;
            template.emitter = BallAnim_GetBurstSkippedEmitter(template.ball);
            template.generic = FALSE;
            ManagedSprite_GetPositionXY(data->managedSprite, &template.x, &template.y);
            data->unkD0 = (int)BallBurstEffect_New(&template);
            data->unk8++;
        }
        break;
    case 2:
        BallBurstEffect_Start((BallBurstEffect *)data->unkD0);
        data->unk8++;
        break;
    case 3:
        if (BallBurstEffect_IsRunning((BallBurstEffect *)data->unkD0) == FALSE) {
            ManagedSprite_SetAnimationFrame(data->managedSprite, 0);
            BallBurstEffect_Free((BallBurstEffect *)data->unkD0);
            data->unk8++;
        }
        break;
    default:
        if (PaletteData_GetSelectedBuffersBitmask(data->unk90.paletteData) == 0) {
            UnkBallData_GoToCaptureState(data, 22);
        }
        break;
    }
    return TRUE;
}

BOOL ov07_02233498(UnkBallData *data) {
    return TRUE;
}

BOOL UnkBallData_CaptureStateRestorePalette(UnkBallData *data) {
    switch (data->unk8) {
    case 0: {
        int offset = ManagedSprite_GetPaletteOverrideOffset(data->managedSprite);
        PaletteData_BeginPaletteFade(data->unk90.paletteData, 1 << PLTTBUF_MAIN_OBJ, (u16)(1 << offset), -5, 10, 0, RGB_BLACK);
        data->unk8++;
        break;
    }
    case 1:
        if (PaletteData_GetSelectedBuffersBitmask(data->unk90.paletteData) == 0) {
            data->unk8++;
        }
        break;
    default:
        UnkBallData_GoToCaptureState(data, 24);
        break;
    }
    return TRUE;
}

BOOL ov07_02233500(UnkBallData *data) {
    UnkBallData_GoToCaptureState(data, 25);
    return TRUE;
}

BOOL UnkBallData_CaptureStateFadeOut(UnkBallData *data) {
    switch (data->unk8) {
    case 0:
        ManagedSprite_SetOamMode(data->managedSprite, GX_OAM_MODE_XLU);
        data->unk8++;
        // fallthrough
    case 1:
        if (data->unk20 != 0) {
            data->unk20--;
            data->unk21++;
        } else {
            data->unk20 = 0;
            data->unk21 = 15;
            ManagedSprite_SetDrawFlag(data->managedSprite, FALSE);
            data->unk8++;
        }
        reg_G2_BLDALPHA = data->unk20 | (data->unk21 << 8);
        break;
    default:
        UnkBallData_GoToCaptureState(data, 26);
        break;
    }
    return TRUE;
}

BOOL ov07_02233590(UnkBallData *data) {
    UnkBallData_GoToCaptureState(data, 27);
    return TRUE;
}

BOOL UnkBallData_CaptureStateEnd(UnkBallData *data) {
    return FALSE;
}

BOOL UnkBallData_RunCaptureState(UnkBallData *data) {
    return sUnkBallDataCaptureStateFuncs[data->unk14](data);
}

BOOL UnkBallData_RunThrowArc(UnkBallData *data) {
    switch (data->unk8) {
    case 0:
        BattleAnimUtil_InitLinearSinMotion(BALL_ARC_MOTION(data), BALL_ARC_MOTION2(data), BALL_X(data), BALL_TARGET_X(data), BALL_Y(data), BALL_TARGET_Y(data), BALL_ARC_DURATION(data), data->unkC8 << FX32_SHIFT);
        data->unkC4 = 0;
        data->unk8++;
        data->unkD4 = 0;
        if (UnkBallData_IsSpinningKind(data->unk90.unk0) == TRUE) {
            ManagedSprite_OffsetAffineZRotation(data->managedSprite, (LCRandom() % 20 + 10) << 13);
        }
        break;
    case 1:
        if (UnkBallData_IsSpinningKind(data->unk90.unk0) == TRUE) {
            ManagedSprite_OffsetAffineZRotation(data->managedSprite, 2 << 12);
            if (data->unkC4 > BALL_ARC_DURATION(data) / 2 + 10) {
                ManagedSprite_OffsetAffineZRotation(data->managedSprite, 2 << 12);
            }
            if (UnkBallData_IsFlashingKind(data->unk90.unk0) == TRUE && data->unkC4 == BALL_ARC_DURATION(data) / 2 + 10) {
                data->unkD4 = (int)BattleAnimUtil_StartPaletteBlend(data->unk90.paletteData, data->unk90.heapID, 2, (u16)(ManagedSprite_GetPaletteOverrideOffset(data->managedSprite) << 4), 16, -2, 2, 0, 14, 0xFFFF, 1002);
            }
        }
        data->unkC4++;
        if (data->unk90.unk0 < 6 || data->unk90.unk0 > 8 || data->unkC4 <= BALL_ARC_DURATION(data) / 2 || data->unkC4 >= BALL_ARC_DURATION(data) / 2 + 5) {
            if (BattleAnimUtil_UpdateLinearSinMotionSprite(BALL_ARC_MOTION(data), BALL_ARC_MOTION2(data), data->managedSprite) == FALSE) {
                UnkBallData_SetRotation(data, 0);
                data->unk8++;
            }
        }
        break;
    case 2:
        if (UnkBallData_IsFlashingKind(data->unk90.unk0) == TRUE && data->unkD4 != 0) {
            if (BattleAnimUtil_IsPaletteBlendActive((void *)data->unkD4) == TRUE) {
                break;
            }
            BattleAnimUtil_FreePaletteBlend((void *)data->unkD4);
            data->unk28 = TRUE;
            return FALSE;
        }
        if (data->unk90.unk0 >= 15) {
            data->unk28 = TRUE;
            return FALSE;
        }
        if (data->unk90.ball == 1025) {
            PlaySE_SetPan(SEQ_SE_DP_W202B, 117);
        }
        data->unk8++;
        break;
    case 3:
        ManagedSprite_SetAnim(data->managedSprite, 1);
        UnkBallData_SetAnimateSprite(data, TRUE);
        data->unkC4 = 0;
        data->unk8++;
        break;
    default:
        if (data->unkC4 == 5) {
            if (data->unk90.unk0 != 12 && data->unk90.unk0 != 13) {
                data->unkD4 = (int)BattleAnimUtil_StartPaletteBlend(data->unk90.paletteData, data->unk90.heapID, 2, (u16)(ManagedSprite_GetPaletteOverrideOffset(data->managedSprite) << 4), 16, -2, 2, 0, 14, 0xFFFF, 1002);
            }
            data->unk28 = TRUE;
        }
        data->unkC4++;
        if (data->unkC4 > 15) {
            if (data->unk90.unk0 != 12 && data->unk90.unk0 != 13) {
                if (BattleAnimUtil_IsPaletteBlendActive((void *)data->unkD4) == TRUE) {
                    break;
                }
                BattleAnimUtil_FreePaletteBlend((void *)data->unkD4);
            }
            return FALSE;
        }
        break;
    }
    return TRUE;
}

BOOL ov07_02233874(UnkBallData *data) {
    switch (data->unk8) {
    case 0:
        BattleAnimUtil_InitLinearSinMotion(BALL_ARC_MOTION(data), BALL_ARC_MOTION2(data), BALL_X(data), BALL_TARGET_X(data), BALL_Y(data), BALL_TARGET_Y(data), BALL_ARC_DURATION(data), data->unkC8 << FX32_SHIFT);
        data->unkC4 = 0;
        data->unk8++;
        data->unkD4 = 0;
        if (UnkBallData_IsSpinningKind(data->unk90.unk0) == TRUE) {
            ManagedSprite_OffsetAffineZRotation(data->managedSprite, (LCRandom() % 20 + 10) << 13);
        }
        break;
    case 1:
        if (UnkBallData_IsSpinningKind(data->unk90.unk0) == TRUE) {
            ManagedSprite_OffsetAffineZRotation(data->managedSprite, 2 << 12);
            if (data->unkC4 > BALL_ARC_DURATION(data) / 2 + 10) {
                ManagedSprite_OffsetAffineZRotation(data->managedSprite, 2 << 12);
            }
            if (UnkBallData_IsFlashingKind(data->unk90.unk0) == TRUE && data->unkC4 == BALL_ARC_DURATION(data) / 2 + 10) {
                data->unkD4 = (int)BattleAnimUtil_StartPaletteBlend(data->unk90.paletteData, data->unk90.heapID, 2, (u16)(ManagedSprite_GetPaletteOverrideOffset(data->managedSprite) << 4), 16, -2, 2, 0, 14, 0xFFFF, 1002);
            }
        }
        data->unkC4++;
        if (data->unk90.unk0 < 6 || data->unk90.unk0 > 8 || data->unkC4 <= BALL_ARC_DURATION(data) / 2 || data->unkC4 >= BALL_ARC_DURATION(data) / 2 + 5) {
            if (BattleAnimUtil_UpdateLinearSinMotionSprite(BALL_ARC_MOTION(data), BALL_ARC_MOTION2(data), data->managedSprite) == FALSE) {
                UnkBallData_SetRotation(data, 0);
                data->unk8++;
            }
        }
        break;
    case 2:
        if (UnkBallData_IsFlashingKind(data->unk90.unk0) == TRUE && data->unkD4 != 0) {
            if (BattleAnimUtil_IsPaletteBlendActive((void *)data->unkD4) == TRUE) {
                break;
            }
            BattleAnimUtil_FreePaletteBlend((void *)data->unkD4);
            data->unk28 = TRUE;
            return FALSE;
        }
        if (data->unk90.unk0 >= 15) {
            data->unk28 = TRUE;
            return FALSE;
        }
        data->unk8++;
        break;
    case 3:
        ManagedSprite_SetAnim(data->managedSprite, 1);
        UnkBallData_SetAnimateSprite(data, FALSE);
        data->unkC4 = 0;
        data->unk8++;
        break;
    default:
        if (data->unkC4 == 5) {
            ManagedSprite_SetAnimationFrame(data->managedSprite, 2);
            data->unkD4 = (int)BattleAnimUtil_StartPaletteBlend(data->unk90.paletteData, data->unk90.heapID, 2, (u16)(ManagedSprite_GetPaletteOverrideOffset(data->managedSprite) << 4), 16, -2, 2, 0, 14, 0xFFFF, 1002);
            data->unk28 = TRUE;
        }
        data->unkC4++;
        if (data->unkC4 > 15) {
            if (BattleAnimUtil_IsPaletteBlendActive((void *)data->unkD4) != TRUE) {
                BattleAnimUtil_FreePaletteBlend((void *)data->unkD4);
                return FALSE;
            }
        }
        break;
    }
    return TRUE;
}

BOOL UnkBallData_OpenInPlace(UnkBallData *data) {
    BallBurstEffectTemplate template;

    switch (data->unk8) {
    case 0:
        UnkBallData_SetAnimateSprite(data, FALSE);
        ManagedSprite_SetAnim(data->managedSprite, 1);
        data->unkC4++;
        if (data->unkC4 >= 2) {
            ManagedSprite_SetAnimationFrame(data->managedSprite, 0);
            data->unkC4 = 0;
            data->unk8++;
        }
        break;
    case 1:
        data->unkC4++;
        if (data->unkC4 >= 2) {
            ManagedSprite_SetAnimationFrame(data->managedSprite, 2);
            data->unkC4 = 0;
            data->unk8++;
        }
        break;
    case 2:
        template.ball = data->unk90.ball;
        template.heapId = data->unk90.heapID;
        template.emitter = 0xFF;
        template.generic = TRUE;
        ManagedSprite_GetPositionXY(data->managedSprite, &template.x, &template.y);
        data->unkD0 = (int)BallBurstEffect_New(&template);
        data->unk8++;
        break;
    case 3:
        BallBurstEffect_Start((BallBurstEffect *)data->unkD0);
        data->unk8++;
        break;
    case 4:
        data->unkC4++;
        if (data->unkC4 >= 6) {
            ManagedSprite_SetAnimationFrame(data->managedSprite, 2);
            data->unkC4 = 0;
            data->unk8++;
        }
        break;
    case 5:
        data->unkC4++;
        if (data->unkC4 >= 6) {
            ManagedSprite_SetAnimationFrame(data->managedSprite, 1);
            data->unkC4 = 0;
            data->unk8++;
        }
        break;
    case 6:
        data->unkC4++;
        if (data->unkC4 >= 6) {
            ManagedSprite_SetAnimationFrame(data->managedSprite, 0);
            data->unkC4 = 0;
            data->unk8++;
        }
        break;
    case 7:
        if (!BallBurstEffect_IsRunning((BallBurstEffect *)data->unkD0)) {
            BallBurstEffect_Free((BallBurstEffect *)data->unkD0);
            data->unk8++;
        }
        break;
    default:
        return FALSE;
    }
    return TRUE;
}

BOOL UnkBallData_Wobble(UnkBallData *data) {
    switch (data->unk8) {
    case 0:
        if (data->unk10 == 0) {
            BattleAnimUtil_InitValueMotion(BALL_ROTATION(data), -0x1FFF, 0x1FFF, 10);
        } else {
            BattleAnimUtil_InitValueMotion(BALL_ROTATION(data), 0x1FFF, -0x1FFF, 10);
        }
        data->unk10 ^= 1;
        data->unk8++;
        break;
    case 1:
        ManagedSprite_SetAffineZRotation(data->managedSprite, *BALL_ROTATION(data));
        if (!BattleAnimUtil_UpdateValueMotion(BALL_ROTATION(data))) {
            if (data->unkC >= 1) {
                data->unk8++;
            } else {
                data->unkC++;
                data->unk8--;
            }
        }
        break;
    default:
        data->unkC4 = 0;
        return FALSE;
    }
    return TRUE;
}

BOOL UnkBallData_TrackPosition(UnkBallData *data) {
    s16 x;
    s16 y;

    if (data->unk8 == 0xFF) {
        return FALSE;
    }
    if (data->managedSprite != NULL) {
        ManagedSprite_GetPositionXY(data->managedSprite, &x, &y);
        BALL_X(data) = x;
        BALL_Y(data) = y;
    }
    return TRUE;
}

void UnkBallData_Task(SysTask *task, void *data) {
    UnkBallData *ballData = data;
    if (ballData->unk1C) {
        if (ballData->unkDC > 0) {
            ballData->unkDC--;
            SpriteSystem_DrawSprites(ballData->spriteManager);
            return;
        }
        if (!sUnkBallDataModeFuncs[ballData->unk90.unk8](ballData)) {
            ballData->unk1C = FALSE;
        }
        if (ballData->unk24 == TRUE) {
            ManagedSprite_TickFrame(ballData->managedSprite);
        }
        SpriteSystem_DrawSprites(ballData->spriteManager);
    }
}

UnkBallData *UnkBallData_New(UnkStruct_134 *unkStruct_134) {
    UnkBallData *data = Heap_Alloc(unkStruct_134->heapID, sizeof(UnkBallData));
    MI_CpuFill8(data, 0, sizeof(UnkBallData));
    GF_ASSERT(data != NULL);
    data->unk90 = *unkStruct_134;
    data->unk8 = 0;
    data->unk10 = 0;
    data->unkC = 0;
    data->unk14 = 0;
    data->unk18 = 0;
    data->unk0 = 0;
    data->unk4 = 0;
    data->spriteManager = SpriteManager_New(data->unk90.spriteSystem);
    data->unkDC = 0;
    data->unk24 = UnkBallData_ShouldAnimateSprite(data->unk90.unk0);
    data->unkC4 = 0;
    data->unk20 = 16;
    data->unk21 = 0;
    BattleAnimSystem_InitBlend();
    if (LCRandom() % 2) {
        data->unk22 = 1;
    } else {
        data->unk22 = -1;
    }
    UnkBallData_LoadResources(data);
    UnkBallData_CreateSprite(data);
    UnkBallData_InitThrowTarget(data);
    data->unk1C = TRUE;
    data->unk28 = FALSE;
    data->unkCC = SysTask_CreateOnMainQueue(UnkBallData_Task, data, 1000);
    return data;
}

BOOL UnkBallData_IsActive(UnkBallData *data) {
    GF_ASSERT(data != NULL);
    if (data->unk1C) {
        return TRUE;
    }
    return FALSE;
}

BOOL UnkBallData_HasOpened(UnkBallData *data) {
    GF_ASSERT(data != NULL);
    if (data->unk28) {
        return TRUE;
    }
    return FALSE;
}

void UnkBallData_SetCaptureState(UnkBallData *data, int state) {
    data->unk14 = state;
}

BOOL UnkBallData_IsCaptureState(UnkBallData *data, int state) {
    if (data->unk14 == state) {
        return TRUE;
    }
    return FALSE;
}

void UnkBallData_Delete(UnkBallData *data) {
    GF_ASSERT(data != NULL);
    SpriteSystem_FreeResourcesAndManager(data->unk90.spriteSystem, data->spriteManager);
    Sprite_DeleteAndFreeResources(data->managedSprite);
    SysTask_Destroy(data->unkCC);
    Heap_Free(data);
}

void UnkBallData_SetMode(UnkBallData *data, int mode) {
    data->unk90.unk8 = mode;
    data->unk8 = 0;
    data->unkC = 0;
    data->unk10 = 0;
    data->unk1C = TRUE;
    ManagedSprite_GetPositionXY(data->managedSprite, &BALL_X(data), &BALL_Y(data));
}

s32 UnkBallData_GetMode(UnkBallData *data) {
    GF_ASSERT(data != NULL);
    return data->unk90.unk8;
}

void UnkBallData_InitThrowTarget(UnkBallData *data) {
    ManagedSprite_GetPositionXY(data->managedSprite, &BALL_X(data), &BALL_Y(data));
    switch (data->unk90.unk0) {
    case 0:
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
        ManagedSprite_GetPositionXY(data->managedSprite, &BALL_TARGET_X(data), &BALL_TARGET_Y(data));
        data->unkC8 = 0;
        BALL_ARC_DURATION(data) = 12;
        return;
    case 6:
        BattleAnimUtil_LookupScreenPos(0, 0, &BALL_TARGET_X(data), &BALL_TARGET_Y(data));
        data->unkC8 = 48;
        BALL_TARGET_Y(data) = BALL_TARGET_Y(data) + 32;
        break;
    case 7:
        ManagedSprite_GetPositionXY(data->managedSprite, &BALL_X(data), &BALL_Y(data));
        BattleAnimUtil_LookupScreenPos(1, 4, &BALL_TARGET_X(data), &BALL_TARGET_Y(data));
        data->unkC8 = 48;
        BALL_TARGET_Y(data) = BALL_TARGET_Y(data) + 32;
        break;
    case 8:
        BattleAnimUtil_LookupScreenPos(1, 2, &BALL_TARGET_X(data), &BALL_TARGET_Y(data));
        data->unkC8 = 48;
        BALL_TARGET_Y(data) = BALL_TARGET_Y(data) + 32;
        break;
    case 9:
        BattleAnimUtil_LookupScreenPos(0, 1, &BALL_TARGET_X(data), &BALL_TARGET_Y(data));
        data->unkC8 = 48;
        BALL_TARGET_Y(data) = BALL_TARGET_Y(data) + 32;
        break;
    case 10:
        BattleAnimUtil_LookupScreenPos(1, 3, &BALL_TARGET_X(data), &BALL_TARGET_Y(data));
        data->unkC8 = 48;
        BALL_TARGET_Y(data) = BALL_TARGET_Y(data) + 32;
        break;
    case 11:
        BattleAnimUtil_LookupScreenPos(1, 5, &BALL_TARGET_X(data), &BALL_TARGET_Y(data));
        data->unkC8 = 48;
        BALL_TARGET_Y(data) = BALL_TARGET_Y(data) + 32;
        break;
    case 12:
        BattleAnimUtil_LookupScreenPos(0, 1, &BALL_TARGET_X(data), &BALL_TARGET_Y(data));
        data->unkC8 = 16;
        BALL_TARGET_Y(data) = BALL_TARGET_Y(data) + 16;
        BALL_ARC_DURATION(data) = 12;
        return;
    case 13:
        BattleAnimUtil_LookupScreenPos(0, 1, &BALL_TARGET_X(data), &BALL_TARGET_Y(data));
        data->unkC8 = 32;
        BALL_TARGET_X(data) = BALL_TARGET_X(data) - 20;
        BALL_TARGET_Y(data) = BALL_TARGET_Y(data) + 38;
        BALL_ARC_DURATION(data) = 16;
        return;
    case 14:
        BALL_TARGET_X(data) = 128;
        BALL_TARGET_Y(data) = 64;
        data->unkC8 = 32;
        BALL_TARGET_Y(data) = BALL_TARGET_Y(data) + 32;
        break;
    case 15:
        BattleAnimUtil_LookupScreenPos(0, 1, &BALL_TARGET_X(data), &BALL_TARGET_Y(data));
        data->unkC8 = 64;
        BALL_TARGET_Y(data) = BALL_TARGET_Y(data) + 8;
        BALL_ARC_DURATION(data) = 16;
        return;
    case 16:
        BattleAnimUtil_LookupScreenPos(1, 3, &BALL_TARGET_X(data), &BALL_TARGET_Y(data));
        data->unkC8 = 64;
        BALL_TARGET_Y(data) = BALL_TARGET_Y(data) + 8;
        BALL_ARC_DURATION(data) = 16;
        return;
    case 17:
        BattleAnimUtil_LookupScreenPos(1, 5, &BALL_TARGET_X(data), &BALL_TARGET_Y(data));
        data->unkC8 = 64;
        BALL_TARGET_Y(data) = BALL_TARGET_Y(data) + 8;
        BALL_ARC_DURATION(data) = 16;
        return;
    }
    BALL_ARC_DURATION(data) = 20;
}

void UnkBallData_GetStartPosition(UnkBallData *data, s16 *x, s16 *y) {
    switch (data->unk90.unk0) {
    case 0:
        BattleAnimUtil_LookupScreenPos(0, 0, x, y);
        *y = *y + 40;
        break;
    case 1:
        BattleAnimUtil_LookupScreenPos(0, 1, x, y);
        *y = *y + 38;
        break;
    case 2:
        BattleAnimUtil_LookupScreenPos(1, 2, x, y);
        *y = *y + 40;
        break;
    case 4:
        BattleAnimUtil_LookupScreenPos(1, 4, x, y);
        *y = *y + 40;
        break;
    case 3:
        BattleAnimUtil_LookupScreenPos(1, 3, x, y);
        *y = *y + 38;
        break;
    case 5:
        BattleAnimUtil_LookupScreenPos(1, 5, x, y);
        *y = *y + 38;
        break;
    case 6:
        *x = 10;
        *y = 100;
        break;
    case 7:
        *x = 10;
        *y = 100;
        break;
    case 8:
        *x = 10;
        *y = 100;
        break;
    case 9:
        BattleAnimUtil_LookupScreenPos(0, 0, x, y);
        break;
    case 10:
        BattleAnimUtil_LookupScreenPos(0, 0, x, y);
        break;
    case 11:
        BattleAnimUtil_LookupScreenPos(0, 0, x, y);
        break;
    case 12:
        BattleAnimUtil_LookupScreenPos(0, 0, x, y);
        break;
    case 13:
        BattleAnimUtil_LookupScreenPos(0, 0, x, y);
        break;
    case 14:
        *x = 230;
        *y = 32;
        break;
    case 15:
    case 16:
    case 17:
        *x = -30;
        *y = 160;
        break;
    }
}

void UnkBallData_LoadResources(UnkBallData *data) {
    SpriteResourceCountsListUnion counts;
    NARC *narc;
    int charRes;
    int plttRes;
    int cellRes;
    int animRes;
    int i;

    SpriteSystem_InitSprites(data->unk90.spriteSystem, data->spriteManager, 10);
    if (data->unk90.unk18 == 0) {
        G2dRenderer_SetSubSurfaceCoords(SpriteSystem_GetRenderer(data->unk90.spriteSystem), 0, FX32_CONST(272));
    }
    for (i = 0; i < GF_GFX_RES_TYPE_MAX; i++) {
        counts.asArray[i] = 10;
    }
    counts.numMcel = 0;
    counts.numManm = 0;
    SpriteSystem_InitManagerWithCapacities(data->unk90.spriteSystem, data->spriteManager, &counts);
    charRes = BallAnim_GetSpriteResource(data->unk90.ball, 0);
    plttRes = BallAnim_GetSpriteResource(data->unk90.ball, 1);
    cellRes = BallAnim_GetSpriteResource(data->unk90.ball, 2);
    animRes = BallAnim_GetSpriteResource(data->unk90.ball, 3);
    narc = NARC_New(NARC_a_0_0_8, data->unk90.heapID);
    SpriteSystem_LoadCharResObjFromOpenNarc(data->unk90.spriteSystem, data->spriteManager, narc, charRes, TRUE, NNS_G2D_VRAM_TYPE_2DMAIN, data->unk90.unkC + 6000);
    SpriteSystem_LoadPaletteBufferFromOpenNarc(data->unk90.paletteData, PLTTBUF_MAIN_OBJ, data->unk90.spriteSystem, data->spriteManager, narc, plttRes, FALSE, 1, NNS_G2D_VRAM_TYPE_2DMAIN, data->unk90.unkC + 6000);
    SpriteSystem_LoadCellResObjFromOpenNarc(data->unk90.spriteSystem, data->spriteManager, narc, cellRes, TRUE, data->unk90.unkC + 6000);
    SpriteSystem_LoadAnimResObjFromOpenNarc(data->unk90.spriteSystem, data->spriteManager, narc, animRes, TRUE, data->unk90.unkC + 6000);
    NARC_Delete(narc);
}

void UnkBallData_CreateSprite(UnkBallData *data) {
    ManagedSpriteTemplate template;
    int i;

    UnkBallData_GetStartPosition(data, &template.x, &template.y);
    template.z = 0;
    template.animation = 0;
    template.drawPriority = 0;
    template.pal = 0;
    template.vram = NNS_G2D_VRAM_TYPE_2DMAIN;
    template.bgPriority = data->unk90.unk14;
    template.vramTransfer = FALSE;
    for (i = 0; i < GF_GFX_RES_TYPE_MAX; i++) {
        template.resIdList[i] = data->unk90.unkC + 6000;
    }
    data->managedSprite = SpriteSystem_NewSprite(data->unk90.spriteSystem, data->spriteManager, &template);
    ManagedSprite_SetDrawFlag(data->managedSprite, TRUE);
    ManagedSprite_SetAffineOverwriteMode(data->managedSprite, 2);
    ManagedSprite_SetAnimationFrame(data->managedSprite, 0);
    ManagedSprite_SetAnim(data->managedSprite, 0);
    ManagedSprite_TickFrame(data->managedSprite);
    BattleAnimSystem_InitBlend();
}

void UnkBallData_SetDrawFlag(UnkBallData *data, int flag) {
    ManagedSprite_SetDrawFlag(data->managedSprite, flag);
}

void UnkBallData_SetPosition(UnkBallData *data, s16 x, s16 y) {
    ManagedSprite_SetPositionXY(data->managedSprite, x, y);
}

void UnkBallData_SetRotation(UnkBallData *data, u16 rotation) {
    ManagedSprite_SetAffineZRotation(data->managedSprite, rotation);
}

void UnkBallData_SetAnimateSprite(UnkBallData *data, BOOL animate) {
    data->unk24 = animate;
}

void UnkBallData_SetDrawPriority(UnkBallData *data, u16 priority) {
    ManagedSprite_SetDrawPriority(data->managedSprite, priority);
}

void UnkBallData_SetPriority(UnkBallData *data, int priority) {
    ManagedSprite_SetPriority(data->managedSprite, priority);
}

void UnkBallData_SetDelay(UnkBallData *data, int delay) {
    data->unkDC = delay;
}

s32 BallSealEffect_GetDistance(s16 x0, s16 y0, s16 x1, s16 y1) {
    s16 dx = x0 - x1;
    s16 dy = -(y0 - y1);
    return FX_Sqrt((dy * dy + dx * dx) * FX32_ONE) / FX32_ONE;
}

s32 BallSealEffect_GetSealDistance(const SEAL *seal) {
    s16 x = SealOnCapsuleGetX(seal);
    return BallSealEffect_GetDistance(x, SealOnCapsuleGetY(seal), 190, 70);
}

u8 BallSealEffect_GetSealEmitDelay(const SEAL *seal) {
    BallSealEmitDelays delays = sBallSealEmitDelays;
    int band;

    if (!sub_0209109C((u8)SealOnCapsuleGetID(seal))) {
        band = BallSealEffect_GetSealDistance(seal);
        if (band >= 56) {
            band = 3;
        } else {
            band = (band + 1) / 20;
            if (band >= 3) {
                band = 3;
            }
        }
        return delays.delays[band];
    }
    return delays.delays[1];
}

BattleAnimEmitterManager *BattleAnimEmitterManager_New(enum HeapID heapId, int *unk) {
    int i;
    BattleAnimEmitterManager *manager = Heap_Alloc(heapId, sizeof(BattleAnimEmitterManager));
    if (manager == NULL) {
        GF_AssertFail();
        return NULL;
    }
    BattleAnimSystem_InitBlend();
    manager->heapId = heapId;
    if (unk != NULL) {
        manager->unk_04 = unk[0];
    }
    for (i = 0; i < 16; i++) {
        manager->emitters[i] = NULL;
    }
    return manager;
}

BattleAnimEmitter *BattleAnimEmitterManager_AddEmitter(BattleAnimEmitterManager *manager, BattleAnimEmitterTemplate *template) {
    BattleAnimEmitter *emitter = NULL;
    int i;

    for (i = 0; i < 16; i++) {
        if (manager->emitters[i] == NULL) {
            manager->emitters[i] = BattleAnimEmitter_New(manager->heapId, template);
            manager->emitters[i]->task = NULL;
            emitter = manager->emitters[i];
            emitter->index = i;
            break;
        }
    }
    GF_ASSERT(emitter != NULL);
    return emitter;
}

void BattleAnimEmitterManager_Free(BattleAnimEmitterManager *manager) {
    int i;
    for (i = 0; i < 16; i++) {
        if (manager->emitters[i] != NULL) {
            Heap_Free(manager->emitters[i]);
        }
    }
    Heap_Free(manager);
}

void BattleAnimEmitterManager_StopAll(BattleAnimEmitterManager *manager) {
    int i;
    for (i = 0; i < 16; i++) {
        if (manager->emitters[i] != NULL) {
            BattleAnimSystem_FreeParticleSystem(manager->emitters[i]->particleSystem);
            if (manager->emitters[i]->active && manager->emitters[i]->task != NULL) {
                SysTask_Destroy(manager->emitters[i]->task);
            }
        }
    }
}

BattleAnimEmitter *BattleAnimEmitter_New(enum HeapID heapId, BattleAnimEmitterTemplate *template) {
    BattleAnimEmitter *emitter = Heap_Alloc(heapId, sizeof(BattleAnimEmitter));
    if (emitter == NULL) {
        GF_AssertFail();
        return NULL;
    }
    emitter->heapId = heapId;
    emitter->template = *template;
    emitter->emitterCount = BattleAnimEmitter_GetEmitterCount(emitter->template.resourceId);
    emitter->particleSystem = BattleAnimSystem_NewParticleSystem(emitter->heapId, emitter->template.resourceId, TRUE);
    return emitter;
}

void BattleAnimEmitter_Start(BattleAnimEmitter *emitter) {
    emitter->active = TRUE;
    BattleAnimEmitter_CreateEmitters(emitter, (void (*)(SPLEmitter *))ov07_02234710);
    emitter->task = SysTask_CreateOnMainQueue(BattleAnimEmitter_WaitTask, emitter, 1000);
}

BOOL BattleAnimEmitter_IsActive(BattleAnimEmitter *emitter) {
    return emitter->active;
}

void BattleAnimEmitter_WaitTask(SysTask *task, void *data) {
    BattleAnimEmitter *emitter = data;
    if (!ParticleSystem_GetActiveEmitterCount(emitter->particleSystem)) {
        emitter->active = FALSE;
        BattleAnimSystem_FreeParticleSystem(emitter->particleSystem);
        SysTask_Destroy(task);
    }
}

void BattleAnimEmitter_CreateEmitters(BattleAnimEmitter *emitter, void (*callback)(SPLEmitter *)) {
    int i;
    for (i = 0; i < emitter->emitterCount; i++) {
        ParticleSystem_CreateEmitterWithParam(emitter->particleSystem, i, callback, emitter);
    }
    ParticleSystem_SetPerspectiveType(emitter->particleSystem, emitter->template.perspective);
}

void *ov07_02234710(void) {
    return ParticleSystem_GetEmitterParam();
}

int BattleAnimEmitter_GetEmitterCount(int resourceId) {
    BattleAnimEmitterCounts counts = sBattleAnimEmitterCounts;
    u32 index = resourceId - 5;
    if (index >= 22) {
        GF_AssertFail();
        return 0;
    }
    return counts.counts[index];
}

void BattleAnimUtil_LoadMonIntoPicSameFacing(BattleAnimBattlerPicParams *params, enum HeapID heapId) {
    BattleAnimUtil_LoadMonIntoPic(params, params->battler, FALSE, heapId);
}

void BattleAnimUtil_LoadMonIntoPicOppositeFacing(BattleAnimBattlerPicParams *params, enum HeapID heapId) {
    BattleAnimUtil_LoadMonIntoPic(params, params->sourceBattler, TRUE, heapId);
}

// Pushes the battler's pic and loads the source battler's graphics into it, facing like the battler in slot
// target (the other way round when oppositeFacing)
#pragma force_active on

void BattleAnimUtil_LoadMonIntoPic(BattleAnimBattlerPicParams *params, int target, BOOL oppositeFacing, enum HeapID heapId) {
    PokepicTemplate template;
    PokepicTemplate *picTemplate;
    int species;
    int gender, shiny, form;
    u32 personality;
    int facing, y;

    struct {
        u8 shadowSize;
        s8 shadowXOffset;
        s8 shadowHeight;
    } shadow;

    Pokepic_Push(params->pokepics[params->battler]);
    species = params->species[params->sourceBattler];
    gender = params->genders[params->sourceBattler];
    shiny = params->shiny[params->sourceBattler];
    form = params->forms[params->sourceBattler];
    personality = params->personalities[params->sourceBattler];
    if (oppositeFacing) {
        if (params->battlerTypes[target] & BATTLER_TYPE_IS_ENEMY) {
            facing = 0;
        } else {
            facing = 2;
        }
    } else {
        if (params->battlerTypes[target] & BATTLER_TYPE_IS_ENEMY) {
            facing = 2;
        } else {
            facing = 0;
        }
    }
    GetMonSpriteCharAndPlttNarcIdsEx(&template, species, gender, facing, shiny, form, personality);
    picTemplate = Pokepic_GetTemplate(params->pokepics[params->battler]);
    *picTemplate = template;
    Pokepic_ScheduleReloadFromNarc(params->pokepics[params->battler]);
    sub_02014540((NarcId)picTemplate->narcID, picTemplate->charDataID, heapId, params->picData[params->battler]->unk0, personality, FALSE, facing, picTemplate->species);
    params->picData[params->battler]->unk4 = picTemplate->narcID;
    params->picData[params->battler]->unk8 = picTemplate->palDataID;
    params->picData[params->battler]->unkC = GetMonPicHeightBySpeciesGenderForm(species, gender, facing, form, personality);
    {
        s8 height = params->picData[params->battler]->unkC;
        y = ov07_02234B5C(params->battlerTypes[params->battler], 1);
        Pokepic_SetAttr(params->pokepics[params->battler], POKEPIC_Y, y + height);
        if (facing == 2) {
            NARC *narc = NARC_New(NARC_a_1_8_0, heapId);
            // the shadow readers' fourth parameter is not passed
            ((void (*)(NARC *, s8 *, u16))sub_020729D8)(narc, &shadow.shadowHeight, species);
            ((void (*)(NARC *, s8 *, u16))sub_020729FC)(narc, &shadow.shadowXOffset, species);
            ((void (*)(NARC *, u8 *, u16))sub_02072A20)(narc, &shadow.shadowSize, species);
            NARC_Delete(narc);
            Pokepic_SetAttr(params->pokepics[params->battler], POKEPIC_SHADOW_SIZE, shadow.shadowSize);
            y += 36;
            Pokepic_SetAttr(params->pokepics[params->battler], POKEPIC_SHADOW_Y, y);
            Pokepic_SetAttr(params->pokepics[params->battler], POKEPIC_SHADOW_XOFFSET, shadow.shadowXOffset);
            Pokepic_SetAttr(params->pokepics[params->battler], POKEPIC_SHADOW_YOFFSET, 36 - height);
            Pokepic_SetAttr(params->pokepics[params->battler], POKEPIC_SHADOW_H, shadow.shadowHeight);
        }
    }
}

#pragma force_active reset

// Pops the battler's pic and reloads its own graphics
#pragma force_active on
#pragma push
#pragma opt_propagation off

void BattleAnimUtil_RestorePic(BattleAnimBattlerPicParams *params, enum HeapID heapId) {
    Pokepic_Pop(params->pokepics[params->battler]);
    PokepicTemplate *picTemplate = Pokepic_GetTemplate(params->pokepics[params->battler]);
    int species = params->species[params->battler];
    int gender = params->genders[params->battler];
    int form = params->forms[params->battler];
    u32 personality = params->personalities[params->battler];
    int facing;
    if (params->battlerTypes[params->battler] & BATTLER_TYPE_IS_ENEMY) {
        facing = 2;
    } else {
        facing = 0;
    }
    sub_02014540((NarcId)picTemplate->narcID, picTemplate->charDataID, heapId, params->picData[params->battler]->unk0, personality, FALSE, facing, picTemplate->species);
    params->picData[params->battler]->unk4 = picTemplate->narcID;
    params->picData[params->battler]->unk8 = picTemplate->palDataID;
    params->picData[params->battler]->unkC = GetMonPicHeightBySpeciesGenderForm((u16)species, (u8)gender, (u8)facing, (u8)form, personality);
    int battler = params->battler;
    u32 slot = battler * sizeof(Pokepic *); // byte offset of the battler's pointers, kept from before the call
    int offset = ov07_02234B5C(params->battlerTypes[battler], 1);
    int height = (*(UnkBattleSystemSub1D0 **)((u8 *)params->picData + slot))->unkC;
    Pokepic_SetAttr(*(Pokepic **)((u8 *)params->pokepics + slot), POKEPIC_Y, height + offset);
}

#pragma pop
#pragma force_active reset

// Loads a fixed graphic of the otherpoke NARC (not a species) into the battler's pic, front or back by its battler type
#pragma force_active on

void ov07_02234A20(BattleAnimBattlerPicParams *params, enum HeapID heapId) {
    PokepicTemplate template;
    PokepicTemplate *picTemplate;
    u8 height;
    int member, y;

    Pokepic_Push(params->pokepics[params->battler]);
    template.narcID = NARC_poketool_pokegra_otherpoke;
    template.palDataID = 258;
    template.species = 0;
    template.isAnimated = FALSE;
    template.personality = 0;
    if (params->battlerTypes[params->battler] & BATTLER_TYPE_IS_ENEMY) {
        template.charDataID = 257;
        member = 135;
    } else {
        template.charDataID = 256;
        member = 134;
    }
    picTemplate = Pokepic_GetTemplate(params->pokepics[params->battler]);
    *picTemplate = template;
    Pokepic_ScheduleReloadFromNarc(params->pokepics[params->battler]);
    sub_02014400((NarcId)picTemplate->narcID, picTemplate->charDataID, heapId, params->picData[params->battler]->unk0);
    params->picData[params->battler]->unk4 = template.narcID;
    params->picData[params->battler]->unk8 = template.palDataID;
    ReadWholeNarcMemberByIdPair(&height, NARC_poketool_pokegra_height_o, member);
    params->picData[params->battler]->unkC = height;
    y = ov07_02234B5C(params->battlerTypes[params->battler], 1) + params->picData[params->battler]->unkC;
    Pokepic_SetAttr(params->pokepics[params->battler], POKEPIC_Y, y);
    if (params->battlerTypes[params->battler] & BATTLER_TYPE_IS_ENEMY) {
        Pokepic_SetAttr(params->pokepics[params->battler], POKEPIC_SHADOW_SIZE, 1);
        Pokepic_SetAttr(params->pokepics[params->battler], POKEPIC_SHADOW_Y, y + (36 - height));
        Pokepic_SetAttr(params->pokepics[params->battler], POKEPIC_SHADOW_XOFFSET, 0);
        Pokepic_SetAttr(params->pokepics[params->battler], POKEPIC_SHADOW_YOFFSET, 36 - height);
        Pokepic_SetAttr(params->pokepics[params->battler], POKEPIC_SHADOW_H, 0);
    }
}

#pragma force_active reset

s16 ov07_02234B5C(u32 battlerType, u32 index) {
    if (index == 0) {
        return ov07_022377DC[battlerType][0];
    }
    return ov07_022377F4[battlerType][index];
}
