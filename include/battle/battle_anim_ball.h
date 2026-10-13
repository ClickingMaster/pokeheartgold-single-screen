#ifndef POKEHEARTGOLD_BATTLE_BATTLE_ANIM_BALL_H
#define POKEHEARTGOLD_BATTLE_BATTLE_ANIM_BALL_H

// Overlay 7: Poke Ball throw / capture animation (UnkBallData), the ball capsule seal and ball burst
// particles, a generic particle emitter manager and the battler pic graphic swap helpers.

#include "battle/battle.h"

#include "filesystem.h"
#include "overlay_07.h"
#include "pokemon.h"
#include "pokepic.h"
#include "seal_case.h"
#include "spl.h"
#include "sys_task_api.h"

// Input of BallSealEffect_New: the particles shown when a Pokemon comes out of its ball
typedef struct BallSealEffectTemplate {
    int battlerType;
    int ball; // used instead of the Pokemon's own ball when nonzero
    Pokemon *mon;
} BallSealEffectTemplate;

// Input of BallBurstEffect_New: the particles of a ball bursting open at a screen position
typedef struct BallBurstEffectTemplate {
    s16 x;
    s16 y;
    int ball;
    enum HeapID heapId;
    int emitter;  // 0xFF: all emitters of the ball's burst (see BallBurstEffect_Start)
    BOOL generic; // the burst of ball 1027 with all its emitters
} BallBurstEffectTemplate;

typedef struct BattleAnimEmitterTemplate {
    int resourceId;  // particle resource of overlay 7 (5..26)
    int perspective; // second argument of ParticleSystem_SetPerspectiveType
} BattleAnimEmitterTemplate;

// The battler pictures handed to the pic swap helpers (BattleAnimBattlerPicParams; its definition is in
// battle/battle_anim_internal.h, src/battle/battle_anim_ball.c keeps a copy until it includes that header)
struct BattleAnimBattlerPicParams;

typedef struct BallSealEffect BallSealEffect;
typedef struct BallBurstEffect BallBurstEffect;
typedef struct BattleAnimEmitter BattleAnimEmitter;
typedef struct BattleAnimEmitterManager BattleAnimEmitterManager;

