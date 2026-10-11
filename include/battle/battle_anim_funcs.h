#ifndef POKEHEARTGOLD_BATTLE_BATTLE_ANIM_FUNCS_H
#define POKEHEARTGOLD_BATTLE_BATTLE_ANIM_FUNCS_H

// Overlay 7: effect functions of the battle move animation scripts (sBattleAnimFuncTable / sprite function table
// entries) and the tasks they start, 0x02223064-0x02226C4C.

#include "battle/battle_anim_battler_util.h"

#include "bg_window.h"
#include "palette.h"
#include "pokepic.h"
#include "spl.h"
#include "sprite_system.h"
#include "sys_task.h"

// Shared overlay-7 types; the same definitions live in battle/battle_anim_internal.h once it lands (switch to it
// then).

typedef struct BattleAnimLinearMotion {
    s16 x;
    s16 y;
    int duration;
    fx32 stepX;
    fx32 stepY;
    fx32 curX;
    fx32 curY;
    u8 unk_18[0xC];
} BattleAnimLinearMotion; // size: 0x24

typedef struct BattleAnimSinMotion {
    s16 x;
    s16 y;
    int duration;
    int angleX;
    int ampX;
    int angleY;
    int ampY;
    int stepX;
    int stepY;
    int unk_20;
} BattleAnimSinMotion; // size: 0x24

typedef struct BattleAnimValueMotion {
    int value;
    int duration;
    int step;
    int current;
    int unk_10;
} BattleAnimValueMotion; // size: 0x14

// Offset (x, y) that alternates +amp, 0, -amp, 0 every interval frames, count cycles
typedef struct BattleAnimShake {
    s16 x;
    s16 y;
    int timer;
    int interval;
    int count;
    int phase;
    int ampX;
    int ampY;
    int prevX;
    int prevY;
} BattleAnimShake; // size: 0x24

// BLDALPHA EVA (x) and EVB (y) moved linearly by BattleAnimTask_BlendAlphaMotion
typedef struct BattleAnimBlendAlphaMotion {
    BattleAnimLinearMotion motion;
    BOOL done;
} BattleAnimBlendAlphaMotion; // size: 0x28

typedef struct BattleAnimPaletteBlend BattleAnimPaletteBlend;

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
void ov07_0222519C(SysTask *task, void *data);
void ov07_022251CC(BattleAnimSystem *system);
void BattleAnimTask_MoveBattlerBy(SysTask *task, void *data);
void BattleAnimFunc_MoveBattlerBy(BattleAnimSystem *system);
void BattleAnimTask_MoveBattlerSin(SysTask *task, void *data);
void BattleAnimFunc_MoveBattlerSin(BattleAnimSystem *system);
void BattleAnimTask_MoveBattlerLinear(SysTask *task, void *data);
void BattleAnimFunc_MoveBattlerLinear(BattleAnimSystem *system);
void ov07_02225640(BattleAnimSystem *system);
void BattleAnimFunc_ResetBattlerPosition(BattleAnimSystem *system);
void BattleAnimTask_WaitPaletteBlend(SysTask *task, void *data);
void BattleAnimFunc_BlendSpritePalette(BattleAnimSystem *system);
void ov07_02225904(SysTask *task, void *data);
void ov07_02225990(BattleAnimSystem *system);
int BattleAnimUtil_GetRoleBattler(BattleAnimSystem *system, int role);
void ov07_02225B38(SysTask *task, void *data);
void ov07_02225BC4(SysTask *task, void *data);
void ov07_02225C50(SysTask *task, void *data);
void ov07_02225CC4(BattleAnimSystem *system);
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
void ov07_02226838(BattleAnimSystem *system, SpriteSystem *spriteSystem, SpriteManager *spriteManager, ManagedSprite *sprite);
void BattleAnimTask_SpriteOrbit(SysTask *task, void *data);
void BattleAnimSpriteFunc_SpriteOrbit(BattleAnimSystem *system, SpriteSystem *spriteSystem, SpriteManager *spriteManager, ManagedSprite *sprite);
void BattleAnimTask_PlaySpriteAnimOnce2(SysTask *task, void *data);
void ov07_02226A9C(BattleAnimSystem *system, SpriteSystem *spriteSystem, SpriteManager *spriteManager, ManagedSprite *sprite);
void ov07_02226B2C(SysTask *task, void *data);
void ov07_02226C4C(BattleAnimSystem *system, SpriteSystem *spriteSystem, SpriteManager *spriteManager, ManagedSprite *sprite);

