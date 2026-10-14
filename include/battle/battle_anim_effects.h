#ifndef POKEHEARTGOLD_BATTLE_BATTLE_ANIM_EFFECTS_H
#define POKEHEARTGOLD_BATTLE_BATTLE_ANIM_EFFECTS_H

// Overlay 7: effect functions of the battle move animation scripts (sBattleAnimFuncTable / sBattleAnimSpriteFuncTable
// entries) and the tasks they start, 0x0222C850-0x022318F8.

#include "battle/battle_anim_funcs.h"
#include "battle/battle_anim_util.h"

#include "bg_window.h"
#include "palette.h"
#include "pokepic.h"
#include "sprite_system.h"
#include "sys_task.h"

// One sprite of BattleAnimSpriteFunc_StackedSpritesOnDefender, pulsing its x scale
typedef struct BattleAnimScaleSprite {
    ManagedSprite *sprite;
    BattleAnimLinearMotion scale; // 0x04
    BOOL flip;                    // 0x28
    int repeats;                  // 0x2C
    int start;                    // 0x30, start and end scale (x10) swap after each pulse
    int end;                      // 0x34
} BattleAnimScaleSprite;          // size: 0x38

// Parameters of the OBJ window BG scroll effects (BattleAnimObjWindowBg_Start)
typedef struct BattleAnimObjWindowBgParams {
    NarcId narcId;
    int graphic; // row of the gBattleAnimObjWindowBgGraphics/10/14 graphic tables
    int target;  // 0: attacker, otherwise defender
    s16 velocityX;
    s16 velocityY;
    s16 x;
    s16 y;
    u16 eva;
    u16 evb;
} BattleAnimObjWindowBgParams; // size: 0x18

typedef struct BattleAnimSkewSlideWork BattleAnimSkewSlideWork;
typedef struct BattleAnimWaveOnBgWork BattleAnimWaveOnBgWork;
typedef struct BattleAnimRisingSpriteStretchWork BattleAnimRisingSpriteStretchWork;
typedef struct BattleAnimZigzagSlashWork BattleAnimZigzagSlashWork;
typedef struct BattleAnimZigzagFlashWork BattleAnimZigzagFlashWork;
typedef struct BattleAnimWaddlePicWork BattleAnimWaddlePicWork;
typedef struct BattleAnimAttackerStretchUpWork BattleAnimAttackerStretchUpWork;
typedef struct BattleAnimPulseSpriteWork BattleAnimPulseSpriteWork;
typedef struct BattleAnimAttackerDipWork BattleAnimAttackerDipWork;
typedef struct BattleAnimFanSpritesWork BattleAnimFanSpritesWork;
typedef struct BattleAnimDarkSpriteJumpWork BattleAnimDarkSpriteJumpWork;
typedef struct BattleAnimScaleSpritesWork BattleAnimScaleSpritesWork;
typedef struct BattleAnimOrbitSpritesWork BattleAnimOrbitSpritesWork;
typedef struct BattleAnimFloatingSpritesWork BattleAnimFloatingSpritesWork;
typedef struct BattleAnimWobbleOnBgWork BattleAnimWobbleOnBgWork;
typedef struct BattleAnimObjWindowBgWork BattleAnimObjWindowBgWork;

