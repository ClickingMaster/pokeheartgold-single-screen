#ifndef POKEHEARTGOLD_APPLICATION_PLAZA_GAME_SWALOT_PLOP_H
#define POKEHEARTGOLD_APPLICATION_PLAZA_GAME_SWALOT_PLOP_H

#include "global.h"

#include "application/plaza_game.h"

#include "bg_window.h"
#include "camera.h"
#include "filesystem.h"
#include "font_sprite.h"
#include "gf_3d_vramman.h"
#include "heap.h"
#include "message_format.h"
#include "model_animation.h"
#include "msgdata.h"
#include "obj_char_transfer.h"
#include "overlay_manager.h"
#include "pm_string.h"
#include "sprite.h"
#include "sprite_resource_header.h"
#include "sprite_resource.h"

// SWALOT Plop, a Wi-Fi Plaza game: every player flicks berries on the touch screen towards a
// spinning SWALOT; berries that land in its mouth score points, and consecutive hits build a combo.

// One entry of the level table (NARC 212 file 0).
typedef struct SwalotPlopLevel {
    int hitCount; // hits needed to reach this level
    u32 spinSpeed;
    u32 cameraHeight;
    u32 cameraPitch;
    u32 mouthOffset;
    u32 scale;
} SwalotPlopLevel;

typedef struct SwalotPlopLevelTable {
    u32 count;
    SwalotPlopLevel *levels;
} SwalotPlopLevelTable;

// Part of SwalotPlopApp shared with the game screen and the tally.
typedef struct SwalotPlopShared {
    SwalotPlopLevelTable levelTable;
    PlayerProfile *profiles[4];
    u32 nameFlags[4];
} SwalotPlopShared;

typedef struct SwalotPlopScores {
    u32 scores[4];
} SwalotPlopScores;

// Run on the parent only: game clock, the scores reported at the end, and the hit count
// that selects the level.
typedef struct SwalotPlopTally {
    int timer;
    BOOL running;
    u32 playerCount;
    u32 scores[4];
    u32 receivedMask;
    u8 unk20[0x10];
    SwalotPlopShared *shared;
    u16 hitCount;
    volatile u8 level;
    u8 levelChanged;
} SwalotPlopTally;

// What a shot looks like on the link (command 24).
typedef struct SwalotPlopShotRecord {
    u16 owner;
    u16 caught;
    s16 dir[3];
    s16 start[3];
    s16 speed;
    s8 combo;
    u8 big;
} SwalotPlopShotRecord;

typedef struct SwalotPlopShotParams {
    u16 owner;
    u16 caught;
    VecFx32 direction;
    fx32 speed;
    VecFx32 start;
    u32 combo;
    u32 big;
} SwalotPlopShotParams;

enum SwalotPlopShotState {
    SHOT_STATE_FREE,
    SHOT_STATE_FLYING,
    SHOT_STATE_FALLING,
    SHOT_STATE_CAUGHT,
    SHOT_STATE_REPORT,
    SHOT_STATE_BOUNCED,
};

// A berry in flight.
typedef struct SwalotPlopShot {
    u8 state;
    u8 remove;
    u16 frames;
    SwalotPlopShotParams params;
    VecFx32 position;
    s16 timer;
    u16 combo;
    BOOL crossedRim;
    fx32 distance;
} SwalotPlopShot;

// The touch samples of one frame.
typedef struct SwalotPlopTouch {
    u16 count;
    TPData samples[8];
} SwalotPlopTouch;

// Touch positions of the current flick.
typedef struct SwalotPlopFlick {
    u16 x;
    u16 y;
} SwalotPlopFlick;

typedef struct SwalotPlopPoint {
    s16 x;
    s16 y;
} SwalotPlopPoint;

typedef struct SwalotPlopFlickQueue {
    BOOL active;
    SwalotPlopFlick points[8];
    u16 read;
    u16 write;
} SwalotPlopFlickQueue;

// SWALOT itself: orientation, mouth and body measurements (scaled per level).
typedef struct SwalotPlopBody {
    u8 active;
    u8 mouthOpen;
    u16 spin;
    u16 tilt;
    u16 spinSpeed;
    fx32 mouthDepth;
    MtxFx33 tiltMtx;
    MtxFx33 spinMtx;
    MtxFx33 rotation;
    VecFx32 normal;
    VecFx32 mouthPos;
    fx32 mouthPlane;
    fx32 pushRadius;
    fx32 pushStrength;
    VecFx32 offset;
    fx32 scale;
    fx32 mouthDepthRange;
    fx32 mouthDepthMin;
    fx32 unkB4;
    fx32 mouthOffsetX;
    fx32 mouthOffsetY;
    fx32 heightMin;
    fx32 heightRange;
    fx32 topRadius;
    fx32 radius;
    fx32 slope;
    fx32 modelScale;
    VecFx32 position;
    fx32 height;
} SwalotPlopBody;

