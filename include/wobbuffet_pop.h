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
typedef struct WobbuffetPopBalloon {
    void *textureFile;     // 0x000
    NNSG3dResTex *texture; // 0x004
    u8 unk_008[8];         // 0x008
    s32 unk_010;           // 0x010
    u8 unk_014[0xC];       // 0x014
    s32 unk_020;           // 0x020
    u8 unk_024[0x1E8];     // 0x024
    s32 touch[4];          // 0x20C: x, y, held, new
    u32 unk_21C;           // 0x21C
    u32 unk_220;           // 0x220
    u8 unk_224[0x10];      // 0x224
    ManagedSprite *sprite; // 0x234
    u32 unk_238;           // 0x238
    s32 unk_23C;           // 0x23C
    s32 unk_240;           // 0x240
    s32 pumpStrength;      // 0x244
    s32 unk_248;           // 0x248
    u16 *paletteSrc;       // 0x24C
    u16 *paletteDest;      // 0x250
    u32 paletteSize;       // 0x254
    u32 paletteAddr;       // 0x258
    s32 air;               // 0x25C: 0..800
    fx32 scaleX;           // 0x260
    fx32 scaleY;           // 0x264
    s16 squash;            // 0x268
    s16 unk_26A;           // 0x26A
    s16 unk_26C;           // 0x26C
    u8 unk_26E[2];         // 0x26E
    u32 selection;         // 0x270
    u8 unk_274;            // 0x274
    u8 flashCounter;       // 0x275
    u8 paletteDirty;       // 0x276
    u8 unk_277;            // 0x277
} WobbuffetPopBalloon;     // size: 0x278

typedef struct WobbuffetPopUnk1428 {
    u8 unk_00[8];    // 0x00
    int unk_08;      // 0x08
    int unk_0C;      // 0x0C
    u8 unk_10[0x1C]; // 0x10
    u8 showBg;       // 0x2C: show the touch screen's BG 3 at the next VBlank
    u8 hideBg;       // 0x2D
    u8 unk_2E[2];    // 0x2E
} WobbuffetPopUnk1428;

typedef struct WobbuffetPopUnk1468 {
    u8 unk_00[0xF0]; // 0x00
    u8 unk_F0;       // 0xF0
    u8 unk_F1;       // 0xF1
    u8 unk_F2[6];    // 0xF2
} WobbuffetPopUnk1468;

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
    WobbuffetPopUnk1428 unk_1428;                                      // 0x1428
    u8 unk_1458[0x10];                                                 // 0x1458
    WobbuffetPopUnk1468 unk_1468;                                      // 0x1468
    u8 unk_1560[0x48];                                                 // 0x1560
    void *unk_15A8;                                                    // 0x15A8
    u8 unk_15AC[6][0x28];                                              // 0x15AC
    u8 unk_169C[0xB0];                                                 // 0x169C
    void *unk_174C;                                                    // 0x174C
    u8 unk_1750[8];                                                    // 0x1750
    s32 demoTouch[4];                                                  // 0x1758
    u32 unk_1768;                                                      // 0x1768
    WobbuffetPopEvent events[WOBBUFFET_POP_EVENT_QUEUE_SIZE];          // 0x176C
    WobbuffetPopEvent playerEvents[4][WOBBUFFET_POP_EVENT_QUEUE_SIZE]; // 0x1C1C
    WobbuffetPopEvent currentEvent;                                    // 0x2EDC
    WobbuffetPopEvent pendingEvent;                                    // 0x2EF0
    s32 playerEventWriteCounts[4];                                     // 0x2F04
    u32 playerEventReadCounts[4];                                      // 0x2F14
    s32 eventWriteCount;                                               // 0x2F24
    s32 eventReadCount;                                                // 0x2F28
    u8 unk_2F2C[0x88];                                                 // 0x2F2C
    u32 timer;                                                         // 0x2FB4
    u32 started;                                                       // 0x2FB8
    u32 unk_2FBC;                                                      // 0x2FBC
    u32 finished;                                                      // 0x2FC0
    s32 eventDelay;                                                    // 0x2FC4
    u32 balloonIndex;                                                  // 0x2FC8
    u32 unk_2FCC;                                                      // 0x2FCC
    u32 total;                                                         // 0x2FD0
    u32 syncKind;                                                      // 0x2FD4
    u32 syncId;                                                        // 0x2FD8
    u8 unk_2FDC[0x3C0];                                                // 0x2FDC
    u32 unk_339C;                                                      // 0x339C
    u8 unk_33A0[0x4AC];                                                // 0x33A0
    u32 frameCounter;                                                  // 0x384C
} WobbuffetPop;                                                        // size: 0x3850

BOOL WobbuffetPopApp_Init(OverlayManager *man, int *state);
BOOL WobbuffetPopApp_Main(OverlayManager *man, int *state);
BOOL WobbuffetPopApp_Exit(OverlayManager *man, int *state);

#endif // POKEHEARTGOLD_WOBBUFFET_POP_H
