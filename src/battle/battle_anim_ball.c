#include "battle/battle_anim_ball.h"

#include "global.h"

#include "constants/battle.h"
#include "constants/gx.h"
#include "constants/pokemon.h"
#include "constants/sndseq.h"
#include "constants/species.h"

#include "battle/battle.h"

#include "camera.h"
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
#include "sprite_resource_header.h"
#include "char_tiles.h"
#include "seal_data.h"

fx32 FX_Sqrt(fx32 x); // not declared by the lib/include fx headers

// BallThrow (battle/battle.h) as this module uses it:
//   ballAnim         BALL_ANIM_* animation run by BallThrow_IsBallAnimationPlaying (sBallThrowBallAnimFuncs)
//   ballAnimStarted  set once that animation has entered its first capture state
//   step             step of the current mode / capture state
//   stateFrames      frame counter of the capture states, wobbleDir wobble direction, shakeCount shake count
//   captureState     capture state (sBallThrowCaptureStateFuncs)
//   modeRunning      TRUE while the mode handler runs
//   fadeAlphaA/B     blend alpha of the fade out, randomDir random direction (+1/-1)
//   animateSprite    the task ticks the sprite animation
//   opened           set when the thrown ball has opened
//   modeFrames       frame counter of the modes, arcHeight height of the throw arc
//   task             the update task, burstEffect the BallBurstEffect, paletteFlash the palette flash,
//                    battlerSprites the battler sprites
//   startDelay       frames the task waits before running the mode
//   params           the BallThrowParams it was created from: kind (start / target position, arc), mode
//                    (sBallThrowModeFuncs), resTagOffset resource tag offset, bgPriority sprite BG priority,
//                    keepSubSurface keep the renderer's sub surface
// Members its declaration still keeps as byte arrays:
#define BALL_ROTATION(data)     ((s32 *)&(data)->unk34[0x34 - 0x34])  // 0x34, value motion of the rotation
#define BALL_ARC_MOTION(data)   ((void *)&(data)->unk34[0x48 - 0x34]) // 0x48, linear part of the throw arc
#define BALL_ARC_MOTION2(data)  ((void *)&(data)->unk34[0x6C - 0x34]) // 0x6C, height part of the throw arc
#define BALL_X(data)            (*(s16 *)&(data)->unkB8[0xB8 - 0xB8]) // sprite position at the mode start
#define BALL_Y(data)            (*(s16 *)&(data)->unkB8[0xBA - 0xB8])
#define BALL_TARGET_X(data)     (*(s16 *)&(data)->unkB8[0xBC - 0xB8]) // end of the throw arc
#define BALL_TARGET_Y(data)     (*(s16 *)&(data)->unkB8[0xBE - 0xB8])
#define BALL_ARC_DURATION(data) (*(int *)&(data)->unkB8[0xC0 - 0xB8])

typedef BOOL (*BallThrowFunc)(BallThrow *data);

