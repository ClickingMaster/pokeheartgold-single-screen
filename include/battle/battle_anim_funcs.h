#ifndef POKEHEARTGOLD_BATTLE_BATTLE_ANIM_FUNCS_H
#define POKEHEARTGOLD_BATTLE_BATTLE_ANIM_FUNCS_H

// Overlay 7: effect functions of the battle move animation scripts (sBattleAnimFuncTable / sprite function table
// entries) and the tasks they start, 0x02223064-0x02226C4C.

#include "battle/battle_anim_util.h"

#include "bg_window.h"
#include "palette.h"
#include "pokepic.h"
#include "spl.h"
#include "sprite_system.h"
#include "sys_task.h"

// src/battle/battle_anim_funcs.c
void BattleAnimTask_OneFrame(SysTask *task, void *data);
void BattleAnimFunc_OneFrameTask(BattleAnimSystem *system);
void BattleAnimTask_OneFrameSound(SysTask *task, void *data);
void BattleAnimFunc_OneFrameSoundTask(BattleAnimSystem *system);
void BattleAnimTask_OneFrameUncounted(SysTask *task, void *data);
void BattleAnimFunc_OneFrameUncountedTask(BattleAnimSystem *system);
void ov07_0222314C(SysTask *task, void *data);
void ov07_022231BC(BattleAnimSystem *system, SpriteSystem *spriteSystem, SpriteManager *spriteManager, ManagedSprite *sprite);
void BattleAnimUtil_ResetPokepicScale(Pokepic *pokepic);
void BattleAnimTask_ShakeTargets(SysTask *task, void *data);
void BattleAnimFunc_ShakeTargets(BattleAnimSystem *system);
void BattleAnimTask_SlidingBg(SysTask *task, void *data);
void BattleAnimFunc_SlidingBg(BattleAnimSystem *system);
void BattleAnimTask_SlidingBg2(SysTask *task, void *data);
void BattleAnimFunc_SlidingBg2(BattleAnimSystem *system);
void BattleAnimTask_RotateBattler(SysTask *task, void *data);
void BattleAnimFunc_RotateBattler(BattleAnimSystem *system);
void BattleAnimTask_CircleBattler(SysTask *task, void *data);
void BattleAnimFunc_CircleBattler(BattleAnimSystem *system);
void BattleAnimSpritePop_Start(void *data, s16 x, s16 y);
BOOL BattleAnimSpritePop_Update(void *data);
BOOL BattleAnimSpritePop_RunSequence(void *data);
void BattleAnimTask_SpritePops(SysTask *task, void *data);
void BattleAnimSpriteFunc_SpritePops(BattleAnimSystem *system, SpriteSystem *spriteSystem, SpriteManager *spriteManager, ManagedSprite *sprite);
void BattleAnimTask_WaitPaletteFade(SysTask *task, void *data);
void BattleAnimFunc_FadeBgPalette(BattleAnimSystem *system);
void BattleAnimTask_BattlerPaletteFade(SysTask *task, void *data);
void BattleAnimFunc_BattlerPaletteFade(BattleAnimSystem *system);
void BattleAnimTask_PulseBattlerSprite(SysTask *task, void *data);
void BattleAnimFunc_PulseBattlerSprite(BattleAnimSystem *system);
void BattleAnimTask_StretchBattler(SysTask *task, void *data);
void BattleAnimFunc_StretchBattler(BattleAnimSystem *system);
void BattleAnimTask_BlendBattlerSprites(SysTask *task, void *data);
void BattleAnimFunc_BlendBattlerSprites(BattleAnimSystem *system);
void BattleAnimFunc_SetBattlerVanished(BattleAnimSystem *system);
void BattleAnimTask_BlinkBattler(SysTask *task, void *data);
void BattleAnimFunc_BlinkBattler(BattleAnimSystem *system);
void BattleAnimTask_SlideBattler(SysTask *task, void *data);
void BattleAnimFunc_SlideBattler(BattleAnimSystem *system);
void BattleAnimTask_SlideBattler2(SysTask *task, void *data);
void BattleAnimFunc_SlideBattler2(BattleAnimSystem *system);
void BattleAnimTask_ShakeAndScaleBattler(SysTask *task, void *data);
void BattleAnimFunc_ShakeAndScaleBattler(BattleAnimSystem *system);
void BattleAnimTask_ShakeAndScaleBattlerXY(SysTask *task, void *data);
void BattleAnimFunc_ShakeAndScaleBattlerXY(BattleAnimSystem *system);
void BattleAnimTask_EndObjWindow(SysTask *task, void *data);
void BattleAnimFunc_ShowBattleBgInObjWindow(BattleAnimSystem *system);
void BattleAnimTask_MoveBattlerBy(SysTask *task, void *data);
void BattleAnimFunc_MoveBattlerBy(BattleAnimSystem *system);
void BattleAnimTask_MoveBattlerSin(SysTask *task, void *data);
void BattleAnimFunc_MoveBattlerSin(BattleAnimSystem *system);
void BattleAnimTask_MoveBattlerLinear(SysTask *task, void *data);
void BattleAnimFunc_MoveBattlerLinear(BattleAnimSystem *system);
void BattleAnimFunc_MoveBattlerOffscreen(BattleAnimSystem *system);
void BattleAnimFunc_ResetBattlerPosition(BattleAnimSystem *system);
void BattleAnimTask_WaitPaletteBlend(SysTask *task, void *data);
void BattleAnimFunc_BlendSpritePalette(BattleAnimSystem *system);
void BattleAnimTask_ChangeBattlerHeight(SysTask *task, void *data);
void BattleAnimFunc_ChangeBattlerHeight(BattleAnimSystem *system);
int BattleAnimUtil_GetRoleBattler(BattleAnimSystem *system, int role);
void ov07_02225B38(SysTask *task, void *data);
void ov07_02225BC4(SysTask *task, void *data);
void ov07_02225C50(SysTask *task, void *data);
void BattleAnimFunc_RevealBattler(BattleAnimSystem *system);
void BattleAnimTask_ShakeBg(SysTask *task, void *data);
void BattleAnimFunc_ShakeBg(BattleAnimSystem *system);
void BattleAnimTask_SpriteMosaic(SysTask *task, void *data);
void BattleAnimFunc_SpriteMosaic(BattleAnimSystem *system);
void BattleAnimEmitterMotion_Init(BattleAnimSystem *system, void *data);
BOOL BattleAnimUtil_IsEmitterActive(SPLEmitter *emitter);
void BattleAnimTask_MoveEmitter(SysTask *task, void *data);
void BattleAnimFunc_MoveEmitter(BattleAnimSystem *system);
void ov07_022262E8(BattleAnimSystem *system);
void BattleAnimTask_MoveEmitterArc(SysTask *task, void *data);
void BattleAnimFunc_MoveEmitterArc(BattleAnimSystem *system);
void BattleAnimTask_EmitterSinMotion(SysTask *task, void *data);
void BattleAnimFunc_EmitterSinMotion(BattleAnimSystem *system);
void BattleAnimTask_PlaySpriteAnimOnce(SysTask *task, void *data);
void BattleAnimSpriteFunc_MoveSpriteAndAnimate(BattleAnimSystem *system, SpriteSystem *spriteSystem, SpriteManager *spriteManager, ManagedSprite *sprite);
void BattleAnimTask_SpriteOrbit(SysTask *task, void *data);
void BattleAnimSpriteFunc_SpriteOrbit(BattleAnimSystem *system, SpriteSystem *spriteSystem, SpriteManager *spriteManager, ManagedSprite *sprite);
void BattleAnimTask_PlaySpriteAnimOnce2(SysTask *task, void *data);
void BattleAnimSpriteFunc_SpriteAboveDefender(BattleAnimSystem *system, SpriteSystem *spriteSystem, SpriteManager *spriteManager, ManagedSprite *sprite);
void BattleAnimTask_SpriteArc(SysTask *task, void *data);
void BattleAnimSpriteFunc_SpriteArc(BattleAnimSystem *system, SpriteSystem *spriteSystem, SpriteManager *spriteManager, ManagedSprite *sprite);