// SWALOT's model, its three animations and the mouth animation.
typedef struct SwalotPlopStage {
    ModelInstance models[3];
    ModelResource resources[3];
    ModelAnimation animations[3];
    fx32 animSpeed;
    u8 mouthActive;
    u8 mouthState;
    s16 mouthTimer;
    fx32 mouthFrame;
    fx32 mouthStep;
} SwalotPlopStage;

// The points that pop up over a player's seat when a berry is caught.
typedef struct SwalotPlopPopups {
    Sprite *sprites[3];
    s32 timers[3];
    VecFx32 pos[3];
    int mode;
} SwalotPlopPopups;

// The window with the other players' names shown during the intro.
typedef struct SwalotPlopIntro {
    u32 state;
    s32 delay;
    u32 counter;
    Window window;
} SwalotPlopIntro;

typedef struct SwalotPlopFinish {
    u32 state;
    u32 counter;
} SwalotPlopFinish;

// The SWALOT model of the top screen with its six animations.
typedef struct SwalotPlopScene {
    ModelInstance models[3];
    ModelResource resources[3];
    ModelAnimation animations[6];
    volatile u16 current;
    u16 state;
    ModelAnimation special;
    volatile s32 frame;
    u16 leader;
    u16 bonus;
} SwalotPlopScene;

typedef struct SwalotPlopBgFade {
    u16 state;
    s16 timer;
    s16 layer;
    u8 palette;
    u8 counter;
} SwalotPlopBgFade;

// Sprite and shadow of a berry.
typedef struct SwalotPlopShotView {
    Sprite *sprite;
    ModelInstance model;
    ModelInstance shadow;
    int state;
    SwalotPlopShot *shot;
    u16 rotX;
    u16 rotZ;
    u16 rotSpeedX;
    u16 rotSpeedZ;
    s16 timer;
} SwalotPlopShotView;

typedef struct SwalotPlopCursor {
    BOOL active;
    Sprite *sprite;
} SwalotPlopCursor;

typedef struct SwalotPlopParticle {
    BOOL active;
    Sprite *sprite;
    VecFx32 position;
    s32 age;
    BOOL direction;
} SwalotPlopParticle;

// Accelerated motion: value = start + slope * step + curvature * step^2 / 2.
typedef struct SwalotPlopCurve {
    s32 value;
    s32 start;
    s32 slope;
    s32 curvature;
    s32 step;
    s32 duration;
} SwalotPlopCurve;

// The combo counter that slides in on the bottom screen.
typedef struct SwalotPlopCombo {
    SpriteResource *res[4];
    SpriteResourcesHeader header;
    Sprite *sprite;
    Window window;
    TextOBJ *textObj;
    TextOBJLayout *textLayout;
    ObjCharSpaceReservation charTransfer;
    String *string;
    SpriteResource *textPltt;
    SwalotPlopCurve slideX;
    SwalotPlopCurve slideY;
    s16 slideFrame;
    s16 bounceFrame;
    u16 state;
    s16 hideDelay;
    enum HeapID heapId;
} SwalotPlopCombo;

// Graphics of the game screens.
typedef struct SwalotPlopGfx {
    BgConfig *bgConfig;
    MessageFormat *msgFormat;
    MsgData *msgData;
    String *string1;
    String *string2;
    u32 unk14;
    GF3DVramMan *vramMan;
    SpriteList *spriteList;
    G2dRenderer renderer;
    GF_2DGfxResMan *resMan[4];
    SpriteResourcesHeader header;
    SpriteResource *res[4];
    FontSystem *fontSystem;
    Camera *camera;
    VecFx32 cameraTarget;
    SwalotPlopStage stage;
    SwalotPlopPopups popups[4];
    SwalotPlopIntro intro;
    SwalotPlopFinish finish;
    u8 unk4A8[0x18];
    SwalotPlopScene scene;
    PlazaGameSign *sign;
    SwalotPlopBgFade bgFade;
    ModelResource shotModels[5];
    ModelResource shadowModel;
    SpriteResource *shotRes[4];
    SpriteResourcesHeader shotHeader;
    SwalotPlopShotView shotViews[0x60];
    SwalotPlopCursor cursor;
    SwalotPlopParticle particles[8];
    SwalotPlopCombo combo;
} SwalotPlopGfx;

// Interpolates the level parameters (spin, camera, scale) over 16 frames.
typedef struct SwalotPlopLevelBlend {
    s32 current[6];
    s32 start[6];
    s32 delta[6];
    s32 step;
} SwalotPlopLevelBlend;

