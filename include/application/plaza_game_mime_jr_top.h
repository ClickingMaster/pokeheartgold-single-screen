#ifndef POKEHEARTGOLD_APPLICATION_PLAZA_GAME_MIME_JR_TOP_H
#define POKEHEARTGOLD_APPLICATION_PLAZA_GAME_MIME_JR_TOP_H

#include "global.h"

#include "application/plaza_game.h"

#include "bg_window.h"
#include "camera.h"
#include "filesystem.h"
#include "gf_3d_vramman.h"
#include "model_animation.h"
#include "overlay_manager.h"
#include "palette.h"
#include "sprite_system.h"
#include "sys_task_api.h"

// Mime Jr. Top (overlay 92), a Wi-Fi Plaza game: Mime Jr. balances on a spinning top and every
// player rubs the touch screen to keep the top upright. While the top keeps spinning the score
// grows, the spin tier rises (bonus points, new effects on the top) and the stage gets livelier;
// when the top tilts too far Mime Jr. falls off and the spin starts over. The other players'
// tops are shown as small sprites. The player list and the results screen come from the shared
// Plaza game code (application/plaza_game.h). The parent keeps the game clock.

// A value moving from start to start + delta over duration frames (linear, or with a start
// velocity and constant acceleration).
typedef struct MimeJrTopTween {
    fx32 value;    // 0x00
    fx32 start;    // 0x04
    fx32 velocity; // 0x08
    fx32 delta;    // 0x0C, acceleration for the accelerated tween
    int frame;     // 0x10
    int duration;  // 0x14
} MimeJrTopTween;

typedef struct MimeJrTopQuat {
    f32 v[4]; // w, x, y, z
} MimeJrTopQuat;

typedef struct MimeJrTopFloatMatrix {
    f32 m[4][4];
} MimeJrTopFloatMatrix;

// A 3D model with its own rotation (a quaternion turned by touch input).
typedef struct MimeJrTopModel {
    ModelInstance instance;              // 0x000
    ModelResource resource;              // 0x078
    ModelAnimation animations[3];        // 0x088
    u8 unkC4[0x8C];                      // 0x0C4
    MimeJrTopFloatMatrix rotationMatrix; // 0x150
    MimeJrTopQuat rotation;              // 0x190
    MimeJrTopQuat lastTurn;              // 0x1A0
    MtxFx43 rotationFx;                  // 0x1B0
    VecFx32 position;                    // 0x1E0
    BOOL animate;                        // 0x1EC
    BOOL loaded;                         // 0x1F0
    int animation;                       // 0x1F4, the animation added to the instance
    int spinFrames;                      // 0x1F8, frames the last flick keeps turning the model
    f32 spinX;                           // 0x1FC
    f32 spinY;                           // 0x200
    f32 spinStepX;                       // 0x204
    f32 spinStepY;                       // 0x208
} MimeJrTopModel;

// Part of MimeJrTopApp handed to the game screen.
typedef struct MimeJrTopResources {
    NARC *narc;                   // 0x00, NARC 193 (models, sprites, BGs)
    NARC *unkNarc;                // 0x04, NARC 189 (opened, not read here)
    SpriteSystem *spriteSystem;   // 0x08
    SpriteManager *spriteManager; // 0x0C
    BgConfig *bgConfig;           // 0x10
    PaletteData *palette;         // 0x14
    GF3DVramMan *vramMan;         // 0x18
    Camera *camera;               // 0x1C
    u32 unk20;                    // 0x20
    NNSFndAllocator allocator;    // 0x24
    u8 connectionLost;            // 0x34, every task stops when set
    u8 unk35[3];                  // 0x35
} MimeJrTopResources;

// Run on the parent only: the game clock.
typedef struct MimeJrTopClock {
    u32 unk00;                     // 0x00
    u32 playerCount;               // 0x04
    s16 timer;                     // 0x08
    u16 unk0A;                     // 0x0A
    MimeJrTopResources *resources; // 0x0C
} MimeJrTopClock;

