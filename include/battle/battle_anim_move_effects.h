#ifndef POKEHEARTGOLD_BATTLE_BATTLE_ANIM_MOVE_EFFECTS_H
#define POKEHEARTGOLD_BATTLE_BATTLE_ANIM_MOVE_EFFECTS_H

// Overlay 7: effect functions of the battle move animation scripts (sBattleAnimFuncTable / sBattleAnimSpriteFuncTable
// entries) and the tasks they start, 0x02226C90-0x0222C850.

#include "battle/battle_anim_funcs.h"
#include "battle/battle_anim_internal.h"

#include "bg_window.h"
#include "palette.h"
#include "pokepic.h"
#include "sprite_system.h"
#include "sys_task.h"

// A sprite circling a point: the angle step of its orbit oscillates (speed), then it runs a second circle
typedef struct BattleAnimMotionSprite {
    ManagedSprite *sprite;
    BattleAnimSinMotion orbit; // 0x04
    BattleAnimCosMotion speed; // 0x28, added to baseAngleStep
    s16 baseAngleStep;         // 0x3C
    s16 x;                     // center of the orbit
    s16 y;
    u16 unk_42;
    int state; // 0x44
    BOOL active;
} BattleAnimMotionSprite; // size: 0x4C

typedef struct BattleAnimShakeDefenderWithOverlayWork BattleAnimShakeDefenderWithOverlayWork;