// The hand that throws two demo berries before the game starts.
typedef struct SwalotPlopDemo {
    SpriteResource *res[4];
    SpriteResourcesHeader header;
    Sprite *sprite;
    u8 active;
    u8 count;
    u8 state;
    u8 thrown;
    int timer;
} SwalotPlopDemo;

// The scores the players report every 30 frames; once all reported, the leader is shown.
typedef struct SwalotPlopScoreReports {
    u32 scores[4];
    u32 mask;
    u32 leader;
} SwalotPlopScoreReports;

typedef struct SwalotPlopSpeedUp {
    u16 state;
    u8 active;
    s8 frame;
} SwalotPlopSpeedUp;

enum SwalotPlopPhase {
    SWALOT_PLOP_PHASE_NORMAL,
    SWALOT_PLOP_PHASE_FAST,
    SWALOT_PLOP_PHASE_FASTEST,
};

typedef struct SwalotPlopGame {
    u32 phase;
    BOOL running;
    u32 frame;
    u32 duration;
    u32 playerCount;
    u32 localIndex;
    s32 score;
    s32 combo;
    SwalotPlopFlickQueue flick;
    u32 releasedFrames;
    SwalotPlopShot shots[0x60];
    SwalotPlopBody body;
    SwalotPlopGfx gfx;
    NNSFndAllocator allocator;
    u32 finalScores[4];
    SwalotPlopTouch touch;
    SwalotPlopLevelBlend levelBlend;
    SwalotPlopShared *shared;
    u16 level;
    u16 prevLevel;
    u8 options[4];
    u32 optionsSet;
    SwalotPlopDemo demo;
    SwalotPlopScoreReports scoreReports;
    SwalotPlopSpeedUp speedUp;
} SwalotPlopGame;

typedef struct SwalotPlopApp {
    PlazaGameEntry *entry;
    PlazaGameResults *results;
    PlazaGameRoster roster;
    PlazaGameResultData resultData;
    SwalotPlopTally *tally;
    SwalotPlopGame *game;
    BOOL startReceived;
    BOOL finishReceived;
    BOOL scoresReceived;
    u16 netId;
    u16 localIndex;
    SwalotPlopShared shared;
    u32 selection;
    u32 playerCount;
    u32 connectionLostState;
    u32 syncRound;
    BOOL canSend;
    u8 syncCounts[4];
} SwalotPlopApp;

// Link command table entry (Comm_RegisterCommandTable).
typedef struct SwalotPlopCommand {
    void (*handler)(int netId, int size, void *data, void *context);
    u32 (*getSize)(void);
    void *getBuffer;
} SwalotPlopCommand;

BOOL SwalotPlop_Init(OverlayManager *man, int *state);
BOOL SwalotPlop_Main(OverlayManager *man, int *state);
BOOL SwalotPlop_Exit(OverlayManager *man, int *state);

