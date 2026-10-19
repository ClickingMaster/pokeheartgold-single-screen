#ifndef POKEHEARTGOLD_WOBBUFFET_POP_H
#define POKEHEARTGOLD_WOBBUFFET_POP_H

#include "application/plaza_game.h"

#include "bg_window.h"
#include "camera.h"
#include "font_sprite.h"
#include "gf_3d_vramman.h"
#include "message_format.h"
#include "model_animation.h"
#include "msgdata.h"
#include "obj_char_transfer.h"
#include "overlay_manager.h"
#include "palette.h"
#include "particle_system.h"
#include "pm_string.h"
#include "sprite_system.h"
#include "sys_task_api.h"

// WOBBUFFET Pop, the Wi-Fi Plaza balloon game: every player pumps air into a balloon by
// touching the lower screen.

// Overlay manager data of the whole game: the player roster and the three screens it runs in turn.
typedef struct WobbuffetPopApp {
    PlazaGameRoster roster;
    PlazaGameResultData results;
    OverlayManager *screen;
    u8 netIds[4];
    u8 playerCount;
    u8 screenId; // 0 player list, 1 game, 2 results
    u8 unk_32[2];
    u32 selection;
    u32 playAgain;
    u8 voiceChat;
    u8 connectionLost;
    u8 connectionLostState;
    u8 unk_3F;
} WobbuffetPopApp;

// One queued game event (local and per-player queues, see WobbuffetPop).
typedef struct WobbuffetPopEvent {
    u32 kind;
    union {
        struct {
            u32 a, b, c, d;
        } words;
        struct {
            u8 b4, b5, b6, b7;
        } bytes;
        struct {
            u16 h4;
            u8 b6, b7, b8;
        } halves;
    } data;
} WobbuffetPopEvent;

#define WOBBUFFET_POP_EVENT_QUEUE_SIZE 60

enum WobbuffetPopGameState {
    WOBBUFFET_POP_GAME_IDLE,
    WOBBUFFET_POP_GAME_START_DEMO,
    WOBBUFFET_POP_GAME_DEMO,
    WOBBUFFET_POP_GAME_START_COUNTDOWN,
    WOBBUFFET_POP_GAME_COUNTDOWN,
    WOBBUFFET_POP_GAME_PLAYING,
    WOBBUFFET_POP_GAME_START_FINISH,
    WOBBUFFET_POP_GAME_FINISH,
    WOBBUFFET_POP_GAME_FINISHED,
};

enum WobbuffetPopEventKind {
    WOBBUFFET_POP_EVENT_NONE,
    WOBBUFFET_POP_EVENT_PUMP,
    WOBBUFFET_POP_EVENT_POP,
    WOBBUFFET_POP_EVENT_NEW_BALLOON,
    WOBBUFFET_POP_EVENT_COUNTDOWN,
    WOBBUFFET_POP_EVENT_FINISH,
    WOBBUFFET_POP_EVENT_SYNC,
    WOBBUFFET_POP_EVENT_DEMO,
};

enum WobbuffetPopScreenId {
    WOBBUFFET_POP_SCREEN_PLAYER_LIST,
    WOBBUFFET_POP_SCREEN_GAME,
    WOBBUFFET_POP_SCREEN_RESULTS,
};

enum WobbuffetPopConnectionLostState {
    WOBBUFFET_POP_CONNECTION_LOST_BLACK_OUT,
    WOBBUFFET_POP_CONNECTION_LOST_WAIT,
    WOBBUFFET_POP_CONNECTION_LOST_FREE,
};

enum WobbuffetPopSyncKind {
    WOBBUFFET_POP_SYNC_NONE,
    WOBBUFFET_POP_SYNC_START,
    WOBBUFFET_POP_SYNC_COUNTDOWN,
    WOBBUFFET_POP_SYNC_FINISH,
    WOBBUFFET_POP_SYNC_NEW_BALLOON,
    WOBBUFFET_POP_SYNC_POP,
    WOBBUFFET_POP_SYNC_DEMO,
    WOBBUFFET_POP_SYNC_WAITING,
};

// State of the host's game steps (start, play, finish).
typedef struct WobbuffetPopSequence {
    u32 step;
    u8 unk_04[8];
    int state;
    u32 unk_10;
    s32 timer;
    u8 syncId;
    u8 unk_19[3];
} WobbuffetPopSequence;

// One animated model of the upper screen, started by WobbuffetPop_AddModelSlot.
typedef struct WobbuffetPopModelSlot {
    u32 active;
    ModelInstance instance;
    ModelAnimation animation;
} WobbuffetPopModelSlot;