// Mime Jr. falling off the top.
typedef struct MimeJrTopFallTask {
    int state;                     // 0x00
    int step;                      // 0x04, 0 = falling off, 1 = climbing back
    int timer;                     // 0x08
    int *pose;                     // 0x0C
    MimeJrTopModel *mimeJr;        // 0x10
    MimeJrTopModel *top;           // 0x14
    u8 unk18[8];                   // 0x18
    VecFx32 tilt;                  // 0x20
    u8 unk2C[0x18];                // 0x2C
    MimeJrTopTween tweenY;         // 0x44
    MimeJrTopTween tweenZ;         // 0x5C
    BOOL selection;                // 0x74
    MimeJrTopResources *resources; // 0x78
} MimeJrTopFallTask;

// Another player's top falling over.
typedef struct MimeJrTopRivalFallTask {
    int state;                     // 0x00
    int step;                      // 0x04
    int timer;                     // 0x08
    ManagedSprite *face;           // 0x0C
    ManagedSprite *body;           // 0x10
    MimeJrTopTween tween;          // 0x14
    MimeJrTopResources *resources; // 0x2C
} MimeJrTopRivalFallTask;

// Another player's tier badge (pulses while the tier holds).
typedef struct MimeJrTopRivalTierTask {
    int state;                     // 0x00
    int mode;                      // 0x04
    int counter;                   // 0x08
    ManagedSprite *sprite;         // 0x0C
    MimeJrTopTween tween;          // 0x10
    int previous;                  // 0x28
    int *current;                  // 0x2C
    BOOL active;                   // 0x30
    u8 unk34[8];                   // 0x34
    MimeJrTopResources *resources; // 0x3C
    BOOL selection;                // 0x40
} MimeJrTopRivalTierTask;

// The local player's tier models (swapped in when the tier changes).
typedef struct MimeJrTopTierModelsTask {
    int state;                     // 0x00
    int unk04;                     // 0x04
    u8 unk08[0x20];                // 0x08
    int previous;                  // 0x28
    int *current;                  // 0x2C
    BOOL active;                   // 0x30
    MimeJrTopModel *modelA;        // 0x34
    MimeJrTopModel *modelB;        // 0x38
    MimeJrTopResources *resources; // 0x3C
    BOOL selection;                // 0x40
} MimeJrTopTierModelsTask;

typedef union MimeJrTopPlayerTask {
    MimeJrTopRivalTierTask rival;
    MimeJrTopTierModelsTask local;
} MimeJrTopPlayerTask;

// Three sparks thrown out where the screen is rubbed.
typedef struct MimeJrTopBurst {
    BOOL active;                   // 0x00
    int state;                     // 0x04
    ManagedSprite *sprites[3];     // 0x08
    MimeJrTopTween tweens[3][2];   // 0x14, x and y per spark
    MimeJrTopResources *resources; // 0xA4
} MimeJrTopBurst;

// One star of the star shower.
typedef struct MimeJrTopStar {
    ManagedSprite *sprite; // 0x00
    fx32 driftX;           // 0x04
    fx32 driftY;           // 0x08
    fx32 originX;          // 0x0C
    fx32 originY;          // 0x10
    fx32 jitterX;          // 0x14
    fx32 jitterY;          // 0x18
    fx32 circleX;          // 0x1C
    fx32 circleY;          // 0x20
    u8 type;               // 0x24
    u8 visible;            // 0x25
    u8 delay;              // 0x26
    u8 jitterPhase;        // 0x27
    int angle;             // 0x28
    s16 fallSpeed;         // 0x2C
    s16 radiusX;           // 0x2E
    s16 radiusY;           // 0x30
    s16 timer;             // 0x32
} MimeJrTopStar;

typedef struct MimeJrTopGame MimeJrTopGame;

