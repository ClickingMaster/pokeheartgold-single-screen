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

// WOBBUFFET Pop, the Wi-Fi Plaza balloon game (overlay 93): every player pumps air into a balloon by
// touching the lower screen.

// Overlay manager data of the whole game: the player roster and the three screens it runs in turn.
typedef struct WobbuffetPopApp {
    PlazaGameRoster roster;      // 0x00
    PlazaGameResultData results; // 0x10
    OverlayManager *screen;      // 0x28
    u8 netIds[4];                // 0x2C
    u8 playerCount;              // 0x30
    u8 screenId;                 // 0x31: 0 player list, 1 game, 2 results
    u8 unk_32[2];                // 0x32
    u32 selection;               // 0x34: player list result
    u32 playAgain;               // 0x38: results screen result
    u8 voiceChat;                // 0x3C
    u8 connectionLost;           // 0x3D
    u8 connectionLostState;      // 0x3E
    u8 unk_3F;                   // 0x3F
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
} WobbuffetPopEvent; // size: 0x14

#define WOBBUFFET_POP_EVENT_QUEUE_SIZE 60

// State of the host's game steps (start, play, finish).
typedef struct WobbuffetPopSequence {
    u32 step;     // 0x00
    u8 unk_04[8]; // 0x04
    int state;    // 0x0C
    u32 unk_10;   // 0x10
    s32 timer;    // 0x14
    u8 syncId;    // 0x18
    u8 unk_19[3]; // 0x19
} WobbuffetPopSequence;

// One animated model of the upper screen, started by WobbuffetPop_AddModelSlot.
typedef struct WobbuffetPopModelSlot {
    u32 active;               // 0x00
    ModelInstance instance;   // 0x04
    ModelAnimation animation; // 0x7C
} WobbuffetPopModelSlot;      // size: 0x90

typedef struct WobbuffetPopModels {
    ModelResource localResource;       // 0x000
    ModelInstance localModel;          // 0x010
    ModelResource slotResources[4];    // 0x088
    WobbuffetPopModelSlot slots[8][4]; // 0x0C8
} WobbuffetPopModels;

typedef struct WobbuffetPopCommonModel {
    ModelResource resource; // 0x00
    ModelInstance instance; // 0x10
} WobbuffetPopCommonModel;

// The balloon the local player pumps (lower screen): texture, palette blend and squash/stretch.
// One row of the touch-screen mesh: its four corners, as positions (0..128 per axis, fx32) or as
// texture coordinates.
typedef struct WobbuffetPopMeshQuad {
    fx32 topLeftX;     // 0x00
    fx32 topLeftY;     // 0x04
    fx32 bottomLeftX;  // 0x08
    fx32 bottomLeftY;  // 0x0C
    fx32 topRightX;    // 0x10
    fx32 topRightY;    // 0x14
    fx32 bottomRightX; // 0x18
    fx32 bottomRightY; // 0x1C
} WobbuffetPopMeshQuad;

#define WOBBUFFET_POP_MESH_ROWS 8

typedef struct WobbuffetPopBalloon {
    void *textureFile;                                       // 0x000
    NNSG3dResTex *texture;                                   // 0x004
    u32 unk_008;                                             // 0x008
    WobbuffetPopMeshQuad vertices[WOBBUFFET_POP_MESH_ROWS];  // 0x00C: row 0 is the pushed top
    WobbuffetPopMeshQuad texCoords[WOBBUFFET_POP_MESH_ROWS]; // 0x10C
    s32 touch[4];                                            // 0x20C: x, y, held, new
    s32 handleX;                                             // 0x21C
    u32 unk_220;                                             // 0x220
    s32 grabX;                                               // 0x224
    s32 grabY;                                               // 0x228
    s32 grabOffsetY;                                         // 0x22C: touch y minus the handle's y
    fx32 pushDepth;                                          // 0x230: how far the top is pushed down
    ManagedSprite *sprite;                                   // 0x234: the handle
    u32 grabbing;                                            // 0x238
    s32 lastTouchY;                                          // 0x23C
    s32 grabAreaPercent;                                     // 0x240: mesh area when the handle was grabbed
    s32 pumpStrength;                                        // 0x244
    s32 reboundSpeed;                                        // 0x248: how fast the released mesh springs back
    u16 *paletteSrc;                                         // 0x24C
    u16 *paletteDest;                                        // 0x250
    u32 paletteSize;                                         // 0x254
    u32 paletteAddr;                                         // 0x258
    s32 air;                                                 // 0x25C: 0..800
    fx32 scaleX;                                             // 0x260
    fx32 scaleY;                                             // 0x264
    s16 squash;                                              // 0x268
    s16 unk_26A;                                             // 0x26A
    s16 unk_26C;                                             // 0x26C
    u8 unk_26E[2];                                           // 0x26E
    u32 selection;                                           // 0x270
    u8 playingPumpSe;                                        // 0x274
    u8 flashCounter;                                         // 0x275
    u8 paletteDirty;                                         // 0x276
    u8 unk_277;                                              // 0x277
} WobbuffetPopBalloon;                                       // size: 0x278