typedef struct WobbuffetPopModels {
    ModelResource localResource;
    ModelInstance localModel;
    ModelResource slotResources[4];
    WobbuffetPopModelSlot slots[8][4];
} WobbuffetPopModels;

typedef struct WobbuffetPopCommonModel {
    ModelResource resource;
    ModelInstance instance;
} WobbuffetPopCommonModel;

// One row of the touch-screen mesh: its four corners, as positions (0..128 per axis, fx32) or as
// texture coordinates.
typedef struct WobbuffetPopMeshQuad {
    fx32 topLeftX;
    fx32 topLeftY;
    fx32 bottomLeftX;
    fx32 bottomLeftY;
    fx32 topRightX;
    fx32 topRightY;
    fx32 bottomRightX;
    fx32 bottomRightY;
} WobbuffetPopMeshQuad;

#define WOBBUFFET_POP_MESH_ROWS 8

// The balloon the local player pumps (lower screen): texture, palette blend and squash/stretch.
typedef struct WobbuffetPopBalloon {
    void *textureFile;
    NNSG3dResTex *texture;
    u32 unk_008;
    WobbuffetPopMeshQuad vertices[WOBBUFFET_POP_MESH_ROWS]; // row 0 is the pushed top
    WobbuffetPopMeshQuad texCoords[WOBBUFFET_POP_MESH_ROWS];
    s32 touch[4]; // x, y, held, new
    s32 handleX;
    u32 unk_220;
    s32 grabX;
    s32 grabY;
    s32 grabOffsetY;       // touch y minus the handle's y
    fx32 pushDepth;        // how far the top is pushed down
    ManagedSprite *sprite; // the handle
    u32 grabbing;
    s32 lastTouchY;
    s32 grabAreaPercent; // mesh area when the handle was grabbed
    s32 pumpStrength;
    s32 reboundSpeed; // how fast the released mesh springs back
    u16 *paletteSrc;
    u16 *paletteDest;
    u32 paletteSize;
    u32 paletteAddr;
    s32 air; // 0..800
    fx32 scaleX;
    fx32 scaleY;
    s16 squash;
    s16 extraScaleYPercent;
    s16 extraScaleXPercent;
    u8 unk_26E[2];
    u32 selection;
    u8 playingPumpSe;
    u8 flashCounter;
    u8 paletteDirty;
    u8 unk_277;
} WobbuffetPopBalloon;

// The balloon being inflated on the upper screen (BG 7, affine-scaled with its air).
typedef struct WobbuffetPopInflation {
    u32 active;
    u32 size; // balloon size (0 small .. 2 large)
    s32 air;
    s32 capacity;     // pops at this much air
    s32 pendingAir;   // air sent but not shown yet
    s32 airStep;      // pendingAir / 6 is added per frame
    s32 playerAir[4]; // per seat
    u32 jitterToggle;
    u8 showBg; // show the touch screen's BG 3 at the next VBlank
    u8 hideBg;
    u8 unk_2E[2];
} WobbuffetPopInflation;

// Two little sprites a bonus target throws off when it lands.
typedef struct WobbuffetPopDust {
    ManagedSprite *sprites[2];
    s32 speedX[2];
    s32 speedY[2];
    u8 active;
    u8 timer;
    u8 unk_1A[2];
} WobbuffetPopDust;

// A Wobbuffet hopping around the ring on the lower screen. A puff that reaches the ring while one stands at the
// front (180 degrees) gets its bonus (x2, x1/2 or x3).
typedef struct WobbuffetPopBonusTarget {
    ManagedSprite *sprite;
    ManagedSprite *effect;
    ManagedSprite *shadow;
    fx32 angle;
    u8 state; // index into the state table
    u8 step;
    u8 timer;
    u8 halfTime;
    u8 bonus;
    u8 atFront;
    u8 unk_16[2];
    s32 duration;
    union {
        struct {
            s16 x;
            s16 y;
        } home; // entering the ring
        struct {
            u32 unk_00;
            fx32 dx;
            fx32 dy;
            fx32 x;
            fx32 y;
        } hop; // hopping to the next spot
    } motion;
    WobbuffetPopDust dust;
} WobbuffetPopBonusTarget;

typedef struct WobbuffetPopBonusRing {
    WobbuffetPopBonusTarget targets[3];
    fx32 angle;
    fx32 angleStep;
    s32 ticks;
    u8 position; // steps of 30 degrees
    u8 done;
    u8 delay;
    u8 stepTimer;
    u8 speed; // row of the speed table
    u8 unk_F5[3];
} WobbuffetPopBonusRing;