// Copy of battle/battle_anim_internal.h's definition (follow-up: include that header once it is on master)
typedef struct BattleAnimBattlerPicParams {
    int battler;
    int sourceBattler;
    MonPicData *picData[4];
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

typedef struct BallThrowSpinningKinds {
    u32 kinds[11];
} BallThrowSpinningKinds;

typedef struct BallThrowFlashingKinds {
    u32 kinds[6];
} BallThrowFlashingKinds;

typedef struct BallThrowStaticSpriteKinds {
    u32 kinds[3];
} BallThrowStaticSpriteKinds;

typedef struct BallThrowBounceSound {
    int frame;
    int seqNo;
} BallThrowBounceSound;

typedef struct BallSealEmitDelays {
    u8 delays[7];
} BallSealEmitDelays;

typedef struct BattleAnimEmitterCounts {
    u32 counts[22];
} BattleAnimEmitterCounts;

// Overlay 7 functions of other modules, declared with the types this file still uses (their own headers,
// battle/battle_anim_util.h and battle/battle_anim_internal.h, cannot be included next to the local copies above)
void BattleAnimSystem_InitBlend(void);
struct BattleAnimBattlerSprites *BattleAnimBattlerSprites_NewAll(BattleSystem *battleSystem, enum HeapID heapId);
void BattleAnimBattlerSprites_Free(struct BattleAnimBattlerSprites *sprites);
SPLEmitter *BattleAnimSystem_NewParticleSystem(enum HeapID heapId, int resId, BOOL loadNow);
SPLEmitter *BattleAnimSystem_NewParticleSystemFromNarc(enum HeapID heapId, NarcId narcId, int resId, BOOL loadNow);
SPLEmitter *BattleAnimSystem_NewParticleSystemFromOpenNarc(NARC *narc, enum HeapID heapId, int member, BOOL loadNow);
void BattleAnimSystem_FreeParticleSystem(SPLEmitter *particleSystem);
void BattleAnimUtil_LookupScreenPos(int set, int battlerType, s16 *x, s16 *y);
void BattleAnimUtil_InitLinearMotion(void *motion, s16 startX, s16 endX, s16 startY, s16 endY, int duration);
BOOL BattleAnimUtil_UpdateLinearMotionSprite(void *motion, ManagedSprite *sprite);
void BattleAnimUtil_InitLinearSinMotion(void *motion, void *motion2, s16 startX, s16 endX, s16 startY, s16 endY, u16 duration, fx32 height);
BOOL BattleAnimUtil_UpdateLinearSinMotionSprite(void *motion, void *motion2, ManagedSprite *sprite);
void BattleAnimUtil_InitValueMotion(s32 *motion, int start, int end, int duration);
BOOL BattleAnimUtil_UpdateValueMotion(s32 *motion);
void *BattleAnimUtil_StartPaletteBlend(PaletteData *paletteData, enum HeapID heapId, int bufferId, u16 offset, u16 count, s8 delay, s8 step, u8 start, u8 end, u16 color, u32 priority);
BOOL BattleAnimUtil_IsPaletteBlendActive(void *blend);
void BattleAnimUtil_FreePaletteBlend(void *blend);
void BattleAnimUtil_GetEmitterPositionKind0Alt(int battlerType, VecFx32 *pos, int unused, int perspective);

// asm/battle_anim_data.s
extern const BallThrowStaticSpriteKinds sBallThrowStaticSpriteKinds;
extern const BallThrowFlashingKinds sBallThrowFlashingKinds;
extern const BallThrowFunc sBallThrowModeFuncs[];
extern const BallThrowFunc sBallThrowBallAnimFuncs[];
extern const BallThrowBounceSound sBallThrowBounceSounds[];
extern const BallThrowSpinningKinds sBallThrowSpinningKinds;
extern const s16 sBallThrowShakeOffsets[];
extern const BallThrowFunc sBallThrowCaptureStateFuncs[];
extern const s16 gBallThrowBounceSteps[]; // bounce steps of 6 bytes: y offset, then (ov07_02237310) the animation frame
extern const u16 ov07_02237310[];
// Per ball (BallAnim_GetBallTableIndex) tables; the inner labels are offsets into the same rows
extern const u32 gBallAnimSealParticles[][2]; // seal particle resource
extern const u32 ov07_02237390[][2]; // seal particle emitter count
extern const u32 gBallAnimBurstParticles[][3]; // burst particle resource
extern const u32 ov07_02237470[][3]; // emitter skipped by the burst
extern const u32 ov07_02237474[][3]; // burst particle emitter count
extern const u32 sBallSpriteResources[][4];
extern const BallSealEmitDelays sBallSealEmitDelays;
extern const BattleAnimEmitterCounts sBattleAnimEmitterCounts;
extern const s16 gBattlerSlideInTargetX[][2];
extern const s16 gBattlerPokepicPositions[][3];

int BallAnim_GetBallTableIndex(int ball) {
    if (ball >= 1 && ball <= 24) {
        return ball - 1;
    }
    if (ball >= 1000 && ball <= 1027) {
        return ball - 1000;
    }
    return 3;
}

BOOL BallThrow_IsSpinningKind(u32 kind) {
    BallThrowSpinningKinds list = sBallThrowSpinningKinds;
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

BOOL BallThrow_IsFlashingKind(u32 kind) {
    BallThrowFlashingKinds list = sBallThrowFlashingKinds;
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

BOOL BallThrow_ShouldAnimateSprite(u32 kind) {
    BallThrowStaticSpriteKinds list = sBallThrowStaticSpriteKinds;
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
    return gBallAnimSealParticles[BallAnim_GetBallTableIndex(ball)][0];
}

u32 BallAnim_GetSealParticleEmitterCount(int ball) {
    return ov07_02237390[BallAnim_GetBallTableIndex(ball)][0];
}

u32 BallAnim_GetBurstParticleResource(int ball) {
    return gBallAnimBurstParticles[BallAnim_GetBallTableIndex(ball)][0];
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
                    resource0 = Seal_GetBattleAnimResId((u8)sealId);
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
                    resource1 = Seal_GetBattleAnimResId((u8)sealId);
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
                    resource2 = Seal_GetBattleAnimResId((u8)sealId);
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
                    resource3 = Seal_GetBattleAnimResId((u8)sealId);
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
        ParticleSystem_SetPerspectiveType(emitTask->particleSystem, CAMERA_PERSPECTIVE_TYPE_ORTHOGRAPHIC);
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
        ParticleSystem_SetPerspectiveType(effect->particleSystems[0], CAMERA_PERSPECTIVE_TYPE_ORTHOGRAPHIC);
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

    BattleAnimUtil_GetEmitterPositionKind0Alt(battlerType, pos, 0, 1);
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
    Seal_IsLetterSeal(SealOnCapsuleGetID(emitTask->seal));
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
    ParticleSystem_SetPerspectiveType(effect->particleSystem, CAMERA_PERSPECTIVE_TYPE_ORTHOGRAPHIC);
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

BOOL BallThrow_BallAnimThrow(BallThrow *data) {
    if (data->ballAnimStarted == 0) {
        data->ballAnimStarted++;
        BallThrow_SetCaptureState(data, 1);
    }
    if (BallThrow_IsCaptureState(data, 2) == TRUE) {
        return FALSE;
    }
    return TRUE;
}

BOOL BallThrow_BallAnimOpen(BallThrow *data) {
    if (data->ballAnimStarted == 0) {
        data->ballAnimStarted++;
        BallThrow_SetCaptureState(data, 3);
    }
    if (BallThrow_IsCaptureState(data, 4) == TRUE) {
        return FALSE;
    }
    return TRUE;
}

BOOL BallThrow_BallAnimDeflect(BallThrow *data) {
    if (data->ballAnimStarted == 0) {
        data->ballAnimStarted++;
        BallThrow_SetCaptureState(data, 5);
    }
    if (BallThrow_IsCaptureState(data, 6) == TRUE) {
        return FALSE;
    }
    return TRUE;
}

BOOL BallThrow_BallAnimFall(BallThrow *data) {
    if (data->ballAnimStarted == 0) {
        data->ballAnimStarted++;
        BallThrow_SetCaptureState(data, 7);
    }
    if (BallThrow_IsCaptureState(data, 14) == TRUE) {
        return FALSE;
    }
    return TRUE;
}

BOOL BallThrow_BallAnimShake(BallThrow *data) {
    if (data->ballAnimStarted == 0) {
        data->ballAnimStarted++;
        BallThrow_SetCaptureState(data, 15);
    }
    if (BallThrow_IsCaptureState(data, 18) == TRUE) {
        return FALSE;
    }
    return TRUE;
}

BOOL BallThrow_BallAnimSkip(BallThrow *data) {
    if (data->ballAnimStarted == 0) {
        data->ballAnimStarted++;
        BallThrow_SetCaptureState(data, 19);
    }
    if (BallThrow_IsCaptureState(data, 27) == TRUE) {
        return FALSE;
    }
    return TRUE;
}

BOOL BallThrow_BallAnimClick(BallThrow *data) {
    if (data->ballAnimStarted == 0) {
        data->ballAnimStarted++;
        BallThrow_SetCaptureState(data, 21);
    }
    if (BallThrow_IsCaptureState(data, 22) == TRUE) {
        return FALSE;
    }
    return TRUE;
}

BOOL BallThrow_BallAnimFade(BallThrow *data) {
    if (data->ballAnimStarted == 0) {
        data->ballAnimStarted++;
        BallThrow_SetCaptureState(data, 23);
    }
    if (BallThrow_IsCaptureState(data, 27) == TRUE) {
        return FALSE;
    }
    return TRUE;
}

void BallThrow_SetBallAnimation(BallThrow *data, s32 ballAnim) {
    data->ballAnim = ballAnim;
    data->ballAnimStarted = 0;
}

BOOL BallThrow_IsBallAnimationPlaying(BallThrow *data, s32 ballAnim_Unused) {
    return sBallThrowBallAnimFuncs[data->ballAnim](data);
}

void BallThrow_GoToCaptureState(BallThrow *data, int state) {
    data->captureState = state;
    data->step = 0;
    data->stateFrames = 0;
    data->wobbleDir = 0;
}

BOOL BallThrow_CaptureStateIdle(BallThrow *data) {
    return TRUE;
}

BOOL BallThrow_CaptureStateThrow(BallThrow *data) {
    if (BallThrow_RunThrowArc(data) == FALSE) {
        BallThrow_GoToCaptureState(data, 2);
    }
    return TRUE;
}

BOOL BallThrow_CaptureStateGoToOpen(BallThrow *data) {
    BallThrow_GoToCaptureState(data, 3);
    return TRUE;
}

BOOL BallThrow_CaptureStateOpen(BallThrow *data) {
    switch (data->step) {
    case 0: {
        ManagedSprite_SetAnim(data->managedSprite, 1);
        BallBurstEffectTemplate template;
        template.ball = data->params.ball;
        template.heapId = data->params.heapID;
        template.emitter = 0xFF;
        template.generic = FALSE;
        ManagedSprite_GetPositionXY(data->managedSprite, &template.x, &template.y);
        data->battlerSprites = (int)BattleAnimBattlerSprites_NewAll(data->params.battleSystem, data->params.heapID);
        data->burstEffect = (int)BallBurstEffect_New(&template);
        data->step++;
        break;
    }
    case 1:
        // compared as a signed int, as the original called it
        if (((int (*)(ManagedSprite *))ManagedSprite_GetAnimationFrame)(data->managedSprite) >= 2) {
            data->animateSprite = FALSE;
            data->step++;
        }
        break;
    case 2:
        BallBurstEffect_Start((BallBurstEffect *)data->burstEffect);
        BattleAnimBattlerSprites_Free((struct BattleAnimBattlerSprites *)data->battlerSprites);
        data->step++;
        break;
    case 3:
        if (BallBurstEffect_IsRunning((BallBurstEffect *)data->burstEffect) == FALSE) {
            ManagedSprite_SetAnimationFrame(data->managedSprite, 0);
            BallBurstEffect_Free((BallBurstEffect *)data->burstEffect);
            data->step++;
        }
        break;
    default:
        BallThrow_GoToCaptureState(data, 4);
        break;
    }
    return TRUE;
}

BOOL BallThrow_CaptureStateOpenDone(BallThrow *data) {
    return TRUE;
}

BOOL BallThrow_CaptureStateDeflect(BallThrow *data) {
    if (data->stateFrames == 0) {
        ManagedSprite_GetPositionXY(data->managedSprite, &BALL_X(data), &BALL_Y(data));
        BALL_TARGET_X(data) = 60;
        BALL_TARGET_Y(data) = 180;
        BALL_ARC_DURATION(data) = 10;
        data->arcHeight = 12;
        data->stateFrames++;
    } else if (BallThrow_RunThrowArc(data) == FALSE) {
        BallThrow_GoToCaptureState(data, 6);
    }
    return TRUE;
}

BOOL BallThrow_CaptureStateDeflectDone(BallThrow *data) {
    return TRUE;
}

BOOL BallThrow_CaptureStateStartFall(BallThrow *data) {
    BallThrow_GoToCaptureState(data, 8);
    return TRUE;
}

BOOL BallThrow_CaptureStateGoToFlash(BallThrow *data) {
    BallThrow_GoToCaptureState(data, 9);
    return TRUE;
}

BOOL BallThrow_CaptureStateFlash(BallThrow *data) {
    switch (data->step) {
    case 0: {
        int offset = ManagedSprite_GetPaletteOverrideOffset(data->managedSprite);
        PaletteData_BeginPaletteFade(data->params.paletteData, 1 << PLTTBUF_MAIN_OBJ, (u16)(1 << offset), -1, 0, 12, RGB(31, 27, 0));
        data->step++;
        break;
    }
    case 1:
        if (PaletteData_GetSelectedBuffersBitmask(data->params.paletteData) == 0) {
            int offset = ManagedSprite_GetPaletteOverrideOffset(data->managedSprite);
            PaletteData_BeginPaletteFade(data->params.paletteData, 1 << PLTTBUF_MAIN_OBJ, (u16)(1 << offset), -1, 12, 0, RGB(31, 27, 0));
            data->step++;
        }
        break;
    default:
        if (PaletteData_GetSelectedBuffersBitmask(data->params.paletteData) == 0) {
            BallThrow_GoToCaptureState(data, 10);
        }
        break;
    }
    return TRUE;
}

BOOL BallThrow_CaptureStateGoToDrop(BallThrow *data) {
    BallThrow_GoToCaptureState(data, 11);
    return TRUE;
}

BOOL BallThrow_CaptureStateDrop(BallThrow *data) {
    switch (data->step) {
    case 0:
        data->stateFrames++;
        if (data->stateFrames >= 1) {
            s16 x, y;
            ManagedSprite_GetPositionXY(data->managedSprite, &x, &y);
            BattleAnimUtil_InitLinearMotion(BALL_ARC_MOTION(data), x, x, y, (s16)(y + 32), 10);
            data->step++;
        }
        break;
    case 1:
        if (BattleAnimUtil_UpdateLinearMotionSprite(BALL_ARC_MOTION(data), data->managedSprite) == FALSE) {
            data->step++;
            BallThrow_GoToCaptureState(data, 13);
        }
        break;
    }
    return TRUE;
}

BOOL BallThrow_CaptureStateNone(BallThrow *data) {
    return TRUE;
}

BOOL BallThrow_CaptureStateBounce(BallThrow *data) {
    switch (data->step) {
    case 0:
        ManagedSprite_SetAnim(data->managedSprite, 1);
        ManagedSprite_SetAnimationFrame(data->managedSprite, 0);
        data->step++;
        // fallthrough
    case 1: {
        int index = data->stateFrames;
        int offset = index * 6;
        s16 dy = *(const s16 *)((const u8 *)gBallThrowBounceSteps + offset);
        int frame = *(const u16 *)((const u8 *)ov07_02237310 + offset);
        ManagedSprite_SetAnimationFrame(data->managedSprite, frame);
        data->stateFrames++;
        for (int i = 0; i < 5; i++) {
            if (data->stateFrames == sBallThrowBounceSounds[i].frame) {
                PlaySE_SetPan(sBallThrowBounceSounds[i].seqNo, 117);
            }
        }
        if (data->stateFrames > 20 || dy == 0xFF) {
            data->step++;
        } else {
            ManagedSprite_OffsetPositionXY(data->managedSprite, 0, dy);
        }
        break;
    }
    default:
        BallThrow_GoToCaptureState(data, 14);
        break;
    }
    return TRUE;
}

BOOL BallThrow_CaptureStateBounceDone(BallThrow *data) {
    return TRUE;
}

BOOL BallThrow_CaptureStateShakeWait(BallThrow *data) {
    int counter = data->stateFrames + 1;
    data->stateFrames = counter;
    if (counter >= 14) {
        data->step++;
        data->stateFrames = 0;
        BallThrow_GoToCaptureState(data, 16);
    }
    return TRUE;
}

BOOL BallThrow_CaptureStateGoToShake(BallThrow *data) {
    BallThrow_GoToCaptureState(data, 17);
    return TRUE;
}

BOOL BallThrow_CaptureStateShake(BallThrow *data) {
    switch (data->step) {
    case 0:
        if (data->shakeCount >= 3) {
            GF_AssertFail();
        }
        data->step++;
        // fallthrough
    case 1: {
        int shake = data->shakeCount;
        const s16 *offsets = (const s16 *)((const u8 *)sBallThrowShakeOffsets + shake * 24);
        int index = data->stateFrames;
        s16 dx = offsets[index];
        data->stateFrames++;
        if (data->stateFrames > 11 || dx == 0xFF) {
            data->step++;
            data->shakeCount++;
            ManagedSprite_SetAffineZRotation(data->managedSprite, 0);
            BallThrow_GoToCaptureState(data, 18);
            return TRUE;
        }
        if (data->stateFrames == 5) {
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

BOOL BallThrow_CaptureStateShakeDone(BallThrow *data) {
    return TRUE;
}

BOOL BallThrow_CaptureStateGoToSkip(BallThrow *data) {
    BallThrow_GoToCaptureState(data, 20);
    return TRUE;
}

BOOL BallThrow_CaptureStateSkip(BallThrow *data) {
    BallThrow_GoToCaptureState(data, 27);
    return TRUE;
}

BOOL BallThrow_CaptureStateClick(BallThrow *data) {
    switch (data->step) {
    case 0: {
        int offset = ManagedSprite_GetPaletteOverrideOffset(data->managedSprite);
        PaletteData_BeginPaletteFade(data->params.paletteData, 1 << PLTTBUF_MAIN_OBJ, (u16)(1 << offset), -5, 0, 10, RGB_BLACK);
        data->step++;
        break;
    }
    case 1:
        if (PaletteData_GetSelectedBuffersBitmask(data->params.paletteData) == 0) {
            BallBurstEffectTemplate template;
            template.ball = data->params.ball;
            template.heapId = data->params.heapID;
            template.emitter = BallAnim_GetBurstSkippedEmitter(template.ball);
            template.generic = FALSE;
            ManagedSprite_GetPositionXY(data->managedSprite, &template.x, &template.y);
            data->burstEffect = (int)BallBurstEffect_New(&template);
            data->step++;
        }
        break;
    case 2:
        BallBurstEffect_Start((BallBurstEffect *)data->burstEffect);
        data->step++;
        break;
    case 3:
        if (BallBurstEffect_IsRunning((BallBurstEffect *)data->burstEffect) == FALSE) {
            ManagedSprite_SetAnimationFrame(data->managedSprite, 0);
            BallBurstEffect_Free((BallBurstEffect *)data->burstEffect);
            data->step++;
        }
        break;
    default:
        if (PaletteData_GetSelectedBuffersBitmask(data->params.paletteData) == 0) {
            BallThrow_GoToCaptureState(data, 22);
        }
        break;
    }
    return TRUE;
}

BOOL BallThrow_CaptureStateClickDone(BallThrow *data) {
    return TRUE;
}

BOOL BallThrow_CaptureStateRestorePalette(BallThrow *data) {
    switch (data->step) {
    case 0: {
        int offset = ManagedSprite_GetPaletteOverrideOffset(data->managedSprite);
        PaletteData_BeginPaletteFade(data->params.paletteData, 1 << PLTTBUF_MAIN_OBJ, (u16)(1 << offset), -5, 10, 0, RGB_BLACK);
        data->step++;
        break;
    }
    case 1:
        if (PaletteData_GetSelectedBuffersBitmask(data->params.paletteData) == 0) {
            data->step++;
        }
        break;
    default:
        BallThrow_GoToCaptureState(data, 24);
        break;
    }
    return TRUE;
}

BOOL BallThrow_CaptureStateGoToFadeOut(BallThrow *data) {
    BallThrow_GoToCaptureState(data, 25);
    return TRUE;
}

BOOL BallThrow_CaptureStateFadeOut(BallThrow *data) {
    switch (data->step) {
    case 0:
        ManagedSprite_SetOamMode(data->managedSprite, GX_OAM_MODE_XLU);
        data->step++;
        // fallthrough
    case 1:
        if (data->fadeAlphaA != 0) {
            data->fadeAlphaA--;
            data->fadeAlphaB++;
        } else {
            data->fadeAlphaA = 0;
            data->fadeAlphaB = 15;
            ManagedSprite_SetDrawFlag(data->managedSprite, FALSE);
            data->step++;
        }
        reg_G2_BLDALPHA = data->fadeAlphaA | (data->fadeAlphaB << 8);
        break;
    default:
        BallThrow_GoToCaptureState(data, 26);
        break;
    }
    return TRUE;
}

BOOL BallThrow_CaptureStateGoToEnd(BallThrow *data) {
    BallThrow_GoToCaptureState(data, 27);
    return TRUE;
}

BOOL BallThrow_CaptureStateEnd(BallThrow *data) {
    return FALSE;
}

BOOL BallThrow_RunCaptureState(BallThrow *data) {
    return sBallThrowCaptureStateFuncs[data->captureState](data);
}

BOOL BallThrow_RunThrowArc(BallThrow *data) {
    switch (data->step) {
    case 0:
        BattleAnimUtil_InitLinearSinMotion(BALL_ARC_MOTION(data), BALL_ARC_MOTION2(data), BALL_X(data), BALL_TARGET_X(data), BALL_Y(data), BALL_TARGET_Y(data), BALL_ARC_DURATION(data), data->arcHeight << FX32_SHIFT);
        data->modeFrames = 0;
        data->step++;
        data->paletteFlash = 0;
        if (BallThrow_IsSpinningKind(data->params.kind) == TRUE) {
            ManagedSprite_OffsetAffineZRotation(data->managedSprite, (LCRandom() % 20 + 10) << 13);
        }
        break;
    case 1:
        if (BallThrow_IsSpinningKind(data->params.kind) == TRUE) {
            ManagedSprite_OffsetAffineZRotation(data->managedSprite, 2 << 12);
            if (data->modeFrames > BALL_ARC_DURATION(data) / 2 + 10) {
                ManagedSprite_OffsetAffineZRotation(data->managedSprite, 2 << 12);
            }
            if (BallThrow_IsFlashingKind(data->params.kind) == TRUE && data->modeFrames == BALL_ARC_DURATION(data) / 2 + 10) {
                data->paletteFlash = (int)BattleAnimUtil_StartPaletteBlend(data->params.paletteData, data->params.heapID, 2, (u16)(ManagedSprite_GetPaletteOverrideOffset(data->managedSprite) << 4), 16, -2, 2, 0, 14, 0xFFFF, 1002);
            }
        }
        data->modeFrames++;
        if (data->params.kind < 6 || data->params.kind > 8 || data->modeFrames <= BALL_ARC_DURATION(data) / 2 || data->modeFrames >= BALL_ARC_DURATION(data) / 2 + 5) {
            if (BattleAnimUtil_UpdateLinearSinMotionSprite(BALL_ARC_MOTION(data), BALL_ARC_MOTION2(data), data->managedSprite) == FALSE) {
                BallThrow_SetRotation(data, 0);
                data->step++;
            }
        }
        break;
    case 2:
        if (BallThrow_IsFlashingKind(data->params.kind) == TRUE && data->paletteFlash != 0) {
            if (BattleAnimUtil_IsPaletteBlendActive((void *)data->paletteFlash) == TRUE) {
                break;
            }
            BattleAnimUtil_FreePaletteBlend((void *)data->paletteFlash);
            data->opened = TRUE;
            return FALSE;
        }
        if (data->params.kind >= 15) {
            data->opened = TRUE;
            return FALSE;
        }
        if (data->params.ball == 1025) {
            PlaySE_SetPan(SEQ_SE_DP_W202B, 117);
        }
        data->step++;
        break;
    case 3:
        ManagedSprite_SetAnim(data->managedSprite, 1);
        BallThrow_SetAnimateSprite(data, TRUE);
        data->modeFrames = 0;
        data->step++;
        break;
    default:
        if (data->modeFrames == 5) {
            if (data->params.kind != 12 && data->params.kind != 13) {
                data->paletteFlash = (int)BattleAnimUtil_StartPaletteBlend(data->params.paletteData, data->params.heapID, 2, (u16)(ManagedSprite_GetPaletteOverrideOffset(data->managedSprite) << 4), 16, -2, 2, 0, 14, 0xFFFF, 1002);
            }
            data->opened = TRUE;
        }
        data->modeFrames++;
        if (data->modeFrames > 15) {
            if (data->params.kind != 12 && data->params.kind != 13) {
                if (BattleAnimUtil_IsPaletteBlendActive((void *)data->paletteFlash) == TRUE) {
                    break;
                }
                BattleAnimUtil_FreePaletteBlend((void *)data->paletteFlash);
            }
            return FALSE;
        }
        break;
    }
    return TRUE;
}

BOOL BallThrow_ThrowArcAndOpen(BallThrow *data) {
    switch (data->step) {
    case 0:
        BattleAnimUtil_InitLinearSinMotion(BALL_ARC_MOTION(data), BALL_ARC_MOTION2(data), BALL_X(data), BALL_TARGET_X(data), BALL_Y(data), BALL_TARGET_Y(data), BALL_ARC_DURATION(data), data->arcHeight << FX32_SHIFT);
        data->modeFrames = 0;
        data->step++;
        data->paletteFlash = 0;
        if (BallThrow_IsSpinningKind(data->params.kind) == TRUE) {
            ManagedSprite_OffsetAffineZRotation(data->managedSprite, (LCRandom() % 20 + 10) << 13);
        }
        break;
    case 1:
        if (BallThrow_IsSpinningKind(data->params.kind) == TRUE) {
            ManagedSprite_OffsetAffineZRotation(data->managedSprite, 2 << 12);
            if (data->modeFrames > BALL_ARC_DURATION(data) / 2 + 10) {
                ManagedSprite_OffsetAffineZRotation(data->managedSprite, 2 << 12);
            }
            if (BallThrow_IsFlashingKind(data->params.kind) == TRUE && data->modeFrames == BALL_ARC_DURATION(data) / 2 + 10) {
                data->paletteFlash = (int)BattleAnimUtil_StartPaletteBlend(data->params.paletteData, data->params.heapID, 2, (u16)(ManagedSprite_GetPaletteOverrideOffset(data->managedSprite) << 4), 16, -2, 2, 0, 14, 0xFFFF, 1002);
            }
        }
        data->modeFrames++;
        if (data->params.kind < 6 || data->params.kind > 8 || data->modeFrames <= BALL_ARC_DURATION(data) / 2 || data->modeFrames >= BALL_ARC_DURATION(data) / 2 + 5) {
            if (BattleAnimUtil_UpdateLinearSinMotionSprite(BALL_ARC_MOTION(data), BALL_ARC_MOTION2(data), data->managedSprite) == FALSE) {
                BallThrow_SetRotation(data, 0);
                data->step++;
            }
        }
        break;
    case 2:
        if (BallThrow_IsFlashingKind(data->params.kind) == TRUE && data->paletteFlash != 0) {
            if (BattleAnimUtil_IsPaletteBlendActive((void *)data->paletteFlash) == TRUE) {
                break;
            }
            BattleAnimUtil_FreePaletteBlend((void *)data->paletteFlash);
            data->opened = TRUE;
            return FALSE;
        }
        if (data->params.kind >= 15) {
            data->opened = TRUE;
            return FALSE;
        }
        data->step++;
        break;
    case 3:
        ManagedSprite_SetAnim(data->managedSprite, 1);
        BallThrow_SetAnimateSprite(data, FALSE);
        data->modeFrames = 0;
        data->step++;
        break;
    default:
        if (data->modeFrames == 5) {
            ManagedSprite_SetAnimationFrame(data->managedSprite, 2);
            data->paletteFlash = (int)BattleAnimUtil_StartPaletteBlend(data->params.paletteData, data->params.heapID, 2, (u16)(ManagedSprite_GetPaletteOverrideOffset(data->managedSprite) << 4), 16, -2, 2, 0, 14, 0xFFFF, 1002);
            data->opened = TRUE;
        }
        data->modeFrames++;
        if (data->modeFrames > 15) {
            if (BattleAnimUtil_IsPaletteBlendActive((void *)data->paletteFlash) != TRUE) {
                BattleAnimUtil_FreePaletteBlend((void *)data->paletteFlash);
                return FALSE;
            }
        }
        break;
    }
    return TRUE;
}

BOOL BallThrow_OpenInPlace(BallThrow *data) {
    BallBurstEffectTemplate template;

    switch (data->step) {
    case 0:
        BallThrow_SetAnimateSprite(data, FALSE);
        ManagedSprite_SetAnim(data->managedSprite, 1);
        data->modeFrames++;
        if (data->modeFrames >= 2) {
            ManagedSprite_SetAnimationFrame(data->managedSprite, 0);
            data->modeFrames = 0;
            data->step++;
        }
        break;
    case 1:
        data->modeFrames++;
        if (data->modeFrames >= 2) {
            ManagedSprite_SetAnimationFrame(data->managedSprite, 2);
            data->modeFrames = 0;
            data->step++;
        }
        break;
    case 2:
        template.ball = data->params.ball;
        template.heapId = data->params.heapID;
        template.emitter = 0xFF;
        template.generic = TRUE;
        ManagedSprite_GetPositionXY(data->managedSprite, &template.x, &template.y);
        data->burstEffect = (int)BallBurstEffect_New(&template);
        data->step++;
        break;
    case 3:
        BallBurstEffect_Start((BallBurstEffect *)data->burstEffect);
        data->step++;
        break;
    case 4:
        data->modeFrames++;
        if (data->modeFrames >= 6) {
            ManagedSprite_SetAnimationFrame(data->managedSprite, 2);
            data->modeFrames = 0;
            data->step++;
        }
        break;
    case 5:
        data->modeFrames++;
        if (data->modeFrames >= 6) {
            ManagedSprite_SetAnimationFrame(data->managedSprite, 1);
            data->modeFrames = 0;
            data->step++;
        }
        break;
    case 6:
        data->modeFrames++;
        if (data->modeFrames >= 6) {
            ManagedSprite_SetAnimationFrame(data->managedSprite, 0);
            data->modeFrames = 0;
            data->step++;
        }
        break;
    case 7:
        if (!BallBurstEffect_IsRunning((BallBurstEffect *)data->burstEffect)) {
            BallBurstEffect_Free((BallBurstEffect *)data->burstEffect);
            data->step++;
        }
        break;
    default:
        return FALSE;
    }
    return TRUE;
}

BOOL BallThrow_Wobble(BallThrow *data) {
    switch (data->step) {
    case 0:
        if (data->wobbleDir == 0) {
            BattleAnimUtil_InitValueMotion(BALL_ROTATION(data), -0x1FFF, 0x1FFF, 10);
        } else {
            BattleAnimUtil_InitValueMotion(BALL_ROTATION(data), 0x1FFF, -0x1FFF, 10);
        }
        data->wobbleDir ^= 1;
        data->step++;
        break;
    case 1:
        ManagedSprite_SetAffineZRotation(data->managedSprite, *BALL_ROTATION(data));
        if (!BattleAnimUtil_UpdateValueMotion(BALL_ROTATION(data))) {
            if (data->stateFrames >= 1) {
                data->step++;
            } else {
                data->stateFrames++;
                data->step--;
            }
        }
        break;
    default:
        data->modeFrames = 0;
        return FALSE;
    }
    return TRUE;
}

BOOL BallThrow_TrackPosition(BallThrow *data) {
    s16 x;
    s16 y;

    if (data->step == 0xFF) {
        return FALSE;
    }
    if (data->managedSprite != NULL) {
        ManagedSprite_GetPositionXY(data->managedSprite, &x, &y);
        BALL_X(data) = x;
        BALL_Y(data) = y;
    }
    return TRUE;
}

void BallThrow_Task(SysTask *task, void *data) {
    BallThrow *ballData = data;
    if (ballData->modeRunning) {
        if (ballData->startDelay > 0) {
            ballData->startDelay--;
            SpriteSystem_DrawSprites(ballData->spriteManager);
            return;
        }
        if (!sBallThrowModeFuncs[ballData->params.mode](ballData)) {
            ballData->modeRunning = FALSE;
        }
        if (ballData->animateSprite == TRUE) {
            ManagedSprite_TickFrame(ballData->managedSprite);
        }
        SpriteSystem_DrawSprites(ballData->spriteManager);
    }
}

BallThrow *BallThrow_New(BallThrowParams *unkStruct_134) {
    BallThrow *data = Heap_Alloc(unkStruct_134->heapID, sizeof(BallThrow));
    MI_CpuFill8(data, 0, sizeof(BallThrow));
    GF_ASSERT(data != NULL);
    data->params = *unkStruct_134;
    data->step = 0;
    data->wobbleDir = 0;
    data->stateFrames = 0;
    data->captureState = 0;
    data->shakeCount = 0;
    data->ballAnim = 0;
    data->ballAnimStarted = 0;
    data->spriteManager = SpriteManager_New(data->params.spriteSystem);
    data->startDelay = 0;
    data->animateSprite = BallThrow_ShouldAnimateSprite(data->params.kind);
    data->modeFrames = 0;
    data->fadeAlphaA = 16;
    data->fadeAlphaB = 0;
    BattleAnimSystem_InitBlend();
    if (LCRandom() % 2) {
        data->randomDir = 1;
    } else {
        data->randomDir = -1;
    }
    BallThrow_LoadResources(data);
    BallThrow_CreateSprite(data);
    BallThrow_InitThrowTarget(data);
    data->modeRunning = TRUE;
    data->opened = FALSE;
    data->task = SysTask_CreateOnMainQueue(BallThrow_Task, data, 1000);
    return data;
}

BOOL BallThrow_IsActive(BallThrow *data) {
    GF_ASSERT(data != NULL);
    if (data->modeRunning) {
        return TRUE;
    }
    return FALSE;
}

BOOL BallThrow_HasOpened(BallThrow *data) {
    GF_ASSERT(data != NULL);
    if (data->opened) {
        return TRUE;
    }
    return FALSE;
}

void BallThrow_SetCaptureState(BallThrow *data, int state) {
    data->captureState = state;
}

BOOL BallThrow_IsCaptureState(BallThrow *data, int state) {
    if (data->captureState == state) {
        return TRUE;
    }
    return FALSE;
}

void BallThrow_Delete(BallThrow *data) {
    GF_ASSERT(data != NULL);
    SpriteSystem_FreeResourcesAndManager(data->params.spriteSystem, data->spriteManager);
    Sprite_DeleteAndFreeResources(data->managedSprite);
    SysTask_Destroy(data->task);
    Heap_Free(data);
}

void BallThrow_SetMode(BallThrow *data, int mode) {
    data->params.mode = mode;
    data->step = 0;
    data->stateFrames = 0;
    data->wobbleDir = 0;
    data->modeRunning = TRUE;
    ManagedSprite_GetPositionXY(data->managedSprite, &BALL_X(data), &BALL_Y(data));
}

s32 BallThrow_GetMode(BallThrow *data) {
    GF_ASSERT(data != NULL);
    return data->params.mode;
}

void BallThrow_InitThrowTarget(BallThrow *data) {
    ManagedSprite_GetPositionXY(data->managedSprite, &BALL_X(data), &BALL_Y(data));
    switch (data->params.kind) {
    case 0:
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
        ManagedSprite_GetPositionXY(data->managedSprite, &BALL_TARGET_X(data), &BALL_TARGET_Y(data));
        data->arcHeight = 0;
        BALL_ARC_DURATION(data) = 12;
        return;
    case 6:
        BattleAnimUtil_LookupScreenPos(0, 0, &BALL_TARGET_X(data), &BALL_TARGET_Y(data));
        data->arcHeight = 48;
        BALL_TARGET_Y(data) = BALL_TARGET_Y(data) + 32;
        break;
    case 7:
        ManagedSprite_GetPositionXY(data->managedSprite, &BALL_X(data), &BALL_Y(data));
        BattleAnimUtil_LookupScreenPos(1, 4, &BALL_TARGET_X(data), &BALL_TARGET_Y(data));
        data->arcHeight = 48;
        BALL_TARGET_Y(data) = BALL_TARGET_Y(data) + 32;
        break;
    case 8:
        BattleAnimUtil_LookupScreenPos(1, 2, &BALL_TARGET_X(data), &BALL_TARGET_Y(data));
        data->arcHeight = 48;
        BALL_TARGET_Y(data) = BALL_TARGET_Y(data) + 32;
        break;
    case 9:
        BattleAnimUtil_LookupScreenPos(0, 1, &BALL_TARGET_X(data), &BALL_TARGET_Y(data));
        data->arcHeight = 48;
        BALL_TARGET_Y(data) = BALL_TARGET_Y(data) + 32;
        break;
    case 10:
        BattleAnimUtil_LookupScreenPos(1, 3, &BALL_TARGET_X(data), &BALL_TARGET_Y(data));
        data->arcHeight = 48;
        BALL_TARGET_Y(data) = BALL_TARGET_Y(data) + 32;
        break;
    case 11:
        BattleAnimUtil_LookupScreenPos(1, 5, &BALL_TARGET_X(data), &BALL_TARGET_Y(data));
        data->arcHeight = 48;
        BALL_TARGET_Y(data) = BALL_TARGET_Y(data) + 32;
        break;
    case 12:
        BattleAnimUtil_LookupScreenPos(0, 1, &BALL_TARGET_X(data), &BALL_TARGET_Y(data));
        data->arcHeight = 16;
        BALL_TARGET_Y(data) = BALL_TARGET_Y(data) + 16;
        BALL_ARC_DURATION(data) = 12;
        return;
    case 13:
        BattleAnimUtil_LookupScreenPos(0, 1, &BALL_TARGET_X(data), &BALL_TARGET_Y(data));
        data->arcHeight = 32;
        BALL_TARGET_X(data) = BALL_TARGET_X(data) - 20;
        BALL_TARGET_Y(data) = BALL_TARGET_Y(data) + 38;
        BALL_ARC_DURATION(data) = 16;
        return;
    case 14:
        BALL_TARGET_X(data) = 128;
        BALL_TARGET_Y(data) = 64;
        data->arcHeight = 32;
        BALL_TARGET_Y(data) = BALL_TARGET_Y(data) + 32;
        break;
    case 15:
        BattleAnimUtil_LookupScreenPos(0, 1, &BALL_TARGET_X(data), &BALL_TARGET_Y(data));
        data->arcHeight = 64;
        BALL_TARGET_Y(data) = BALL_TARGET_Y(data) + 8;
        BALL_ARC_DURATION(data) = 16;
        return;
    case 16:
        BattleAnimUtil_LookupScreenPos(1, 3, &BALL_TARGET_X(data), &BALL_TARGET_Y(data));
        data->arcHeight = 64;
        BALL_TARGET_Y(data) = BALL_TARGET_Y(data) + 8;
        BALL_ARC_DURATION(data) = 16;
        return;
    case 17:
        BattleAnimUtil_LookupScreenPos(1, 5, &BALL_TARGET_X(data), &BALL_TARGET_Y(data));
        data->arcHeight = 64;
        BALL_TARGET_Y(data) = BALL_TARGET_Y(data) + 8;
        BALL_ARC_DURATION(data) = 16;
        return;
    }
    BALL_ARC_DURATION(data) = 20;
}

void BallThrow_GetStartPosition(BallThrow *data, s16 *x, s16 *y) {
    switch (data->params.kind) {
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

void BallThrow_LoadResources(BallThrow *data) {
    SpriteResourceCountsListUnion counts;
    NARC *narc;
    int charRes;
    int plttRes;
    int cellRes;
    int animRes;
    int i;

    SpriteSystem_InitSprites(data->params.spriteSystem, data->spriteManager, 10);
    if (data->params.keepSubSurface == 0) {
        G2dRenderer_SetSubSurfaceCoords(SpriteSystem_GetRenderer(data->params.spriteSystem), 0, FX32_CONST(272));
    }
    for (i = 0; i < GF_GFX_RES_TYPE_MAX; i++) {
        counts.asArray[i] = 10;
    }
    counts.numMcel = 0;
    counts.numManm = 0;
    SpriteSystem_InitManagerWithCapacities(data->params.spriteSystem, data->spriteManager, &counts);
    charRes = BallAnim_GetSpriteResource(data->params.ball, 0);
    plttRes = BallAnim_GetSpriteResource(data->params.ball, 1);
    cellRes = BallAnim_GetSpriteResource(data->params.ball, 2);
    animRes = BallAnim_GetSpriteResource(data->params.ball, 3);
    narc = NARC_New(NARC_a_0_0_8, data->params.heapID);
    SpriteSystem_LoadCharResObjFromOpenNarc(data->params.spriteSystem, data->spriteManager, narc, charRes, TRUE, NNS_G2D_VRAM_TYPE_2DMAIN, data->params.resTagOffset + 6000);
    SpriteSystem_LoadPaletteBufferFromOpenNarc(data->params.paletteData, PLTTBUF_MAIN_OBJ, data->params.spriteSystem, data->spriteManager, narc, plttRes, FALSE, 1, NNS_G2D_VRAM_TYPE_2DMAIN, data->params.resTagOffset + 6000);
    SpriteSystem_LoadCellResObjFromOpenNarc(data->params.spriteSystem, data->spriteManager, narc, cellRes, TRUE, data->params.resTagOffset + 6000);
    SpriteSystem_LoadAnimResObjFromOpenNarc(data->params.spriteSystem, data->spriteManager, narc, animRes, TRUE, data->params.resTagOffset + 6000);
    NARC_Delete(narc);
}

void BallThrow_CreateSprite(BallThrow *data) {
    ManagedSpriteTemplate template;
    int i;

    BallThrow_GetStartPosition(data, &template.x, &template.y);
    template.z = 0;
    template.animation = 0;
    template.drawPriority = 0;
    template.pal = 0;
    template.vram = NNS_G2D_VRAM_TYPE_2DMAIN;
    template.bgPriority = data->params.bgPriority;
    template.vramTransfer = FALSE;
    for (i = 0; i < GF_GFX_RES_TYPE_MAX; i++) {
        template.resIdList[i] = data->params.resTagOffset + 6000;
    }
    data->managedSprite = SpriteSystem_NewSprite(data->params.spriteSystem, data->spriteManager, &template);
    ManagedSprite_SetDrawFlag(data->managedSprite, TRUE);
    ManagedSprite_SetAffineOverwriteMode(data->managedSprite, NNS_G2D_RND_AFFINE_OVERWRITE_DOUBLE);
    ManagedSprite_SetAnimationFrame(data->managedSprite, 0);
    ManagedSprite_SetAnim(data->managedSprite, 0);
    ManagedSprite_TickFrame(data->managedSprite);
    BattleAnimSystem_InitBlend();
}

void BallThrow_SetDrawFlag(BallThrow *data, int flag) {
    ManagedSprite_SetDrawFlag(data->managedSprite, flag);
}

void BallThrow_SetPosition(BallThrow *data, s16 x, s16 y) {
    ManagedSprite_SetPositionXY(data->managedSprite, x, y);
}

void BallThrow_SetRotation(BallThrow *data, u16 rotation) {
    ManagedSprite_SetAffineZRotation(data->managedSprite, rotation);
}

void BallThrow_SetAnimateSprite(BallThrow *data, BOOL animate) {
    data->animateSprite = animate;
}

void BallThrow_SetDrawPriority(BallThrow *data, u16 priority) {
    ManagedSprite_SetDrawPriority(data->managedSprite, priority);
}

void BallThrow_SetPriority(BallThrow *data, int priority) {
    ManagedSprite_SetPriority(data->managedSprite, priority);
}

void BallThrow_SetDelay(BallThrow *data, int delay) {
    data->startDelay = delay;
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

    if (!Seal_IsLetterSeal((u8)SealOnCapsuleGetID(seal))) {
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
    BattleAnimEmitter_CreateEmitters(emitter, (void (*)(SPLEmitter *))BattleAnimEmitter_GetEmitterParam);
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

void *BattleAnimEmitter_GetEmitterParam(void) {
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
    CharTiles_LoadMonPicAsObjFromNarc((NarcId)picTemplate->narcID, picTemplate->charDataID, heapId, params->picData[params->battler]->charBuffer, personality, FALSE, facing, picTemplate->species);
    params->picData[params->battler]->narcId = picTemplate->narcID;
    params->picData[params->battler]->paletteId = picTemplate->palDataID;
    params->picData[params->battler]->height = GetMonPicHeightBySpeciesGenderForm(species, gender, facing, form, personality);
    {
        s8 height = params->picData[params->battler]->height;
        y = BattleAnimUtil_GetBattlerTypeCoord(params->battlerTypes[params->battler], 1);
        Pokepic_SetAttr(params->pokepics[params->battler], POKEPIC_Y, y + height);
        if (facing == 2) {
            NARC *narc = NARC_New(NARC_a_1_8_0, heapId);
            // the shadow readers' fourth parameter is not passed
            ((void (*)(NARC *, s8 *, u16))NARC_ReadPokepicShadowHeight)(narc, &shadow.shadowHeight, species);
            ((void (*)(NARC *, s8 *, u16))NARC_ReadPokepicShadowXOffset)(narc, &shadow.shadowXOffset, species);
            ((void (*)(NARC *, u8 *, u16))NARC_ReadPokepicShadowSize)(narc, &shadow.shadowSize, species);
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
    CharTiles_LoadMonPicAsObjFromNarc((NarcId)picTemplate->narcID, picTemplate->charDataID, heapId, params->picData[params->battler]->charBuffer, personality, FALSE, facing, picTemplate->species);
    params->picData[params->battler]->narcId = picTemplate->narcID;
    params->picData[params->battler]->paletteId = picTemplate->palDataID;
    params->picData[params->battler]->height = GetMonPicHeightBySpeciesGenderForm((u16)species, (u8)gender, (u8)facing, (u8)form, personality);
    int battler = params->battler;
    u32 slot = battler * sizeof(Pokepic *); // byte offset of the battler's pointers, kept from before the call
    int offset = BattleAnimUtil_GetBattlerTypeCoord(params->battlerTypes[battler], 1);
    int height = (*(MonPicData **)((u8 *)params->picData + slot))->height;
    Pokepic_SetAttr(*(Pokepic **)((u8 *)params->pokepics + slot), POKEPIC_Y, height + offset);
}

#pragma pop
#pragma force_active reset

// Loads a fixed graphic of the otherpoke NARC (not a species) into the battler's pic, front or back by its battler type
#pragma force_active on

void BattleAnimUtil_LoadSubstituteIntoPic(BattleAnimBattlerPicParams *params, enum HeapID heapId) {
    PokepicTemplate template;
    PokepicTemplate *picTemplate;
    u8 height;
    int member, y;

    Pokepic_Push(params->pokepics[params->battler]);
    template.narcID = NARC_poketool_pokegra_otherpoke;
    template.palDataID = 258;
    template.species = SPECIES_NONE;
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
    CharTiles_LoadPicAsObjFromNarc((NarcId)picTemplate->narcID, picTemplate->charDataID, heapId, params->picData[params->battler]->charBuffer);
    params->picData[params->battler]->narcId = template.narcID;
    params->picData[params->battler]->paletteId = template.palDataID;
    ReadWholeNarcMemberByIdPair(&height, NARC_poketool_pokegra_height_o, member);
    params->picData[params->battler]->height = height;
    y = BattleAnimUtil_GetBattlerTypeCoord(params->battlerTypes[params->battler], 1) + params->picData[params->battler]->height;
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

s16 BattleAnimUtil_GetBattlerTypeCoord(u32 battlerType, u32 index) {
    if (index == 0) {
        return gBattlerSlideInTargetX[battlerType][0];
    }
    return gBattlerPokepicPositions[battlerType][index];
}
