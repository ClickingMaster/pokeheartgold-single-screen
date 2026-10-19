#ifndef POKEHEARTGOLD_BATTLE_BATTLE_ANIM_INTERNAL_H
#define POKEHEARTGOLD_BATTLE_BATTLE_ANIM_INTERNAL_H

#include "battle/battle.h"
#include "battle/battle_anim_util.h"

#include "bg_window.h"
#include "filesystem.h"
#include "overlay_07.h"
#include "palette.h"
#include "pokepic.h"
#include "sav_chatot.h"
#include "spl.h"
#include "sprite_system.h"

typedef void (*BattleAnimRunner)(BattleAnimSystem *system);
typedef void (*BattleAnimScriptCmd)(BattleAnimSystem *system);

typedef struct BattleAnimSoundTaskData BattleAnimSoundTaskData;
typedef struct BattleAnimBrightnessFlashWork BattleAnimBrightnessFlashWork;
typedef struct BattleAnimBgSwitchWork BattleAnimBgSwitchWork;
typedef struct BattleAnimIndexedBytes BattleAnimIndexedBytes;

// Per-move data (a copy of the battle's move animation packet) and the battlers' state
typedef struct BattleAnimContext {
    u8 command;
    u8 unk_01;
    u16 move;
    int damage;
    u16 power;
    u16 friendship;
    u32 fieldCondition;
    u16 moveAnimVariant;
    u16 terrain;
    u16 attacker;
    u16 defender;
    int currentParticleSystem;
    SPLEmitter *particleSystems[16];
    SPLEmitter *emitters[16];
    u8 unk_9C[0x10];
    SpriteSystem *spriteSystem;
    MonPicData *picData[4];
    u8 battlerTypes[4];
    Pokepic *pokepics[4];
    u32 battleType;
    u16 species[4];
    u8 genders[4];
    u8 shiny[4];
    u8 forms[4];
    u32 personalities[4];
    u32 moveEffectFlags[4];
    SOUND_CHATOT *chatotCry;
    u8 *battleBgTiles;
    u16 *battleBgPalette;
    int isTransformed : 1;
    int attackerActive : 1;
    int defenderActive : 1;
    int unk_118_3 : 29;
} BattleAnimContext;

// BeginLoop/EndLoop state
typedef struct BattleAnimScriptLoop {
    u32 *start;
    u8 counter;
    u8 count;
    BOOL active;
} BattleAnimScriptLoop;

// Draw task entry of the battler sprite copies (StartBattlerSpriteDraw)
typedef struct BattleAnimBattlerSpriteDraw {
    BattleAnimSystem *system;
    SpriteManager *manager;
    ManagedSprite *sprite;
    BOOL active;
} BattleAnimBattlerSpriteDraw;

// Keeps a sprite or BG2 on a battler's pokepic
typedef struct BattleAnimFollowWork {
    void *target; // ManagedSprite or BgConfig
    u8 counter;
    u8 interval;
    Pokepic *pokepic;
    SysTask *task;
} BattleAnimFollowWork;

// Common head of the background effect task data registered in BattleAnimSystem.bgEffects
typedef struct BattleAnimBgEffect {
    u8 unk_00[0x18];
    BOOL stop;
    int kind;
} BattleAnimBgEffect;

struct BattleAnimSystem {
    enum HeapID heapId;
    NarcId scriptNarc;
    int unk_08;
    BOOL initialized;
    BOOL active;
    void *scriptData;
    u32 *scriptPtr;
    u32 *returnStack[3];
    BattleAnimScriptLoop loops[3];
    BattleAnimBattlerSpriteDraw battlerSpriteDraws[2];
    u8 particlePerspective[16]; // per particle system
    u8 particleFlipY[16];
    u8 unk_8C;
    u8 delay;
    u16 taskCount;
    u16 soundTaskCount;
    u32 args[10];
    BattleAnimRunner runner;
    BattleAnimContext *context;
    BgConfig *bgConfig;
    PaletteData *paletteData;
    SpriteManager *spriteManagers[4];
    ManagedSprite *sprites[10];
    ManagedSpriteTemplate spriteTemplate;
    SpriteManager *battlerSpriteManager;
    ManagedSprite *battlerSprites[5];
    BOOL battlerSpriteActive[5];
    BattleAnimFollowWork *followWorks[6]; // [5]: the battler copy on BG2
    u8 bgSwitchState;
    u8 seWaitTimer;
    u8 endPhase;
    u8 unk_17F;
    BattleAnimBgEffect *bgEffects[4];
    BattleAnimBgGraphics battleBg;
    u32 battleBgPaletteMask;
    u8 bgPriorities[4];
    NARC *narcs[6];
};