// Bonus another player got, dropping in from the top of the lower screen.
typedef struct WobbuffetPopBonusPopup {
    ManagedSprite *sprite;
    ManagedSprite *effect;
    fx32 x;
    fx32 y;
    s16 targetY;
    u8 bonus;
    u8 state;
    u8 timer;
    u8 active;
    s16 delay;
} WobbuffetPopBonusPopup;

// A text OBJ made by WobbuffetPop_NewTextObj.
typedef struct WobbuffetPopTextObj {
    TextOBJ *textObj;
    ObjCharSpaceReservation charAlloc;
    u16 width;
    u8 unk_12[2];
} WobbuffetPopTextObj;

// The score on the upper screen: six rolling digits.
typedef struct WobbuffetPopScoreDisplay {
    ManagedSprite *frame;
    struct {
        WobbuffetPopTextObj top;
        WobbuffetPopTextObj bottom;
    } digits[6];
    WobbuffetPopTextObj title;
    WobbuffetPopTextObj labels[6];
    u8 current[6]; // roll position of each digit (16 per digit value, 160 per turn)
    u8 goal[6];
    u8 next[6];
    u8 passes[6];
    s8 countdown[6];
    s8 delay;
    u8 settled;
    u8 state;
    u8 unk_1A1[3];
} WobbuffetPopScoreDisplay;

// The how-to-play demo: a stylus pressing the handle down twice, fed to the balloon as touches.
typedef struct WobbuffetPopDemo {
    ManagedSprite *stylus;
    s16 state;
    vs16 timer;
    u32 unk_08;
    s32 touch[4]; // x, y, held, new
} WobbuffetPopDemo;

// Air sent by one player, in the local queue until a bonus target decides its bonus.
typedef struct WobbuffetPopPump {
    u8 netId;
    u8 bonus; // 0 none, 1 x2, 2 x1/2, 3 x3
    u16 balloonIndex;
    s32 amount;
    s32 bonusAmount;
    u8 bonusSet;
    u8 unk_D[3];
} WobbuffetPopPump;

// A puff of air flying from a player's side of the lower screen to the ring.
typedef struct WobbuffetPopPuff {
    u32 active;
    u16 netId;
    u16 balloonIndex;
    s32 amount;
    s16 delay;
    u8 bonus;
    u8 unk_0F;
    ManagedSprite *sprite;
    const u8 *config; // x, y, animation, palette, ..., direction (7)
} WobbuffetPopPuff;

// One piece of the confetti shown when a balloon pops.
typedef struct WobbuffetPopConfettiPiece {
    ManagedSprite *sprite;
    fx32 radius;
    fx32 radiusSpeed;
    fx32 angle;
    fx32 angleSpeed;
    s16 life;
    u16 unk_16;
} WobbuffetPopConfettiPiece;

typedef struct WobbuffetPopConfetti {
    u32 active;
    WobbuffetPopConfettiPiece playerPieces[36]; // in the players' colours, by share of the air
    WobbuffetPopConfettiPiece pieces[8];
    WobbuffetPopConfettiPiece balloonPieces[3]; // in the balloon's colour
    u32 unk_46C;
} WobbuffetPopConfetti;

// One of the next balloons, queued at the top of the lower screen.
typedef struct WobbuffetPopBalloonIcon {
    ManagedSprite *sprite;
    u8 size;
    u8 state;    // 1 waiting, 2 nearly full, 3 popping
    u8 position; // 0 is the current balloon
    u8 unk_7;
    s16 number;
    u16 unk_A;
} WobbuffetPopBalloonIcon;

