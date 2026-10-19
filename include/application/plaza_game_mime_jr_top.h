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

// Mime Jr. Top, a Wi-Fi Plaza game: Mime Jr. balances on a spinning top and every player rubs the
// touch screen to keep the top upright.

// A value moving from start to start + delta over duration frames (linear, or with a start
// velocity and constant acceleration).
typedef struct MimeJrTopTween {
    fx32 value;
    fx32 start;
    fx32 velocity;
    fx32 delta; // acceleration for the accelerated tween
    int frame;
    int duration;
} MimeJrTopTween;

typedef struct MimeJrTopQuat {
    f32 v[4]; // w, x, y, z
} MimeJrTopQuat;

typedef struct MimeJrTopFloatMatrix {
    f32 m[4][4];
} MimeJrTopFloatMatrix;

// A 3D model with its own rotation (a quaternion turned by touch input).
typedef struct MimeJrTopModel {
    ModelInstance instance;
    ModelResource resource;
    ModelAnimation animations[3];
    u8 unkC4[0x8C];
    MimeJrTopFloatMatrix rotationMatrix;
    MimeJrTopQuat rotation;
    MimeJrTopQuat lastTurn;
    MtxFx43 rotationFx;
    VecFx32 position;
    BOOL animate;
    BOOL loaded;
    int animation;
    int spinFrames; // frames the last flick keeps turning the model
    f32 spinX;
    f32 spinY;
    f32 spinStepX;
    f32 spinStepY;
} MimeJrTopModel;

// Part of MimeJrTopApp handed to the game screen.
typedef struct MimeJrTopResources {
    NARC *narc;    // NARC 193 (models, sprites, BGs)
    NARC *unkNarc; // NARC 189 (opened, not read here)
    SpriteSystem *spriteSystem;
    SpriteManager *spriteManager;
    BgConfig *bgConfig;
    PaletteData *palette;
    GF3DVramMan *vramMan;
    Camera *camera;
    u32 unk20;
    NNSFndAllocator allocator;
    u8 connectionLost; // every task stops when set
    u8 unk35[3];
} MimeJrTopResources;

// Run on the parent only: the game clock.
typedef struct MimeJrTopClock {
    u32 unk00;
    u32 playerCount;
    s16 timer;
    u16 unk0A;
    MimeJrTopResources *resources;
} MimeJrTopClock;

typedef struct MimeJrTopFallTask {
    int state;
    int step; // 0 = falling off, 1 = climbing back
    int timer;
    int *pose;
    MimeJrTopModel *mimeJr;
    MimeJrTopModel *top;
    u8 unk18[8];
    VecFx32 tilt;
    u8 unk2C[0x18];
    MimeJrTopTween tweenY;
    MimeJrTopTween tweenZ;
    BOOL selection;
    MimeJrTopResources *resources;
} MimeJrTopFallTask;

// Another player's top falling over.
typedef struct MimeJrTopRivalFallTask {
    int state;
    int step;
    int timer;
    ManagedSprite *face;
    ManagedSprite *body;
    MimeJrTopTween tween;
    MimeJrTopResources *resources;
} MimeJrTopRivalFallTask;

// Another player's tier badge (pulses while the tier holds).
typedef struct MimeJrTopRivalTierTask {
    int state;
    int mode;
    int counter;
    ManagedSprite *sprite;
    MimeJrTopTween tween;
    int previous;
    int *current;
    BOOL active;
    u8 unk34[8];
    MimeJrTopResources *resources;
    BOOL selection;
} MimeJrTopRivalTierTask;

// The local player's tier models (swapped in when the tier changes).
typedef struct MimeJrTopTierModelsTask {
    int state;
    int unk04;
    u8 unk08[0x20];
    int previous;
    int *current;
    BOOL active;
    MimeJrTopModel *modelA;
    MimeJrTopModel *modelB;
    MimeJrTopResources *resources;
    BOOL selection;
} MimeJrTopTierModelsTask;

typedef union MimeJrTopPlayerTask {
    MimeJrTopRivalTierTask rival;
    MimeJrTopTierModelsTask local;
} MimeJrTopPlayerTask;

// Three sparks thrown out where the screen is rubbed.
typedef struct MimeJrTopBurst {
    BOOL active;
    int state;
    ManagedSprite *sprites[3];
    MimeJrTopTween tweens[3][2]; // x and y per spark
    MimeJrTopResources *resources;
} MimeJrTopBurst;

typedef struct MimeJrTopStar {
    ManagedSprite *sprite;
    fx32 driftX;
    fx32 driftY;
    fx32 originX;
    fx32 originY;
    fx32 jitterX;
    fx32 jitterY;
    fx32 circleX;
    fx32 circleY;
    u8 type;
    u8 visible;
    u8 delay;
    u8 jitterPhase;
    int angle;
    s16 fallSpeed;
    s16 radiusX;
    s16 radiusY;
    s16 timer;
} MimeJrTopStar;

typedef struct MimeJrTopGame MimeJrTopGame;

typedef struct MimeJrTopStarsTask {
    MimeJrTopGame *game;
    s32 state;
    s32 timer;
    s32 group;
    s32 *stop;
    MimeJrTopStar stars[36];
} MimeJrTopStarsTask;

// One effect of the stage animation (flash, pop-ups, audience, stars).
typedef struct MimeJrTopStageEffect {
    int state;
    BOOL active;
    void *targets[8];
    int timer;
    u8 unk2C[0x20];
    int audienceTimer;
    BOOL stop;
    BOOL starsRunning;
    u8 unk58[0x20];
} MimeJrTopStageEffect;

// Stage effects started by the phase (how long the top has kept spinning).
typedef struct MimeJrTopStage {
    u32 previousPhase;
    u32 *phase;
    MimeJrTopStageEffect effects[5];
    SysTask *task;
    MimeJrTopResources *resources;
} MimeJrTopStage;