void SwalotPlop_ReceiveStart(SwalotPlopApp *app);
void SwalotPlop_ReceiveFinish(SwalotPlopApp *app);
void SwalotPlop_TallyShot(SwalotPlopApp *app, const SwalotPlopShotRecord *record, int netId);
void SwalotPlop_ReceiveShot(SwalotPlopApp *app, const SwalotPlopShotRecord *record, int netId);
void SwalotPlop_ReceiveFinalScore(SwalotPlopApp *app, u32 score, int netId);
void SwalotPlop_ReceiveScores(SwalotPlopApp *app, const u32 *scores, int netId);
void SwalotPlop_ReceiveLevel(SwalotPlopApp *app, u32 level);
void SwalotPlop_ReceiveScoreReport(SwalotPlopApp *app, u32 score, int netId);
void SwalotPlop_ReceiveSync(SwalotPlopApp *app);
void SwalotPlop_VBlankCallback(void *data);
void SwalotPlop_SendShots(SwalotPlopApp *app);
void SwalotPlop_SendScoreReport(SwalotPlopApp *app);
void SwalotPlop_LoadSharedData(SwalotPlopApp *app, int heapId);
void SwalotPlop_FreeSharedData(SwalotPlopApp *app);
void SwalotPlop_Abort(SwalotPlopApp *app, PlazaGameArgs *args);
BOOL SwalotPlop_SendCommand(SwalotPlopApp *app, int command, void *data, int size);
SwalotPlopTally *SwalotPlopTally_New(enum HeapID heapId, int duration, u32 playerCount, SwalotPlopShared *shared);
void SwalotPlopTally_Free(SwalotPlopTally *tally);
BOOL SwalotPlopTally_Update(SwalotPlopTally *tally);
u8 SwalotPlopTally_LevelChanged(SwalotPlopTally *tally);
void SwalotPlopTally_ClearLevelChanged(SwalotPlopTally *tally);
u8 SwalotPlopTally_GetLevel(SwalotPlopTally *tally);
void SwalotPlopTally_AddShot(SwalotPlopTally *tally, const SwalotPlopShotRecord *record, int index);
void SwalotPlopTally_SetScore(SwalotPlopTally *tally, u32 score, int index);
BOOL SwalotPlopTally_HasAllScores(SwalotPlopTally *tally);
void SwalotPlopTally_GetScores(SwalotPlopTally *tally, u32 *scores);
u32 SwalotPlopTally_CountScores(SwalotPlopTally *tally);
SwalotPlopGame *SwalotPlopGame_New(enum HeapID heapId, u32 duration, u32 playerCount, u32 localIndex, SwalotPlopShared *shared);
void SwalotPlopGame_Free(SwalotPlopGame *game);
BOOL SwalotPlopGame_RunIntro(SwalotPlopGame *game, BOOL ready);
BOOL SwalotPlopGame_RunFinish(SwalotPlopGame *game, BOOL ready);
BOOL SwalotPlopGame_Update(SwalotPlopGame *game);
void SwalotPlopGame_VBlank(SwalotPlopGame *game);
u32 SwalotPlopGame_GetFrame(SwalotPlopGame *game);
void SwalotPlopGame_AddRemoteShot(SwalotPlopGame *game, const SwalotPlopShotRecord *record);
BOOL SwalotPlopGame_TakeShotReport(SwalotPlopGame *game, SwalotPlopShotRecord *record);
void SwalotPlopGame_SetFinalScores(SwalotPlopGame *game, const u32 *scores);
u32 SwalotPlopGame_GetScore(SwalotPlopGame *game);
void SwalotPlopGame_SetLevel(SwalotPlopGame *game, u32 level);
void SwalotPlopGame_SetScoreReport(SwalotPlopGame *game, u32 score, int index);
void SwalotPlopGame_SetOptions(SwalotPlopGame *game, const u8 *options);
void SwalotPlopCurve_Init(SwalotPlopCurve *curve, s32 start, s32 end, s32 slope, s32 duration);
BOOL SwalotPlopCurve_Update(SwalotPlopCurve *curve);
void SwalotPlop_GetPopupPos(int seat, int playerCount, int index, u32 *x, u32 *y);
void SwalotPlopGame_ThrowDemoShot(SwalotPlopGame *game, s32 x, s32 y, s32 endX, u16 endY, int state);
u32 SwalotPlopGame_GetHitPoints(SwalotPlopGame *game);
void SwalotPlopPopups_Init(SwalotPlopPopups *popups, SwalotPlopGfx *gfx, int playerCount, int localIndex, enum HeapID heapId);
void SwalotPlopPopups_Free(SwalotPlopPopups *popups, SwalotPlopGfx *gfx);
void SwalotPlopPopups_Show(SwalotPlopPopups *popups, int seat, int combo, int phase);
void SwalotPlopPopups_Update(SwalotPlopPopups *popups);
void SwalotPlopPopups_InitSeat(SwalotPlopPopups *popups, SpriteTemplate *template, SwalotPlopGfx *gfx, int playerCount, int seat, int localIndex, enum HeapID heapId);
void SwalotPlopPopups_FreeSeat(SwalotPlopPopups *popups);
void SwalotPlopPopups_ShowSeat(SwalotPlopPopups *popups, int seat, u32 combo, int phase);
void SwalotPlopPopups_UpdateSeat(SwalotPlopPopups *popups);
void SwalotPlopPopups_SetPos(SwalotPlopPopups *popups, int i);
void SwalotPlopPopups_HideSprite(SwalotPlopPopups *popups, int i);
void SwalotPlopIntro_Init(SwalotPlopIntro *intro, SwalotPlopGfx *gfx, SwalotPlopShared *shared, u32 playerCount, u32 localIndex, NARC *narc, enum HeapID heapId);
void SwalotPlopIntro_Free(SwalotPlopIntro *intro, SwalotPlopGfx *gfx);
void SwalotPlopIntro_ShowWindow(SwalotPlopIntro *intro, SwalotPlopGfx *gfx);
BOOL SwalotPlopIntro_IsCountingDown(SwalotPlopIntro *intro, SwalotPlopGfx *gfx);
void SwalotPlopIntro_HideWindow(SwalotPlopIntro *intro, SwalotPlopGfx *gfx);
void SwalotPlopIntro_StartScene(SwalotPlopIntro *intro, SwalotPlopGfx *gfx);
BOOL SwalotPlopIntro_Wait(SwalotPlopIntro *intro, SwalotPlopGfx *gfx);
void SwalotPlopGfx_SetScenePos(SwalotPlopGfx *gfx, SwalotPlopBody *body);
void SwalotPlopFinish_Init(SwalotPlopGfx *gfx, u32 playerCount, u32 localIndex, enum HeapID heapId);
void SwalotPlopGfx_FreeDummy1(SwalotPlopGfx *gfx, u32 playerCount);
void SwalotPlopFinish_Start(SwalotPlopGfx *gfx);
BOOL SwalotPlopFinish_Update(SwalotPlopGfx *gfx);
BOOL SwalotPlopFinish_IsSignDone(SwalotPlopGfx *gfx);
void SwalotPlopBody_Init(SwalotPlopGame *game, SwalotPlopBody *body);
void SwalotPlopBody_Update(SwalotPlopGame *game, SwalotPlopBody *body);
void SwalotPlopBody_SetActive(SwalotPlopBody *body, u8 active);
void SwalotPlopBody_SetTilt(SwalotPlopBody *body, int tilt);
void SwalotPlopBody_SetSpin(SwalotPlopBody *body, u16 spin);
int SwalotPlopBody_CheckMouth(SwalotPlopBody *body, const VecFx32 *position, VecFx32 *move, VecFx32 *push);
BOOL SwalotPlopBody_IsInside(SwalotPlopBody *body, const VecFx32 *pos);
fx32 SwalotPlopBody_CalcMouthDepth(SwalotPlopBody *body, s32 tilt);
fx32 SwalotPlopBody_CalcHeight(SwalotPlopBody *body, s32 tilt);
void SwalotPlopBody_SetScale(SwalotPlopBody *body, fx32 scale);
void SwalotPlopBody_SetSpinSpeed(SwalotPlopBody *body, u16 spinSpeed);
void SwalotPlopBody_SetMouthOffset(SwalotPlopBody *body, u32 offset);
int SwalotPlopBody_GetTiltNormal(SwalotPlopBody *body, u32 frame, u32 duration);
int SwalotPlopBody_GetTiltFast(SwalotPlopBody *body, u32 frame, u32 duration);
u16 SwalotPlopBody_GetTiltFastest(SwalotPlopBody *body, u32 frame, u32 duration);
void SwalotPlopShot_ToRecord(const SwalotPlopShotParams *params, SwalotPlopShotRecord *record);
void SwalotPlopShot_FromRecord(const SwalotPlopShotRecord *record, SwalotPlopShotParams *params);
void SwalotPlopGame_AddShot(SwalotPlopGame *game, const SwalotPlopShotParams *params, u32 state);
void SwalotPlopShot_End(SwalotPlopGame *game, SwalotPlopShot *shot, u16 caught, u32 combo);
void SwalotPlopShot_Clear(SwalotPlopShot *shot);
void SwalotPlopGame_UpdateShots(SwalotPlopGame *game);
void SwalotPlopGame_UpdateFlyingShot(SwalotPlopGame *game, SwalotPlopShot *shot);
void SwalotPlopGame_RemoveEndedShots(SwalotPlopGame *game);
void SwalotPlopGame_DropEndedShots(SwalotPlopGame *game);
void SwalotPlopShot_CheckRemove(SwalotPlopShot *shot, SwalotPlopGame *game);
void SwalotPlopGame_DropFlyingShots(SwalotPlopGame *game);
BOOL SwalotPlopGame_HasActiveShots(SwalotPlopGame *game);
fx32 SwalotPlopShot_GetSpeed(SwalotPlopShot *shot);
void SwalotPlopShot_GetMove(SwalotPlopShot *shot, VecFx32 *move);
void SwalotPlopShot_Bounce(SwalotPlopShot *shot, SwalotPlopGame *game, BOOL bounce);
void SwalotPlopShot_BounceEx(SwalotPlopShot *shot, SwalotPlopGame *game, BOOL reverse, fx32 damping, BOOL jitter);
void SwalotPlopGame_UpdateBouncedShot(SwalotPlopShot *shot, SwalotPlopGame *game);
BOOL SwalotPlopShot_IsOnRim(SwalotPlopShot *shot);
BOOL SwalotPlopShot_IsInPool(SwalotPlopShot *shot);
BOOL SwalotPlopShot_CheckOutside(SwalotPlopShot *shot);
void SwalotPlopShot_Catch(SwalotPlopShot *shot, SwalotPlopGame *game);
void SwalotPlopGame_UpdateCaughtShot(SwalotPlopShot *shot, SwalotPlopGame *game);
void SwalotPlopShot_Move(SwalotPlopShot *shot, const VecFx32 *move);
void SwalotPlopGame_HandleTouch(SwalotPlopGame *game);
void SwalotPlopGame_Throw(SwalotPlopGame *game);
void SwalotPlopGame_TrackFlick(SwalotPlopGame *game);
BOOL SwalotPlopGame_CanStartFlick(SwalotPlopGame *game);
void SwalotPlopFlickQueue_Clear(SwalotPlopFlickQueue *queue);
void SwalotPlopFlickQueue_Push(SwalotPlopFlickQueue *queue, SwalotPlopFlick point);
BOOL SwalotPlopFlickQueue_Pop(SwalotPlopFlickQueue *queue, SwalotPlopFlick *point);
BOOL SwalotPlopFlickQueue_PeekFirst(SwalotPlopFlickQueue *queue, SwalotPlopPoint *point);
BOOL SwalotPlopFlickQueue_PeekLast(SwalotPlopFlickQueue *queue, SwalotPlopPoint *point);
u32 SwalotPlopFlickQueue_Count(SwalotPlopFlickQueue *queue);
void SwalotPlopGame_EndFlick(SwalotPlopGame *game);
void SwalotPlopLevelBlend_Init(SwalotPlopLevelBlend *blend, SwalotPlopShared *shared, const u8 *options);
void SwalotPlopLevelBlend_Start(SwalotPlopLevelBlend *blend, SwalotPlopShared *shared, const u8 *options, int from, u16 to);
BOOL SwalotPlopLevelBlend_Update(SwalotPlopLevelBlend *blend);
void SwalotPlopGame_ApplyLevel(SwalotPlopGame *game, SwalotPlopLevelBlend *blend);
void SwalotPlopGame_ApplyLevelParams(SwalotPlopGame *game, SwalotPlopLevelBlend *blend);
void SwalotPlop_GetLevel(SwalotPlopShared *shared, const u8 *options, int index, SwalotPlopLevel *level);
void SwalotPlopDemo_Init(SwalotPlopDemo *demo, SwalotPlopGfx *gfx, enum HeapID heapId);
void SwalotPlopDemo_Free(SwalotPlopDemo *demo, SwalotPlopGfx *gfx);
void SwalotPlopDemo_Start(SwalotPlopDemo *demo);
BOOL SwalotPlopDemo_Update(SwalotPlopDemo *demo, SwalotPlopGame *game);
void SwalotPlopScoreReports_Init(SwalotPlopScoreReports *reports);
void SwalotPlopScoreReports_Set(SwalotPlopScoreReports *reports, u32 score, u32 index);
void SwalotPlopScoreReports_Reset(SwalotPlopScoreReports *reports);
u32 SwalotPlopScoreReports_GetLeader(SwalotPlopScoreReports *reports);
BOOL SwalotPlopScoreReports_HasAll(SwalotPlopScoreReports *reports, u32 playerCount);
void SwalotPlopGfx_SetBanks(void);
void SwalotPlopGfx_Setup3D(void);
void SwalotPlopGfx_Init3D(SwalotPlopGfx *gfx, int playerCount, int localIndex, enum HeapID heapId);
void SwalotPlopGfx_Free3D(SwalotPlopGfx *gfx);
void SwalotPlopGame_InitGfx(SwalotPlopGame *game, enum HeapID heapId);
void SwalotPlopGame_FreeGfx(SwalotPlopGame *game);
void SwalotPlopGame_UpdateGfx(SwalotPlopGame *game, SwalotPlopGfx *gfx);
void SwalotPlopGame_Draw(SwalotPlopGame *game, SwalotPlopGfx *gfx);
void SwalotPlopGfx_InitBgs(BgConfig **bgConfig, enum HeapID heapId);
void SwalotPlopGfx_FreeBgs(SwalotPlopGfx *gfx);
void SwalotPlopGfx_InitSprites(SwalotPlopGfx *gfx, enum HeapID heapId);
void SwalotPlopGfx_FreeSprites(SwalotPlopGfx *gfx);
void SwalotPlopGfx_LoadSpriteRes(SwalotPlopGfx *gfx, NARC *narc, enum HeapID heapId);
void SwalotPlopGfx_FreeSpriteRes(SwalotPlopGfx *gfx);
void SwalotPlopGfx_InitText(SwalotPlopGfx *gfx, enum HeapID heapId);
void SwalotPlopGfx_FreeText(SwalotPlopGfx *gfx);
void SwalotPlopGfx_InitCamera(SwalotPlopGfx *gfx, int playerCount, int localIndex, enum HeapID heapId);
void SwalotPlopGfx_FreeCamera(SwalotPlopGfx *gfx);
void SwalotPlopGfx_ApplyCamera(SwalotPlopGfx *gfx);
void SwalotPlopGfx_LoadSubBgs(SwalotPlopGfx *gfx, NARC *narc, int localIndex, enum HeapID heapId);
void SwalotPlopGfx_FreeDummy2(SwalotPlopGfx *gfx);
void SwalotPlopScene_Init(SwalotPlopScene *scene, NARC *narc, enum HeapID heapId, NNSFndAllocator *allocator);
void SwalotPlopScene_Free(SwalotPlopScene *scene, NNSFndAllocator *allocator);
void SwalotPlopScene_Update(SwalotPlopScene *scene, SwalotPlopBody *body, u32 phase);
void SwalotPlopScene_Animate(SwalotPlopScene *scene, SwalotPlopBody *body, u32 phase);
void SwalotPlopScene_SetPos(SwalotPlopScene *scene, SwalotPlopBody *body);
void SwalotPlopScene_SetSpin(SwalotPlopScene *scene, SwalotPlopBody *body);
void SwalotPlopScene_SetScale(SwalotPlopScene *scene, SwalotPlopBody *body);
void SwalotPlopScene_Draw(SwalotPlopScene *scene, SwalotPlopBody *body);
void SwalotPlopScene_SetTiltFrame(SwalotPlopScene *scene, u16 tilt);
void SwalotPlopScene_SetAnimation(SwalotPlopScene *scene, u32 animation);
BOOL SwalotPlopScene_AdvanceAnimation(SwalotPlopScene *scene, fx32 step);
void SwalotPlopScene_SetBaseAnimation(SwalotPlopScene *scene, int state);
void SwalotPlopScene_SetBonus(SwalotPlopScene *scene, u16 bonus);
void SwalotPlopScene_SetLeader(SwalotPlopScene *scene, u32 leader);
void SwalotPlopScene_UpdateSpecial(SwalotPlopScene *scene);
void SwalotPlop_TouchToWorld(int x, int y, VecFx32 *pos);
void SwalotPlop_WorldToTouch(const VecFx32 *pos, SwalotPlopPoint *point);
void SwalotPlop_RotateToSeat(const VecFx32 *src, VecFx32 *dst, int seat, int playerCount);
void SwalotPlop_RotateFromSeat(const VecFx32 *src, VecFx32 *dst, int seat, int playerCount);
void SwalotPlopGfx_InitShotViews(SwalotPlopGfx *gfx, NARC *narc, enum HeapID heapId);
void SwalotPlopGfx_FreeShotViews(SwalotPlopGfx *gfx);
void SwalotPlopGfx_AddShotView(SwalotPlopGfx *gfx, SwalotPlopShot *shot, u32 playerCount, u32 localIndex);
void SwalotPlopGfx_RemoveShotView(SwalotPlopGfx *gfx, SwalotPlopShot *shot);
void SwalotPlopGfx_DrawShotViews(SwalotPlopGfx *gfx, u32 playerCount);
void SwalotPlopShotView_Init(SwalotPlopGfx *gfx, SwalotPlopShotView *view, enum HeapID heapId);
void SwalotPlopShotView_Free(SwalotPlopGfx *gfx, SwalotPlopShotView *view);
void SwalotPlopShotView_StartSprite(SwalotPlopGfx *gfx, SwalotPlopShotView *view, SwalotPlopShot *shot, u32 playerCount);
void SwalotPlopShotView_StartModel(SwalotPlopGfx *gfx, SwalotPlopShotView *view, SwalotPlopShot *shot);
void SwalotPlopShotView_Draw(SwalotPlopShotView *view, SwalotPlopGfx *gfx, u32 playerCount);
void SwalotPlopShotView_PlaceShadow(SwalotPlopShotView *view);
void SwalotPlopShotView_ResetPolygonAttr(SwalotPlopShotView *view);
void SwalotPlopShotView_Clear(SwalotPlopShotView *view);
SwalotPlopShotView *SwalotPlopGfx_GetFreeShotView(SwalotPlopGfx *gfx);
BOOL SwalotPlopShotView_UpdateSprite(SwalotPlopShotView *view, SwalotPlopShot *shot, u32 playerCount);
void SwalotPlopShotView_SwitchToModel(SwalotPlopShotView *view, SwalotPlopGfx *gfx);
ModelResource *SwalotPlop_GetShotModel(SwalotPlopShot *shot, ModelResource *models);
void SwalotPlopShotView_Spin(SwalotPlopShotView *view);
void SwalotPlopCursor_Init(SwalotPlopGfx *gfx, SwalotPlopCursor *cursor, int palette, enum HeapID heapId);
void SwalotPlopCursor_Free(SwalotPlopCursor *cursor);
void SwalotPlopCursor_Show(SwalotPlopCursor *cursor);
void SwalotPlopCursor_Update(SwalotPlopCursor *cursor);
void SwalotPlopCursor_SetPos(SwalotPlopCursor *cursor, int x, int y);
void SwalotPlopCursor_Hide(SwalotPlopCursor *cursor);
void SwalotPlopParticles_Init(SwalotPlopGfx *gfx, SwalotPlopParticle *particle, int palette, enum HeapID heapId);
void SwalotPlopParticles_Free(SwalotPlopGfx *gfx, SwalotPlopParticle *particle);
void SwalotPlopParticles_Update(SwalotPlopParticle *particles);
void SwalotPlopParticles_Spawn(SwalotPlopParticle *particles, s32 x, s32 y);
void SwalotPlop_CalcArcStep(int step, int steps, int radius, int startAngle, int endAngle, int *x, int *y);
void SwalotPlopStage_Init(SwalotPlopStage *stage, SwalotPlopGfx *gfx, NARC *narc, int playerCount, int localIndex, enum HeapID heapId, NNSFndAllocator *volatile allocator);
void SwalotPlopStage_Free(SwalotPlopStage *stage, SwalotPlopGfx *gfx, NNSFndAllocator *allocator);
void SwalotPlopStage_Draw(SwalotPlopStage *stage);
void SwalotPlopStage_StartMouth(SwalotPlopStage *stage);
void SwalotPlopStage_SetAnimSpeed(SwalotPlopStage *stage, fx32 speed);
void SwalotPlop_IntersectPlane(const VecFx32 *pos, const VecFx32 *dir, const VecFx32 *normal, fx32 planeDist, VecFx32 *hit, fx32 *t);
void SwalotPlopGfx_SetCameraHeight(SwalotPlopGfx *gfx, fx32 height);
void SwalotPlopGfx_SetCameraPitch(SwalotPlopGfx *gfx, u16 pitch);
void SwalotPlopBgFade_Init(SwalotPlopBgFade *fade, u8 palette);
void SwalotPlopBgFade_Update(SwalotPlopBgFade *fade, SwalotPlopGfx *gfx);
void SwalotPlopBgFade_Step(SwalotPlopBgFade *fade);
void SwalotPlopBgFade_SetLayers(SwalotPlopGfx *gfx, int top, int palette);
void SwalotPlopCombo_Init(SwalotPlopCombo *combo, SwalotPlopGfx *gfx, NARC *narc, enum HeapID heapId);
void SwalotPlopCombo_Free(SwalotPlopCombo *combo, SwalotPlopGfx *gfx);
void SwalotPlopCombo_Show(SwalotPlopCombo *combo, SwalotPlopGfx *gfx, int count);
void SwalotPlopCombo_Hide(SwalotPlopCombo *combo);
void SwalotPlopCombo_SetCount(SwalotPlopCombo *combo, SwalotPlopGfx *gfx, int count);
void SwalotPlopCombo_Update(SwalotPlopCombo *combo);
void SwalotPlopSpeedUp_Start(SwalotPlopSpeedUp *speedUp);
void SwalotPlopSpeedUp_Update(SwalotPlopSpeedUp *speedUp, SwalotPlopStage *stage, SwalotPlopScene *scene);
void SwalotPlopSpeedUp_ResetTempo(SwalotPlopSpeedUp *speedUp);
const SwalotPlopCommand *SwalotPlop_GetCommandTable(void);
int SwalotPlop_GetCommandCount(void);
void SwalotPlop_RegisterCommands(SwalotPlopApp *app);
u32 SwalotPlop_GetNoDataSize(void);
void SwalotPlop_HandleStart(int netId, int size, void *data, void *context);
void SwalotPlop_HandleFinish(int netId, int size, void *data, void *context);
void SwalotPlop_HandleShot(int netId, int size, void *data, void *context);
void SwalotPlop_HandleFinalScore(int netId, int size, void *data, void *context);
void SwalotPlop_HandleScores(int netId, int size, void *data, void *context);
void SwalotPlop_HandleScoreReport(int netId, int size, void *data, void *context);
void SwalotPlop_HandleSync(int netId, int size, void *data, void *context);
void SwalotPlop_HandleLevel(int netId, int size, void *data, void *context);
u32 SwalotPlop_GetShotSize(void);
u32 SwalotPlop_GetScoresSize(void);
u32 SwalotPlop_GetScoreSize(void);
u32 SwalotPlop_GetLevelSize(void);

#endif // POKEHEARTGOLD_APPLICATION_PLAZA_GAME_SWALOT_PLOP_H