// Overlay manager data of the game screen.
typedef struct WobbuffetPop {
    WobbuffetPopApp *app;
    u32 hostNetId;
    u32 readyCount;
    u32 playerValues[4];
    PlazaGameSign *sign;
    u32 gameState;
    SpriteSystem *spriteSystem;
    SpriteManager *spriteManager;
    BgConfig *bgConfig;
    Window playerNameWindows[4];
    Window messageWindow;
    MsgData *msgData;
    MessageFormat *msgFormat;
    String *string;
    PaletteData *paletteData;
    FontSystem *fontSystem;
    SysTask *drawTask;
    GF3DVramMan *vramMan;
    Camera *camera;
    Camera *particleCamera;
    SPLEmitter *particleSystem;
    NNSFndAllocator allocator;
    WobbuffetPopSequence sequence;
    WobbuffetPopBalloon *balloon;
    WobbuffetPopModels models;
    WobbuffetPopCommonModel common;
    WobbuffetPopInflation inflation;
    ManagedSprite *seatSprites[4];
    WobbuffetPopBonusRing bonusRing;
    WobbuffetPopBonusPopup bonusPopups[3];
    WobbuffetPopScoreDisplay scoreDisplay;
    WobbuffetPopDemo demo;
    u32 demoDone;
    WobbuffetPopEvent events[WOBBUFFET_POP_EVENT_QUEUE_SIZE];
    WobbuffetPopEvent playerEvents[4][WOBBUFFET_POP_EVENT_QUEUE_SIZE];
    WobbuffetPopEvent currentEvent;
    WobbuffetPopEvent pendingEvent;
    s32 playerEventWriteCounts[4];
    u32 playerEventReadCounts[4];
    s32 eventWriteCount;
    s32 eventReadCount;
    WobbuffetPopPump pumpQueue[8];
    s32 pumpQueueRead;
    s32 pumpQueueWrite;
    u32 timer;
    u32 started;
    u32 gameOver; // no more pumping
    u32 finished;
    s32 eventDelay;
    u32 balloonIndex;
    u32 balloonInPlay;
    u32 total;
    u32 syncKind;
    u32 syncId;
    WobbuffetPopPuff puffs[40];
    WobbuffetPopConfetti confetti;
    WobbuffetPopBalloonIcon balloonIcons[5];
    s32 score;
    u32 frameCounter;
} WobbuffetPop;

BOOL WobbuffetPopApp_Init(OverlayManager *man, int *state);
BOOL WobbuffetPopApp_Main(OverlayManager *man, int *state);
BOOL WobbuffetPopApp_Exit(OverlayManager *man, int *state);
BOOL WobbuffetPopPlazaScreen_Init(OverlayManager *man, int *state);
BOOL WobbuffetPopPlazaScreen_Main(OverlayManager *man, int *state);
BOOL WobbuffetPopPlazaScreen_Exit(OverlayManager *man, int *state);

void WobbuffetPopBalloon_ReleaseTouch(WobbuffetPopBalloon *balloon);
void WobbuffetPopBalloon_LoadShininessTable(void);
void WobbuffetPopBalloon_InitMeshVertices(WobbuffetPopBalloon *balloon);
void WobbuffetPopBalloon_InitMeshTexCoords(WobbuffetPopBalloon *balloon);
void WobbuffetPopBalloon_DrawMesh(WobbuffetPopBalloon *balloon);
int WobbuffetPopBalloon_TryGrabHandle(WobbuffetPopBalloon *balloon);
int WobbuffetPopBalloon_UpdateDrag(WobbuffetPopBalloon *balloon);
s32 WobbuffetPopBalloon_CalcMeshAreaPercent(WobbuffetPopBalloon *balloon);
void WobbuffetPop_LoadHandleSpriteResources(WobbuffetPop *game);
void WobbuffetPop_FreeHandleSpriteResources(WobbuffetPop *game);
ManagedSprite *WobbuffetPop_CreateHandleSprite(WobbuffetPop *game);
void WobbuffetPopBalloon_UpdateHandleSprite(WobbuffetPopBalloon *balloon, ManagedSprite *sprite);
void WobbuffetPop_DeleteHandleSprite(ManagedSprite *sprite);

int WobbuffetPop_GetPlayerSeat(WobbuffetPop *game, int netId);
BOOL WobbuffetPop_AddModelSlot(WobbuffetPop *game, u32 amount);
void WobbuffetPop_QueuePlayerEvent(WobbuffetPop *game, int netId, const WobbuffetPopEvent *event);
void WobbuffetPop_QueueEvent(WobbuffetPop *game, const WobbuffetPopEvent *event);
void WobbuffetPop_RequestSync(WobbuffetPop *game, u32 kind, u32 syncId);