int BallAnim_GetBallTableIndex(int ball);
BOOL UnkBallData_IsSpinningKind(u32 kind);
BOOL UnkBallData_IsFlashingKind(u32 kind);
BOOL UnkBallData_ShouldAnimateSprite(u32 kind);
u32 BallAnim_GetSealParticleResource(int ball);
u32 BallAnim_GetSealParticleEmitterCount(int ball);
u32 BallAnim_GetBurstParticleResource(int ball);
u32 BallAnim_GetBurstParticleEmitterCount(int ball);
u32 BallAnim_GetBurstSkippedEmitter(int ball);
u32 BallAnim_GetSpriteResource(int ball, int index);
BallSealEffect *BallSealEffect_Alloc(enum HeapID heapId);
BallSealEffect *BallSealEffect_New(enum HeapID heapId, const BallSealEffectTemplate *template);
void BallSealEffect_ReadMon(BallSealEffect *effect, const BallSealEffectTemplate *template);
void BallSealEffect_LoadSealParticlesTask(SysTask *task, void *data);
void BallSealEffect_LoadResources(BallSealEffect *effect);
BOOL BallSealEffect_IsLoaded(BallSealEffect *effect);
void BallSealEffect_StartWithCallback(BallSealEffect *effect, void (*callback)(SPLEmitter *));
void BallSealEffect_Start(BallSealEffect *effect);
BOOL BallSealEffect_IsActive(BallSealEffect *effect);
void BallSealEffect_WaitTask(SysTask *task, void *data);
void BallSealEffect_Free(BallSealEffect *effect);
void BallSealEffect_DelayedEmitTask(SysTask *task, void *data);
void BallSealEffect_Emit(BallSealEffect *effect, void (*callback)(SPLEmitter *));
void BallSealEffect_GetBasePosition(int battlerType, VecFx32 *pos);
void BallSealEffect_SetEmitterPosition(SPLEmitter *emitter);
void BallSealEffect_SetEmitterPositionFromSeal(SPLEmitter *emitter);
void BallBurstEffect_SetEmitterPosition(SPLEmitter *emitter);
BallBurstEffect *BallBurstEffect_New(const BallBurstEffectTemplate *template);
void BallBurstEffect_Start(BallBurstEffect *effect);
BOOL BallBurstEffect_IsRunning(BallBurstEffect *effect);
void BallBurstEffect_Free(BallBurstEffect *effect);
BOOL UnkBallData_BallAnimThrow(UnkBallData *data);
BOOL UnkBallData_BallAnimOpen(UnkBallData *data);
BOOL UnkBallData_BallAnimDeflect(UnkBallData *data);
BOOL UnkBallData_BallAnimFall(UnkBallData *data);
BOOL UnkBallData_BallAnimShake(UnkBallData *data);
BOOL UnkBallData_BallAnimSkip(UnkBallData *data);
BOOL UnkBallData_BallAnimClick(UnkBallData *data);
BOOL UnkBallData_BallAnimFade(UnkBallData *data);
void UnkBallData_GoToCaptureState(UnkBallData *data, int state);
BOOL UnkBallData_CaptureStateIdle(UnkBallData *data);
BOOL UnkBallData_CaptureStateThrow(UnkBallData *data);
BOOL UnkBallData_CaptureStateGoToOpen(UnkBallData *data);
BOOL UnkBallData_CaptureStateOpen(UnkBallData *data);
BOOL UnkBallData_CaptureStateOpenDone(UnkBallData *data);
BOOL UnkBallData_CaptureStateDeflect(UnkBallData *data);
BOOL UnkBallData_CaptureStateDeflectDone(UnkBallData *data);
BOOL UnkBallData_CaptureStateStartFall(UnkBallData *data);
BOOL UnkBallData_CaptureStateGoToFlash(UnkBallData *data);
BOOL UnkBallData_CaptureStateFlash(UnkBallData *data);
BOOL UnkBallData_CaptureStateGoToDrop(UnkBallData *data);
BOOL UnkBallData_CaptureStateDrop(UnkBallData *data);
BOOL UnkBallData_CaptureStateNone(UnkBallData *data);
BOOL UnkBallData_CaptureStateBounce(UnkBallData *data);
BOOL UnkBallData_CaptureStateBounceDone(UnkBallData *data);
BOOL UnkBallData_CaptureStateShakeWait(UnkBallData *data);
BOOL UnkBallData_CaptureStateGoToShake(UnkBallData *data);
BOOL UnkBallData_CaptureStateShake(UnkBallData *data);
BOOL UnkBallData_CaptureStateShakeDone(UnkBallData *data);
BOOL UnkBallData_CaptureStateGoToSkip(UnkBallData *data);
BOOL UnkBallData_CaptureStateSkip(UnkBallData *data);
BOOL UnkBallData_CaptureStateClick(UnkBallData *data);
BOOL UnkBallData_CaptureStateClickDone(UnkBallData *data);
BOOL UnkBallData_CaptureStateRestorePalette(UnkBallData *data);
BOOL UnkBallData_CaptureStateGoToFadeOut(UnkBallData *data);
BOOL UnkBallData_CaptureStateFadeOut(UnkBallData *data);
BOOL UnkBallData_CaptureStateGoToEnd(UnkBallData *data);
BOOL UnkBallData_CaptureStateEnd(UnkBallData *data);
BOOL UnkBallData_RunCaptureState(UnkBallData *data);
BOOL UnkBallData_RunThrowArc(UnkBallData *data);
BOOL UnkBallData_ThrowArcAndOpen(UnkBallData *data);
BOOL UnkBallData_OpenInPlace(UnkBallData *data);
BOOL UnkBallData_Wobble(UnkBallData *data);
BOOL UnkBallData_TrackPosition(UnkBallData *data);
void UnkBallData_Task(SysTask *task, void *data);
BOOL UnkBallData_IsActive(UnkBallData *data);
BOOL UnkBallData_HasOpened(UnkBallData *data);
void UnkBallData_SetCaptureState(UnkBallData *data, int state);
BOOL UnkBallData_IsCaptureState(UnkBallData *data, int state);
void UnkBallData_SetMode(UnkBallData *data, int mode);
void UnkBallData_InitThrowTarget(UnkBallData *data);
void UnkBallData_GetStartPosition(UnkBallData *data, s16 *x, s16 *y);
void UnkBallData_LoadResources(UnkBallData *data);
void UnkBallData_CreateSprite(UnkBallData *data);
void UnkBallData_SetDrawFlag(UnkBallData *data, int flag);
void UnkBallData_SetPosition(UnkBallData *data, s16 x, s16 y);
void UnkBallData_SetRotation(UnkBallData *data, u16 rotation);
void UnkBallData_SetAnimateSprite(UnkBallData *data, BOOL animate);
void UnkBallData_SetDrawPriority(UnkBallData *data, u16 priority);
void UnkBallData_SetPriority(UnkBallData *data, int priority);
void UnkBallData_SetDelay(UnkBallData *data, int delay);
s32 BallSealEffect_GetDistance(s16 x0, s16 y0, s16 x1, s16 y1);
s32 BallSealEffect_GetSealDistance(const SEAL *seal);
u8 BallSealEffect_GetSealEmitDelay(const SEAL *seal);
BattleAnimEmitterManager *BattleAnimEmitterManager_New(enum HeapID heapId, int *unk);
BattleAnimEmitter *BattleAnimEmitterManager_AddEmitter(BattleAnimEmitterManager *manager, BattleAnimEmitterTemplate *template);
void BattleAnimEmitterManager_Free(BattleAnimEmitterManager *manager);
void BattleAnimEmitterManager_StopAll(BattleAnimEmitterManager *manager);
BattleAnimEmitter *BattleAnimEmitter_New(enum HeapID heapId, BattleAnimEmitterTemplate *template);
void BattleAnimEmitter_Start(BattleAnimEmitter *emitter);
BOOL BattleAnimEmitter_IsActive(BattleAnimEmitter *emitter);
void BattleAnimEmitter_WaitTask(SysTask *task, void *data);
void BattleAnimEmitter_CreateEmitters(BattleAnimEmitter *emitter, void (*callback)(SPLEmitter *));
void *UnkBallData_GetEmitterParam(void);
int BattleAnimEmitter_GetEmitterCount(int resourceId);
void BattleAnimUtil_LoadMonIntoPicSameFacing(struct BattleAnimBattlerPicParams *params, enum HeapID heapId);
void BattleAnimUtil_LoadMonIntoPicOppositeFacing(struct BattleAnimBattlerPicParams *params, enum HeapID heapId);
void BattleAnimUtil_LoadMonIntoPic(struct BattleAnimBattlerPicParams *params, int target, BOOL oppositeFacing, enum HeapID heapId);
void BattleAnimUtil_RestorePic(struct BattleAnimBattlerPicParams *params, enum HeapID heapId);
void BattleAnimUtil_LoadSubstituteIntoPic(struct BattleAnimBattlerPicParams *params, enum HeapID heapId);
s16 ov07_02234B5C(u32 battlerType, u32 index);

