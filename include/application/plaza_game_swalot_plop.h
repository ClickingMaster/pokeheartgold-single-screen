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

// SWALOT Plop (overlay 91), a Wi-Fi Plaza game: every player flicks berries on the touch
// screen towards a spinning SWALOT; berries that land in its mouth score points, and
// consecutive hits build a combo. The player list and the results screen come from the
// shared Plaza game code (application/plaza_game.h). The parent keeps the game clock and
// the hit count (SwalotPlopTally) and tells the others when to speed SWALOT up.

// One entry of the level table (NARC 212 file 0).
typedef struct SwalotPlopLevel {
    int hitCount;     // 0x00, hits needed to reach this level
    u32 spinSpeed;    // 0x04
    u32 cameraHeight; // 0x08
    u32 cameraPitch;  // 0x0C
    u32 mouthOffset;  // 0x10
    u32 scale;        // 0x14
} SwalotPlopLevel;

typedef struct SwalotPlopLevelTable {
    u32 count;
    SwalotPlopLevel *levels;
} SwalotPlopLevelTable;

// Part of SwalotPlopApp shared with the game screen and the tally.
typedef struct SwalotPlopShared {
    SwalotPlopLevelTable levelTable; // 0x00
    PlayerProfile *profiles[4];      // 0x08
    u32 nameFlags[4];                // 0x18
} SwalotPlopShared;

typedef struct SwalotPlopScores {
    u32 scores[4];
} SwalotPlopScores;

// Run on the parent only: game clock, the scores reported at the end, and the hit count
// that selects the level.
typedef struct SwalotPlopTally {
    int timer;                // 0x00
    BOOL running;             // 0x04
    u32 playerCount;          // 0x08
    u32 scores[4];            // 0x0C
    u32 receivedMask;         // 0x1C
    u8 unk20[0x10];           // 0x20
    SwalotPlopShared *shared; // 0x30
    u16 hitCount;             // 0x34
    volatile u8 level;        // 0x36
    u8 levelChanged;          // 0x37
} SwalotPlopTally;

// What a shot looks like on the link (command 24).
typedef struct SwalotPlopShotRecord {
    u16 owner;    // 0x00
    u16 caught;   // 0x02
    s16 dir[3];   // 0x04
    s16 start[3]; // 0x0A
    s16 speed;    // 0x10
    s8 combo;     // 0x12
    u8 big;       // 0x13
} SwalotPlopShotRecord;

typedef struct SwalotPlopShotParams {
    u16 owner;         // 0x00
    u16 caught;        // 0x02
    VecFx32 direction; // 0x04
    fx32 speed;        // 0x10
    VecFx32 start;     // 0x14
    u32 combo;         // 0x20
    u32 big;           // 0x24
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
    u8 state;                    // 0x00
    u8 remove;                   // 0x01
    u16 frames;                  // 0x02
    SwalotPlopShotParams params; // 0x04
    VecFx32 position;            // 0x2C
    s16 timer;                   // 0x38
    u16 combo;                   // 0x3A
    BOOL crossedRim;             // 0x3C
    fx32 distance;               // 0x40
} SwalotPlopShot;

// The touch samples of one frame (src/touch_sampling.c).
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
    BOOL active;               // 0x00
    SwalotPlopFlick points[8]; // 0x04
    u16 read;                  // 0x24
    u16 write;                 // 0x26
} SwalotPlopFlickQueue;

// SWALOT itself: orientation, mouth and body measurements (scaled per level).
typedef struct SwalotPlopBody {
    u8 active;            // 0x00
    u8 mouthOpen;         // 0x01
    u16 spin;             // 0x02
    u16 tilt;             // 0x04
    u16 spinSpeed;        // 0x06
    fx32 mouthDepth;      // 0x08
    MtxFx33 tiltMtx;      // 0x0C
    MtxFx33 spinMtx;      // 0x30
    MtxFx33 rotation;     // 0x54
    VecFx32 normal;       // 0x78
    VecFx32 mouthPos;     // 0x84
    fx32 mouthPlane;      // 0x90
    fx32 pushRadius;      // 0x94
    fx32 pushStrength;    // 0x98
    VecFx32 offset;       // 0x9C
    fx32 scale;           // 0xA8
    fx32 mouthDepthRange; // 0xAC
    fx32 mouthDepthMin;   // 0xB0
    fx32 unkB4;           // 0xB4
    fx32 mouthOffsetX;    // 0xB8
    fx32 mouthOffsetY;    // 0xBC
    fx32 heightMin;       // 0xC0
    fx32 heightRange;     // 0xC4
    fx32 topRadius;       // 0xC8
    fx32 radius;          // 0xCC
    fx32 slope;           // 0xD0
    fx32 modelScale;      // 0xD4
    VecFx32 position;     // 0xD8
    fx32 height;          // 0xE4
} SwalotPlopBody;

