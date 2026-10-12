#ifndef POKEHEARTGOLD_FIELD_ENCOUNTER_EFFECT_H
#define POKEHEARTGOLD_FIELD_ENCOUNTER_EFFECT_H

#include "bg_window.h"
#include "field_system.h"
#include "filesystem.h"
#include "sprite.h"
#include "sys_task.h"

// Screen transitions played when a battle starts. Overlay 01 keeps a table of one callback per
// effect id (BattleStartGetTransition picks the id); it loads the overlay holding the effect and
// runs the callback as a SysTask until the effect calls EncounterEffect_Finish.
//   0-5   wild battles (overlay 120)          6-11  trainer battles (overlay 119)
//   12-27 gym leaders, 28 rival, 29-32 Elite Four, 33 champion (overlay 115)
//   39-44 Team Rocket (overlay 117)           45    Kimono Girls (overlay 118)
//   46    PKMN Trainer Red (overlay 120)

typedef struct EncounterEffectTask {
    u32 state;
    BOOL fadeDone; // set by EncounterEffect_StartFlash when its brightness fade ends
    u32 unk_08;    // frame counter in some effects
    void *work;    // effect-specific, freed by EncounterEffect_Finish
    FieldSystem *fieldSystem;
    BOOL *done;  // set to TRUE by the effect when the screen is ready for the battle
    BOOL unk_18; // read by ov01_021EFE30
    u32 unk_1C;
    NARC *narc; // deleted by EncounterEffect_Finish
} EncounterEffectTask;

// Interpolators run by overlay 01 (one step per frame, value in fx32 or plain units)
typedef struct EncounterEffectInterp {
    s32 value;
    s32 start;
    s32 delta;
    s32 frame;
    s32 duration;
} EncounterEffectInterp;

typedef struct EncounterEffectQuadInterp {
    s32 value;
    s32 start;
    s32 velocity;
    s32 acceleration;
    s32 frame;
    s32 duration;
} EncounterEffectQuadInterp;

// Sprite manager and one resource set (character, palette, cell, animation) set up by overlay 01
typedef struct EncounterEffectSpriteManager {
    SpriteList *spriteList;
    u8 unk_04[0x128];
    void *resourceManagers[4];
} EncounterEffectSpriteManager;

typedef struct EncounterEffectSpriteResources {
    void *resourceObjects[4];
    u8 unk_10[0x24];
} EncounterEffectSpriteResources;

// HBlank BG offset wave run by overlay 01
typedef struct EncounterEffectHBlankWave {
    u8 unk_00[0xC];
} EncounterEffectHBlankWave;

// Encounter effect core (src/field/encounter_effect.c): overlay loading, the effect task, brightness flashes
// and the interpolators.
void Task_RunEncounterEffect(SysTask *task, EncounterEffectTask *effect);
void EncounterEffect_LoadOverlay(s32 effect);
void EncounterEffect_UnloadOverlay(s32 effect);
void EncounterEffect_Start(s32 effect, FieldSystem *fieldSystem, BOOL *done);
void EncounterEffect_Finish(EncounterEffectTask *effect, SysTask *task);
void EncounterEffect_StartFlash(int screen, int brightness, int otherBrightness, BOOL *done, int count);
BOOL ov01_021EFE30(EncounterEffectTask *effect);
void EncounterEffectInterp_Init(EncounterEffectInterp *interp, s32 start, s32 end, s32 duration);
BOOL EncounterEffectInterp_Update(EncounterEffectInterp *interp);
void EncounterEffectInterp_InitFx(EncounterEffectInterp *interp, s32 start, s32 end, s32 duration);
BOOL EncounterEffectInterp_UpdateFx(EncounterEffectInterp *interp);
void EncounterEffectQuadInterp_Init(EncounterEffectQuadInterp *interp, s32 start, s32 end, s32 velocity, s32 duration);
BOOL EncounterEffectQuadInterp_Update(EncounterEffectQuadInterp *interp);

// Overlay 01 helpers used by the effects
void ov01_021F05C4(EncounterEffectSpriteManager *spriteManager, int maxSprites, int resourceCount);
void ov01_021F05F4(EncounterEffectSpriteManager *spriteManager);
void ov01_021F0614(NARC *narc, EncounterEffectSpriteManager *spriteManager, EncounterEffectSpriteResources *resources, u32 charMember, int compressed, u32 plttMember, u32 cellMember, u32 animMember, u32 resourceId);
void ov01_021F06EC(EncounterEffectSpriteManager *spriteManager, EncounterEffectSpriteResources *resources);
Sprite *ov01_021F0718(EncounterEffectSpriteManager *spriteManager, EncounterEffectSpriteResources *resources, int x, int y, int z, int animSeq);
void ov01_021F0960(Window *window, s16 top, s16 bottom, s16 left, s16 right, u8 fillColor);
void ov01_021F12B4(EncounterEffectHBlankWave *wave, int heapID);
void ov01_021F12D0(EncounterEffectHBlankWave *wave);
void ov01_021F12E8(EncounterEffectHBlankWave *wave, int startLine, int endLine, int arg3, int arg4, int arg5, void *reg, int arg7, int heapID);