// Overlay 7 functions of other modules that no header declares yet (types as their modules declare them)
void BattleAnimSystem_InitBlend(void);
struct BattleAnimBattlerSprites *BattleAnimBattlerSprites_NewAll(BattleSystem *battleSystem, enum HeapID heapId);
void BattleAnimBattlerSprites_Free(struct BattleAnimBattlerSprites *sprites);
SPLEmitter *BattleAnimSystem_NewParticleSystem(enum HeapID heapId, int resId, BOOL loadNow);
SPLEmitter *BattleAnimSystem_NewParticleSystemFromNarc(enum HeapID heapId, NarcId narcId, int resId, BOOL loadNow);
SPLEmitter *BattleAnimSystem_NewParticleSystemFromOpenNarc(NARC *narc, enum HeapID heapId, int member, BOOL loadNow);
void BattleAnimSystem_FreeParticleSystem(SPLEmitter *particleSystem);
void BattleAnimUtil_LookupScreenPos(int a0, int battlerType, s16 *x, s16 *y);
void BattleAnimUtil_InitLinearMotion(void *motion, s16 startX, s16 endX, s16 startY, s16 endY, int duration);
BOOL BattleAnimUtil_UpdateLinearMotionSprite(void *motion, ManagedSprite *sprite);
void BattleAnimUtil_InitLinearSinMotion(void *motion, void *motion2, s16 startX, s16 endX, s16 startY, s16 endY, u16 duration, fx32 height);
BOOL BattleAnimUtil_UpdateLinearSinMotionSprite(void *motion, void *motion2, ManagedSprite *sprite);
void BattleAnimUtil_InitValueMotion(s32 *motion, int start, int end, int duration);
BOOL BattleAnimUtil_UpdateValueMotion(s32 *motion);
void *BattleAnimUtil_StartPaletteBlend(PaletteData *paletteData, enum HeapID heapId, int bufferId, u16 offset, u16 count, s8 a5, s8 a6, u8 start, u8 end, u16 color, u32 priority);
BOOL BattleAnimUtil_IsPaletteBlendActive(void *blend);
void BattleAnimUtil_FreePaletteBlend(void *blend);
void BattleAnimUtil_GetEmitterPositionKind0Alt(int battlerType, VecFx32 *pos, int unused, int perspective);

#endif // POKEHEARTGOLD_BATTLE_BATTLE_ANIM_BALL_H
