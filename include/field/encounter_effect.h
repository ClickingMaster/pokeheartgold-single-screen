#ifndef POKEHEARTGOLD_FIELD_ENCOUNTER_EFFECT_H
#define POKEHEARTGOLD_FIELD_ENCOUNTER_EFFECT_H

#include "bg_window.h"
#include "field_system.h"
#include "filesystem.h"
#include "scanline_effect.h"
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
    u32 timer;     // frame counter in some effects
    void *work;    // effect-specific, freed by EncounterEffect_Finish
    FieldSystem *fieldSystem;
    BOOL *done;    // set to TRUE by the effect when the screen is ready for the battle
    BOOL wipeDone; // read by EncounterEffect_IsWipeDone
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
    G2dRenderer renderer;
    GF_2DGfxResMan *resourceManagers[4];
} EncounterEffectSpriteManager;

typedef struct EncounterEffectSpriteResources {
    SpriteResource *resourceObjects[4];
    SpriteResourcesHeader header;
} EncounterEffectSpriteResources;

// HBlank BG offset wave run by overlay 01
typedef struct EncounterEffectHBlankWave {
    ScanlineEffect *effect;
    SysTask *task; // swaps the scanline buffers every other frame
    u32 frames;
} EncounterEffectHBlankWave;

// Encounter effect core: overlay loading, the effect task, brightness flashes
// and the interpolators.
void Task_RunEncounterEffect(SysTask *task, EncounterEffectTask *effect);
void EncounterEffect_LoadOverlay(s32 effect);
void EncounterEffect_UnloadOverlay(s32 effect);
void EncounterEffect_Start(s32 effect, FieldSystem *fieldSystem, BOOL *done);
void EncounterEffect_Finish(EncounterEffectTask *effect, SysTask *task);
void EncounterEffect_StartFlash(int screen, int brightness, int otherBrightness, BOOL *done, int count);
BOOL EncounterEffect_IsWipeDone(EncounterEffectTask *effect);
int EncounterEffect_GetDefaultEffect(BattleSetup *setup);
void EncounterEffectInterp_Init(EncounterEffectInterp *interp, s32 start, s32 end, s32 duration);
BOOL EncounterEffectInterp_Update(EncounterEffectInterp *interp);
void EncounterEffectInterp_InitFx(EncounterEffectInterp *interp, s32 start, s32 end, s32 duration);
BOOL EncounterEffectInterp_UpdateFx(EncounterEffectInterp *interp);
void EncounterEffectQuadInterp_Init(EncounterEffectQuadInterp *interp, s32 start, s32 end, s32 velocity, s32 duration);
BOOL EncounterEffectQuadInterp_Update(EncounterEffectQuadInterp *interp);

// Window wipes and BG loaders
void EncounterEffect_LoadBgGraphics(NARC *narc, int screenMember, int charMember, int paletteMember, int palette, int paletteCount, BgConfig *bgConfig, int bgId);
void EncounterEffect_LoadBgGraphicsMirrored(NARC *narc, int screenMember, int charMember, int paletteMember, int palette, int paletteCount, BgConfig *bgConfig, int bgId, int mirrorBgId);

typedef struct StripeWipe StripeWipe;
typedef struct SplitWipe SplitWipe;

StripeWipe *StripeWipe_New(void);
void StripeWipe_Free(StripeWipe *wipe);
void StripeWipe_Start(EncounterEffectTask *effect, StripeWipe *wipe, u32 stripeHeight, s32 duration, s32 start, s32 end, s32 velocity);
SplitWipe *SplitWipe_New(void);
void SplitWipe_Free(SplitWipe *wipe);
void SplitWipe_Start(EncounterEffectTask *effect, SplitWipe *wipe, s32 duration, s32 velocityX, s32 velocityY);

// Sprites, filled rectangles, wipes and main screen brightness
void EncounterEffect_InitSprites(EncounterEffectSpriteManager *spriteManager, int maxSprites, int resourceCount);
void EncounterEffect_FreeSprites(EncounterEffectSpriteManager *spriteManager);
void EncounterEffect_LoadSpriteResources(NARC *narc, EncounterEffectSpriteManager *spriteManager, EncounterEffectSpriteResources *resources, int plttMember, int plttCount, int charMember, int cellMember, int animMember, int resourceId);
void EncounterEffect_FreeSpriteResources(EncounterEffectSpriteManager *spriteManager, EncounterEffectSpriteResources *resources);
Sprite *EncounterEffect_CreateSprite(EncounterEffectSpriteManager *spriteManager, EncounterEffectSpriteResources *resources, fx32 x, fx32 y, fx32 z, int priority);
void EncounterEffect_SetVecFx32(VecFx32 *vec, fx32 x, fx32 y, fx32 z);
void EncounterEffect_FillWindowRect(Window *window, s16 top, s16 bottom, s16 left, s16 right, u8 fill);
SysTask *EncounterEffect_ApplyMainBrightness(EncounterEffectInterp *brightness);

typedef struct FilledRect FilledRect;
typedef struct BlockWipe BlockWipe;
typedef struct ZigzagWipe ZigzagWipe;

FilledRect *RectTrail_New(enum HeapID heapID);
void RectTrail_Free(FilledRect *rect);
void RectTrail_Start(FilledRect *rect, int startX, int endX, int startY, int endY, s32 duration, Window *window, int width, int height, u8 fill);
BOOL RectTrail_Update(FilledRect *rect);
BlockWipe *BlockWipe_New(enum HeapID heapID);
void BlockWipe_Free(BlockWipe *wipe);
void BlockWipe_Start(BlockWipe *wipe, u8 duration, u8 delay, Window *window, u8 fill);
BOOL BlockWipe_Update(BlockWipe *wipe);
ZigzagWipe *ZigzagWipe_New(void);
void ZigzagWipe_Free(ZigzagWipe *wipe);
SysTask *ZigzagWipe_Start(EncounterEffectTask *effect, ZigzagWipe *wipe, s32 duration, u32 period, u32 width, u32 insidePlanes, u32 outsidePlanes);

// State of the running encounter effect
typedef struct EncounterEffectParticles EncounterEffectParticles;

typedef struct EncounterEffectState {
    EncounterEffectParticles *particles; // particle mode of the leader effects
    s32 effect;
    u32 frameCount;
} EncounterEffectState;

extern EncounterEffectState gEncounterEffectState;

// HBlank BG offset wave
void EncounterEffectHBlankWave_Init(EncounterEffectHBlankWave *wave, int heapID);
void EncounterEffectHBlankWave_Free(EncounterEffectHBlankWave *wave);
void EncounterEffectHBlankWave_Start(EncounterEffectHBlankWave *wave, u8 firstLine, u8 lastLine, u32 step, fx32 amplitude, s16 speed, void *destination, u32 fill, u32 priority);
void EncounterEffectHBlankWave_AlternateDirection(EncounterEffectHBlankWave *wave, u32 period);

// Particle mode of the leader effects
void EncounterEffect_StartParticleMode(FieldSystem *fieldSystem);
void EncounterEffect_RequestParticleSetup(void);
u8 EncounterEffect_IsParticleSetupDone(void);
void EncounterEffect_DrawParticles(void);
void EncounterEffect_EndParticleMode(void);
void EncounterEffect_LoadParticles(NARC *narc, int member);
void EncounterEffect_CreateEmitters(u32 count);
BOOL EncounterEffect_UpdateParticles(void);
BOOL EncounterEffect_AreParticlesDone(void);
void EncounterEffect_FreeParticles(void);

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