// The balloon being inflated on the upper screen (BG 7, affine-scaled with its air).
typedef struct WobbuffetPopInflation {
    u32 active;       // 0x00
    u32 size;         // 0x04: balloon size (0 small .. 2 large)
    s32 air;          // 0x08
    s32 capacity;     // 0x0C: pops at this much air
    s32 pendingAir;   // 0x10: air sent but not shown yet
    s32 airStep;      // 0x14: pendingAir / 6 is added per frame
    s32 playerAir[4]; // 0x18: per seat
    u32 jitterToggle; // 0x28
    u8 showBg;        // 0x2C: show the touch screen's BG 3 at the next VBlank
    u8 hideBg;        // 0x2D
    u8 unk_2E[2];     // 0x2E
} WobbuffetPopInflation;

// Two little sprites a bonus target throws off when it lands.
typedef struct WobbuffetPopDust {
    ManagedSprite *sprites[2]; // 0x00
    s32 speedX[2];             // 0x08
    s32 speedY[2];             // 0x10
    u8 active;                 // 0x18
    u8 timer;                  // 0x19
    u8 unk_1A[2];              // 0x1A
} WobbuffetPopDust;

// A Wobbuffet hopping around the ring on the lower screen. A puff that reaches the ring while one stands at the
// front (180 degrees) gets its bonus (x2, x1/2 or x3).
typedef struct WobbuffetPopBonusTarget {
    ManagedSprite *sprite; // 0x00
    ManagedSprite *effect; // 0x04
    ManagedSprite *shadow; // 0x08
    fx32 angle;            // 0x0C
    u8 state;              // 0x10: index into the state table
    u8 step;               // 0x11
    u8 timer;              // 0x12
    u8 unk_13;             // 0x13
    u8 bonus;              // 0x14
    u8 atFront;            // 0x15
    u8 unk_16[2];          // 0x16
    s32 duration;          // 0x18
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
        } hop;             // hopping to the next spot
    } motion;              // 0x1C
    WobbuffetPopDust dust; // 0x30
} WobbuffetPopBonusTarget; // size: 0x4C

typedef struct WobbuffetPopBonusRing {
    WobbuffetPopBonusTarget targets[3]; // 0x00
    fx32 angle;                         // 0xE4
    fx32 angleStep;                     // 0xE8
    s32 ticks;                          // 0xEC
    u8 position;                        // 0xF0: steps of 30 degrees
    u8 done;                            // 0xF1
    u8 delay;                           // 0xF2
    u8 stepTimer;                       // 0xF3
    u8 speed;                           // 0xF4: row of the speed table
    u8 unk_F5[3];                       // 0xF5
} WobbuffetPopBonusRing;

// Bonus another player got, dropping in from the top of the lower screen.
typedef struct WobbuffetPopBonusPopup {
    ManagedSprite *sprite; // 0x00
    ManagedSprite *effect; // 0x04
    fx32 x;                // 0x08
    fx32 y;                // 0x0C
    s16 targetY;           // 0x10
    u8 bonus;              // 0x12
    u8 state;              // 0x13
    u8 timer;              // 0x14
    u8 active;             // 0x15
    s16 delay;             // 0x16
} WobbuffetPopBonusPopup;

// A text OBJ made by WobbuffetPop_NewTextObj.
typedef struct WobbuffetPopTextObj {
    TextOBJ *textObj;             // 0x00
    ObjCharSpaceReservation charAlloc; // 0x04
    u16 width;                    // 0x10
    u8 unk_12[2];                 // 0x12
} WobbuffetPopTextObj;

// The score on the upper screen: six rolling digits.
typedef struct WobbuffetPopScoreDisplay {
    ManagedSprite *frame; // 0x000
    struct {
        WobbuffetPopTextObj top;    // 0x00
        WobbuffetPopTextObj bottom; // 0x14
    } digits[6];                    // 0x004
    WobbuffetPopTextObj title;      // 0x0F4
    WobbuffetPopTextObj labels[6];  // 0x108
    u8 current[6];                  // 0x180: roll position of each digit (16 per digit value, 160 per turn)
    u8 goal[6];                     // 0x186
    u8 next[6];                     // 0x18C
    u8 passes[6];                   // 0x192
    s8 countdown[6];                // 0x198
    s8 delay;                       // 0x19E
    u8 settled;                     // 0x19F
    u8 state;                       // 0x1A0
    u8 unk_1A1[3];                  // 0x1A1
} WobbuffetPopScoreDisplay;