// A sub screen dancer moved along tweens or circles.
typedef struct MimeJrTopDancer {
    ManagedSprite *sprite;
    int state;
    s16 angle;
    u8 unk0A[6];
    int cycles;
    s16 baseX;
    s16 baseY;
    fx32 radiusX;
    fx32 radiusY;
    MimeJrTopTween tweenX;
    MimeJrTopTween tweenY;
} MimeJrTopDancer;

// The sub screen dance shown from level 1 on (one routine per level).
typedef struct MimeJrTopDance {
    MimeJrTopDancer dancers[4]; // two front and two back dancers
    int count;
    int backSpeed;
    int frontSpeed;
    int loopCount;
    u8 unk150[4];
    SysTask *task;
    u32 *level;
    int *flashBlend;
    int *danceBlend;
    u32 timer;
    int routineState;
    u8 unk16C[4];
    u32 slideState;
    BOOL enabled;
    BOOL shown;
    MimeJrTopResources *resources;
    MimeJrTopTween slides[3];
} MimeJrTopDance;

struct MimeJrTopGame {
    int localIndex;
    int playerCount;
    int demoFrame;
    int demoStep;
    u32 unk10;
    MimeJrTopResources *resources;
    u8 unk18[4];
    ManagedSprite *rivalFaces[3];
    ManagedSprite *rivalBodies[3];
    ManagedSprite *rivalBadges[3];
    ManagedSprite *stars[36];
    ManagedSprite *popups[4];
    ManagedSprite *backDancers[2];
    ManagedSprite *frontDancers[2];
    ManagedSprite *demoHand;
    u16 rivalSlots[4];
    u8 unkFC[0xC];
    int flashBlend;
    int danceBlend;
    int mimeJrPose;
    MimeJrTopModel top;
    MimeJrTopModel mimeJr[3];
    MimeJrTopModel stage;
    MimeJrTopModel audience[8];
    MimeJrTopModel tierModels[2];
    VecFx32 tilt;
    int lastTouchX;
    int lastTouchY;
    u8 unk1FDC[4];
    Window instructions;
    Window nameWindows[3];
    u8 unk2020[0x10];
    MimeJrTopDance dance;
    u8 unk21F8[0x30];
    MimeJrTopStage stageEffects;
    MimeJrTopBurst bursts[8];
    MimeJrTopPlayerTask playerTasks[4];
    int wobbleTick;
    int frame;
    u32 phase;
    u32 tier;
    u32 score;
    u32 level;
    u32 previousLevel;
    s16 recoverTimer;
    s16 levelTimer;
    s16 rivalSpinFrames[4];
    u32 rivalTiers[4];
    int spinFrames;
    s16 wobbleX;
    s16 wobbleY;
    s16 wobbleTime;
    s16 wobbleStep;
    MimeJrTopTween settleX;
    MimeJrTopTween settleY;
    MimeJrTopTween kickX;
    MimeJrTopTween kickY;
    BOOL touched;
    BOOL settled;
    BOOL wobbleDue;
    BOOL wobbling;
    BOOL everTouched;
    s16 nextWobbleX;
    s16 nextWobbleY;
    u32 playerStates[4];
    u32 unk2BAC;
    u32 seed;
    BOOL selection;
    BOOL rivalFallShown;
};

// Command 22: sent by the parent to start the game.
typedef struct MimeJrTopStartMessage {
    u8 selection;
    u8 unk01[3];
    u32 seed;
} MimeJrTopStartMessage;

typedef struct MimeJrTopPlayerScore {
    u32 score;
    u32 unk04;
} MimeJrTopPlayerScore;

typedef struct MimeJrTopApp {
    MimeJrTopClock *clock;
    MimeJrTopGame *game;
    BOOL startReceived;
    BOOL finishReceived;
    u32 unk10;
    MimeJrTopStartMessage start;
    u32 finalScore; // command 25
    u8 unk20[4];
    MimeJrTopPlayerScore scores[4];
    int netId;
    MimeJrTopResources resources;
    u32 savedSeed;
    PlazaGameSign *sign;
    PlazaGameArgs *args;
    PlazaGameRoster roster;
    PlazaGameEntry *entry;
    PlazaGameResultData resultData;
    PlazaGameResults *results;
    u32 netIds[4];
    BOOL keepPlaying;
} MimeJrTopApp;

typedef void (*MimeJrTopReceiveFunc)(int netId, int size, void *data, MimeJrTopApp *app);
typedef u32 (*MimeJrTopSizeFunc)(void);

typedef struct MimeJrTopCommand {
    MimeJrTopReceiveFunc receive;
    MimeJrTopSizeFunc getSize;
    void *(*getBuffer)(int netId, void *context, int size);
} MimeJrTopCommand;

void MimeJrTop_ReceiveStart(int netId, int size, void *data, MimeJrTopApp *app);
void MimeJrTop_ReceiveFinish(int netId, int size, void *data, MimeJrTopApp *app);
void MimeJrTop_ReceivePlayerState(int netId, int size, void *data, MimeJrTopApp *app);
void MimeJrTop_ReceiveFinalScore(int netId, int size, void *data, MimeJrTopApp *app);
void MimeJrTop_ReceiveCommand4(int netId, int size, void *data, MimeJrTopApp *app);
u32 MimeJrTop_GetFinishSize(void);
u32 MimeJrTop_GetStartSize(void);
u32 MimeJrTop_GetPlayerStateSize(void);
u32 MimeJrTop_GetFinalScoreSize(void);
u32 MimeJrTop_GetCommand4Size(void);
const MimeJrTopCommand *MimeJrTop_GetCommandTable(void);
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