typedef struct MimeJrTopStarsTask {
    MimeJrTopGame *game;     // 0x000
    s32 state;               // 0x004
    s32 timer;               // 0x008
    s32 group;               // 0x00C
    s32 *stop;               // 0x010
    MimeJrTopStar stars[36]; // 0x014
} MimeJrTopStarsTask;

// One effect of the stage animation (flash, pop-ups, audience, stars).
typedef struct MimeJrTopStageEffect {
    int state;         // 0x00
    BOOL active;       // 0x04
    void *targets[8];  // 0x08
    int timer;         // 0x28
    u8 unk2C[0x20];    // 0x2C
    int audienceTimer; // 0x4C
    BOOL stop;         // 0x50
    BOOL starsRunning; // 0x54
    u8 unk58[0x20];    // 0x58
} MimeJrTopStageEffect;

// Stage effects started by the phase (how long the top has kept spinning).
typedef struct MimeJrTopStage {
    u32 previousPhase;               // 0x000
    u32 *phase;                      // 0x004
    MimeJrTopStageEffect effects[5]; // 0x008
    SysTask *task;                   // 0x260
    MimeJrTopResources *resources;   // 0x264
} MimeJrTopStage;

// A sub screen dancer moved along tweens or circles.
typedef struct MimeJrTopDancer {
    ManagedSprite *sprite; // 0x00
    int state;             // 0x04
    s16 angle;             // 0x08
    u8 unk0A[6];           // 0x0A
    int cycles;            // 0x10
    s16 baseX;             // 0x14
    s16 baseY;             // 0x16
    fx32 radiusX;          // 0x18
    fx32 radiusY;          // 0x1C
    MimeJrTopTween tweenX; // 0x20
    MimeJrTopTween tweenY; // 0x38
} MimeJrTopDancer;

// The sub screen dance shown from level 1 on (one routine per level).
typedef struct MimeJrTopDance {
    MimeJrTopDancer dancers[4];    // 0x000, two front and two back dancers
    int count;                     // 0x140
    int backSpeed;                 // 0x144
    int frontSpeed;                // 0x148
    int loopCount;                 // 0x14C
    u8 unk150[4];                  // 0x150
    SysTask *task;                 // 0x154
    u32 *level;                    // 0x158
    int *flashBlend;               // 0x15C
    int *danceBlend;               // 0x160
    u32 timer;                     // 0x164
    int routineState;              // 0x168
    u8 unk16C[4];                  // 0x16C
    u32 slideState;                // 0x170
    BOOL enabled;                  // 0x174
    BOOL shown;                    // 0x178
    MimeJrTopResources *resources; // 0x17C
    MimeJrTopTween slides[3];      // 0x180
} MimeJrTopDance;