// The how-to-play demo: a stylus pressing the handle down twice, fed to the balloon as touches.
typedef struct WobbuffetPopDemo {
    ManagedSprite *stylus; // 0x00
    s16 state;             // 0x04
    vs16 timer;            // 0x06: volatile for the match
    u32 unk_08;            // 0x08
    s32 touch[4];          // 0x0C: x, y, held, new
} WobbuffetPopDemo;

// Air sent by one player, in the local queue until a bonus target decides its bonus.
typedef struct WobbuffetPopPump {
    u8 netId;         // 0x0
    u8 bonus;         // 0x1: 0 none, 1 x2, 2 x1/2, 3 x3
    u16 balloonIndex; // 0x2
    s32 amount;       // 0x4
    s32 bonusAmount;  // 0x8
    u8 bonusSet;      // 0xC
    u8 unk_D[3];      // 0xD
} WobbuffetPopPump;

// A puff of air flying from a player's side of the lower screen to the ring.
typedef struct WobbuffetPopPuff {
    u32 active;            // 0x00
    u16 netId;             // 0x04
    u16 balloonIndex;      // 0x06
    s32 amount;            // 0x08
    s16 delay;             // 0x0C
    u8 bonus;              // 0x0E
    u8 unk_0F;             // 0x0F
    ManagedSprite *sprite; // 0x10
    const u8 *config;      // 0x14: x, y, animation, palette, ..., direction (7)
} WobbuffetPopPuff;

// One piece of the confetti shown when a balloon pops.
typedef struct WobbuffetPopConfettiPiece {
    ManagedSprite *sprite; // 0x00
    fx32 radius;           // 0x04
    fx32 radiusSpeed;      // 0x08
    fx32 angle;            // 0x0C
    fx32 angleSpeed;       // 0x10
    s16 life;              // 0x14
    u16 unk_16;            // 0x16
} WobbuffetPopConfettiPiece;

typedef struct WobbuffetPopConfetti {
    u32 active;                                 // 0x000
    WobbuffetPopConfettiPiece playerPieces[36]; // 0x004: in the players' colours, by share of the air
    WobbuffetPopConfettiPiece pieces[8];        // 0x364
    WobbuffetPopConfettiPiece balloonPieces[3]; // 0x424: in the balloon's colour
    u32 unk_46C;                                // 0x46C
} WobbuffetPopConfetti;

// One of the next balloons, queued at the top of the lower screen.
typedef struct WobbuffetPopBalloonIcon {
    ManagedSprite *sprite; // 0x0
    u8 size;               // 0x4
    u8 state;              // 0x5: 1 waiting, 2 nearly full, 3 popping
    u8 position;           // 0x6: 0 is the current balloon
    u8 unk_7;              // 0x7
    s16 number;            // 0x8
    u16 unk_A;             // 0xA
} WobbuffetPopBalloonIcon;

// Overlay manager data of the game screen.
typedef struct WobbuffetPop {
    WobbuffetPopApp *app;                                              // 0x0000
    u32 hostNetId;                                                     // 0x0004
    u32 readyCount;                                                    // 0x0008
    u32 unk_000C[4];                                                   // 0x000C
    PlazaGameSign *sign;                                               // 0x001C
    u32 gameState;                                                     // 0x0020
    SpriteSystem *spriteSystem;                                        // 0x0024
    SpriteManager *spriteManager;                                      // 0x0028
    BgConfig *bgConfig;                                                // 0x002C
    Window playerNameWindows[4];                                       // 0x0030
    Window messageWindow;                                              // 0x0070
    MsgData *msgData;                                                  // 0x0080
    MessageFormat *msgFormat;                                          // 0x0084
    String *string;                                                    // 0x0088
    PaletteData *paletteData;                                          // 0x008C
    FontSystem *fontSystem;                                            // 0x0090
    SysTask *drawTask;                                                 // 0x0094
    GF3DVramMan *vramMan;                                              // 0x0098
    Camera *camera;                                                    // 0x009C
    Camera *particleCamera;                                            // 0x00A0
    SPLEmitter *particleSystem;                                        // 0x00A4
    NNSFndAllocator allocator;                                         // 0x00A8
    WobbuffetPopSequence sequence;                                     // 0x00B8
    WobbuffetPopBalloon *balloon;                                      // 0x00D4
    WobbuffetPopModels models;                                         // 0x00D8
    WobbuffetPopCommonModel common;                                    // 0x13A0
    WobbuffetPopInflation inflation;                                   // 0x1428
    ManagedSprite *seatSprites[4];                                     // 0x1458: one per seat
    WobbuffetPopBonusRing bonusRing;                                   // 0x1468
    WobbuffetPopBonusPopup bonusPopups[3];                             // 0x1560
    WobbuffetPopScoreDisplay scoreDisplay;                             // 0x15A8
    WobbuffetPopDemo demo;                                             // 0x174C
    u32 demoDone;                                                      // 0x1768
    WobbuffetPopEvent events[WOBBUFFET_POP_EVENT_QUEUE_SIZE];          // 0x176C
    WobbuffetPopEvent playerEvents[4][WOBBUFFET_POP_EVENT_QUEUE_SIZE]; // 0x1C1C
    WobbuffetPopEvent currentEvent;                                    // 0x2EDC
    WobbuffetPopEvent pendingEvent;                                    // 0x2EF0
    s32 playerEventWriteCounts[4];                                     // 0x2F04
    u32 playerEventReadCounts[4];                                      // 0x2F14
    s32 eventWriteCount;                                               // 0x2F24
    s32 eventReadCount;                                                // 0x2F28
    WobbuffetPopPump pumpQueue[8];                                     // 0x2F2C
    s32 pumpQueueRead;                                                 // 0x2FAC
    s32 pumpQueueWrite;                                                // 0x2FB0
    u32 timer;                                                         // 0x2FB4
    u32 started;                                                       // 0x2FB8
    u32 gameOver;                                                      // 0x2FBC: no more pumping
    u32 finished;                                                      // 0x2FC0
    s32 eventDelay;                                                    // 0x2FC4
    u32 balloonIndex;                                                  // 0x2FC8
    u32 balloonInPlay;                                                 // 0x2FCC
    u32 total;                                                         // 0x2FD0
    u32 syncKind;                                                      // 0x2FD4
    u32 syncId;                                                        // 0x2FD8
    WobbuffetPopPuff puffs[40];                                        // 0x2FDC
    WobbuffetPopConfetti confetti;                                     // 0x339C
    WobbuffetPopBalloonIcon balloonIcons[5];                           // 0x380C
    s32 score;                                                         // 0x3848
    u32 frameCounter;                                                  // 0x384C
} WobbuffetPop;                                                        // size: 0x3850