int WobbuffetPop_GetBalloonSize(int n);
void WobbuffetPop_UpdateInflation(WobbuffetPop *game, BgConfig *bgConfig, WobbuffetPopInflation *inflation);
BOOL WobbuffetPop_LaunchPuff(WobbuffetPop *game, const WobbuffetPopPump *pump);
void WobbuffetPop_UpdatePuffs(WobbuffetPop *game);
void WobbuffetPop_FreePuffs(WobbuffetPop *game);
void WobbuffetPop_FreeConfetti(WobbuffetPop *game);
BOOL WobbuffetPop_UpdateConfetti(WobbuffetPop *game);
void WobbuffetPop_InitBalloonIcons(WobbuffetPop *game);
void WobbuffetPop_FreeBalloonIcons(WobbuffetPop *game);
BOOL WobbuffetPop_UpdateBalloonIcons(WobbuffetPop *game);
void WobbuffetPop_SetupThreePlayerSubBg(WobbuffetPop *game);
void WobbuffetPop_SetPlayerSubBgColors(WobbuffetPop *game);
void WobbuffetPop_SetPlayerSubObjPalettes(WobbuffetPop *game);
void WobbuffetPop_SetPlayerSubObjColors(WobbuffetPop *game);
void WobbuffetPop_InitPump(WobbuffetPop *game, u32 balloonIndex, u32 amount, WobbuffetPopPump *pump);
BOOL WobbuffetPop_QueuePump(WobbuffetPop *game, const WobbuffetPopPump *pump);
void WobbuffetPop_CreateSeatSprites(WobbuffetPop *game, ManagedSprite **sprites);
void WobbuffetPop_FreeSeatSprites(WobbuffetPop *game, ManagedSprite **sprites);
void WobbuffetPopBonusRing_CreateSprites(WobbuffetPop *game, WobbuffetPopBonusRing *ring);
void WobbuffetPopBonusRing_FreeSprites(WobbuffetPop *game, WobbuffetPopBonusRing *ring);
void WobbuffetPopBonusRing_Init(WobbuffetPop *game, WobbuffetPopBonusRing *ring);
void WobbuffetPopBonusRing_Update(WobbuffetPop *game, WobbuffetPopBonusRing *ring);
void WobbuffetPop_CreateBonusPopupSprites(WobbuffetPop *game, WobbuffetPopBonusPopup *popups);
void WobbuffetPop_FreeBonusPopupSprites(WobbuffetPop *game, WobbuffetPopBonusPopup *popups);
void WobbuffetPop_UpdateBonusPopups(WobbuffetPop *game, WobbuffetPopBonusPopup *popups);
void WobbuffetPop_NewTextObj(BgConfig *bgConfig, SpriteManager *spriteManager, FontSystem *fontSystem, WobbuffetPopTextObj *textObj, String *string, int fontId, u32 color, int setPalOffset, int plttResId, int x, int y, int centered, int unk_20, int vram, int height);
void WobbuffetPopTextObj_Free(WobbuffetPopTextObj *textObj);
void WobbuffetPopScoreDisplay_PlaceDigits(WobbuffetPopScoreDisplay *display);
void WobbuffetPopScoreDisplay_Update(WobbuffetPop *game, WobbuffetPopScoreDisplay *display);
ManagedSprite *WobbuffetPop_CreateScoreFrameSprite(WobbuffetPop *game);
void WobbuffetPop_CreateScoreLabels(WobbuffetPop *game);
void WobbuffetPop_FreeScoreLabels(WobbuffetPop *game);
void WobbuffetPop_DeleteScoreFrameSprite(WobbuffetPop *game, ManagedSprite *sprite);
ManagedSprite *WobbuffetPop_CreateDemoStylusSprite(WobbuffetPop *game);
void WobbuffetPop_DeleteDemoStylusSprite(WobbuffetPop *game, ManagedSprite *sprite);
BOOL WobbuffetPop_RunDemo(WobbuffetPop *game, WobbuffetPopDemo *demo);
void WobbuffetPop_RegisterCommands(WobbuffetPop *game);
BOOL WobbuffetPop_SendEvent(WobbuffetPop *game, WobbuffetPopEvent *event);
BOOL WobbuffetPop_SendFinished(WobbuffetPop *game);
BOOL WobbuffetPop_SendPlayerValue(void);
BOOL WobbuffetPop_RunEvent(WobbuffetPop *game, WobbuffetPopEvent *event);
void WobbuffetPop_QueuePopEvent(WobbuffetPop *game, u32 balloonIndex, u8 syncId);
void WobbuffetPop_QueueNewBalloonEvent(WobbuffetPop *game, int balloonIndex, int size, u8 syncId);
void WobbuffetPop_QueueCountdownEvent(WobbuffetPop *game, u8 syncId);
void WobbuffetPop_QueueFinishEvent(WobbuffetPop *game, u8 syncId);
void WobbuffetPop_QueueSyncEvent(WobbuffetPop *game, int syncKind, u8 syncId);
void WobbuffetPop_QueueDemoEvent(WobbuffetPop *game, u8 syncId);

#endif // POKEHEARTGOLD_WOBBUFFET_POP_H