// src/battle/battle_anim_effects.c
void BattleAnimTask_SkewSlideAttackerBgResetScroll(BattleAnimSkewSlideWork *work, u32 *buffer);
BOOL BattleAnimTask_SkewSlideAttackerBgUpdateScroll(BattleAnimSkewSlideWork *work);
void BattleAnimTask_SkewSlideAttackerBg(SysTask *task, void *data);
void BattleAnimFunc_SkewSlideAttackerBg(BattleAnimSystem *system);
void BattleAnimTask_FadeInShrinkSprite(SysTask *task, void *data);
void BattleAnimTask_ShakeDefenderBlackFlash(SysTask *task, void *data);
void BattleAnimFunc_FadeInShrinkSprite(BattleAnimSystem *system);
void BattleAnimFunc_ShakeDefenderBlackFlash(BattleAnimSystem *system);
BOOL BattleAnimTask_WaveDefenderOnBgUpdate(BattleAnimWaveOnBgWork *work);
void BattleAnimTask_WaveDefenderOnBg(SysTask *task, void *data);
void BattleAnimFunc_WaveDefenderOnBg(BattleAnimSystem *system);
void BattleAnimTask_GrowRockShrinkSprite(SysTask *task, void *data);
void BattleAnimSpriteFunc_GrowRockShrinkSprite(BattleAnimSystem *system, SpriteSystem *spriteSystem, SpriteManager *spriteManager, ManagedSprite *sprite);
void BattleAnimScaleSprite_Init(BattleAnimScaleSprite *scaleSprite, ManagedSprite *sprite, s16 x, s16 y, BOOL flip);
void BattleAnimScaleSprite_Delete(BattleAnimScaleSprite *scaleSprite);
void BattleAnimScaleSprite_Show(BattleAnimScaleSprite *scaleSprite);
BOOL BattleAnimScaleSprite_Update(BattleAnimScaleSprite *scaleSprite);
void BattleAnimTask_StackedSpritesOnDefender(SysTask *task, void *data);
void BattleAnimSpriteFunc_StackedSpritesOnDefender(BattleAnimSystem *system, SpriteSystem *spriteSystem, SpriteManager *spriteManager, ManagedSprite *sprite);
void BattleAnimTask_BoomerangSprite(SysTask *task, void *data);
void BattleAnimSpriteFunc_BoomerangSprite(BattleAnimSystem *system, SpriteSystem *spriteSystem, SpriteManager *spriteManager, ManagedSprite *sprite);
void BattleAnimTask_AttackerCircleFade(SysTask *task, void *data);
void BattleAnimFunc_AttackerCircleFade(BattleAnimSystem *system);
BOOL BattleAnimTask_RisingSpriteStretchAttackerPic(BattleAnimRisingSpriteStretchWork *work);
BOOL BattleAnimTask_RisingSpriteStretchAttackerSprite(BattleAnimRisingSpriteStretchWork *work);
void BattleAnimTask_RisingSpriteStretchAttacker(SysTask *task, void *data);
void BattleAnimSpriteFunc_RisingSpriteStretchAttacker(BattleAnimSystem *system, SpriteSystem *spriteSystem, SpriteManager *spriteManager, ManagedSprite *sprite);
void BattleAnimTask_ZigzagSlashSetLeg(BattleAnimZigzagSlashWork *work);
void BattleAnimTask_ZigzagSlash(SysTask *task, void *data);
void BattleAnimSpriteFunc_ZigzagSlash(BattleAnimSystem *system, SpriteSystem *spriteSystem, SpriteManager *spriteManager, ManagedSprite *sprite);
void BattleAnimTask_ZigzagFlashSetLeg(BattleAnimZigzagFlashWork *work);
void BattleAnimTask_ZigzagFlash(SysTask *task, void *data);
void BattleAnimSpriteFunc_ZigzagFlash(BattleAnimSystem *system, SpriteSystem *spriteSystem, SpriteManager *spriteManager, ManagedSprite *sprite);
void BattleAnimTask_WaddlePicStartStep(BattleAnimWaddlePicWork *work, BOOL flip);
BOOL BattleAnimTask_WaddlePicUpdateStep(BattleAnimWaddlePicWork *work);
void BattleAnimTask_WaddlePic(SysTask *task, void *data);
void BattleAnimFunc_WaddlePic(BattleAnimSystem *system);
BOOL BattleAnimTask_AttackerStretchUpStep(BattleAnimAttackerStretchUpWork *work);
void BattleAnimTask_AttackerStretchUp(SysTask *task, void *data);
void BattleAnimFunc_AttackerStretchUp(BattleAnimSystem *system);
void BattleAnimTask_PulseSpriteBrightnessFlashStartPulse(BattleAnimPulseSpriteWork *work);
BOOL BattleAnimTask_PulseSpriteBrightnessFlashUpdatePulse(BattleAnimPulseSpriteWork *work);
BOOL BattleAnimTask_PulseSpriteBrightnessFlashUpdatePulses(BattleAnimPulseSpriteWork *work);
void BattleAnimSpriteFunc_PulseSpriteBrightnessFlash(BattleAnimSystem *system, SpriteSystem *spriteSystem, SpriteManager *spriteManager, ManagedSprite *sprite);
void BattleAnimTask_AttackerDipSetLeg(BattleAnimAttackerDipWork *work);
void BattleAnimFunc_AttackerDip(BattleAnimSystem *system);
void BattleAnimFunc_AttackerLungeKnockback(BattleAnimSystem *system);
void BattleAnimTask_FanSpritesStartAttackerPulse(BattleAnimFanSpritesWork *work);
BOOL BattleAnimTask_FanSpritesUpdateAttackerPulse(BattleAnimFanSpritesWork *work);
void BattleAnimUtil_StartSpriteAnim(ManagedSprite *sprite, u32 *state, fx32 speed);
BOOL BattleAnimUtil_IsSpriteAnimDone(ManagedSprite *sprite, u32 *state);
void BattleAnimTask_FanSpritesAroundAttacker(SysTask *task, void *data);
void BattleAnimSpriteFunc_FanSpritesAroundAttacker(BattleAnimSystem *system, SpriteSystem *spriteSystem, SpriteManager *spriteManager, ManagedSprite *sprite);
void BattleAnimTask_ShrinkAttackerIntoSprite(SysTask *task, void *data);
void BattleAnimSpriteFunc_ShrinkAttackerIntoSprite(BattleAnimSystem *system, SpriteSystem *spriteSystem, SpriteManager *spriteManager, ManagedSprite *sprite);
void BattleAnimTask_DarkSpriteJumpDarkenSprite(BattleAnimDarkSpriteJumpWork *work, ManagedSprite *sprite);
void BattleAnimTask_DarkSpriteJumpStartRise(BattleAnimDarkSpriteJumpWork *work);
BOOL BattleAnimTask_DarkSpriteJumpUpdateRise(BattleAnimDarkSpriteJumpWork *work);
void BattleAnimTask_DarkSpriteJumpStartDrop(BattleAnimDarkSpriteJumpWork *work);
BOOL BattleAnimTask_DarkSpriteJumpUpdateDrop(BattleAnimDarkSpriteJumpWork *work);
void BattleAnimTask_DarkSpriteJump(SysTask *task, void *data);
void BattleAnimFunc_DarkSpriteJump(BattleAnimSystem *system);
void BattleAnimTask_ScaleSpritesOverDefenderStartGrow(BattleAnimScaleSpritesWork *work);
void BattleAnimTask_ScaleSpritesOverDefenderStartShrink(BattleAnimScaleSpritesWork *work);
void BattleAnimTask_ScaleSpritesOverDefender(SysTask *task, void *data);
void BattleAnimSpriteFunc_ScaleSpritesOverDefender(BattleAnimSystem *system, SpriteSystem *spriteSystem, SpriteManager *spriteManager, ManagedSprite *sprite);
void BattleAnimTask_SpritesOrbitAttackerBobbingInit(BattleAnimOrbitSpritesWork *work, int direction);
void BattleAnimTask_SpritesOrbitAttackerBobbingUpdate(BattleAnimOrbitSpritesWork *work);
void BattleAnimTask_SpritesOrbitAttackerBobbing(SysTask *task, void *data);
void BattleAnimSpriteFunc_SpritesOrbitAttackerBobbing(BattleAnimSystem *system, SpriteSystem *spriteSystem, SpriteManager *spriteManager, ManagedSprite *sprite);
void BattleAnimTask_FloatingSpriteStreamLaunch(ManagedSprite *sprite, BattleAnimLinearMotion *motion, int dirX, int dirY);
BOOL BattleAnimTask_FloatingSpriteStreamUpdateSprite(ManagedSprite *sprite, BattleAnimLinearMotion *motion);
BOOL BattleAnimTask_FloatingSpriteStreamUpdate(BattleAnimFloatingSpritesWork *work);
void BattleAnimTask_FloatingSpriteStream(SysTask *task, void *data);
void BattleAnimSpriteFunc_FloatingSpriteStream(BattleAnimSystem *system, SpriteSystem *spriteSystem, SpriteManager *spriteManager, ManagedSprite *sprite);
void BattleAnimTask_WobbleDefenderOnBgSetPhase(BattleAnimWobbleOnBgWork *work);
void BattleAnimTask_WobbleDefenderOnBgUpdateScanlines(BattleAnimWobbleOnBgWork *work);
void BattleAnimTask_WobbleDefenderOnBg(SysTask *task, void *data);
void BattleAnimFunc_WobbleDefenderOnBg(BattleAnimSystem *system);
void BattleAnimTask_WindowSweepFlashWhite(SysTask *task, void *data);
void BattleAnimFunc_WindowSweepFlashWhite(BattleAnimSystem *system);
void BattleAnimTask_FadeSpriteAnim(SysTask *task, void *data);
void BattleAnimSpriteFunc_FadeSpriteAnim(BattleAnimSystem *system, SpriteSystem *spriteSystem, SpriteManager *spriteManager, ManagedSprite *sprite);
void BattleAnimTask_StretchShakeDefender(SysTask *task, void *data);
void BattleAnimFunc_StretchShakeDefender(BattleAnimSystem *system);
void BattleAnimTask_ShootSpriteFromAttacker(SysTask *task, void *data);
void BattleAnimFunc_ShootSpriteFromAttacker(BattleAnimSystem *system);
void BattleAnimTask_AttackerShakeLunge(SysTask *task, void *data);
void BattleAnimFunc_AttackerShakeLunge(BattleAnimSystem *system);
void BattleAnimTask_DefenderKnockback(SysTask *task, void *data);
void BattleAnimFunc_DefenderKnockback(BattleAnimSystem *system);
void BattleAnimSpriteFunc_RisingShrinkSprites(BattleAnimSystem *system, SpriteSystem *spriteSystem, SpriteManager *spriteManager, ManagedSprite *sprite);
void BattleAnimTask_RisingShrinkSprites(SysTask *task, void *data);
void BattleAnimTask_RisingShrinkSpritesLaunch(ManagedSprite *sprite, BattleAnimLinearMotion *move, BattleAnimLinearMotion *scale, int direction);
BOOL BattleAnimTask_RisingShrinkSpritesUpdate(ManagedSprite *sprite, BattleAnimLinearMotion *move, BattleAnimLinearMotion *scale);
void BattleAnimSpriteFunc_PaletteCycleSprite(BattleAnimSystem *system, SpriteSystem *spriteSystem, SpriteManager *spriteManager, ManagedSprite *sprite);
void BattleAnimTask_PaletteCycleSprite(SysTask *task, void *data);
void BattleAnimSpriteFunc_SpriteRowSequence(BattleAnimSystem *system, SpriteSystem *spriteSystem, SpriteManager *spriteManager, ManagedSprite *sprite);
void BattleAnimTask_SpriteRowSequencePlace(ManagedSprite *sprite, int *visibleTime, int *delay, int index, int direction);
BOOL BattleAnimTask_SpriteRowSequenceUpdate(ManagedSprite *sprite, int *visibleTime, int *delay);
void BattleAnimTask_SpriteRowSequence(SysTask *task, void *data);
void BattleAnimTask_SpritesOrbitAttackerInit(BattleAnimOrbitSpritesWork *work, int direction);
void BattleAnimTask_SpritesOrbitAttackerUpdate(BattleAnimOrbitSpritesWork *work);
void BattleAnimTask_SpritesOrbitAttacker(SysTask *task, void *data);
void BattleAnimSpriteFunc_SpritesOrbitAttacker(BattleAnimSystem *system, SpriteSystem *spriteSystem, SpriteManager *spriteManager, ManagedSprite *sprite);
void BattleAnimObjWindowBg_SetScrollVelocity(BattleAnimObjWindowBgParams *params, s16 x, s16 y);
void BattleAnimObjWindowBg_SetBlend(BattleAnimObjWindowBgParams *params, s16 eva, s16 evb);
void BattleAnimObjWindowBg_LoadGraphics(BattleAnimObjWindowBgWork *work, int bgLayer);
void BattleAnimTask_ObjWindowBgScroll(SysTask *task, void *data);
void BattleAnimObjWindowBg_InitParams(BattleAnimSystem *system, BattleAnimObjWindowBgParams *params);
void BattleAnimObjWindowBg_Start(BattleAnimSystem *system, const BattleAnimObjWindowBgParams *params);
void BattleAnimFunc_ObjWindowBgScrollUp(BattleAnimSystem *system);
void BattleAnimFunc_ObjWindowBgScrollDown(BattleAnimSystem *system);
void BattleAnimFunc_ObjWindowBgScrollUpAlt(BattleAnimSystem *system);
void BattleAnimFunc_ObjWindowBgScrollDownFast(BattleAnimSystem *system);

// Overlay 7 functions of src/battle/battle_anim_system.c and of sources not merged yet
ManagedSprite *BattleAnimSystem_GetSprite(BattleAnimSystem *system, int index);
u32 BattleAnimSystem_GetBgLayer(BattleAnimSystem *system, int index);
u8 BattleAnimUtil_GetSpritePaletteSlot(ManagedSprite *sprite);
int BattleAnimUtil_GetBattlerSideSign(BattleAnimSystem *system, int battler);

#endif // POKEHEARTGOLD_BATTLE_BATTLE_ANIM_EFFECTS_H