// SWALOT's model, its three animations and the mouth animation.
typedef struct SwalotPlopStage {
    ModelInstance models[3];      // 0x000
    ModelResource resources[3];   // 0x168
    ModelAnimation animations[3]; // 0x198
    fx32 animSpeed;               // 0x1D4
    u8 mouthActive;               // 0x1D8
    u8 mouthState;                // 0x1D9
    s16 mouthTimer;               // 0x1DA
    fx32 mouthFrame;              // 0x1DC
    fx32 mouthStep;               // 0x1E0
} SwalotPlopStage;

// The points that pop up over a player's seat when a berry is caught.
typedef struct SwalotPlopPopups {
    Sprite *sprites[3]; // 0x00
    s32 timers[3];      // 0x0C
    VecFx32 pos[3];     // 0x18
    int mode;           // 0x3C
} SwalotPlopPopups;

// The window with the other players' names shown during the intro.
typedef struct SwalotPlopIntro {
    u32 state;     // 0x00
    s32 delay;     // 0x04
    u32 counter;   // 0x08
    Window window; // 0x0C
} SwalotPlopIntro;

typedef struct SwalotPlopFinish {
    u32 state;
    u32 counter;
} SwalotPlopFinish;

// The SWALOT model of the top screen with its six animations.
typedef struct SwalotPlopScene {
    ModelInstance models[3];      // 0x000
    ModelResource resources[3];   // 0x168
    ModelAnimation animations[6]; // 0x198
    volatile u16 current;         // 0x210
    u16 state;                    // 0x212
    ModelAnimation special;       // 0x214
    volatile s32 frame;           // 0x228
    u16 leader;                   // 0x22C
    u16 bonus;                    // 0x22E
} SwalotPlopScene;

typedef struct SwalotPlopBgFade {
    u16 state;  // 0x0
    s16 timer;  // 0x2
    s16 layer;  // 0x4
    u8 palette; // 0x6
    u8 counter; // 0x7
} SwalotPlopBgFade;

// Sprite and shadow of a berry.
typedef struct SwalotPlopShotView {
    Sprite *sprite;       // 0x000
    ModelInstance model;  // 0x004
    ModelInstance shadow; // 0x07C
    int state;            // 0x0F4
    SwalotPlopShot *shot; // 0x0F8
    u16 rotX;             // 0x0FC
    u16 rotZ;             // 0x0FE
    u16 rotSpeedX;        // 0x100
    u16 rotSpeedZ;        // 0x102
    s16 timer;            // 0x104
} SwalotPlopShotView;

typedef struct SwalotPlopCursor {
    BOOL active;
    Sprite *sprite;
} SwalotPlopCursor;

typedef struct SwalotPlopParticle {
    BOOL active;      // 0x00
    Sprite *sprite;   // 0x04
    VecFx32 position; // 0x08
    s32 age;          // 0x14
    BOOL direction;   // 0x18
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
    SpriteResource *res[4];          // 0x00
    SpriteResourcesHeader header;    // 0x10
    Sprite *sprite;                  // 0x34
    Window window;                   // 0x38
    TextOBJ *textObj;                // 0x48
    TextOBJLayout *textLayout;       // 0x4C
    ObjCharSpaceReservation charTransfer; // 0x50
    String *string;                  // 0x5C
    SpriteResource *textPltt;        // 0x60
    SwalotPlopCurve slideX;          // 0x64
    SwalotPlopCurve slideY;          // 0x7C
    s16 slideFrame;                  // 0x94
    s16 bounceFrame;                 // 0x96
    u16 state;                       // 0x98
    s16 hideDelay;                   // 0x9A
    enum HeapID heapId;              // 0x9C
} SwalotPlopCombo;

// Graphics of the game screens.
typedef struct SwalotPlopGfx {
    BgConfig *bgConfig;                 // 0x0000
    MessageFormat *msgFormat;           // 0x0004
    MsgData *msgData;                   // 0x0008
    String *string1;                    // 0x000C
    String *string2;                    // 0x0010
    u32 unk14;                          // 0x0014
    GF3DVramMan *vramMan;               // 0x0018
    SpriteList *spriteList;             // 0x001C
    G2dRenderer renderer;               // 0x0020
    GF_2DGfxResMan *resMan[4];          // 0x0148
    SpriteResourcesHeader header;       // 0x0158
    SpriteResource *res[4];             // 0x017C
    FontSystem *fontSystem;             // 0x018C
    Camera *camera;                     // 0x0190
    VecFx32 cameraTarget;               // 0x0194
    SwalotPlopStage stage;              // 0x01A0
    SwalotPlopPopups popups[4];         // 0x0384
    SwalotPlopIntro intro;              // 0x0484
    SwalotPlopFinish finish;            // 0x04A0
    u8 unk4A8[0x18];                    // 0x04A8
    SwalotPlopScene scene;              // 0x04C0
    PlazaGameSign *sign;                // 0x06F0
    SwalotPlopBgFade bgFade;            // 0x06F4
    ModelResource shotModels[5];        // 0x06FC
    ModelResource shadowModel;          // 0x074C
    SpriteResource *shotRes[4];         // 0x075C
    SpriteResourcesHeader shotHeader;   // 0x076C
    SwalotPlopShotView shotViews[0x60]; // 0x0790
    SwalotPlopCursor cursor;            // 0x6A90
    SwalotPlopParticle particles[8];    // 0x6A98
    SwalotPlopCombo combo;              // 0x6B78
} SwalotPlopGfx;