BOOL WobbuffetPopApp_Init(OverlayManager *man, int *state);
BOOL WobbuffetPopApp_Main(OverlayManager *man, int *state);
BOOL WobbuffetPopApp_Exit(OverlayManager *man, int *state);

void WobbuffetPopBalloon_ReleaseTouch(WobbuffetPopBalloon *balloon);
void WobbuffetPopBalloon_LoadShininessTable(void);
void WobbuffetPopBalloon_InitMeshVertices(WobbuffetPopBalloon *balloon);
void WobbuffetPopBalloon_InitMeshTexCoords(WobbuffetPopBalloon *balloon);
void WobbuffetPopBalloon_DrawMesh(WobbuffetPopBalloon *balloon);
void WobbuffetPopBalloon_ToVtx16(s32 x, s32 y, s16 *outX, s16 *outY);
int WobbuffetPopBalloon_TryGrabHandle(WobbuffetPopBalloon *balloon);
int WobbuffetPopBalloon_UpdateDrag(WobbuffetPopBalloon *balloon);
s32 WobbuffetPopBalloon_DeformMesh(WobbuffetPopBalloon *balloon, s32 mode, fx32 amount);
s32 WobbuffetPopBalloon_GetPushSide(WobbuffetPopBalloon *balloon, s32 mode);
s32 WobbuffetPopBalloon_CalcSidePush(WobbuffetPopBalloon *balloon, s32 amount, s32 pushed, s32 other, s32 *outPushed, s32 *outOther);
s32 WobbuffetPopBalloon_CalcMeshArea(WobbuffetPopBalloon *balloon);
s32 WobbuffetPopBalloon_CalcMeshAreaPercent(WobbuffetPopBalloon *balloon);
void WobbuffetPop_LoadHandleSpriteResources(WobbuffetPop *game);
void WobbuffetPop_FreeHandleSpriteResources(WobbuffetPop *game);
ManagedSprite *WobbuffetPop_CreateHandleSprite(WobbuffetPop *game);
void WobbuffetPopBalloon_UpdateHandleSprite(WobbuffetPopBalloon *balloon, ManagedSprite *sprite);
void WobbuffetPop_DeleteHandleSprite(ManagedSprite *sprite);

// wobbuffet_pop.c, used by the other parts of the game.
int WobbuffetPop_GetPlayerSeat(WobbuffetPop *game, int netId);
BOOL WobbuffetPop_AddModelSlot(WobbuffetPop *game, u32 amount);
void WobbuffetPop_QueuePlayerEvent(WobbuffetPop *game, int netId, const WobbuffetPopEvent *event);
void WobbuffetPop_QueueEvent(WobbuffetPop *game, const WobbuffetPopEvent *event);
void WobbuffetPop_RequestSync(WobbuffetPop *game, u32 kind, u32 syncId);

// wobbuffet_pop_scene.c, used by wobbuffet_pop.c.
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