// Effect callbacks (entries of overlay 01's effect table)
void EncounterEffect_WildRowsWhite(SysTask *task, EncounterEffectTask *effect);
void EncounterEffect_WildRowsBlack(SysTask *task, EncounterEffectTask *effect);
void EncounterEffect_WildWaveWhite(SysTask *task, EncounterEffectTask *effect);
void EncounterEffect_WildWaveBlack(SysTask *task, EncounterEffectTask *effect);
void EncounterEffect_WildDiagonalsWhite(SysTask *task, EncounterEffectTask *effect);
void EncounterEffect_WildDiagonalsBlack(SysTask *task, EncounterEffectTask *effect);
void EncounterEffect_TrainerSpinApart(SysTask *task, EncounterEffectTask *effect);
void EncounterEffect_TrainerSpinZoom(SysTask *task, EncounterEffectTask *effect);
void EncounterEffect_TrainerBallDrop(SysTask *task, EncounterEffectTask *effect);
void EncounterEffect_TrainerFadeSpin(SysTask *task, EncounterEffectTask *effect);
void EncounterEffect_TrainerBallFall(SysTask *task, EncounterEffectTask *effect);
void EncounterEffect_TrainerRisingBall(SysTask *task, EncounterEffectTask *effect);
void EncounterEffect_LeaderFalkner(SysTask *task, EncounterEffectTask *effect);
void EncounterEffect_LeaderBugsy(SysTask *task, EncounterEffectTask *effect);
void EncounterEffect_LeaderWhitney(SysTask *task, EncounterEffectTask *effect);
void EncounterEffect_LeaderMorty(SysTask *task, EncounterEffectTask *effect);
void EncounterEffect_LeaderJasmine(SysTask *task, EncounterEffectTask *effect);
void EncounterEffect_LeaderChuck(SysTask *task, EncounterEffectTask *effect);
void EncounterEffect_LeaderPryce(SysTask *task, EncounterEffectTask *effect);
void EncounterEffect_LeaderClair(SysTask *task, EncounterEffectTask *effect);
void EncounterEffect_LeaderBrock(SysTask *task, EncounterEffectTask *effect);
void EncounterEffect_LeaderMisty(SysTask *task, EncounterEffectTask *effect);
void EncounterEffect_LeaderLtSurge(SysTask *task, EncounterEffectTask *effect);
void EncounterEffect_LeaderErika(SysTask *task, EncounterEffectTask *effect);
void EncounterEffect_LeaderJanine(SysTask *task, EncounterEffectTask *effect);
void EncounterEffect_LeaderSabrina(SysTask *task, EncounterEffectTask *effect);
void EncounterEffect_LeaderBlaine(SysTask *task, EncounterEffectTask *effect);
void EncounterEffect_LeaderBlue(SysTask *task, EncounterEffectTask *effect);
void EncounterEffect_Rival(SysTask *task, EncounterEffectTask *effect);
void EncounterEffect_EliteFourWill(SysTask *task, EncounterEffectTask *effect);
void EncounterEffect_EliteFourKoga(SysTask *task, EncounterEffectTask *effect);
void EncounterEffect_EliteFourBruno(SysTask *task, EncounterEffectTask *effect);
void EncounterEffect_EliteFourKaren(SysTask *task, EncounterEffectTask *effect);
void EncounterEffect_Champion(SysTask *task, EncounterEffectTask *effect);
void EncounterEffect_TeamRocket(SysTask *task, EncounterEffectTask *effect);
void EncounterEffect_ExecutivePetrel(SysTask *task, EncounterEffectTask *effect);
void EncounterEffect_ExecutiveProton(SysTask *task, EncounterEffectTask *effect);
void EncounterEffect_ExecutiveArcher(SysTask *task, EncounterEffectTask *effect);
void EncounterEffect_ExecutiveAriana(SysTask *task, EncounterEffectTask *effect);
void EncounterEffect_RocketBoss(SysTask *task, EncounterEffectTask *effect);
void EncounterEffect_KimonoGirl(SysTask *task, EncounterEffectTask *effect);
void EncounterEffect_PkmnTrainerRed(SysTask *task, EncounterEffectTask *effect);

#endif // POKEHEARTGOLD_FIELD_ENCOUNTER_EFFECT_H