// Game screen work.
struct MimeJrTopGame {
    int localIndex;                     // 0x0000
    int playerCount;                    // 0x0004
    int demoFrame;                      // 0x0008
    int demoStep;                       // 0x000C
    u32 unk10;                          // 0x0010
    MimeJrTopResources *resources;      // 0x0014
    u8 unk18[4];                        // 0x0018
    ManagedSprite *rivalFaces[3];       // 0x001C
    ManagedSprite *rivalBodies[3];      // 0x0028
    ManagedSprite *rivalBadges[3];      // 0x0034
    ManagedSprite *stars[36];           // 0x0040
    ManagedSprite *popups[4];           // 0x00D0
    ManagedSprite *backDancers[2];      // 0x00E0
    ManagedSprite *frontDancers[2];     // 0x00E8
    ManagedSprite *demoHand;            // 0x00F0
    u16 rivalSlots[4];                  // 0x00F4
    u8 unkFC[0xC];                      // 0x00FC
    int flashBlend;                     // 0x0108
    int danceBlend;                     // 0x010C
    int mimeJrPose;                     // 0x0110
    MimeJrTopModel top;                 // 0x0114
    MimeJrTopModel mimeJr[3];           // 0x0320
    MimeJrTopModel stage;               // 0x0944
    MimeJrTopModel audience[8];         // 0x0B50
    MimeJrTopModel tierModels[2];       // 0x1BB0
    VecFx32 tilt;                       // 0x1FC8
    int lastTouchX;                     // 0x1FD4
    int lastTouchY;                     // 0x1FD8
    u8 unk1FDC[4];                      // 0x1FDC
    Window instructions;                // 0x1FE0
    Window nameWindows[3];              // 0x1FF0
    u8 unk2020[0x10];                   // 0x2020
    MimeJrTopDance dance;               // 0x2030
    u8 unk21F8[0x30];                   // 0x21F8
    MimeJrTopStage stageEffects;        // 0x2228
    MimeJrTopBurst bursts[8];           // 0x2490
    MimeJrTopPlayerTask playerTasks[4]; // 0x29D0
    int wobbleTick;                     // 0x2AE0
    int frame;                          // 0x2AE4
    u32 phase;                          // 0x2AE8
    u32 tier;                           // 0x2AEC
    u32 score;                          // 0x2AF0
    u32 level;                          // 0x2AF4
    u32 previousLevel;                  // 0x2AF8
    s16 recoverTimer;                   // 0x2AFC
    s16 levelTimer;                     // 0x2AFE
    s16 rivalSpinFrames[4];             // 0x2B00
    u32 rivalTiers[4];                  // 0x2B08
    int spinFrames;                     // 0x2B18
    s16 wobbleX;                        // 0x2B1C
    s16 wobbleY;                        // 0x2B1E
    s16 wobbleTime;                     // 0x2B20
    s16 wobbleStep;                     // 0x2B22
    MimeJrTopTween settleX;             // 0x2B24
    MimeJrTopTween settleY;             // 0x2B3C
    MimeJrTopTween kickX;               // 0x2B54
    MimeJrTopTween kickY;               // 0x2B6C
    BOOL touched;                       // 0x2B84
    BOOL settled;                       // 0x2B88
    BOOL wobbleDue;                     // 0x2B8C
    BOOL wobbling;                      // 0x2B90
    BOOL everTouched;                   // 0x2B94
    s16 nextWobbleX;                    // 0x2B98
    s16 nextWobbleY;                    // 0x2B9A
    u32 playerStates[4];                // 0x2B9C
    u32 unk2BAC;                        // 0x2BAC
    u32 seed;                           // 0x2BB0
    BOOL selection;                     // 0x2BB4
    BOOL rivalFallShown;                // 0x2BB8
};

// Command 22: sent by the parent to start the game.
typedef struct MimeJrTopStartMessage {
    u8 selection; // 0x00
    u8 unk01[3];  // 0x01
    u32 seed;     // 0x04
} MimeJrTopStartMessage;

typedef struct MimeJrTopPlayerScore {
    u32 score; // 0x00
    u32 unk04; // 0x04
} MimeJrTopPlayerScore;

// Overlay work (heap 0x71).
typedef struct MimeJrTopApp {
    MimeJrTopClock *clock;          // 0x00
    MimeJrTopGame *game;            // 0x04
    BOOL startReceived;             // 0x08
    BOOL finishReceived;            // 0x0C
    u32 unk10;                      // 0x10
    MimeJrTopStartMessage start;    // 0x14
    u32 finalScore;                 // 0x1C, command 25 (8 bytes)
    u8 unk20[4];                    // 0x20
    MimeJrTopPlayerScore scores[4]; // 0x24
    int netId;                      // 0x44
    MimeJrTopResources resources;   // 0x48
    u32 savedSeed;                  // 0x80
    PlazaGameSign *sign;            // 0x84
    PlazaGameArgs *args;            // 0x88
    PlazaGameRoster roster;         // 0x8C
    PlazaGameEntry *entry;          // 0x9C
    PlazaGameResultData resultData; // 0xA0
    PlazaGameResults *results;      // 0xB8
    u32 netIds[4];                  // 0xBC
    BOOL keepPlaying;               // 0xCC
} MimeJrTopApp;