// Overlay 7 functions defined in sources not merged yet (same types as their packages' headers; switch to
// battle/battle_anim_internal.h when it lands; battle_anim_battler_util.h declares the rest)
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
int ov07_0221FAA0(BattleAnimSystem *system, int battler);
BOOL BattleAnimSystem_IsBattlerSemiInvulnerable(BattleAnimSystem *system, int battler);
int ov07_0221FAE8(BattleAnimSystem *system);
int BattleAnimSystem_GetBgPriority(BattleAnimSystem *system, int kind);
BOOL ov07_0221FB78(BattleAnimSystem *system, int side);
u32 BattleAnimSystem_GetBackgroundFile(u32 bgId, int kind);
void ov07_02221F38(BattleAnimSystem *system, int battler, s16 *x, s16 *y);
s16 ov07_02221F80(BattleAnimSystem *system, int battler, u32 coord);
int ov07_02222004(BattleAnimSystem *system, int battler);
void ov07_022220FC(BattleAnimSinMotion *motion, int startX, int endX, int startY, u16 endY, fx32 ampX, fx32 ampY, int duration);
BOOL ov07_02222180(BattleAnimSinMotion *motion);
BOOL ov07_02222240(BattleAnimSinMotion *motion, int x, int y, Pokepic *pokepic);
void ov07_02222268(BattleAnimLinearMotion *motion, s16 startX, s16 endX, s16 startY, s16 endY, u16 duration);
BOOL ov07_022222B4(BattleAnimLinearMotion *motion);
void ov07_02222338(BattleAnimLinearMotion *linear, BattleAnimSinMotion *sin, s16 startX, s16 endX, s16 startY, s16 endY, u16 duration, fx32 amplitude);
BOOL ov07_02222384(BattleAnimLinearMotion *linear, BattleAnimSinMotion *sin);
BOOL ov07_022223CC(BattleAnimLinearMotion *linear, BattleAnimSinMotion *sin, ManagedSprite *sprite);
void ov07_022223F0(BattleAnimValueMotion *motion, int start, int end, int duration);
BOOL ov07_02222440(BattleAnimValueMotion *motion);
void ov07_02222508(BattleAnimLinearMotion *motion, int start, int divisor, int end, int duration);
BOOL ov07_02222558(BattleAnimLinearMotion *motion);
void ov07_02222590(BattleAnimLinearMotion *motion, int startX, int endX, int startY, s16 endY, s16 divisor, int duration);
BOOL ov07_0222260C(BattleAnimLinearMotion *motion);
void ov07_02222644(BattleAnimLinearMotion *motion, f32 *x, f32 *y);
s16 ov07_02222674(int y, int height, fx32 scale);
void ov07_022226C4(Pokepic *pokepic, int y, int height, fx32 scale, int mode);
void ov07_022226FC(ManagedSprite *sprite, int y, int height, fx32 scale, int mode);
BOOL ov07_02222748(BattleAnimLinearMotion *motion, ManagedSprite *sprite);
BOOL ov07_02222768(BattleAnimLinearMotion *motion, Pokepic *pokepic);
BOOL ov07_02222788(BattleAnimLinearMotion *motion, Pokepic *pokepic);
void ov07_022227A8(BattleAnimShake *shake, int ampX, int ampY, int interval, int count);
BOOL ov07_022227D8(BattleAnimShake *shake);
void ov07_02222A44(BattleAnimSinMotion *motion, int multiplier, int duration);
void ov07_02222AC4(BattleAnimBlendAlphaMotion *blend, s16 startEva, s16 endEva, s16 startEvb, s16 endEvb, int duration);
BOOL ov07_02222AF4(BattleAnimBlendAlphaMotion *blend);
BOOL ov07_02222EE8(BattleAnimPaletteBlend *blend);
void ov07_02222EF8(BattleAnimPaletteBlend *blend);
BattleAnimPaletteBlend *ov07_02222F10(PaletteData *paletteData, enum HeapID heapId, PaletteBufferId bufferId, u16 offset, u16 size, s8 delay, s8 step, u8 start, u8 end, u16 color, u32 priority);

#endif // POKEHEARTGOLD_BATTLE_BATTLE_ANIM_FUNCS_H