// Battler picture data handed to the pic swap helpers (BattleAnimSystem_BuildBattlerPicParams)
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
} BattleAnimBattlerPicParams;

void BattleAnimSystem_WaitDelay(BattleAnimSystem *system);
void BattleAnimSystem_ExecuteCommands(BattleAnimSystem *system);
void BattleAnimSystem_CreateTaskOfKind(int kind, BattleAnimSystem *system, SysTaskFunc func, void *data, u32 priority);
void BattleAnimSystem_DestroyTaskOfKind(int kind, BattleAnimSystem *system, SysTask *task);
int BattleAnimSystem_GetField08(BattleAnimSystem *system);
BOOL BattleAnimSystem_IsInitialized(BattleAnimSystem *system);
void BattleAnimSystem_CreateTask(BattleAnimSystem *system, SysTaskFunc func, void *data, u32 priority);
void BattleAnimSystem_CreateTaskDefaultPriority(BattleAnimSystem *system, SysTaskFunc func, void *data);
void BattleAnimSystem_CreateSoundTask(BattleAnimSystem *system, SysTaskFunc func, void *data, u32 priority);
void BattleAnimSystem_DestroyTask(BattleAnimSystem *system, SysTask *task);
void BattleAnimSystem_DestroySoundTask(BattleAnimSystem *system, SysTask *task);
u16 BattleAnimSystem_GetAttacker(BattleAnimSystem *system);
u16 BattleAnimSystem_GetDefender(BattleAnimSystem *system);
SPLEmitter *BattleAnimSystem_GetCurrentParticleSystem(BattleAnimSystem *system);
SPLEmitter *BattleAnimSystem_GetParticleSystem(BattleAnimSystem *system, int index);
SPLEmitter *BattleAnimSystem_GetEmitter(BattleAnimSystem *system, int index);
BgConfig *BattleAnimSystem_GetBgConfig(BattleAnimSystem *system);
u32 BattleAnimSystem_GetArg(BattleAnimSystem *system, int index);
ManagedSprite *BattleAnimSystem_GetSprite(BattleAnimSystem *system, int index);
ManagedSprite *BattleAnimSystem_GetBattlerSprite(BattleAnimSystem *system, int index);
SpriteManager *BattleAnimSystem_GetBattlerSpriteManager(BattleAnimSystem *system);
SpriteManager *BattleAnimSystem_GetFirstSpriteManager(BattleAnimSystem *system);
SpriteSystem *BattleAnimSystem_GetSpriteSystem(BattleAnimSystem *system);
BattleAnimSoundTaskData *BattleAnimSystem_NewSoundTaskData(BattleAnimSystem *system);
void BattleAnimSystem_StartSoundTask(BattleAnimSystem *system, BattleAnimSoundTaskData *data);
void BattleAnimTask_Sound(SysTask *task, BattleAnimSoundTaskData *data);
BOOL BattleAnimSoundFunc_None(BattleAnimSoundTaskData *data);
BOOL BattleAnimSoundFunc_PanSweep(BattleAnimSoundTaskData *data);
BOOL BattleAnimSoundFunc_PanSweep2(BattleAnimSoundTaskData *data);
BOOL BattleAnimSoundFunc_PanSweep3(BattleAnimSoundTaskData *data);
BOOL BattleAnimSoundFunc_RepeatSE(BattleAnimSoundTaskData *data);
BOOL BattleAnimSoundFunc_DelayedSE(BattleAnimSoundTaskData *data);
void BattleAnimSystem_InitBlend(void);
void BattleAnimScriptCmd_WaitForButtonCombo(BattleAnimSystem *system);
void BattleAnimScriptCmd_Wait(BattleAnimSystem *system);
void BattleAnimScriptCmd_WaitForTasks(BattleAnimSystem *system);
void BattleAnimScriptCmd_SetArg(BattleAnimSystem *system);
void BattleAnimScriptCmd_ClearArgs(BattleAnimSystem *system);
u32 BattleAnimSystem_GetMoveParam(BattleAnimSystem *system, u32 param);
void BattleAnimSystem_BuildBattlerPicParams(BattleAnimSystem *system, BattleAnimBattlerPicParams *params, u32 mode);
void BattleAnimScriptCmd_ChangeBattlerPic(BattleAnimSystem *system);
void BattleAnimScriptCmd_RestoreBattlerPic(BattleAnimSystem *system);
void BattleAnimScriptCmd_BeginLoop(BattleAnimSystem *system);
void BattleAnimScriptCmd_EndLoop(BattleAnimSystem *system);
void BattleAnimScriptCmd_End(BattleAnimSystem *system);
void BattleAnimScriptCmd_CreateEmitter(BattleAnimSystem *system);
void BattleAnimScriptCmd_CreateEmitterInSlot(BattleAnimSystem *system);
int BattleAnimSystem_SelectEmitterOf6(BattleAnimSystem *system);
int BattleAnimSystem_SelectEmitterOf4(BattleAnimSystem *system);
void BattleAnimScriptCmd_CreateEmitterByPosition6(BattleAnimSystem *system);
void BattleAnimScriptCmd_CreateEmitterByPosition4(BattleAnimSystem *system);
void BattleAnimScriptCmd_WaitForParticles(BattleAnimSystem *system);
void BattleAnimScriptCmd_LoadParticleSystem(BattleAnimSystem *system);
void BattleAnimScriptCmd_LoadParticleSystemEx(BattleAnimSystem *system);
void BattleAnimScriptCmd_FreeParticleSystem(BattleAnimSystem *system);
void BattleAnimScriptCmd_Call(BattleAnimSystem *system);
void BattleAnimScriptCmd_Return(BattleAnimSystem *system);
void BattleAnimScriptCmd_JumpIfArgEqual(BattleAnimSystem *system);
void BattleAnimScriptCmd_JumpIfContextField01(BattleAnimSystem *system);
void BattleAnimScriptCmd_CallFunc(BattleAnimSystem *system);
void BattleAnimScriptCmd_JumpByVariantBit0(BattleAnimSystem *system);
void BattleAnimScriptCmd_JumpBySide(BattleAnimSystem *system);
void BattleAnimScriptCmd_JumpByWeather(BattleAnimSystem *system);
void BattleAnimScriptCmd_JumpIfField08Is1(BattleAnimSystem *system);
void BattleAnimScriptCmd_JumpIfAttackerActive(BattleAnimSystem *system);
void BattleAnimScriptCmd_JumpIfSameSide(BattleAnimSystem *system);
void BattleAnimScriptCmd_JumpIfVariantEqual(BattleAnimSystem *system);
void BattleAnimScriptCmd_Jump(BattleAnimSystem *system);
void BattleAnimSystem_StartBrightnessFade(BattleAnimBrightnessFlashWork *work, BOOL fadeIn);
void BattleAnimTask_BrightnessFlash(SysTask *task, BattleAnimBrightnessFlashWork *work);
void BattleAnimScriptCmd_BrightnessFlash(BattleAnimSystem *system);
int BattleAnimSystem_GetBattlerByRole(BattleAnimSystem *system, u32 role);
void BattleAnimTask_SpriteFollowBattler(SysTask *task, void *data);
void BattleAnimTask_BgFollowBattler(SysTask *task, void *data);
void BattleAnimSystem_StopFollowTask(BattleAnimSystem *system, u32 index);
void BattleAnimScriptCmd_Nop6(BattleAnimSystem *system);
void BattleAnimScriptCmd_Nop7(BattleAnimSystem *system);
void BattleAnimScriptCmd_CopyBattlerToBg(BattleAnimSystem *system);
void BattleAnimScriptCmd_ClearBattlerBg(BattleAnimSystem *system);
void BattleAnimScriptCmd_InitBattlerSpriteManager(BattleAnimSystem *system);
void BattleAnimScriptCmd_LoadBattlerSpriteResources(BattleAnimSystem *system);
void BattleAnimScriptCmd_CreateBattlerSprite(BattleAnimSystem *system);
void BattleAnimScriptCmd_FreeBattlerSpriteManager(BattleAnimSystem *system);
void BattleAnimScriptCmd_DeleteBattlerSprite(BattleAnimSystem *system);
void BattleAnimTask_DrawBattlerSprites(SysTask *task, void *data);
void BattleAnimScriptCmd_StartBattlerSpriteDraw(BattleAnimSystem *system);
void BattleAnimScriptCmd_StopBattlerSpriteDraw(BattleAnimSystem *system);
void BattleAnimScriptCmd_StopFollowTask(BattleAnimSystem *system);
void BattleAnimScriptCmd_SetParticlePerspective(BattleAnimSystem *system);
void BattleAnimScriptCmd_SetParticleFlipY(BattleAnimSystem *system);
void BattleAnimScriptCmd_Nop40(BattleAnimSystem *system);
void BattleAnimScriptCmd_Nop41(BattleAnimSystem *system);
void BattleAnimSystem_StopBgEffects(BattleAnimBgEffect **effects, int kind);
void BattleAnimSystem_RegisterBgEffect(BattleAnimBgEffect **effects, void *effect, int kind);
void *BattleAnimSystem_FindBgEffect(BattleAnimBgEffect **effects, int kind);
BOOL BattleAnimBackground_ShouldFlip(BattleAnimBgSwitchWork *work, BattleAnimSystem *system, int argIndex);
void BattleAnimBackground_LoadGraphics(BattleAnimBgSwitchWork *work, BattleAnimSystem *system, GFBgLayer layer, u32 bgId);
BOOL BattleAnimBackground_HasEffectFlags(u32 flags, u32 mask);
void BattleAnimBackground_StartEffects(BattleAnimBgSwitchWork *work);
BattleAnimBgSwitchWork *BattleAnimBackground_NewSwitchWork(BattleAnimSystem *system);
BOOL BattleAnimBackground_BlendIn(SysTask *task, BattleAnimBgSwitchWork *work);
BOOL BattleAnimBackground_BlendOut(SysTask *task, BattleAnimBgSwitchWork *work);
BOOL BattleAnimBackground_FadeIn(SysTask *task, BattleAnimBgSwitchWork *work);
BOOL BattleAnimBackground_FadeOut(SysTask *task, BattleAnimBgSwitchWork *work);
BOOL BattleAnimBackground_StartEffectsOnly(SysTask *task, BattleAnimBgSwitchWork *work);
BOOL BattleAnimBackground_StartEffectsAndStopMarked(SysTask *task, BattleAnimBgSwitchWork *work);
void BattleAnimTask_BgScroll(SysTask *task, void *data);
void BattleAnimBackground_StopMarkedEffects(BattleAnimBgSwitchWork *work);
u32 BattleAnimSystem_GetBattleBgPaletteMask(BattleAnimSystem *system);
BOOL BattleAnimBackground_EffectNone(BattleAnimBgSwitchWork *work);
BOOL BattleAnimBackground_StartScroll(BattleAnimBgSwitchWork *work);
BOOL BattleAnimBackground_StartWave(BattleAnimBgSwitchWork *work);
BOOL BattleAnimBackground_MarkWaveForStop(BattleAnimBgSwitchWork *work);
void BattleAnimTask_BgWave(SysTask *task, void *data);
void BattleAnimTask_BgPaletteCycle(SysTask *task, void *data);
BOOL BattleAnimBackground_StartPaletteCycle(BattleAnimBgSwitchWork *work);
BOOL BattleAnimBackground_MarkPaletteCycleForStop(BattleAnimBgSwitchWork *work);
void BattleAnimBackground_LoadBattleBgToLayerDefault(BattleAnimSystem *system, GFBgLayer layer);
void BattleAnimBackground_LoadBattleBgToLayer(BattleAnimSystem *system, GFBgLayer layer, int colorMode);
void BattleAnimBackground_ResetLayer(BattleAnimSystem *system, GFBgLayer layer);
BOOL BattleAnimBackground_MarkScrollForStop(BattleAnimBgSwitchWork *work);
BOOL BattleAnimBackground_StopAllEffects(BattleAnimBgSwitchWork *work);
void BattleAnimTask_BackgroundSwitch(SysTask *task, void *data);
void BattleAnimScriptCmd_SwitchBackground(BattleAnimSystem *system);
void BattleAnimScriptCmd_SwitchBackgroundWithFadeWait(BattleAnimSystem *system);
void BattleAnimScriptCmd_SetBgScroll(BattleAnimSystem *system);
void BattleAnimScriptCmd_RestoreBackground(BattleAnimSystem *system);
void BattleAnimScriptCmd_WaitForBackgroundLoaded(BattleAnimSystem *system);
void BattleAnimScriptCmd_WaitForBackgroundSwitch(BattleAnimSystem *system);
void BattleAnimScriptCmd_SetBackground(BattleAnimSystem *system);
void BattleAnimScriptCmd_SwitchBackgroundBySide(BattleAnimSystem *system);
void BattleAnimScriptCmd_Nop37(BattleAnimSystem *system);
void BattleAnimScriptCmd_Nop38(BattleAnimSystem *system);
void BattleAnimScriptCmd_Nop39(BattleAnimSystem *system);
void BattleAnimScriptCmd_Nop42(BattleAnimSystem *system);
void BattleAnimScriptCmd_Nop43(BattleAnimSystem *system);
void BattleAnimScriptCmd_PlaySE(BattleAnimSystem *system);
void BattleAnimScriptCmd_StopSE(BattleAnimSystem *system);
void BattleAnimScriptCmd_PlaySEWithPan(BattleAnimSystem *system);
void BattleAnimScriptCmd_SetSEPan(BattleAnimSystem *system);
void BattleAnimScriptCmd_PanSweepSE(BattleAnimSystem *system);
void BattleAnimScriptCmd_PanSweepSERaw(BattleAnimSystem *system);
void BattleAnimScriptCmd_PanSweepSEConverted(BattleAnimSystem *system);
void BattleAnimScriptCmd_RepeatSE(BattleAnimSystem *system);
void BattleAnimScriptCmd_DelayedSE(BattleAnimSystem *system);
void BattleAnimScriptCmd_Nop28(BattleAnimSystem *system);
void BattleAnimScriptCmd_WaitForSoundTasks(BattleAnimSystem *system);
void BattleAnimScriptCmd_SetSubScreenBlend(BattleAnimSystem *system);
void BattleAnimScriptCmd_ResetBlend(BattleAnimSystem *system);
void BattleAnimScriptCmd_Nop27(BattleAnimSystem *system);
void BattleAnimScriptCmd_InitSpriteManager(BattleAnimSystem *system);
void BattleAnimScriptCmd_LoadCharResource(BattleAnimSystem *system);
void BattleAnimScriptCmd_LoadPaletteResource(BattleAnimSystem *system);
void BattleAnimScriptCmd_LoadCellResource(BattleAnimSystem *system);
void BattleAnimScriptCmd_LoadAnimResource(BattleAnimSystem *system);
void BattleAnimScriptCmd_CreateSpriteWithFunc(BattleAnimSystem *system);
void BattleAnimScriptCmd_CreateSprite(BattleAnimSystem *system);
void BattleAnimScriptCmd_FreeSpriteManager(BattleAnimSystem *system);
void BattleAnimScriptCmd_SetBattlerSpriteVisible(BattleAnimSystem *system);
void BattleAnimScriptCmd_Invalid55(BattleAnimSystem *system);
void BattleAnimScriptCmd_PlayCry(BattleAnimSystem *system);
void BattleAnimScriptCmd_WaitForCry(BattleAnimSystem *system);
void BattleAnimScriptCmd_Nop54(BattleAnimSystem *system);
BattleAnimScriptCmd BattleAnimSystem_GetScriptCmd(u32 cmd);
u8 BattleAnimContext_GetField01(BattleAnimContext *context);
s8 BattleAnimSystem_ConvertPan(BattleAnimSystem *system, int pan);
s8 BattleAnimSystem_GetPanStep(int start, int end, int step);
BOOL BattleAnimSystem_ReadScriptArgs(BattleAnimSystem *system, u32 *out, int count);
void BattleAnimSystem_GetSpriteTemplate(ManagedSpriteTemplate *template, BattleAnimSystem *system);
u8 BattleAnimSystem_GetBattlerType(BattleAnimSystem *system, int battler);
u8 BattleAnimSystem_GetBattlerGender(BattleAnimSystem *system, int battler);
u16 BattleAnimSystem_GetBattlerSpecies(BattleAnimSystem *system, int battler);
u8 BattleAnimSystem_GetBattlerForm(BattleAnimSystem *system, int battler);
u32 BattleAnimSystem_GetBattlerPersonality(BattleAnimSystem *system, int battler);
Pokepic *BattleAnimSystem_GetBattlerPokepic(BattleAnimSystem *system, int battler);
PaletteData *BattleAnimSystem_GetPaletteData(BattleAnimSystem *system);
int BattleAnimSystem_GetBattlerPaletteId(BattleAnimSystem *system, int battler);
int BattleAnimSystem_GetBattlerNarcId(BattleAnimSystem *system, int battler);
int BattleAnimSystem_GetBattlerHeight(BattleAnimSystem *system, int battler);
BOOL BattleAnimSystem_IsDoubleBattle(BattleAnimSystem *system);
BOOL BattleAnimSystem_IsBattlerSemiInvulnerable(BattleAnimSystem *system, int battler);
int BattleAnimSystem_GetBasePriority(BattleAnimSystem *system);
u32 BattleAnimSystem_GetBgLayer(BattleAnimSystem *system, int index);
u32 BattleAnimSystem_GetBgMaskLayer(BattleAnimSystem *system, int index);
int BattleAnimSystem_GetBgPriority(BattleAnimSystem *system, int kind);
void BattleAnimSystem_LoadBattleBgTiles(BattleAnimSystem *system, GFBgLayer layer);
void BattleAnimSystem_LoadBattleBgPalette(BattleAnimSystem *system);
BOOL BattleAnimSystem_ShouldFlipSide(BattleAnimSystem *system, int side);
u32 BattleAnimSystem_GetBackgroundFile(u32 bgId, int kind);
u8 BattleAnimIndexedBytes_GetCurrentValue(BattleAnimIndexedBytes *data);
void BattleAnimEmitterCb_None(SPLEmitter *emitter);
SPLEmitter *BattleAnimSystem_NewParticleSystem(enum HeapID heapId, int resId, BOOL loadNow);
SPLEmitter *BattleAnimSystem_NewParticleSystemFromNarc(enum HeapID heapId, NarcId narcId, int resId, BOOL loadNow);
void *BattleAnimSystem_ReadNarcMember(NARC *narc, int member, enum HeapID heapId);
SPLEmitter *BattleAnimSystem_NewParticleSystemFromOpenNarc(NARC *narc, enum HeapID heapId, int member, BOOL loadNow);
SPLEmitter *BattleAnimSystem_CreateEmitter(SPLEmitter *particleSystem, int resId, int callback, BattleAnimSystem *system);
void BattleAnimSystem_FreeParticleSystem(SPLEmitter *particleSystem);
int BattleAnimSystem_GetSideSign(BattleAnimSystem *system, int battlerA, int battlerB);
void BattleAnimEmitterCb_Dummy(void);

s16 BattleAnimSystem_GetBattlerScreenCoord(BattleAnimSystem *system, int battler, u32 coord);
void BattleAnimUtil_LoadSubstituteIntoPic(BattleAnimBattlerPicParams *params, enum HeapID heapId);
void BattleAnimUtil_LoadMonIntoPicSameFacing(BattleAnimBattlerPicParams *params, enum HeapID heapId);
void BattleAnimUtil_LoadMonIntoPicOppositeFacing(BattleAnimBattlerPicParams *params, enum HeapID heapId);
void BattleAnimUtil_RestorePic(BattleAnimBattlerPicParams *params, enum HeapID heapId);

// Sound effect panning
void SetSeqTrackPan(u16 seqNo, u32 trackMask, int pan);
void SetAllSEPan(int pan);

// Particle system camera
void ParticleSystem_GetCameraUp(SPLEmitter *particleSystem, VecFx32 *out);
void ParticleSystem_SetCameraUp(SPLEmitter *particleSystem, VecFx32 *up);

#endif // POKEHEARTGOLD_BATTLE_BATTLE_ANIM_INTERNAL_H