void MimeJrTop_ReceiveStart(int netId, int size, volatile u32 *data, MimeJrTopApp *app);
void MimeJrTop_ReceiveFinish(int netId, int size, void *data, MimeJrTopApp *app);
void MimeJrTop_ReceivePlayerState(int netId, int size, const u32 *data, MimeJrTopApp *app);
void MimeJrTop_ReceiveFinalScore(int netId, int size, const u32 *data, MimeJrTopApp *app);
void MimeJrTop_ReceiveCommand4(void);
u32 MimeJrTop_GetFinishSize(void);
u32 MimeJrTop_GetStartSize(void);
u32 MimeJrTop_GetPlayerStateSize(void);
u32 MimeJrTop_GetFinalScoreSize(void);
u32 MimeJrTop_GetCommand4Size(void);
const void *MimeJrTop_GetCommandTable(void);
int MimeJrTop_GetCommandCount(void);
void MimeJrTop_RegisterCommands(MimeJrTopApp *app);
void MimeJrTop_InitGame(MimeJrTopApp *app);
BOOL MimeJrTop_Init(OverlayManager *man, int *state);
void MimeJrTopGame_RubDemo(MimeJrTopGame *game, int x, int y);
BOOL MimeJrTopGame_RunDemo(MimeJrTopGame *game);
BOOL MimeJrTop_Main(OverlayManager *man, int *state);
void MimeJrTop_FreeGame(MimeJrTopApp *app);
void MimeJrTop_ResetRound(MimeJrTopApp *app);
BOOL MimeJrTop_Exit(OverlayManager *man, int *state);
void MimeJrTop_InitGraphics(MimeJrTopApp *app);
void MimeJrTop_FreeGraphics(MimeJrTopApp *app);
void MimeJrTop_InitBgs(BgConfig *bgConfig);
void MimeJrTop_InitSprites(MimeJrTopApp *app);
BOOL MimeJrTop_StartFade(int type);
void MimeJrTop_SetStateIf(MimeJrTopApp *app, BOOL condition, u32 next, int *state);
void MimeJrTop_VBlank(void *data);
BOOL MimeJrTop_IsParent(MimeJrTopApp *app);
void MimeJrTop_SetStartReceived(MimeJrTopApp *app);
void MimeJrTop_SetFinishReceived(MimeJrTopApp *app);
int MimeJrTop_HandleConnectionLost(MimeJrTopApp *app);
void MimeJrTopModel_Draw(MimeJrTopModel *model, MtxFx43 *rotation, VecFx32 *position);
void MimeJrTopModel_Load(MimeJrTopModel *model, NARC *narc, int fileId);
void MimeJrTopModel_Init(MimeJrTopModel *model, int large);
void MimeJrTopModel_Free(MimeJrTopModel *model);
void MimeJrTop_DrawScene(MimeJrTopApp *app);
void MimeJrTopGame_SetMimeJrAnimation(MimeJrTopGame *game, int animation);
void MimeJrTop_InitCamera(MimeJrTopApp *app);
void MimeJrTop_InitSpriteTemplate(ManagedSpriteTemplate *template, s16 x, s16 y, u32 vram, int pal, int resId);
void MimeJrTopGame_CreateStars(MimeJrTopGame *game);
void MimeJrTopGame_CreatePopups(MimeJrTopGame *game);
void MimeJrTopGame_FreePopups(MimeJrTopGame *game);
void MimeJrTopGame_SetStarsVisible(MimeJrTopGame *game, BOOL visible, int unused);
void MimeJrTopGame_UpdateStars(MimeJrTopGame *game);
void MimeJrTopGame_FreeStars(MimeJrTopGame *game);
void MimeJrTopGame_SetRivalAnim(MimeJrTopGame *game, int player, int anim);
int MimeJrTopGame_GetRivalAnim(MimeJrTopGame *game, int player);
void MimeJrTopGame_UpdateRivalSprites(MimeJrTopGame *game);
void MimeJrTopGame_FreeRivalSprites(MimeJrTopGame *game);
void MimeJrTopGame_ShowInstructions(MimeJrTopGame *game);
void MimeJrTopGame_HideInstructions(MimeJrTopGame *game);
void MimeJrTop_LoadPlayerPalettes(MimeJrTopApp *app, int index);
int MimeJrTop_GetCenteredX(Window *window, String *string);
void MimeJrTopGame_ShowRivalNames(MimeJrTopApp *app, MimeJrTopGame *game);
void MimeJrTopGame_HideRivalNames(MimeJrTopGame *game);
void MimeJrTop_LoadSpriteResources(MimeJrTopApp *app);
void MimeJrTopGame_CreateRivalSprites(MimeJrTopApp *app, MimeJrTopGame *game);
ManagedSprite *MimeJrTopGame_NewSpark(MimeJrTopGame *game, u16 x, u16 y);
void MimeJrTopGame_CreateDancerSprites(MimeJrTopGame *game);
void MimeJrTopGame_FreeDancerSprites(MimeJrTopGame *game);
void MimeJrTopGame_CreateDemoHand(MimeJrTopGame *game);
void MimeJrTopGame_FreeDemoHand(MimeJrTopGame *game);
void MimeJrTop_LoadBgGraphics(MimeJrTopApp *app);
MimeJrTopClock *MimeJrTopClock_New(u32 playerCount, MimeJrTopResources *resources);
void MimeJrTopClock_Free(MimeJrTopClock *clock);
BOOL MimeJrTopClock_Tick(MimeJrTopClock *clock);
MimeJrTopGame *MimeJrTopGame_New(u32 playerCount, u32 localIndex, MimeJrTopResources *resources);
void MimeJrTopGame_ResetBalance(MimeJrTopGame *game);
void MimeJrTopGame_Free(MimeJrTopGame *game);
BOOL MimeJrTopGame_HasFallen(MimeJrTopGame *game, int unused);
void MimeJrTopGame_UpdateMimeJrAnimation(MimeJrTopGame *game);
SysTask *MimeJrTopGame_StartRivalFall(MimeJrTopGame *game, int player);
void MimeJrTopGame_StartRivalTier(MimeJrTopGame *game, int player);
void MimeJrTopGame_StartTierModels(MimeJrTopGame *game, int player);
void MimeJrTopGame_StartFall(MimeJrTopGame *game);
void MimeJrTopFall_Task(SysTask *sysTask, MimeJrTopFallTask *task);
void MimeJrTopRivalFall_Task(SysTask *sysTask, void *data);
void MimeJrTopRivalTier_Task(SysTask *sysTask, void *data);
void MimeJrTopTierModels_Task(SysTask *sysTask, MimeJrTopTierModelsTask *task);
void MimeJrTopGame_AddTierScore(MimeJrTopGame *game);
void MimeJrTopGame_AddTierBonus(MimeJrTopGame *game);
BOOL MimeJrTopGame_UpdateTier(MimeJrTopGame *game);
BOOL MimeJrTop_GetTier(MimeJrTopGame *game, int spinFrames, u32 *tier);
void MimeJrTopGame_UpdateRivals(MimeJrTopGame *game);
void MimeJrTopGame_UpdateLocalPlayer(MimeJrTopGame *game);
void MimeJrTopGame_Update(MimeJrTopGame *game);
void MimeJrTopGame_HandleTouch(MimeJrTopGame *game);
void MimeJrTopGame_ResetMotion(MimeJrTopGame *game);
void MimeJrTopGame_UpdateLevel(MimeJrTopGame *game);
void MimeJrTopGame_UpdateWobble(MimeJrTopGame *game);
BOOL MimeJrTopModel_Turn(MimeJrTopModel *model, s32 x, s32 y, s32 targetX, s32 targetY, f32 scale, BOOL saveVelocity);
BOOL MimeJrTopModel_ContinueSpin(MimeJrTopModel *model, f32 unused);
int MimeJrTop_GetPlayerIndex(MimeJrTopApp *app, u32 netId);
void MimeJrTopTween_InitAccel(MimeJrTopTween *tween, fx32 start, fx32 end, fx32 velocity, int frames);
BOOL MimeJrTopTween_UpdateAccel(MimeJrTopTween *tween);
void MimeJrTopTween_Init(MimeJrTopTween *tween, fx32 start, fx32 end, int frames);
BOOL MimeJrTopTween_Update(MimeJrTopTween *tween);
void MimeJrTopBurst_Task(SysTask *sysTask, MimeJrTopBurst *burst);
void MimeJrTopGame_StartBurst(MimeJrTopGame *game, u16 x, u16 y);
void MimeJrTopStars_Task(SysTask *sysTask, MimeJrTopStarsTask *task);
void MimeJrTopGame_StartStars(MimeJrTopGame *game, s32 *stop);
void MimeJrTopStage_UpdateFlash(MimeJrTopStageEffect *effect);
void MimeJrTopStage_UpdatePopups(MimeJrTopStageEffect *effect);
void MimeJrTopStage_UpdateAudience(MimeJrTopStageEffect *effect);
void MimeJrTopStage_UpdateStars(MimeJrTopStageEffect *effect);
void MimeJrTopStage_Task(SysTask *sysTask, void *data);
void MimeJrTopGame_StartStage(MimeJrTopGame *game);
void MimeJrTopDance_SlideIn(MimeJrTopDance *dance);
void MimeJrTopDance_SlideOut(MimeJrTopDance *dance);
int MimeJrTop_GetDanceStepFrames(int speed);
void MimeJrTopDance_Task(SysTask *sysTask, void *data);
void MimeJrTopGame_StartDance(MimeJrTopGame *game);
void MimeJrTopDance_Routine1(SysTask *sysTask, MimeJrTopDance *dance);
void MimeJrTopDancer_Reset(MimeJrTopDancer *dancer);
BOOL MimeJrTopDancer_Slide(MimeJrTopDancer *dancer, int speed, s32 xdir, s32 ydir);
BOOL MimeJrTopDancer_EnterLow(MimeJrTopDancer *dancer, int speed, s32 xdir, s32 ydir);
BOOL MimeJrTopDancer_EnterHigh(MimeJrTopDancer *dancer, int speed, s32 xdir, s32 ydir);
BOOL MimeJrTopDancer_Circle(MimeJrTopDancer *dancer, int speed, s32 xdir, s32 cycles);
void MimeJrTopDance_Routine2(SysTask *sysTask, MimeJrTopDance *dance);
void MimeJrTopDance_Routine3(SysTask *sysTask, MimeJrTopDance *dance);
void MimeJrTopDance_Routine4(SysTask *sysTask, MimeJrTopDance *dance);
void MimeJrTopDance_Routine5(SysTask *sysTask, MimeJrTopDance *dance);
void MimeJrTop_QuatIdentity(MimeJrTopQuat *quat);
void MimeJrTop_QuatCopy(MimeJrTopQuat *dst, const MimeJrTopQuat *src);
void MimeJrTop_QuatMul(f32 *dst, const f32 *a, const f32 *b);
f32 MimeJrTop_QuatLengthSq(const MimeJrTopQuat *quat);
f32 MimeJrTop_QuatLength(const MimeJrTopQuat *quat);
void MimeJrTop_QuatDivide(f32 *dst, const f32 *src, f32 divisor);
void MimeJrTop_QuatToMatrix(MimeJrTopFloatMatrix *dst, const MimeJrTopQuat *quat);
void MimeJrTop_FloatMatrixToFx44(const MimeJrTopFloatMatrix *src, MtxFx44 *dst);
void MimeJrTop_FloatMatrixToFx43(const MimeJrTopFloatMatrix *src, MtxFx43 *dst);

#endif // POKEHEARTGOLD_APPLICATION_PLAZA_GAME_MIME_JR_TOP_H