// Interpolates the level parameters (spin, camera, scale) over 16 frames.
typedef struct SwalotPlopLevelBlend {
    s32 current[6]; // 0x00
    s32 start[6];   // 0x18
    s32 delta[6];   // 0x30
    s32 step;       // 0x48
} SwalotPlopLevelBlend;

// The hand that throws two demo berries before the game starts.
typedef struct SwalotPlopDemo {
    SpriteResource *res[4];       // 0x00
    SpriteResourcesHeader header; // 0x10
    Sprite *sprite;               // 0x34
    u8 active;                    // 0x38
    u8 count;                     // 0x39
    u8 state;                     // 0x3A
    u8 thrown;                    // 0x3B
    int timer;                    // 0x3C
} SwalotPlopDemo;

// The scores the players report every 30 frames; once all reported, the leader is shown.
typedef struct SwalotPlopScoreReports {
    u32 scores[4]; // 0x00
    u32 mask;      // 0x10
    u32 leader;    // 0x14
} SwalotPlopScoreReports;

typedef struct SwalotPlopSpeedUp {
    u16 state; // 0x0
    u8 active; // 0x2
    s8 frame;  // 0x3
} SwalotPlopSpeedUp;

enum SwalotPlopPhase {
    SWALOT_PLOP_PHASE_NORMAL,
    SWALOT_PLOP_PHASE_FAST,
    SWALOT_PLOP_PHASE_FASTEST,
};

// The game screen (heap 0x6A).
typedef struct SwalotPlopGame {
    u32 phase;                           // 0x0000
    BOOL running;                        // 0x0004
    u32 frame;                           // 0x0008
    u32 duration;                        // 0x000C
    u32 playerCount;                     // 0x0010
    u32 localIndex;                      // 0x0014
    s32 score;                           // 0x0018
    s32 combo;                           // 0x001C
    SwalotPlopFlickQueue flick;          // 0x0020
    u32 releasedFrames;                  // 0x0048
    SwalotPlopShot shots[0x60];          // 0x004C
    SwalotPlopBody body;                 // 0x19CC
    SwalotPlopGfx gfx;                   // 0x1AB4
    NNSFndAllocator allocator;           // 0x86CC
    u32 finalScores[4];                  // 0x86DC
    SwalotPlopTouch touch;               // 0x86EC
    SwalotPlopLevelBlend levelBlend;     // 0x8730
    SwalotPlopShared *shared;            // 0x877C
    u16 level;                           // 0x8780
    u16 prevLevel;                       // 0x8782
    u8 options[4];                       // 0x8784
    u32 optionsSet;                      // 0x8788
    SwalotPlopDemo demo;                 // 0x878C
    SwalotPlopScoreReports scoreReports; // 0x87CC
    SwalotPlopSpeedUp speedUp;           // 0x87E4
} SwalotPlopGame;

// Overlay work (heap 0x6A).
typedef struct SwalotPlopApp {
    PlazaGameEntry *entry;          // 0x00
    PlazaGameResults *results;      // 0x04
    PlazaGameRoster roster;         // 0x08
    PlazaGameResultData resultData; // 0x18
    SwalotPlopTally *tally;         // 0x30
    SwalotPlopGame *game;           // 0x34
    BOOL startReceived;             // 0x38
    BOOL finishReceived;            // 0x3C
    BOOL scoresReceived;            // 0x40
    u16 netId;                      // 0x44
    u16 localIndex;                 // 0x46
    SwalotPlopShared shared;        // 0x48
    u32 selection;                  // 0x70
    u32 playerCount;                // 0x74
    u32 connectionLostState;        // 0x78
    u32 syncRound;                  // 0x7C
    BOOL canSend;                   // 0x80
    u8 syncCounts[4];               // 0x84
} SwalotPlopApp;

// Link command table entry (Comm_RegisterCommandTable).
typedef struct SwalotPlopCommand {
    void (*handler)(int netId, int size, void *data, void *context);
    u32 (*getSize)(void);
    void *unk08;
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