// src/battle/battle_anim_move_effects.c
void BattleAnimFunc_SetBgPaletteGrayscale(BattleAnimSystem *system);
void BattleAnimTask_DropSpriteStepwiseUpdate(void *data);
void BattleAnimTask_DropSpriteStepwise(SysTask *task, void *data);
void BattleAnimFunc_DropSpriteStepwise(BattleAnimSystem *system);
void BattleAnimTask_AnimateSprites(SysTask *task, void *data);
void BattleAnimFunc_AnimateSprites(BattleAnimSystem *system);
void BattleAnimTask_BattleBgWave(SysTask *task, void *data);
void BattleAnimFunc_BattleBgWave(BattleAnimSystem *system);
void BattleAnimTask_BlinkScaleFadeSprites(SysTask *task, void *data);
void BattleAnimSpriteFunc_BlinkScaleFadeSprites(BattleAnimSystem *system, SpriteSystem *spriteSystem, SpriteManager *spriteManager, ManagedSprite *sprite);
void BattleAnimTask_AttackerGrowFlashRed(SysTask *task, void *data);
void BattleAnimFunc_AttackerGrowFlashRed(BattleAnimSystem *system);
void BattleAnimTask_AttackerSquashStretch(SysTask *task, void *data);
void BattleAnimFunc_AttackerSquashStretch(BattleAnimSystem *system);
void BattleAnimTask_AttackerAfterimages(SysTask *task, void *data);
void BattleAnimFunc_AttackerAfterimages(BattleAnimSystem *system);
void BattleAnimTask_AttackerPulseWhite(SysTask *task, void *data);
void BattleAnimFunc_AttackerPulseWhite(BattleAnimSystem *system);
void BattleAnimTask_AttackerStretchSteps(SysTask *task, void *data);
void BattleAnimFunc_AttackerStretchSteps(BattleAnimSystem *system);
void BattleAnimTask_AttackerLeapOffscreen(SysTask *task, void *data);
void BattleAnimFunc_AttackerLeapOffscreen(BattleAnimSystem *system);
void BattleAnimTask_AttackerSilhouette(SysTask *task, void *data);
void BattleAnimFunc_AttackerSilhouette(BattleAnimSystem *system);
void BattleAnimTask_AttackerHop(SysTask *task, void *data);
void BattleAnimFunc_AttackerHop(BattleAnimSystem *system);
void BattleAnimTask_ShowAttackerObjWindowSprites(SysTask *task, void *data);
void BattleAnimFunc_ShowAttackerObjWindowSprites(BattleAnimSystem *system);
void BattleAnimTask_AttackerSquashAfterimages(SysTask *task, void *data);
void BattleAnimFunc_AttackerSquashAfterimages(BattleAnimSystem *system);
void BattleAnimTask_ShakeBattlefieldFlashing(SysTask *task, void *data);
void BattleAnimFunc_ShakeBattlefieldFlashing(BattleAnimSystem *system);
void BattleAnimTask_SpriteDriftFade(SysTask *task, void *data);
void BattleAnimFunc_SpriteDriftFade(BattleAnimSystem *system);
void BattleAnimTask_RockAttacker(SysTask *task, void *data);
void BattleAnimTask_ShakePic(SysTask *task, void *data);
void BattleAnimFunc_RockOrShakeAttacker(BattleAnimSystem *system);
void BattleAnimTask_ShakeBattlefieldByPower(SysTask *task, void *data);
void BattleAnimFunc_ShakeBattlefieldByPower(BattleAnimSystem *system);
void BattleAnimTask_ShakeSpritePair(SysTask *task, void *data);
void BattleAnimFunc_ShakeSpritePair(BattleAnimSystem *system);
void BattleAnimTask_ThrowSpriteArc(SysTask *task, void *data);
void BattleAnimSpriteFunc_ThrowSpriteArc(BattleAnimSystem *system, SpriteSystem *spriteSystem, SpriteManager *spriteManager, ManagedSprite *sprite);
void BattleAnimTask_StretchFadeSprite(SysTask *task, void *data);
void BattleAnimFunc_StretchFadeSprite(BattleAnimSystem *system);
void BattleAnimTask_CrossingSprites(SysTask *task, void *data);
void BattleAnimSpriteFunc_CrossingSprites(BattleAnimSystem *system, SpriteSystem *spriteSystem, SpriteManager *spriteManager, ManagedSprite *sprite);
void BattleAnimTask_BlinkSpritesCenter(SysTask *task, void *data);
void BattleAnimSpriteFunc_BlinkSpritesCenter(BattleAnimSystem *system, SpriteSystem *spriteSystem, SpriteManager *spriteManager, ManagedSprite *sprite);
void BattleAnimTask_SlidingSpritePairs(SysTask *task, void *data);
void BattleAnimSpriteFunc_SlidingSpritePairs(BattleAnimSystem *system, SpriteSystem *spriteSystem, SpriteManager *spriteManager, ManagedSprite *sprite);
void BattleAnimTask_PopAndScatterSprite(SysTask *task, void *data);
void BattleAnimTask_PopAndScatterSprites(SysTask *task, void *data);
void BattleAnimSpriteFunc_PopAndScatterSprites(BattleAnimSystem *system, SpriteSystem *spriteSystem, SpriteManager *spriteManager, ManagedSprite *sprite);
void BattleAnimTask_WaveAttackerOnBg(SysTask *task, void *data);
void BattleAnimFunc_WaveAttackerOnBg(BattleAnimSystem *system);
void BattleAnimUtil_TickSpriteIfVisible(ManagedSprite *sprite);
void BattleAnimTask_SpriteQuad(SysTask *task, void *data);
void BattleAnimSpriteFunc_SpriteQuad(BattleAnimSystem *system, SpriteSystem *spriteSystem, SpriteManager *spriteManager, ManagedSprite *sprite);
void BattleAnimTask_FlashingSpritesAroundAttacker(SysTask *task, void *data);
void BattleAnimSpriteFunc_FlashingSpritesAroundAttacker(BattleAnimSystem *system, SpriteSystem *spriteSystem, SpriteManager *spriteManager, ManagedSprite *sprite);
void BattleAnimTask_ShrinkSpriteOnDefender(SysTask *task, void *data);
void BattleAnimFunc_ShrinkSpriteOnDefender(BattleAnimSystem *system);
void BattleAnimTask_AttackerDashAfterimages(SysTask *task, void *data);
void BattleAnimFunc_AttackerDashAfterimages(BattleAnimSystem *system);
void BattleAnimTask_SpriteTrailToDefender(SysTask *task, void *data);
void BattleAnimSpriteFunc_SpriteTrailToDefender(BattleAnimSystem *system, SpriteSystem *spriteSystem, SpriteManager *spriteManager, ManagedSprite *sprite);
void BattleAnimTask_SquashSprite(SysTask *task, void *data);
void BattleAnimFunc_SquashSprite(BattleAnimSystem *system);
void BattleAnimTask_SlideOutAndTransform(SysTask *task, void *data);
void BattleAnimFunc_SlideOutAndTransform(BattleAnimSystem *system);
void BattleAnimUtil_SetPicVisibleRect(Pokepic *pokepic, int x, int width, int yFromBottom, int height);
void BattleAnimTask_WipeOutDefender(SysTask *task, void *data);
void BattleAnimFunc_WipeOutDefender(BattleAnimSystem *system);
void BattleAnimTask_PicOrbit(SysTask *task, void *data);
void BattleAnimFunc_PicOrbit(BattleAnimSystem *system);
void BattleAnimTask_AttackerRecoilTilt(SysTask *task, void *data);
void BattleAnimFunc_AttackerRecoilTilt(BattleAnimSystem *system);
void BattleAnimMotionSprite_Init(BattleAnimMotionSprite *motionSprite, SpriteSystem *spriteSystem, SpriteManager *spriteManager, const ManagedSpriteTemplate *template, const BattleAnimMotionSprite *source);
void BattleAnimMotionSprite_Delete(BattleAnimMotionSprite *motionSprite);
void BattleAnimMotionSprite_SetDrawFlag(BattleAnimMotionSprite *motionSprite, BOOL draw);
BOOL BattleAnimMotionSprite_Update(BattleAnimMotionSprite *motionSprite);
void BattleAnimTask_OrbitingSpriteTrail(SysTask *task, void *data);
void BattleAnimSpriteFunc_OrbitingSpriteTrail(BattleAnimSystem *system, SpriteSystem *spriteSystem, SpriteManager *spriteManager, ManagedSprite *sprite);
void BattleAnimTask_ShakeDefenderWithOverlayApply(BattleAnimShakeDefenderWithOverlayWork *work);
void BattleAnimTask_ShakeDefenderWithOverlay(SysTask *task, void *data);
void BattleAnimFunc_ShakeDefenderWithOverlay(BattleAnimSystem *system);

// Overlay 7 functions declared in battle/battle_anim_emitter.h, which cannot be included together with
// battle/battle_anim_util.h
s16 BattleAnimSystem_GetBattlerScreenCoord(BattleAnimSystem *system, int battler, u32 coord);
int BattleAnimUtil_GetBattlerSideSign(BattleAnimSystem *system, int battler);

#endif // POKEHEARTGOLD_BATTLE_BATTLE_ANIM_MOVE_EFFECTS_H