// Overlay 7 functions of src/battle/battle_anim_system.c and of sources not merged yet
void BattleAnimSystem_CreateTask(BattleAnimSystem *system, SysTaskFunc func, void *data, u32 priority);
void BattleAnimSystem_CreateTaskDefaultPriority(BattleAnimSystem *system, SysTaskFunc func, void *data);
void BattleAnimSystem_CreateSoundTask(BattleAnimSystem *system, SysTaskFunc func, void *data, u32 priority);
void BattleAnimSystem_DestroyTask(BattleAnimSystem *system, SysTask *task);
void BattleAnimSystem_DestroySoundTask(BattleAnimSystem *system, SysTask *task);
SPLEmitter *BattleAnimSystem_GetParticleSystem(BattleAnimSystem *system, int index);
SPLEmitter *BattleAnimSystem_GetEmitter(BattleAnimSystem *system, int index);
u32 BattleAnimSystem_GetArg(BattleAnimSystem *system, int index);
u32 BattleAnimSystem_GetBattleBgPaletteMask(BattleAnimSystem *system);
void BattleAnimBackground_LoadBattleBgToLayerDefault(BattleAnimSystem *system, GFBgLayer layer);
void BattleAnimBackground_ResetLayer(BattleAnimSystem *system, GFBgLayer layer);
void BattleAnimSystem_GetSpriteTemplate(ManagedSpriteTemplate *template, BattleAnimSystem *system);
int BattleAnimSystem_GetBattlerHeight(BattleAnimSystem *system, int battler);
BOOL BattleAnimSystem_IsBattlerSemiInvulnerable(BattleAnimSystem *system, int battler);
int ov07_0221FAE8(BattleAnimSystem *system);
int BattleAnimSystem_GetBgPriority(BattleAnimSystem *system, int kind);
BOOL ov07_0221FB78(BattleAnimSystem *system, int side);
u32 BattleAnimSystem_GetBackgroundFile(u32 bgId, int kind);
void BattleAnimSystem_GetBattlerScreenPos(BattleAnimSystem *system, int battler, s16 *x, s16 *y);
s16 BattleAnimSystem_GetBattlerScreenCoord(BattleAnimSystem *system, int battler, u32 coord);
int ov07_02222004(BattleAnimSystem *system, int battler);

#endif // POKEHEARTGOLD_BATTLE_BATTLE_ANIM_FUNCS_H
