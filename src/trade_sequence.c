#include "trade_sequence.h"

#include "global.h"

#include "constants/pokemon.h"
#include "constants/sndseq.h"
#include "constants/species.h"

#include "msgdata/msg.naix"
#include "msgdata/msg/msg_0179.h"

#include "assert.h"
#include "bg_window.h"
#include "camera.h"
#include "char_tiles.h"
#include "filesystem.h"
#include "gf_3d_render.h"
#include "gf_3d_vramman.h"
#include "gf_gfx_loader.h"
#include "gf_gfx_planes.h"
#include "heap.h"
#include "math_util.h"
#include "message_format.h"
#include "model_box_test.h"
#include "msgdata.h"
#include "oam_manager.h"
#include "options.h"
#include "palette.h"
#include "pm_string.h"
#include "pokemon.h"
#include "pokepic.h"
#include "render_window.h"
#include "screen_fade_legacy.h"
#include "sound_playback.h"
#include "sound_system.h"
#include "sprite.h"
#include "sprite_resource_header.h"
#include "swap_3d_buffers.h"
#include "sys_task.h"
#include "sys_task_api.h"
#include "system.h"
#include "text.h"
#include "trade_anim.h"
#include "trade_sequence_arrive_internal.h"
#include "trade_sequence_internal.h"
#include "trade_sequence_models.h"
#include "wireless_status_icon.h"

// Trade animation: the app that plays the scenes of a trade (or of receiving a Pokemon) and the helpers they share
// (palette pulses, background zoom, Pokemon pictures, sprites), the scenes' 3D models, then scenes 0 to 5.

#define TRADE_SEQUENCE_NUM_SCENES       7
#define TRADE_SEQUENCE_MAX_DEFERRED     32
#define TRADE_SEQUENCE_NUM_PULSE_COLORS 0x60
#define TRADE_SEQUENCE_MAX_PULSES       8

struct TradeSequence {
    TRADE_ANIM_WORK *args;    // 0x000
    u32 scene;                // 0x004
    BgConfig *bgConfig;       // 0x008
    MessageFormat *msgFormat; // 0x00C
    MsgData *msgData;         // 0x010
    String *string;           // 0x014
    SpriteList *spriteList;   // 0x018
    G2dRenderer renderer;     // 0x01C
    SysTask *drawSpritesTask; // 0x144
    BOOL receivedMonCantFlip; // 0x148
    u16 sentSpecies;          // 0x14C
    u16 sentForm;             // 0x14E
    u16 receivedSpecies;      // 0x150
    u16 receivedForm;         // 0x152
    void *sceneWork;          // 0x154
}; // size: 0x158

// Light pulses running along the link: each pulse moves one white color through a BG extended palette.
struct TradeSequencePulses {
    u16 colors[TRADE_SEQUENCE_NUM_PULSE_COLORS];    // 0x000
    SysTask *task;                                  // 0x0C0
    SysTask *pulseTasks[TRADE_SEQUENCE_MAX_PULSES]; // 0x0C4
    int *running;                                   // 0x0E4, 1 while the pulses are alive
    int stop;                                       // 0x0E8
    int dirty;                                      // 0x0EC
    int timer;                                      // 0x0F0
    int count;                                      // 0x0F4
    int nextSlot;                                   // 0x0F8
    int direction;                                  // 0x0FC
}; // size: 0x100

typedef struct TradeSequencePulse {
    TradeSequencePulses *pulses; // 0x00
    s32 colorIndex;              // 0x04
    s32 stepFrames;              // 0x08
    s32 frameCounter;            // 0x0C
    s32 slot;                    // 0x10
    s32 direction;               // 0x14
} TradeSequencePulse;            // size: 0x18

// Accelerating affine zoom of BG 2 on both screens.
struct TradeSequenceBgZoom {
    BgConfig *bgConfig; // 0x00
    int *finished;      // 0x04
    fx32 scale;         // 0x08
    fx32 targetScale;   // 0x0C
    fx32 speed;         // 0x10
    fx32 acceleration;  // 0x14
    s32 direction;      // 0x18, 0: grow, 1: shrink
    s32 centerYIndex;   // 0x1C
    SysTask *task;      // 0x20
}; // size: 0x24

typedef void *(*TradeSequenceSceneInitFunc)(TradeSequence *ts);
typedef BOOL (*TradeSequenceSceneMainFunc)(void *sceneWork, int *state);
typedef void (*TradeSequenceSceneExitFunc)(void *sceneWork);

// The scene table: init, main and exit function and trade mode of each of the 7 scenes.
extern const TradeSequenceSceneInitFunc sTradeSequenceSceneInit[];
extern const TradeSequenceSceneMainFunc sTradeSequenceSceneMain[];
extern const TradeSequenceSceneExitFunc sTradeSequenceSceneExit[];
extern const u32 sTradeSequenceSceneModes[];
extern const s32 sTradeSequenceBgZoomCenterY[];
extern const TileRect sTradeSequenceMonPicSettings;

BOOL TradeSequence_BoxMonCantFlip(BoxPokemon *boxMon);
void TradeSequence_DrawSpritesTask(SysTask *task, void *data);
void TradeSequence_PulsesTask(SysTask *task, void *data);
void TradeSequence_SetPulseColor(TradeSequencePulses *pulses, int clearIndex, int setIndex, u16 color);
void TradeSequence_StartPulse(TradeSequencePulses *pulses);
void TradeSequence_DestroyPulse(TradeSequencePulses *pulses, int slot);
void TradeSequence_PulseTask(SysTask *task, void *data);
void TradeSequence_BgZoomTask(SysTask *task, void *data);
void TradeSequence_ApplyBgZoom(TradeSequenceBgZoom *zoom);
void TradeSequence_ResetDeferredFrees(void);
void TradeSequence_FlushDeferredFrees(void);

void TradeSequence_DrawModel(TradeSequenceModel *model);
void TradeSequence_InitModel(TradeSequenceModel *model);
void TradeSequence_FreeModel(TradeSequenceModel *model);
void TradeSequence_InitCamera(TradeSequenceModels *models, fx32 targetX, fx32 targetY, fx32 targetZ);
void TradeSequence_FreeCamera(TradeSequenceModels *models);

#define TRADE_SEQUENCE_SEND_NUM_STATES 4

typedef struct TradeSequenceSend {
    TradeSequence *ts;                // 0x00
    u32 subState;                     // 0x04
    int timer;                        // 0x08
    PokepicManager *pokepicManager;   // 0x0C
    Pokepic *pokepic;                 // 0x10
    PokepicAnimScript animScript[10]; // 0x14
    Sprite *sprites[2];               // 0x3C
    TradeSequenceCellAnim cellAnim;   // 0x44
    BgConfig *bgConfig;               // 0x54
    Window window;                    // 0x58
    String *msgTemplate;              // 0x68
    String *msgString;                // 0x6C
    TradeSequenceModels *models;      // 0x70
    TradeSequenceModel *model;        // 0x74
    SysTask *brightnessTask;          // 0x78
    SysTask *bounceTask;              // 0x7C
    NARC *pokeAnimNarc;               // 0x80
} TradeSequenceSend;                  // size: 0x84

typedef struct TradeSequenceSendBrightness {
    TradeSequenceSend *send;   // 0x00
    fx32 value;                // 0x04
    fx32 target;               // 0x08
    fx32 step;                 // 0x0C
    int frames;                // 0x10
} TradeSequenceSendBrightness; // size: 0x14

typedef struct TradeSequenceSendBounce {
    TradeSequenceSend *send;        // 0x00
    TradeSequenceModel *model;      // 0x04
    VecFx32 position;               // 0x08
    TradeSequenceRotation rotation; // 0x14
    u16 padding_1A;                 // 0x1A
    fx32 speed;                     // 0x1C
    int bounces;                    // 0x20
    BOOL landed;                    // 0x24
    int frames;                     // 0x28
    s16 spin;                       // 0x2C
} TradeSequenceSendBounce;          // size: 0x30

typedef BOOL (*TradeSequenceSendStateFunc)(TradeSequenceSend *send, u32 *state);

extern const TradeSequenceRotation sTradeSequenceSendModelRotation;
extern const TradeSequenceSendStateFunc sTradeSequenceSendStates[TRADE_SEQUENCE_SEND_NUM_STATES];
extern const GraphicsModes sTradeSequenceSendGraphicsModes;
extern const BgTemplate sTradeSequenceSendBgTemplate_Text;
extern const BgTemplate sTradeSequenceSendBgTemplate_Background;
extern const GraphicsBanks sTradeSequenceSendGraphicsBanks;

void DrawFrame2AroundWindow(Window *window, u32 baseTile, u32 paletteNum);
void WirelessStatusIcon_ShowOnBothScreens(int topFirst, enum HeapID heapID);

TradeSequenceSend *TradeSequenceSend_Init(TradeSequence *ts);
void TradeSequenceSend_Exit(TradeSequenceSend *send);
BOOL TradeSequenceSend_Main(TradeSequenceSend *send, u32 *state);
void TradeSequenceSend_Draw(TradeSequenceSend *send);
BOOL TradeSequenceSend_SetupScreens(TradeSequenceSend *send, u32 *state);
BOOL TradeSequenceSend_WaitFadeIn(TradeSequenceSend *send, u32 *state);
BOOL TradeSequenceSend_ShowMessages(TradeSequenceSend *send, u32 *state);
BOOL TradeSequenceSend_ReplaceMonWithModel(TradeSequenceSend *send, u32 *state);
void TradeSequenceSend_Init3D(TradeSequenceSend *send);
Pokepic *TradeSequenceSend_CreateMonPic(TradeSequenceSend *send);
void TradeSequenceSend_CreateSprites(TradeSequenceSend *send);
void TradeSequenceSend_DeleteSprites(TradeSequenceSend *send);
void TradeSequenceSend_StartBrightness(TradeSequenceSend *send, int start, int target, int frames);
BOOL TradeSequenceSend_IsBrightnessDone(TradeSequenceSend *send);
void TradeSequenceSend_StopBrightness(TradeSequenceSend *send);
void TradeSequenceSend_BrightnessTask(SysTask *task, void *data);
void TradeSequenceSend_StartBounce(TradeSequenceSend *send);
BOOL TradeSequenceSend_IsBounceDone(TradeSequenceSend *send);
int TradeSequenceSend_GetBounceCount(TradeSequenceSend *send);
void TradeSequenceSend_StopBounce(TradeSequenceSend *send);
void TradeSequenceSend_BounceTask(SysTask *task, void *data);

#define TRADE_SEQUENCE_DEPART_NUM_STATES  4
#define TRADE_SEQUENCE_DEPART_NUM_STREAKS 20

// Rotation speed around one axis, changing by step until it reaches target.
typedef struct TradeSequenceDepartSpin {
    s16 speed;             // 0x0
    s16 step;              // 0x2
    s16 target;            // 0x4
    u16 frames;            // 0x6
} TradeSequenceDepartSpin; // size: 0x8

// Moves the ball model: it circles a center (an ellipse of radii radiusX/radiusZ, turning by angleSpeed every frame)
// while its height and its rotation speeds change over time.
typedef struct TradeSequenceDepartFlight {
    TradeSequenceModel *model;        // 0x00
    TradeSequenceModels *models;      // 0x04
    VecFx32 center;                   // 0x08
    VecFx32 position;                 // 0x14
    TradeSequenceRotation rotation;   // 0x20
    u8 padding_26[2];                 // 0x26
    fx32 height;                      // 0x28
    fx32 speed;                       // 0x2C
    fx32 acceleration;                // 0x30
    u8 unk_34[4];                     // 0x34
    fx32 radiusX;                     // 0x38
    fx32 radiusZ;                     // 0x3C
    fx32 radiusXStep;                 // 0x40
    fx32 radiusZStep;                 // 0x44
    fx32 radiusXTarget;               // 0x48
    fx32 radiusZTarget;               // 0x4C
    int radiusFrames;                 // 0x50
    s16 angle;                        // 0x54
    s16 angleSpeed;                   // 0x56
    s16 angleSpeedTarget;             // 0x58
    s16 angleSpeedStep;               // 0x5A
    TradeSequenceDepartSpin spins[3]; // 0x5C, x, y, z
    int angleSpeedFrames;             // 0x74
    BOOL outOfView;                   // 0x78
    BOOL flyingAway;                  // 0x7C
    int frames;                       // 0x80
} TradeSequenceDepartFlight;          // size: 0x84

typedef struct TradeSequenceDepartStreak {
    Sprite *head;            // 0x0
    Sprite *tail;            // 0x4
} TradeSequenceDepartStreak; // size: 0x8

typedef struct TradeSequenceDepart {
    TradeSequence *ts;                                                    // 0x000
    u32 subState;                                                         // 0x004
    int timer;                                                            // 0x008
    TradeSequenceCellAnim cellAnim;                                       // 0x00C
    TradeSequenceDepartStreak streaks[TRADE_SEQUENCE_DEPART_NUM_STREAKS]; // 0x01C
    u32 savedRngSeed;                                                     // 0x0BC
    BgConfig *bgConfig;                                                   // 0x0C0
    TradeSequencePulses *pulses;                                          // 0x0C4
    int pulsesRunning;                                                    // 0x0C8
    TradeSequenceBgZoom *bgZoom;                                          // 0x0CC
    int bgZoomFinished;                                                   // 0x0D0
    TradeSequenceModels *models;                                          // 0x0D4
    TradeSequenceModel *ball;                                             // 0x0D8
    TradeSequenceDepartFlight flight;                                     // 0x0DC
    SysTask *flightTask;                                                  // 0x160
    SysTask *scrollTask;                                                  // 0x164
    BOOL swapScreens;                                                     // 0x168
} TradeSequenceDepart;                                                    // size: 0x16C

// Makes the light streaks fall down both screens, revealing one more every 12 frames.
typedef struct TradeSequenceDepartStreakFall {
    SysTask **task;                                       // 0x000
    TradeSequenceDepart *depart;                          // 0x004
    BOOL visible[TRADE_SEQUENCE_DEPART_NUM_STREAKS];      // 0x008
    VecFx32 positions[TRADE_SEQUENCE_DEPART_NUM_STREAKS]; // 0x058
    int delay;                                            // 0x148
    int revealed;                                         // 0x14C
} TradeSequenceDepartStreakFall;                          // size: 0x150

// Scrolls the backgrounds of both screens up faster and faster; starts the falling streaks on the way.
typedef struct TradeSequenceDepartScroll {
    TradeSequenceDepart *depart;        // 0x000
    TradeSequenceDepartStreakFall fall; // 0x004
    BgConfig *bgConfig;                 // 0x154
    fx32 mainY;                         // 0x158
    fx32 subY;                          // 0x15C
    fx32 speed;                         // 0x160
    fx32 distance;                      // 0x164
    SysTask **task;                     // 0x168
    SysTask *fallTask;                  // 0x16C
} TradeSequenceDepartScroll;            // size: 0x170

typedef struct TradeSequenceDepartAlphaFade {
    fx32 alpha;                 // 0x00
    fx32 step;                  // 0x04
    fx32 target;                // 0x08
    int frames;                 // 0x0C
    TradeSequenceModel *model;  // 0x10
} TradeSequenceDepartAlphaFade; // size: 0x14

typedef BOOL (*TradeSequenceDepartStateFunc)(TradeSequenceDepart *depart, u32 *state);

extern const u16 sTradeSequenceDepartPaletteOffsets[];
extern const GraphicsModes sTradeSequenceDepartGraphicsModes;
extern const TradeSequenceDepartStateFunc sTradeSequenceDepartStates[TRADE_SEQUENCE_DEPART_NUM_STATES];
extern const GXRgb sTradeSequenceDepartEdgeColors[8];
extern const BgTemplate sTradeSequenceDepartBgTemplate_Layer2;
extern const BgTemplate sTradeSequenceDepartBgTemplate_Layer3;
extern const GraphicsBanks sTradeSequenceDepartGraphicsBanks;
extern s32 ov71_0224C0C4;
extern CameraAngle sTradeSequenceDepartCameraAngle;

TradeSequenceDepart *TradeSequenceDepart_Init(TradeSequence *ts);
void TradeSequenceDepart_Exit(TradeSequenceDepart *depart);
BOOL TradeSequenceDepart_Main(TradeSequenceDepart *depart, u32 *state);
void TradeSequenceDepart_Draw(TradeSequenceDepart *depart);
BOOL TradeSequenceDepart_SetupScreens(TradeSequenceDepart *depart, u32 *state);
BOOL TradeSequenceDepart_WaitBgZoom(TradeSequenceDepart *depart, u32 *state);
BOOL TradeSequenceDepart_FlyOffTopScreen(TradeSequenceDepart *depart, u32 *state);
BOOL TradeSequenceDepart_FlyThroughBottomScreen(TradeSequenceDepart *depart, u32 *state);
void TradeSequenceDepart_InitBgs(TradeSequenceDepart *depart);
void TradeSequenceDepart_FreeBgs(TradeSequenceDepart *depart);
void TradeSequenceDepart_Init3D(TradeSequenceDepart *depart);
void TradeSequenceDepart_Free3D(TradeSequenceDepart *depart);
void TradeSequenceDepart_CreateStreaks(TradeSequenceDepart *depart);
void TradeSequenceDepart_DeleteStreaks(TradeSequenceDepart *depart);
void TradeSequenceDepart_RequestScreenSwap(TradeSequenceDepart *depart);
void TradeSequenceDepart_ScreenSwapTask(SysTask *task, void *data);
void TradeSequenceDepart_StartStreakFall(TradeSequenceDepart *depart, SysTask **task, TradeSequenceDepartStreakFall *fall);
void TradeSequenceDepart_StreakFallTask(SysTask *task, void *data);
void TradeSequenceDepart_StopStreakFall(SysTask *task);
void TradeSequenceDepart_StartScroll(TradeSequenceDepart *depart, SysTask **task);
void TradeSequenceDepart_ScrollTask(SysTask *task, void *data);
void TradeSequenceDepart_StopScroll(SysTask *task);
void TradeSequenceDepart_StartAlphaFade(TradeSequenceModel *model, int start, int target, int frames);
void TradeSequenceDepart_AlphaFadeTask(SysTask *task, void *data);
SysTask *TradeSequenceDepart_StartFlight(TradeSequenceDepart *depart);
void TradeSequenceDepart_SetAngleSpeed(TradeSequenceDepartFlight *flight, int target, int frames);
void TradeSequenceDepart_SetRadius(TradeSequenceDepartFlight *flight, fx32 radiusX, fx32 radiusZ, int frames);
void TradeSequenceDepart_SetSpin(TradeSequenceDepartFlight *flight, int axis, int target, int frames);
void TradeSequenceDepart_UpdateFlight(TradeSequenceDepartFlight *flight);
void TradeSequenceDepart_FlightTask_Appear(SysTask *task, void *data);
void TradeSequenceDepart_FlightTask_FlyAway(SysTask *task, void *data);
BOOL TradeSequenceDepart_IsFlightOutOfView(SysTask *task);
BOOL TradeSequenceDepart_IsFlightFlyingAway(SysTask *task);
void TradeSequenceDepart_RestartFlightFromBelow(SysTask *task);
void TradeSequenceDepart_FlightTask_Rise(SysTask *task, void *data);
void TradeSequenceDepart_FlightTask_Hover(SysTask *task, void *data);
void TradeSequenceDepart_FlightTask_Sink(SysTask *task, void *data);
void TradeSequenceDepart_FlightTask_Pause(SysTask *task, void *data);
void TradeSequenceDepart_FlightTask_Leave(SysTask *task, void *data);

#define TRADE_SEQUENCE_RECEDE_NUM_STATES 2

// Moves the ball model along an ellipse (radii radiusX/radiusY, turning by angleSpeed every frame) around a center
// that itself moves every frame; radii and scale can change over a number of frames.
typedef struct TradeSequenceRecedeFlight {
    struct TradeSequenceRecede *recede; // 0x00
    SysTask *task;                      // 0x04
    TradeSequenceModels *models;        // 0x08
    TradeSequenceModel *ball;           // 0x0C
    VecFx32 center;                     // 0x10
    VecFx32 offset;                     // 0x1C
    VecFx32 position;                   // 0x28
    TradeSequenceRotation rotation;     // 0x34
    u8 padding_3A[2];                   // 0x3A
    fx32 centerStepY;                   // 0x3C
    fx32 centerStepZ;                   // 0x40
    fx32 radiusX;                       // 0x44
    fx32 radiusY;                       // 0x48
    fx32 radiusXStep;                   // 0x4C
    fx32 radiusYStep;                   // 0x50
    fx32 scale;                         // 0x54
    fx32 scaleStep;                     // 0x58
    int stepFrames;                     // 0x5C
    int state;                          // 0x60
    int timer;                          // 0x64
    int elapsed;                        // 0x68
    s16 angle;                          // 0x6C
    s16 angleSpeed;                     // 0x6E
} TradeSequenceRecedeFlight;            // size: 0x70

typedef struct TradeSequenceRecede {
    TradeSequence *ts;                // 0x00
    u32 subState;                     // 0x04
    int timer;                        // 0x08
    BOOL swapScreens;                 // 0x0C
    TradeSequenceCellAnim cellAnim;   // 0x10
    Sprite *sprite;                   // 0x20
    BgConfig *bgConfig;               // 0x24
    TradeSequenceModels *models;      // 0x28
    TradeSequenceModel *ball;         // 0x2C
    u8 unk_30[4];                     // 0x30
    TradeSequenceRecedeFlight flight; // 0x34
} TradeSequenceRecede;                // size: 0xA4

typedef BOOL (*TradeSequenceRecedeStateFunc)(TradeSequenceRecede *recede, u32 *state);

extern const TradeSequenceRecedeStateFunc sTradeSequenceRecedeStates[TRADE_SEQUENCE_RECEDE_NUM_STATES];
extern const GraphicsModes sTradeSequenceRecedeGraphicsModes;
extern const GXRgb sTradeSequenceRecedeEdgeColors[8];
extern const BgTemplate sTradeSequenceRecedeBgTemplate;
extern const GraphicsBanks sTradeSequenceRecedeGraphicsBanks;

TradeSequenceRecede *TradeSequenceRecede_Init(TradeSequence *ts);
void TradeSequenceRecede_Exit(TradeSequenceRecede *recede);
BOOL TradeSequenceRecede_Main(TradeSequenceRecede *recede, u32 *state);
void TradeSequenceRecede_Draw(TradeSequenceRecede *recede);
BOOL TradeSequenceRecede_SetupScreens(TradeSequenceRecede *recede, u32 *state);
BOOL TradeSequenceRecede_Run(TradeSequenceRecede *recede, u32 *state);
void TradeSequenceRecede_InitBgs(TradeSequenceRecede *recede);
void TradeSequenceRecede_FreeBgs(TradeSequenceRecede *recede);
void TradeSequenceRecede_Init3D(TradeSequenceRecede *recede);
void TradeSequenceRecede_Free3D(TradeSequenceRecede *recede);
void TradeSequenceRecede_CreateSprite(TradeSequenceRecede *recede);
void TradeSequenceRecede_DeleteSprite(TradeSequenceRecede *recede);
void TradeSequenceRecede_InitFlight(TradeSequenceRecedeFlight *flight);
void TradeSequenceRecede_StopFlight(TradeSequenceRecedeFlight *flight);
void TradeSequenceRecede_StartFlight(TradeSequenceRecede *recede, TradeSequenceRecedeFlight *flight);
BOOL TradeSequenceRecede_IsFlightDone(TradeSequenceRecedeFlight *flight);
void TradeSequenceRecede_SetFlightTargets(TradeSequenceRecedeFlight *flight, fx32 radiusX, fx32 radiusY, fx32 scale, int frames);
void TradeSequenceRecede_UpdateFlight(TradeSequenceRecedeFlight *flight);
void TradeSequenceRecede_FlightTask(SysTask *task, void *data);
void TradeSequenceRecede_RequestScreenSwap(TradeSequenceRecede *recede);
void TradeSequenceRecede_ScreenSwapTask(SysTask *task, void *data);

#define TRADE_SEQUENCE_EXCHANGE_NUM_STATES 2

typedef struct TradeSequenceExchange {
    TradeSequence *ts;   // 0x00
    u32 subState;        // 0x04
    int frames;          // 0x08
    BgConfig *bgConfig;  // 0x0C
    fx32 sentY;          // 0x10
    fx32 receivedY;      // 0x14
    fx32 sentStep;       // 0x18
    fx32 receivedStep;   // 0x1C
    BOOL slideDone;      // 0x20
} TradeSequenceExchange; // size: 0x24

typedef BOOL (*TradeSequenceExchangeStateFunc)(TradeSequenceExchange *exchange, u32 *state);

extern const TradeSequenceExchangeStateFunc sTradeSequenceExchangeStates[TRADE_SEQUENCE_EXCHANGE_NUM_STATES];
extern const GraphicsModes sTradeSequenceExchangeGraphicsModes;
extern const BgTemplate sTradeSequenceExchangeBgTemplate_SentMon;
extern const BgTemplate sTradeSequenceExchangeBgTemplate_ReceivedMon;
extern const BgTemplate sTradeSequenceExchangeBgTemplate_Background;
extern const GraphicsBanks sTradeSequenceExchangeGraphicsBanks;

TradeSequenceExchange *TradeSequenceExchange_Init(TradeSequence *ts);
void TradeSequenceExchange_Exit(TradeSequenceExchange *exchange);
BOOL TradeSequenceExchange_Main(TradeSequenceExchange *exchange, u32 *state);
BOOL TradeSequenceExchange_SetupScreens(TradeSequenceExchange *exchange, u32 *state);
BOOL TradeSequenceExchange_Slide(TradeSequenceExchange *exchange, u32 *state);
void TradeSequenceExchange_SlideTask(SysTask *task, void *data);
void TradeSequenceExchange_InitBgs(TradeSequenceExchange *exchange);
void TradeSequenceExchange_FreeBgs(TradeSequenceExchange *exchange);
void TradeSequenceExchange_SetMonPicPositions(BgConfig *bgConfig, u32 sentY, u32 receivedY);

#define TRADE_SEQUENCE_APPROACH_NUM_STATES 2

// Moves the ball model along an ellipse (radii radiusX/radiusY, turning by angleSpeed every frame) around a center
// that itself moves every frame; radii and scale can change over a number of frames.
typedef struct TradeSequenceApproachFlight {
    struct TradeSequenceApproach *approach; // 0x00
    SysTask *task;                          // 0x04
    TradeSequenceModels *models;            // 0x08
    TradeSequenceModel *ball;               // 0x0C
    VecFx32 center;                         // 0x10
    VecFx32 offset;                         // 0x1C
    VecFx32 position;                       // 0x28
    TradeSequenceRotation rotation;         // 0x34
    u8 padding_3A[2];                       // 0x3A
    fx32 centerStepY;                       // 0x3C
    fx32 centerStepZ;                       // 0x40
    fx32 radiusX;                           // 0x44
    fx32 radiusY;                           // 0x48
    fx32 radiusXStep;                       // 0x4C
    fx32 radiusYStep;                       // 0x50
    fx32 radiusXTarget;                     // 0x54
    fx32 radiusYTarget;                     // 0x58
    fx32 scale;                             // 0x5C
    fx32 scaleStep;                         // 0x60
    fx32 scaleTarget;                       // 0x64
    int stepFrames;                         // 0x68
    int state;                              // 0x6C
    int timer;                              // 0x70
    int frames;                             // 0x74
    s16 angle;                              // 0x78
    s16 angleSpeed;                         // 0x7A
} TradeSequenceApproachFlight;              // size: 0x7C

typedef struct TradeSequenceApproach {
    TradeSequence *ts;                  // 0x00
    u32 subState;                       // 0x04
    u8 unk_08[4];                       // 0x08
    BOOL swapScreens;                   // 0x0C
    TradeSequenceCellAnim cellAnim;     // 0x10
    Sprite *sprite;                     // 0x20
    BgConfig *bgConfig;                 // 0x24
    TradeSequenceModels *models;        // 0x28
    TradeSequenceModel *ball;           // 0x2C
    u8 unk_30[4];                       // 0x30
    TradeSequenceApproachFlight flight; // 0x34
} TradeSequenceApproach;                // size: 0xB0

typedef BOOL (*TradeSequenceApproachStateFunc)(TradeSequenceApproach *approach, u32 *state);

extern const TradeSequenceApproachStateFunc sTradeSequenceApproachStates[TRADE_SEQUENCE_APPROACH_NUM_STATES];
extern const GraphicsModes sTradeSequenceApproachGraphicsModes;
extern const GXRgb sTradeSequenceApproachEdgeColors[8];
extern const BgTemplate sTradeSequenceApproachBgTemplate;
extern const GraphicsBanks sTradeSequenceApproachGraphicsBanks;

TradeSequenceApproach *TradeSequenceApproach_Init(TradeSequence *ts);
void TradeSequenceApproach_Exit(TradeSequenceApproach *approach);
BOOL TradeSequenceApproach_Main(TradeSequenceApproach *approach, u32 *state);
BOOL TradeSequenceApproach_SetupScreens(TradeSequenceApproach *approach, u32 *state);
BOOL TradeSequenceApproach_Run(TradeSequenceApproach *approach, u32 *state);
void TradeSequenceApproach_InitBgs(TradeSequenceApproach *approach);
void TradeSequenceApproach_FreeBgs(TradeSequenceApproach *approach);
void TradeSequenceApproach_Init3D(TradeSequenceApproach *approach);
void TradeSequenceApproach_Free3D(TradeSequenceApproach *approach);
void TradeSequenceApproach_CreateSprite(TradeSequenceApproach *approach);
void TradeSequenceApproach_DeleteSprite(TradeSequenceApproach *approach);
void TradeSequenceApproach_InitFlight(TradeSequenceApproachFlight *flight);
void TradeSequenceApproach_StopFlight(TradeSequenceApproachFlight *flight);
void TradeSequenceApproach_StartFlight(TradeSequenceApproach *approach, TradeSequenceApproachFlight *flight);
BOOL TradeSequenceApproach_IsFlightDone(TradeSequenceApproachFlight *flight);
void TradeSequenceApproach_SetFlightTargets(TradeSequenceApproachFlight *flight, fx32 radiusX, fx32 radiusY, fx32 scale, int frames);
void TradeSequenceApproach_UpdateFlight(TradeSequenceApproachFlight *flight);
void TradeSequenceApproach_FlightTask(SysTask *task, void *data);
void TradeSequenceApproach_RequestScreenSwap(TradeSequenceApproach *approach);
void TradeSequenceApproach_VBlankCallback(void *data);

#define TRADE_SEQUENCE_ARRIVE_NUM_STATES 3

typedef BOOL (*TradeSequenceArriveStateFunc)(TradeSequenceArrive *arrive, u32 *state);

extern const u16 sTradeSequenceArrivePaletteOffsets[];
extern const TradeSequenceArriveStateFunc sTradeSequenceArriveStates[TRADE_SEQUENCE_ARRIVE_NUM_STATES];
extern const GraphicsModes sTradeSequenceArriveGraphicsModes;
extern const BgTemplate sTradeSequenceArriveBgTemplate_Layer2;
extern const BgTemplate sTradeSequenceArriveBgTemplate_Layer3;
extern const GraphicsBanks sTradeSequenceArriveGraphicsBanks;

TradeSequenceArrive *TradeSequenceArrive_Init(TradeSequence *ts);
void TradeSequenceArrive_Exit(TradeSequenceArrive *arrive);
BOOL TradeSequenceArrive_Main(TradeSequenceArrive *arrive, u32 *state);
void TradeSequenceArrive_Draw(TradeSequenceArrive *arrive);
BOOL TradeSequenceArrive_SetupScreens(TradeSequenceArrive *arrive, u32 *state);
BOOL TradeSequenceArrive_LowerAndFade(TradeSequenceArrive *arrive, u32 *state);
BOOL TradeSequenceArrive_ZoomOut(TradeSequenceArrive *arrive, u32 *state);
void TradeSequenceArrive_InitBgs(TradeSequenceArrive *arrive);
void TradeSequenceArrive_FreeBgs(TradeSequenceArrive *arrive);

static s32 sTradeSequenceDeferredFreeCount;

static void *sTradeSequenceDeferredFrees[TRADE_SEQUENCE_MAX_DEFERRED];

// when 0 the ball only follows the center instead of spiralling around it
static u32 sTradeSequenceBallSpirals[] = { 1, 0, 0, 0, 0, 0, 0, 0 };

BOOL TradeSequence_Init(OverlayManager *man, int *state) {
    TradeSequence *ts;

    if (IsPaletteFadeFinished()) {
        Heap_Create(HEAP_ID_3, HEAP_ID_56, 3 << 15);
        Heap_Create(HEAP_ID_3, HEAP_ID_57, 3 << 15);
        TradeSequence_ResetDeferredFrees();
        Sound_SetSceneAndPlayBGM(3, SEQ_GS_KOUKAN, 1);
        ts = OverlayManager_CreateAndGetData(man, sizeof(TradeSequence), HEAP_ID_56);
        if (ts) {
            ts->args = OverlayManager_GetArgs(man);
            ts->scene = 0;
            ts->bgConfig = BgConfig_Alloc(HEAP_ID_56);
            ts->string = String_New(400, HEAP_ID_56);
            ts->msgData = NewMsgDataFromNarc(MSGDATA_LOAD_DIRECT, NARC_msgdata_msg, NARC_msg_msg_0179_bin, HEAP_ID_56);
            ts->msgFormat = MessageFormat_New(HEAP_ID_56);
            switch (ts->args->is_ingame) {
            case 1: // full trade
                BufferBoxMonNickname(ts->msgFormat, 0, ts->args->my_boxmon);
                BufferBoxMonNickname(ts->msgFormat, 1, ts->args->trade_boxmon);
                BufferPlayersName(ts->msgFormat, 2, ts->args->trade_profile);
                ts->receivedMonCantFlip = TradeSequence_BoxMonCantFlip(ts->args->trade_boxmon);
                break;
            case 2: // sending only (msg 0179: "will be put on offer")
                BufferBoxMonNickname(ts->msgFormat, 0, ts->args->my_boxmon);
                break;
            case 4: // receiving only (msg 0179: "was taken out")
                BufferBoxMonNickname(ts->msgFormat, 1, ts->args->trade_boxmon);
                ts->receivedMonCantFlip = TradeSequence_BoxMonCantFlip(ts->args->trade_boxmon);
                break;
            }
            NNS_G2dInitOamManagerModule();
            OamManager_Create(0, 128, 0, 32, 1, 127, 0, 32, HEAP_ID_56);
            ts->spriteList = G2dRenderer_Init(64, &ts->renderer, HEAP_ID_56);
            G2dRenderer_SetSubSurfaceCoords(&ts->renderer, 0, FX32_CONST(232));
            ts->sentSpecies = GetBoxMonData(ts->args->my_boxmon, MON_DATA_SPECIES, NULL);
            ts->receivedSpecies = GetBoxMonData(ts->args->trade_boxmon, MON_DATA_SPECIES, NULL);
            ts->sentForm = GetBoxMonData(ts->args->my_boxmon, MON_DATA_FORM, NULL);
            ts->receivedForm = GetBoxMonData(ts->args->trade_boxmon, MON_DATA_FORM, NULL);
            ts->sceneWork = NULL;
            Main_SetVBlankIntrCB(NULL, NULL);
            HBlankInterruptDisable();
            GfGfx_DisableEngineAPlanes();
            GfGfx_DisableEngineBPlanes();
            GX_SetVisiblePlane(0);
            GXS_SetVisiblePlane(0);
            ts->drawSpritesTask = SysTask_CreateOnPrintQueue(TradeSequence_DrawSpritesTask, ts, 1);
            ts->sceneWork = NULL;
        }
        return TRUE;
    } else {
        return FALSE;
    }
}

BOOL TradeSequence_BoxMonCantFlip(BoxPokemon *boxMon) {
    u32 species = GetBoxMonData(boxMon, MON_DATA_SPECIES, NULL);
    u32 form = GetBoxMonData(boxMon, MON_DATA_FORM, NULL);
    if (GetMonBaseStat_HandleAlternateForm(species, form, BASE_FLIP) == 0) {
        return TRUE;
    }
    return FALSE;
}

BOOL TradeSequence_Exit(OverlayManager *man, int *state) {
    OSIntrMode enabled = OS_DisableInterrupts();
    TradeSequence *ts = OverlayManager_GetData(man);
    SysTask_Destroy(ts->drawSpritesTask);
    WirelessStatusIcon_Destroy();
    MessageFormat_Delete(ts->msgFormat);
    DestroyMsgData(ts->msgData);
    String_Delete(ts->string);
    Heap_Free(ts->bgConfig);
    SpriteList_Delete(ts->spriteList);
    OamManager_Free();
    OverlayManager_FreeData(man);
    Heap_Destroy(HEAP_ID_56);
    Heap_Destroy(HEAP_ID_57);
    OS_RestoreInterrupts(enabled);
    return TRUE;
}

BOOL TradeSequence_Main(OverlayManager *man, int *state) {
    TradeSequence *ts = OverlayManager_GetData(man);

    if (ts->scene < TRADE_SEQUENCE_NUM_SCENES) {
        if (ts->args->is_ingame & sTradeSequenceSceneModes[ts->scene * 4]) {
            if (ts->sceneWork == NULL) {
                ts->sceneWork = sTradeSequenceSceneInit[ts->scene * 4](ts);
            }
            if (sTradeSequenceSceneMain[ts->scene * 4](ts->sceneWork, state)) {
                sTradeSequenceSceneExit[ts->scene * 4](ts->sceneWork);
                ts->sceneWork = NULL;
                ts->scene++;
                *state = 0;
            }
        } else {
            ts->scene = ts->scene + 1;
            *state = 0;
        }
        TradeSequence_FlushDeferredFrees();
        return FALSE;
    }
    return TRUE;
}

void TradeSequence_DrawSpritesTask(SysTask *task, void *data) {
    TradeSequence *ts = data;
    vu32 *dtcm = (vu32 *)0x027E0000; // OS_SetIrqCheckFlag(OS_IE_V_BLANK) through the DTCM base address

    SpriteList_RenderAndAnimateSprites(ts->spriteList);
    OamManager_ApplyAndResetBuffers();
    dtcm[0x3FF8 / sizeof(u32)] |= 1;
}

TradeSequencePulses *TradeSequence_CreatePulses(int *running, int direction) {
    NNSG2dPaletteData *plttData;
    void *pltt;
    int i;
    TradeSequencePulses *pulses = Heap_Alloc(HEAP_ID_56, sizeof(TradeSequencePulses));

    *running = 0;
    if (pulses != NULL) {
        pltt = GfGfxLoader_GetPlttData(NARC_a_0_8_9, 6, &plttData, HEAP_ID_56);
        if (pltt != NULL) {
            MI_CpuClear16(pulses, 0x60);
            Heap_Free(pltt);
        }
        pulses->dirty = 0;
        pulses->stop = 0;
        pulses->timer = 0;
        pulses->count = 0;
        pulses->running = running;
        pulses->nextSlot = 0;
        pulses->direction = direction;
        for (i = 0; i < TRADE_SEQUENCE_MAX_PULSES; i++) {
            pulses->pulseTasks[i] = NULL;
        }
        G2_SetBlendAlpha(GX_BLEND_PLANEMASK_BG2, GX_BLEND_PLANEMASK_BG2 | GX_BLEND_PLANEMASK_BG3, 16, 8);
        G2S_SetBlendAlpha(GX_BLEND_PLANEMASK_BG2, GX_BLEND_PLANEMASK_BG2 | GX_BLEND_PLANEMASK_BG3, 16, 8);
        pulses->task = SysTask_CreateOnVWaitQueue(TradeSequence_PulsesTask, pulses, 20);
        if (pulses->task != NULL) {
            *pulses->running = 1;
        }
    }
    return pulses;
}

void TradeSequence_StopPulses(TradeSequencePulses *pulses) {
    if (*pulses->running == 1) {
        pulses->stop = 1;
    }
}

void TradeSequence_DestroyPulses(TradeSequencePulses *pulses) {
    if (*pulses->running == 1) {
        int i;
        for (i = 0; i < TRADE_SEQUENCE_MAX_PULSES; i++) {
            if (pulses->pulseTasks[i]) {
                TradeSequence_DestroyPulse(pulses, i);
            }
        }
        *pulses->running = 0;
        SysTask_Destroy(pulses->task);
        Heap_Free(pulses);
    }
}

void TradeSequence_PulsesTask(SysTask *task, void *data) {
    TradeSequencePulses *pulses = data;

    if (pulses->stop == 0) {
        pulses->timer--;
        if (pulses->timer <= 0) {
            PlaySE(SEQ_SE_DP_KOUKAN08);
            pulses->timer = 30;
            TradeSequence_StartPulse(pulses);
        }
        if (pulses->dirty) {
            GX_BeginLoadBGExtPltt();
            GXS_BeginLoadBGExtPltt();
            DC_FlushRange(pulses, 96);
            GX_LoadBGExtPltt(pulses, 0x6000, 96);
            GXS_LoadBGExtPltt(pulses, 0x6000, 96);
            GX_EndLoadBGExtPltt();
            GXS_EndLoadBGExtPltt();
            pulses->dirty = 0;
        }
    } else {
        if (pulses->dirty) {
            GX_BeginLoadBGExtPltt();
            GXS_BeginLoadBGExtPltt();
            DC_FlushRange(pulses, 96);
            GX_LoadBGExtPltt(pulses, 0x6000, 96);
            GXS_LoadBGExtPltt(pulses, 0x6000, 96);
            GX_EndLoadBGExtPltt();
            GXS_EndLoadBGExtPltt();
            pulses->dirty = 0;
        }
        if (pulses->count == 0) {
            *pulses->running = 0;
            Heap_Free(pulses);
            SysTask_Destroy(task);
        }
    }
}

void TradeSequence_SetPulseColor(TradeSequencePulses *pulses, int clearIndex, int setIndex, u16 color) {
    if (clearIndex >= 0 && clearIndex < TRADE_SEQUENCE_NUM_PULSE_COLORS) {
        pulses->colors[clearIndex] = 0;
    }
    if (setIndex >= 0 && setIndex < TRADE_SEQUENCE_NUM_PULSE_COLORS) {
        pulses->colors[setIndex] = color;
    }
    pulses->dirty = 1;
}

void TradeSequence_StartPulse(TradeSequencePulses *pulses) {
    TradeSequencePulse *pulse;

    if (pulses->pulseTasks[pulses->nextSlot] != NULL) {
        return;
    }
    pulse = Heap_Alloc(HEAP_ID_56, sizeof(TradeSequencePulse));
    if (pulse) {
        pulse->pulses = pulses;
        pulse->colorIndex = pulses->direction == 1 ? -1 : 15;
        pulse->frameCounter = 0;
        pulse->stepFrames = 2;
        pulse->slot = pulses->nextSlot;
        pulse->direction = pulses->direction;
        pulses->pulseTasks[pulses->nextSlot] = SysTask_CreateOnMainQueue(TradeSequence_PulseTask, pulse, 10);
        if (pulses->pulseTasks[pulses->nextSlot]) {
            pulses->count++;
            pulses->nextSlot++;
            if (pulses->nextSlot >= TRADE_SEQUENCE_MAX_PULSES) {
                pulses->nextSlot = 0;
            }
        } else {
            Heap_Free(pulse);
        }
    }
}

void TradeSequence_DestroyPulse(TradeSequencePulses *pulses, int slot) {
    if (pulses->pulseTasks[slot] != NULL) {
        Heap_Free(SysTask_GetData(pulses->pulseTasks[slot]));
        SysTask_Destroy(pulses->pulseTasks[slot]);
        pulses->pulseTasks[slot] = NULL;
        pulses->count = pulses->count - 1;
    }
}

void TradeSequence_PulseTask(SysTask *task, void *data) {
    TradeSequencePulse *pulse = data;

    if (++pulse->frameCounter >= pulse->stepFrames) {
        pulse->frameCounter = 0;
        if (pulse->direction == 1) {
            if (pulse->colorIndex + 1 >= 15) {
                TradeSequence_SetPulseColor(pulse->pulses, pulse->colorIndex, pulse->colorIndex + 1, 0);
                TradeSequence_DestroyPulse(pulse->pulses, pulse->slot);
                return;
            }
            TradeSequence_SetPulseColor(pulse->pulses, pulse->colorIndex, pulse->colorIndex + 1, RGB_WHITE);
            pulse->colorIndex++;
            return;
        }
        if (pulse->colorIndex < 0) {
            TradeSequence_SetPulseColor(pulse->pulses, pulse->colorIndex, pulse->colorIndex + 1, 0);
            TradeSequence_DestroyPulse(pulse->pulses, pulse->slot);
            return;
        }
        TradeSequence_SetPulseColor(pulse->pulses, pulse->colorIndex + 1, pulse->colorIndex, RGB_WHITE);
        pulse->colorIndex--;
    }
}

TradeSequenceBgZoom *TradeSequence_StartBgZoom(BgConfig *bgConfig, fx32 scale, fx32 targetScale, fx32 speed, fx32 acceleration, int centerYIndex, int *finished) {
    TradeSequenceBgZoom *zoom = Heap_Alloc(HEAP_ID_56, sizeof(TradeSequenceBgZoom));

    *finished = 1;
    if (zoom != NULL) {
        zoom->bgConfig = bgConfig;
        zoom->scale = scale;
        zoom->targetScale = targetScale;
        zoom->speed = speed;
        zoom->acceleration = acceleration;
        zoom->direction = scale < targetScale ? 0 : 1;
        zoom->centerYIndex = centerYIndex;
        zoom->finished = finished;
        *finished = 0;
        zoom->task = SysTask_CreateOnVBlankQueue(TradeSequence_BgZoomTask, zoom, 0);
        if (zoom->task == NULL) {
            *finished = 1;
            Heap_Free(zoom);
        }
    }
    return zoom;
}

void TradeSequence_BgZoomTask(SysTask *task, void *data) {
    TradeSequenceBgZoom *zoom = data;
    s32 amount = ((zoom->scale * zoom->speed) >> FX32_SHIFT) / 32;
    s32 scale = zoom->scale;

    switch (zoom->direction) {
    case 0: {
        s32 target = zoom->targetScale;
        scale += amount;
        if (scale < target) {
            zoom->scale = scale;
        } else {
            zoom->scale = target;
            *zoom->finished = 1;
        }
    } break;
    case 1: {
        s32 target = zoom->targetScale;
        scale -= amount;
        if (scale > target) {
            zoom->scale = scale;
        } else {
            zoom->scale = target;
            *zoom->finished = 1;
        }
    } break;
    }
    TradeSequence_ApplyBgZoom(zoom);
    if (*zoom->finished != 0) {
        TradeSequence_DeferFree(zoom);
        SysTask_Destroy(task);
    } else {
        zoom->speed += zoom->acceleration;
    }
}

void TradeSequence_ApplyBgZoom(TradeSequenceBgZoom *zoom) {
    MtxFx22 matrix;
    matrix._00 = zoom->scale;
    matrix._01 = 0;
    matrix._10 = 0;
    matrix._11 = zoom->scale;
    SetBgAffine(zoom->bgConfig, GF_BG_LYR_MAIN_2, &matrix, 128, sTradeSequenceBgZoomCenterY[zoom->centerYIndex]);
    SetBgAffine(zoom->bgConfig, GF_BG_LYR_SUB_2, &matrix, 128, sTradeSequenceBgZoomCenterY[zoom->centerYIndex]);
}

void TradeSequence_LoadMonPicToBg(TradeSequence *ts, u32 whichMon, u32 bgId, u32 palette, BOOL large) {
    u32 size = large ? 6400 : 3200;
    void *buffer = Heap_AllocAtEnd(HEAP_ID_56, size);

    struct {
        TileRect settings;
        PokepicTemplate template;
    } local;

    u16 species;
    BOOL lock;
    BoxPokemon *boxMon;
    u32 personality;

    if (buffer) {
        local.settings = sTradeSequenceMonPicSettings;
        if (whichMon == 0) {
            boxMon = ts->args->my_boxmon;
        } else {
            boxMon = ts->args->trade_boxmon;
        }
        lock = AcquireBoxMonLock(boxMon);
        GetBoxmonSpriteCharAndPlttNarcIds(&local.template, boxMon, 2, FALSE);
        personality = GetBoxMonData(boxMon, MON_DATA_PERSONALITY, NULL);
        species = GetBoxMonData(boxMon, MON_DATA_SPECIES, NULL);
        if (large) {
            local.settings.w *= 2;
        }
        if (GetBoxMonData(boxMon, MON_DATA_IS_EGG, NULL) == TRUE && species == SPECIES_SPINDA) {
            species = SPECIES_EGG;
        }
        CharTiles_LoadMonRectAsObjFromNarc((NarcId)local.template.narcID, local.template.charDataID, HEAP_ID_56, &local.settings, buffer, personality, large, 2, species);
        DC_FlushRange(buffer, size);
        BG_LoadCharTilesData(ts->bgConfig, (u8)bgId, buffer, size, 0);
        ReleaseBoxMonLock(boxMon, lock);
        Heap_Free(buffer);
    }
    GfGfxLoader_GXLoadPal((NarcId)local.template.narcID, local.template.palDataID, bgId >= GF_BG_LYR_SUB_0 ? GF_PAL_LOCATION_SUB_BG : GF_PAL_LOCATION_MAIN_BG, (enum GFPalSlotOffset)(palette * 32), 32, HEAP_ID_56);
}

void TradeSequence_LoadMonPicTilemap(TradeSequence *ts, int whichMon, int bgId, int palette, int x, int y) {
    NNSG2dScreenData *screen;
    void *buffer;
    int member;

    if (whichMon == 0) {
        member = 0x10A;
    } else if (ts->receivedMonCantFlip) {
        member = 0x10B;
    } else {
        member = 0x10A;
    }
    buffer = GfGfxLoader_GetScrnData(NARC_a_0_0_7, member, FALSE, &screen, HEAP_ID_56);
    if (buffer != NULL) {
        CopyToBgTilemapRect(ts->bgConfig, bgId, x, y, 10, 10, screen->rawData, 0, 0, 32, 32);
        BgTilemapRectChangePalette(ts->bgConfig, bgId, x, y, 10, 10, palette);
        Heap_Free(buffer);
    }
}

void TradeSequence_LoadCellAnim(TradeSequenceCellAnim *cellAnim, NarcId narcId, s32 cellBankId, s32 animBankId) {
    cellAnim->cellBankFile = GfGfxLoader_GetCellBank(narcId, cellBankId, TRUE, &cellAnim->cellBank, HEAP_ID_57);
    cellAnim->animBankFile = GfGfxLoader_GetAnimBank(narcId, animBankId, TRUE, &cellAnim->animBank, HEAP_ID_57);
}

void TradeSequence_FreeCellAnim(TradeSequenceCellAnim *cellAnim) {
    if (cellAnim->cellBankFile != NULL) {
        Heap_Free(cellAnim->cellBankFile);
        cellAnim->cellBankFile = NULL;
    }
    if (cellAnim->animBankFile != NULL) {
        Heap_Free(cellAnim->animBankFile);
        cellAnim->animBankFile = NULL;
    }
}

void TradeSequence_InitSpriteHeader(SpriteResourcesHeader *header, TradeSequenceCellAnim *cellAnim, const NNSG2dImageProxy *imageProxy, const NNSG2dImagePaletteProxy *plttProxy, u32 priority) {
    header->imageProxy = imageProxy;
    header->plttProxy = plttProxy;
    header->cellData = cellAnim->cellBank;
    header->cellAnim = cellAnim->animBank;
    header->priority = priority;
    header->charData = NULL;
    header->multiCellData = NULL;
    header->multiCellAnim = NULL;
    header->flag = 0;
}

Sprite *TradeSequence_CreateSprite(TradeSequence *ts, const SpriteResourcesHeader *header, int x, int y, u32 priority, NNS_G2D_VRAM_TYPE whichScreen) {
    SimpleSpriteTemplate template;
    Sprite *sprite;

    template.spriteList = ts->spriteList;
    template.header = header;
    template.position.x = x << FX32_SHIFT;
    template.position.y = y << FX32_SHIFT;
    template.position.z = 0;
    template.priority = priority;
    template.whichScreen = whichScreen;
    template.heapID = HEAP_ID_56;
    sprite = Sprite_Create(&template);
    if (sprite != NULL) {
        Sprite_SetAnimActiveFlag(sprite, TRUE);
        Sprite_SetAnimSpeed(sprite, FX32_ONE);
    }
    return sprite;
}

BgConfig *TradeSequence_GetBgConfig(TradeSequence *ts) {
    return ts->bgConfig;
}

MessageFormat *TradeSequence_GetMsgFormat(TradeSequence *ts) {
    return ts->msgFormat;
}

MsgData *TradeSequence_GetMsgData(TradeSequence *ts) {
    return ts->msgData;
}

BoxPokemon *TradeSequence_GetSentBoxMon(TradeSequence *ts) {
    return ts->args->my_boxmon;
}

BoxPokemon *TradeSequence_GetReceivedBoxMon(TradeSequence *ts) {
    return ts->args->trade_boxmon;
}

int TradeSequence_GetTimeOfDay(TradeSequence *ts) {
    return ts->args->time_of_day;
}

u16 TradeSequence_GetSentSpecies(TradeSequence *ts) {
    return ts->sentSpecies;
}

u16 TradeSequence_GetSentForm(TradeSequence *ts) {
    return ts->sentForm;
}

u16 TradeSequence_GetReceivedSpecies(TradeSequence *ts) {
    return ts->receivedSpecies;
}

u16 TradeSequence_GetReceivedForm(TradeSequence *ts) {
    return ts->receivedForm;
}

u32 TradeSequence_GetFrameType(TradeSequence *ts) {
    return Options_GetFrame(ts->args->options);
}

int TradeSequence_GetTradeMode(TradeSequence *ts) {
    return ts->args->is_ingame;
}

void TradeSequence_ResetDeferredFrees(void) {
    sTradeSequenceDeferredFreeCount = 0;
}

void TradeSequence_FlushDeferredFrees(void) {
    s32 i;

    if (sTradeSequenceDeferredFreeCount != 0) {
        for (i = 0; i < sTradeSequenceDeferredFreeCount; i++) {
            Heap_Free(sTradeSequenceDeferredFrees[i]);
        }
        sTradeSequenceDeferredFreeCount = 0;
    }
}

void TradeSequence_DeferFree(void *ptr) {
    GF_ASSERT(sTradeSequenceDeferredFreeCount < TRADE_SEQUENCE_MAX_DEFERRED);
    sTradeSequenceDeferredFrees[sTradeSequenceDeferredFreeCount++] = ptr;
}

TradeSequenceModels *TradeSequence_CreateModels(u32 count, fx32 targetX, fx32 targetY, fx32 targetZ) {
    TradeSequenceModels *models = Heap_Alloc(HEAP_ID_57, sizeof(TradeSequenceModels));
    u32 i;

    if (models != NULL) {
        TradeSequence_InitCamera(models, targetX, targetY, targetZ);
        models->models = Heap_Alloc(HEAP_ID_57, sizeof(TradeSequenceModel) * count);
        models->count = count;
        for (i = 0; i < count; i++) {
            TradeSequence_InitModel(&models->models[i]);
        }
    }
    return models;
}

void TradeSequence_DestroyModels(TradeSequenceModels *models) {
    u32 i;

    for (i = 0; i < models->count; i++) {
        TradeSequence_FreeModel(&models->models[i]);
    }
    Heap_Free(models->models);
    TradeSequence_FreeCamera(models);
    Heap_Free(models);
}

void TradeSequence_DrawModels(TradeSequenceModels *models) {
    u32 i;
    u32 one;

    NNS_G3dGeBufferOP_N(G3OP_MTX_PUSH, NULL, 0);
    Camera_PushLookAtToNNSGlb();
    for (i = 0; i < models->count; i++) {
        if (models->models[i].visible != FALSE) {
            TradeSequence_DrawModel(&models->models[i]);
        }
    }
    one = 1;
    NNS_G3dGeBufferOP_N(G3OP_MTX_POP, &one, 1);
}

void TradeSequence_DrawModel(TradeSequenceModel *model) {
    MtxFx33 rotation;
    MtxFx33 temp;

    MTX_Identity33_(&rotation);
    MTX_RotX33_(&temp, FX_SinIdx((u16)model->rotation.x), FX_CosIdx((u16)model->rotation.x));
    MTX_Concat33(&temp, &rotation, &rotation);
    MTX_RotY33_(&temp, FX_SinIdx((u16)model->rotation.y), FX_CosIdx((u16)model->rotation.y));
    MTX_Concat33(&temp, &rotation, &rotation);
    MTX_RotZ33_(&temp, FX_SinIdx((u16)model->rotation.z), FX_CosIdx((u16)model->rotation.z));
    MTX_Concat33(&temp, &rotation, &rotation);
    if (model->alpha != 31) {
        NNS_G3dGlbPolygonAttr(0, GX_POLYGONMODE_MODULATE, GX_CULL_ALL, 0, model->alpha, 0);
    }
    GF3dRender_DrawModel(&model->renderObj, &model->translation, &rotation, &model->scale);
}

void TradeSequence_InitModel(TradeSequenceModel *model) {
    model->visible = FALSE;
    model->file = NULL;
    model->rotation.x = model->rotation.y = model->rotation.z = 0;
    model->scale.x = model->scale.y = model->scale.z = FX32_ONE;
    model->translation.x = model->translation.y = model->translation.z = 0;
}

void TradeSequence_FreeModel(TradeSequenceModel *model) {
    if (model->file != NULL) {
        Heap_Free(model->file);
        model->file = NULL;
        model->visible = FALSE;
    }
}

TradeSequenceModel *TradeSequence_LoadModel(TradeSequenceModels *models, int index, NarcId narcId, s32 fileId, fx32 x, fx32 y, fx32 z, BOOL visible) {
    TradeSequenceModel *base = models->models;
    TradeSequenceModel *model = &base[index];

    base[index].file = GfGfxLoader_LoadFromNarc(narcId, fileId, FALSE, HEAP_ID_57, TRUE);
    DC_FlushRange(base[index].file, GetNarcMemberSizeByIdPair(narcId, fileId));
    if (base[index].file) {
        model->mdlSet = NNS_G3dGetMdlSet(base[index].file);
        model->mdl = NNS_G3dGetMdlByIdx(model->mdlSet, 0);
        model->tex = NNS_G3dGetTex(model->file);
        GF3dRender_AllocAndLoadTexResources(model->tex);
        GF3dRender_BindModelSet(model->file, model->tex);
        NNS_G3dRenderObjInit(&model->renderObj, model->mdl);
        model->translation.x = x;
        model->translation.y = y;
        model->translation.z = z;
        model->alpha = 31;
        model->visible = visible;
    }
    return model;
}

void TradeSequence_GetModelPosition(TradeSequenceModel *model, VecFx32 *position) {
    *position = model->translation;
}

void TradeSequence_SetModelPosition(TradeSequenceModel *model, const VecFx32 *position) {
    model->translation = *position;
}

void TradeSequence_GetModelRotation(TradeSequenceModel *model, TradeSequenceRotation *rotation) {
    rotation->x = *(u16 *)&model->rotation.x;
    rotation->y = *(u16 *)&model->rotation.y;
    rotation->z = *(u16 *)&model->rotation.z;
}

void TradeSequence_SetModelRotation(TradeSequenceModel *model, const TradeSequenceRotation *rotation) {
    model->rotation.x = *(const u16 *)&rotation->x;
    model->rotation.y = *(const u16 *)&rotation->y;
    model->rotation.z = *(const u16 *)&rotation->z;
}

void TradeSequence_SetModelVisible(TradeSequenceModel *model, BOOL visible) {
    model->visible = visible;
}

void TradeSequence_SetModelAlpha(TradeSequenceModel *model, u32 alpha) {
    model->alpha = alpha;
    if (alpha != 31) {
        NNSi_G3dModifyPolygonAttrMask(model->mdl, FALSE, REG_G3_POLYGON_ATTR_ALPHA_MASK);
    } else {
        NNSi_G3dModifyPolygonAttrMask(model->mdl, TRUE, REG_G3_POLYGON_ATTR_ALPHA_MASK);
    }
}

void TradeSequence_SetModelScale(TradeSequenceModel *model, fx32 scale) {
    model->scale.x = scale;
    model->scale.y = scale;
    model->scale.z = scale;
}

BOOL TradeSequence_IsModelInView(TradeSequenceModel *model) {
    MtxFx33 matrix;
    MtxFx33 rot;
    int one;
    int result;

    MTX_Identity33(&matrix);
    MTX_RotX33(&rot, FX_SinIdx((u16)model->rotation.x), FX_CosIdx((u16)model->rotation.x));
    MTX_Concat33(&rot, &matrix, &matrix);
    MTX_RotY33(&rot, FX_SinIdx((u16)model->rotation.y), FX_CosIdx((u16)model->rotation.y));
    MTX_Concat33(&rot, &matrix, &matrix);
    MTX_RotZ33(&rot, FX_SinIdx((u16)model->rotation.z), FX_CosIdx((u16)model->rotation.z));
    MTX_Concat33(&rot, &matrix, &matrix);
    NNS_G3dGeBufferOP_N(G3OP_MTX_PUSH, NULL, 0);
    Camera_PushLookAtToNNSGlb();
    result = GF3d_ModelBoxTest(model->mdl, &model->translation, &matrix, &model->scale);
    one = 1;
    NNS_G3dGeBufferOP_N(G3OP_MTX_POP, (u32 *)&one, 1);
    return result;
}

void TradeSequence_GetCameraPos(TradeSequenceModels *models, VecFx32 *pos) {
    VecFx32 camPos;
    camPos = Camera_GetLookAtCamPos(models->camera);
    *pos = camPos;
}

void TradeSequence_SetCameraAngle(TradeSequenceModels *models, const CameraAngle *angle) {
    Camera_SetAnglePos(angle, models->camera);
}

void TradeSequence_AdjustCameraAngle(TradeSequenceModels *models, const CameraAngle *delta) {
    Camera_AdjustAngleTarget(delta, models->camera);
}

void TradeSequence_SetCameraPerspectiveType(TradeSequenceModels *models, u8 perspectiveType) {
    Camera_ApplyPerspectiveType(perspectiveType, models->camera);
}

void TradeSequence_SetCameraPerspectiveAngle(TradeSequenceModels *models, u16 perspectiveAngle) {
    Camera_SetPerspectiveAngle(perspectiveAngle, models->camera);
}

void TradeSequence_InitCamera(TradeSequenceModels *models, fx32 targetX, fx32 targetY, fx32 targetZ) {
    VecFx32 up;

    models->camera = Camera_New(HEAP_ID_57);
    models->cameraTarget.x = targetX;
    models->cameraTarget.y = targetY;
    models->cameraTarget.z = targetZ;
    models->cameraAngle.x = 0;
    models->cameraAngle.y = 0;
    models->cameraAngle.z = 0;
    Camera_Init_FromTargetDistanceAndAngle(&models->cameraTarget, FX32_CONST(300), &models->cameraAngle, 4004, 0, TRUE, models->camera);
    up.x = 0;
    up.y = FX32_ONE;
    up.z = 0;
    Camera_SetLookAtCamUp(&up, models->camera);
    Camera_SetStaticPtr(models->camera);
    Camera_SetPerspectiveClippingPlane(0, FX32_CONST(1000), models->camera);
}

void TradeSequence_FreeCamera(TradeSequenceModels *models) {
    Camera_UnsetStaticPtr();
    Camera_Delete(models->camera);
}

// Scene 0 of the trade animation: the sent Pokemon's picture and the "will be sent to" / "Bye-bye" messages, then
// a flash replaces the picture by a 3D model that bounces until the screen fades out.

TradeSequenceSend *TradeSequenceSend_Init(TradeSequence *ts) {
    TradeSequenceSend *send = Heap_Alloc(HEAP_ID_57, sizeof(TradeSequenceSend));
    if (send) {
        send->ts = ts;
        send->subState = 0;
        send->bgConfig = TradeSequence_GetBgConfig(ts);
        send->pokepicManager = PokepicManager_Create(HEAP_ID_57);
        send->pokepic = NULL;
        send->msgTemplate = String_New(300, HEAP_ID_57);
        send->msgString = String_New(300, HEAP_ID_57);
        send->brightnessTask = NULL;
        send->bounceTask = NULL;
        send->pokeAnimNarc = NARC_New(NARC_a_1_8_0, HEAP_ID_57);
    }
    return send;
}

void TradeSequenceSend_Exit(TradeSequenceSend *send) {
    if (send) {
        TradeSequenceSend_StopBrightness(send);
        TradeSequenceSend_StopBounce(send);
        TradeSequenceSend_DeleteSprites(send);
        String_Delete(send->msgTemplate);
        String_Delete(send->msgString);
        TradeSequence_DestroyModels(send->models);
        FreeBgTilemapBuffer(send->bgConfig, GF_BG_LYR_MAIN_1);
        FreeBgTilemapBuffer(send->bgConfig, GF_BG_LYR_MAIN_2);
        FreeBgTilemapBuffer(send->bgConfig, GF_BG_LYR_SUB_2);
        RemoveWindow(&send->window);
        NARC_Delete(send->pokeAnimNarc);
        if (send->pokepic) {
            Pokepic_Delete(send->pokepic);
        }
        PokepicManager_Delete(send->pokepicManager);
        Heap_Free(send);
    }
}

BOOL TradeSequenceSend_Main(TradeSequenceSend *send, u32 *state) {
    if (*state < TRADE_SEQUENCE_SEND_NUM_STATES) {
        if (sTradeSequenceSendStates[*state](send, &send->subState)) {
            *state = *state + 1;
            send->subState = 0;
        }
        TradeSequenceSend_Draw(send);
        return FALSE;
    }
    return TRUE;
}

void TradeSequenceSend_Draw(TradeSequenceSend *send) {
    u32 one;

    G3X_Reset();
    NNS_G3dGeBufferOP_N(G3OP_MTX_PUSH, NULL, 0);
    NNS_G3dGeFlushBuffer();
    NNS_G2dSetupSoftwareSpriteCamera();
    PokepicManager_HandleLoadImgAndOrPltt(send->pokepicManager);
    PokepicManager_DrawAll(send->pokepicManager);
    one = 1;
    NNS_G3dGeBufferOP_N(G3OP_MTX_POP, &one, 1);
    TradeSequence_DrawModels(send->models);
    RequestSwap3DBuffers(GX_SORTMODE_AUTO, GX_BUFFERMODE_Z);
}

BOOL TradeSequenceSend_SetupScreens(TradeSequenceSend *send, u32 *state) {
    TradeSequenceRotation rotation;

    GfGfx_SetBanks(&sTradeSequenceSendGraphicsBanks);
    reg_GX_POWCNT &= ~REG_GX_POWCNT_DSEL_MASK;
    SetBothScreensModesAndDisable(&sTradeSequenceSendGraphicsModes);
    InitBgFromTemplate(send->bgConfig, GF_BG_LYR_MAIN_1, &sTradeSequenceSendBgTemplate_Text, GF_BG_TYPE_TEXT);
    InitBgFromTemplate(send->bgConfig, GF_BG_LYR_MAIN_2, &sTradeSequenceSendBgTemplate_Background, GF_BG_TYPE_TEXT);
    InitBgFromTemplate(send->bgConfig, GF_BG_LYR_SUB_2, &sTradeSequenceSendBgTemplate_Background, GF_BG_TYPE_TEXT);
    GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG0, GF_PLANE_TOGGLE_ON);
    G2_SetBG0Priority(1);
    BG_FillCharDataRange(send->bgConfig, GF_BG_LYR_MAIN_1, 0, 1, 0);
    FillBgTilemapRect(send->bgConfig, GF_BG_LYR_MAIN_1, 0, 0, 0, 32, 32, 0);
    LoadUserFrameGfx2(send->bgConfig, GF_BG_LYR_MAIN_1, 109, 2, (u8)TradeSequence_GetFrameType(send->ts), HEAP_ID_57);
    AddWindowParameterized(send->bgConfig, &send->window, GF_BG_LYR_MAIN_1, 2, 19, 27, 4, 1, 1);
    FillWindowPixelBuffer(&send->window, 15);
    GfGfxLoader_GXLoadPal(NARC_graphic_font, 8, GF_PAL_LOCATION_MAIN_BG, GF_PAL_SLOT_1_OFFSET, 32, HEAP_ID_57);
    GfGfxLoader_LoadCharData(NARC_a_0_8_9, 22, send->bgConfig, GF_BG_LYR_MAIN_2, 0, 0, TRUE, HEAP_ID_57);
    GfGfxLoader_LoadScrnData(NARC_a_0_8_9, 21, send->bgConfig, GF_BG_LYR_MAIN_2, 0, 0, TRUE, HEAP_ID_57);
    GfGfxLoader_GXLoadPal(NARC_a_0_8_9, 23, GF_PAL_LOCATION_MAIN_BG, GF_PAL_SLOT_0_OFFSET, 32, HEAP_ID_57);
    GfGfxLoader_LoadCharData(NARC_a_0_8_9, 22, send->bgConfig, GF_BG_LYR_SUB_2, 0, 0, TRUE, HEAP_ID_57);
    GfGfxLoader_LoadScrnData(NARC_a_0_8_9, 21, send->bgConfig, GF_BG_LYR_SUB_2, 0, 0, TRUE, HEAP_ID_57);
    GfGfxLoader_GXLoadPal(NARC_a_0_8_9, 23, GF_PAL_LOCATION_SUB_BG, GF_PAL_SLOT_0_OFFSET, 32, HEAP_ID_57);
    BgCommitTilemapBufferToVram(send->bgConfig, GF_BG_LYR_MAIN_1);
    TradeSequenceSend_Init3D(send);
    send->models = TradeSequence_CreateModels(1, 0, 0, 0);
    send->pokepic = TradeSequenceSend_CreateMonPic(send);
    send->model = TradeSequence_LoadModel(send->models, 0, NARC_a_0_8_9, 27, 0, FX32_CONST(26.5), FX32_CONST(115.5), FALSE);
    rotation = sTradeSequenceSendModelRotation;
    TradeSequence_SetModelRotation(send->model, &rotation);
    GfGfx_EngineATogglePlanes(GX_PLANEMASK_OBJ, GF_PLANE_TOGGLE_ON);
    GfGfx_EngineBTogglePlanes(GX_PLANEMASK_OBJ, GF_PLANE_TOGGLE_ON);
    TradeSequenceSend_CreateSprites(send);
    G2_BlendNone();
    WirelessStatusIcon_Create();
    WirelessStatusIcon_ShowOnBothScreens(1, HEAP_ID_56);
    BeginNormalPaletteFade(0, 1, 1, RGB_BLACK, 16, 1, HEAP_ID_57);
    return TRUE;
}

BOOL TradeSequenceSend_WaitFadeIn(TradeSequenceSend *send, u32 *state) {
    if (*state == 0 && IsPaletteFadeFinished()) {
        Sprite_SetAnimCtrlSeq(send->sprites[0], 1);
        Sprite_SetDrawFlag(send->sprites[0], TRUE);
        return TRUE;
    }
    return FALSE;
}

BOOL TradeSequenceSend_ShowMessages(TradeSequenceSend *send, u32 *state) {
    switch (*state) {
    case 0:
        send->timer = 0;
        (*state)++;
        break;
    case 1:
        if (++send->timer > 10) {
            MsgData *msgData = TradeSequence_GetMsgData(send->ts);
            MessageFormat *msgFormat = TradeSequence_GetMsgFormat(send->ts);
            int msgId, next;
            if (TradeSequence_GetTradeMode(send->ts) == 1) {
                msgId = msg_0179_00000;
                next = 2;
            } else {
                msgId = msg_0179_00004;
                next = 3;
            }
            ReadMsgDataIntoString(msgData, msgId, send->msgTemplate);
            StringExpandPlaceholders(msgFormat, send->msgString, send->msgTemplate);
            AddTextPrinterParameterized(&send->window, 1, send->msgString, 0, 0, TEXT_SPEED_NOTRANSFER, NULL);
            DrawFrame2AroundWindow(&send->window, 109, 2);
            CopyWindowToVram(&send->window);
            send->timer = 0;
            *state = next;
        }
        break;
    case 2:
        if (++send->timer > 60) {
            MsgData *msgData = TradeSequence_GetMsgData(send->ts);
            MessageFormat *msgFormat = TradeSequence_GetMsgFormat(send->ts);
            ReadMsgDataIntoString(msgData, msg_0179_00001, send->msgTemplate);
            StringExpandPlaceholders(msgFormat, send->msgString, send->msgTemplate);
            if (GetBoxMonData(TradeSequence_GetSentBoxMon(send->ts), MON_DATA_IS_EGG, NULL) == FALSE) {
                u8 cryDelay;
                ((void (*)(Pokepic *, int))Pokepic_StartAnim)(send->pokepic, 1); // called with an extra argument
                NARC_ReadPokepicCryDelay(send->pokeAnimNarc, &cryDelay, TradeSequence_GetSentSpecies(send->ts), 1);
                u16 species = TradeSequence_GetSentSpecies(send->ts);
                PlayCryDelayed(species, cryDelay, (u8)TradeSequence_GetSentForm(send->ts));
            }
            FillWindowPixelBuffer(&send->window, 15);
            AddTextPrinterParameterized(&send->window, 1, send->msgString, 0, 0, TEXT_SPEED_NOTRANSFER, NULL);
            CopyWindowPixelsToVram_TextMode(&send->window);
            send->timer = 0;
            (*state)++;
        }
        break;
    case 3:
        if (++send->timer > 60) {
            ClearFrameAndWindow2(&send->window, FALSE);
            send->timer = 0;
            (*state)++;
        }
        break;
    case 4:
        if (++send->timer > 20) {
            return TRUE;
        }
        break;
    }
    return FALSE;
}

BOOL TradeSequenceSend_ReplaceMonWithModel(TradeSequenceSend *send, u32 *state) {
    switch (*state) {
    case 0:
        PlaySE(SEQ_SE_DP_KOUKAN01);
        TradeSequenceSend_StartBrightness(send, 0, 16, 12);
        (*state)++;
        break;
    case 1:
        if (TradeSequenceSend_IsBrightnessDone(send)) {
            Pokepic_Delete(send->pokepic);
            Sprite_SetAnimCtrlSeq(send->sprites[1], 0);
            Sprite_SetDrawFlag(send->sprites[1], TRUE);
            (*state)++;
        }
        break;
    case 2:
        if (!Sprite_IsAnimated(send->sprites[1])) {
            TradeSequence_SetModelVisible(send->model, TRUE);
            TradeSequenceSend_StartBrightness(send, 16, 0, 16);
            TradeSequenceSend_StartBounce(send);
            (*state)++;
        }
        break;
    case 3:
        if (TradeSequenceSend_GetBounceCount(send) >= 2 || TradeSequenceSend_IsBounceDone(send) == TRUE) {
            BeginNormalPaletteFade(0, 0, 0, RGB_BLACK, 24, 1, HEAP_ID_57);
            (*state)++;
        }
        break;
    case 4:
        if (IsPaletteFadeFinished() && TradeSequenceSend_IsBounceDone(send)) {
            return TRUE;
        }
        break;
    default:
        break;
    }
    return FALSE;
}

void TradeSequenceSend_Init3D(TradeSequenceSend *send) {
    u32 texKey, plttKey;

    NNS_G3dInit();
    G3X_InitMtxStack();
    G3X_SetShading(GX_SHADING_TOON);
    G3X_AntiAlias(TRUE);
    G3X_AlphaTest(FALSE, 0);
    G3X_AlphaBlend(TRUE);
    G3X_SetClearColor(RGB_BLACK, 0, 0x7FFF, 63, FALSE);
    G3_SwapBuffers(GX_SORTMODE_AUTO, GX_BUFFERMODE_W);
    G3_ViewPort(0, 0, 255, 191);
    GF_3DVramMan_InitFrameTexVramManager(1, TRUE);
    GF_3DVramMan_InitFramePlttVramManager(0x4000, TRUE);
    texKey = NNS_GfdDefaultFuncAllocTexVram(0x4000, FALSE, 0);
    plttKey = NNS_GfdDefaultFuncAllocPlttVram(0x80, FALSE, 1);
    PokepicManager_SetCharBaseAddrAndSize(send->pokepicManager, (texKey & 0xFFFF) << 3, ((texKey & 0x7FFF0000) >> 16) << 4);
    PokepicManager_SetPlttBaseAddrAndSize(send->pokepicManager, (plttKey & 0xFFFF) << 3, ((plttKey & 0xFFFF0000) >> 16) << 3);
}

Pokepic *TradeSequenceSend_CreateMonPic(TradeSequenceSend *send) {
    PokepicTemplate template;
    BoxPokemon *boxMon = TradeSequence_GetSentBoxMon(send->ts);

    GetBoxmonSpriteCharAndPlttNarcIds(&template, boxMon, 2, FALSE);
    NARC_ReadPokepicAnimScript(send->pokeAnimNarc, send->animScript, TradeSequence_GetSentSpecies(send->ts), 1);
    {
        int y = GetBoxMonPicHeightEx(boxMon, 2, FALSE) + 80;
        return PokepicManager_CreatePokepic(send->pokepicManager, &template, 128, y, 0, 0, send->animScript, NULL);
    }
}

void TradeSequenceSend_CreateSprites(TradeSequenceSend *send) {
    NNSG2dImagePaletteProxy palette;
    NNSG2dImageProxy image;
    SpriteResourcesHeader header;

    TradeSequence_LoadCellAnim(&send->cellAnim, NARC_a_0_8_9, 7, 8);
    NNS_G2dInitImagePaletteProxy(&palette);
    NNS_G2dInitImageProxy(&image);
    GfGfxLoader_LoadImageMapping(NARC_a_0_8_9, 9, TRUE, GF_BG_LYR_MAIN_0, 0, NNS_G2D_VRAM_TYPE_2DMAIN, 0, HEAP_ID_57, &image);
    GfGfxLoader_PartiallyLoadPalette(NARC_a_0_8_9, 10, NNS_G2D_VRAM_TYPE_2DMAIN, 0, HEAP_ID_57, &palette);
    TradeSequence_InitSpriteHeader(&header, &send->cellAnim, &image, &palette, 2);
    send->sprites[0] = TradeSequence_CreateSprite(send->ts, &header, 128, 100, 0, NNS_G2D_VRAM_TYPE_2DMAIN);
    send->sprites[1] = TradeSequence_CreateSprite(send->ts, &header, 128, 90, 0, NNS_G2D_VRAM_TYPE_2DMAIN);
    Sprite_SetPriority(send->sprites[1], 1);
    Sprite_SetDrawFlag(send->sprites[0], FALSE);
    Sprite_SetDrawFlag(send->sprites[1], FALSE);
}

void TradeSequenceSend_DeleteSprites(TradeSequenceSend *send) {
    s32 i;
    TradeSequenceSend *p;

    i = 0;
    p = send;
    do {
        if (p->sprites[0] != NULL) {
            Sprite_Delete(p->sprites[0]);
        }
        i++;
        p = (TradeSequenceSend *)((u8 *)p + sizeof(Sprite *)); // walks sprites[] through a shifted base pointer
    } while (i < 2);
    TradeSequence_FreeCellAnim(&send->cellAnim);
}

void TradeSequenceSend_StartBrightness(TradeSequenceSend *send, int start, int target, int frames) {
    TradeSequenceSendBrightness *brightness = Heap_Alloc(HEAP_ID_57, sizeof(*brightness));
    if (brightness) {
        brightness->send = send;
        brightness->value = start << FX32_SHIFT;
        brightness->target = target << FX32_SHIFT;
        brightness->step = (brightness->target - brightness->value) / frames;
        brightness->frames = frames;
        send->brightnessTask = SysTask_CreateOnVBlankQueue(TradeSequenceSend_BrightnessTask, brightness, 0);
        G2_SetBlendBrightness(GX_BLEND_PLANEMASK_BG0, start);
    }
}

BOOL TradeSequenceSend_IsBrightnessDone(TradeSequenceSend *send) {
    if (send->brightnessTask == NULL) {
        return TRUE;
    }
    return FALSE;
}

void TradeSequenceSend_StopBrightness(TradeSequenceSend *send) {
    if (send->brightnessTask != NULL) {
        TradeSequence_DeferFree(SysTask_GetData(send->brightnessTask));
        SysTask_Destroy(send->brightnessTask);
        send->brightnessTask = NULL;
    }
}

void TradeSequenceSend_BrightnessTask(SysTask *task, void *data) {
    TradeSequenceSendBrightness *brightness = data;

    if (--brightness->frames > 0) {
        brightness->value += brightness->step;
        G2_SetBlendBrightness(GX_BLEND_PLANEMASK_BG0, brightness->value >> FX32_SHIFT);
    } else {
        G2_SetBlendBrightness(GX_BLEND_PLANEMASK_BG0, brightness->target >> FX32_SHIFT);
        TradeSequenceSend_StopBrightness(brightness->send);
    }
}

void TradeSequenceSend_StartBounce(TradeSequenceSend *send) {
    TradeSequenceSendBounce *bounce = Heap_Alloc(HEAP_ID_57, sizeof(*bounce));
    if (bounce) {
        bounce->send = send;
        bounce->model = send->model;
        TradeSequence_GetModelPosition(bounce->model, &bounce->position);
        TradeSequence_GetModelRotation(bounce->model, &bounce->rotation);
        bounce->speed = 0x2F60;
        bounce->bounces = 0;
        bounce->spin = 0;
        bounce->landed = FALSE;
        bounce->frames = 0;
        send->bounceTask = SysTask_CreateOnMainQueue(TradeSequenceSend_BounceTask, bounce, 0);
    }
}

BOOL TradeSequenceSend_IsBounceDone(TradeSequenceSend *send) {
    if (send->bounceTask == NULL) {
        return TRUE;
    }
    return FALSE;
}

int TradeSequenceSend_GetBounceCount(TradeSequenceSend *send) {
    if (send->bounceTask != NULL) {
        return ((TradeSequenceSendBounce *)SysTask_GetData(send->bounceTask))->bounces;
    }
    return 0;
}

void TradeSequenceSend_StopBounce(TradeSequenceSend *send) {
    if (send->bounceTask != NULL) {
        Heap_Free(SysTask_GetData(send->bounceTask));
        SysTask_Destroy(send->bounceTask);
        send->bounceTask = NULL;
    }
}

void TradeSequenceSend_BounceTask(SysTask *task, void *data) {
    TradeSequenceSendBounce *bounce = data;

    if (!bounce->landed) {
        bounce->position.y += bounce->speed;
        if (bounce->speed >= 0) {
            bounce->speed -= 1920;
        } else {
            bounce->speed -= 1920;
            if (bounce->position.y <= -FX32_CONST(4)) {
                bounce->position.y = -FX32_CONST(4);
                bounce->speed = -(bounce->speed * 44 / 100);
                if (bounce->speed < 4000) {
                    bounce->landed = TRUE;
                }
                PlaySE(SEQ_SE_DP_KON);
                bounce->bounces++;
                switch (bounce->bounces) {
                case 1:
                    bounce->spin = 176;
                    break;
                case 3:
                    bounce->spin += 80;
                    break;
                }
            }
        }
    }
    bounce->rotation.x += bounce->spin;
    bounce->rotation.z -= bounce->spin;
    TradeSequence_SetModelRotation(bounce->model, &bounce->rotation);
    bounce->position.x += bounce->spin * 5;
    bounce->position.z += bounce->spin * 5;
    TradeSequence_SetModelPosition(bounce->model, &bounce->position);
    if (bounce->landed) {
        bounce->spin -= 14;
        bounce->frames++;
        if (bounce->frames > 30) {
            TradeSequenceSend_StopBounce(bounce->send);
        }
    }
}

// Scene 1 of the trade animation: the Poke Ball model fades in over a zooming background and circles up off the top
// screen; the screens swap and it rises through the bottom screen while the background scrolls and light streaks
// fall, then the screen fades to white.

TradeSequenceDepart *TradeSequenceDepart_Init(TradeSequence *ts) {
    TradeSequenceDepart *depart = Heap_Alloc(HEAP_ID_57, sizeof(TradeSequenceDepart));
    if (depart != NULL) {
        depart->ts = ts;
        depart->subState = 0;
        depart->bgConfig = TradeSequence_GetBgConfig(ts);
        depart->models = NULL;
        depart->pulses = NULL;
        depart->bgZoom = NULL;
        depart->flightTask = NULL;
        depart->scrollTask = NULL;
        depart->savedRngSeed = MTRandom();
        depart->bgZoomFinished = FALSE;
        depart->swapScreens = FALSE;
        SysTask_CreateOnVWaitQueue(TradeSequenceDepart_ScreenSwapTask, depart, 0);
    }
    return depart;
}

void TradeSequenceDepart_Exit(TradeSequenceDepart *depart) {
    OSIntrMode enabled = OS_DisableInterrupts();
    if (depart != NULL) {
        TradeSequenceDepart_FreeBgs(depart);
        TradeSequenceDepart_Free3D(depart);
        TradeSequenceDepart_DeleteStreaks(depart);
        if (depart->flightTask != NULL) {
            SysTask_Destroy(depart->flightTask);
        }
        if (depart->scrollTask != NULL) {
            TradeSequenceDepart_StopScroll(depart->scrollTask);
        }
        SetMTRNGSeed(depart->savedRngSeed);
        Heap_Free(depart);
    }
    OS_RestoreInterrupts(enabled);
}

BOOL TradeSequenceDepart_Main(TradeSequenceDepart *depart, u32 *state) {
    if (*state < TRADE_SEQUENCE_DEPART_NUM_STATES) {
        if (sTradeSequenceDepartStates[*state](depart, &depart->subState)) {
            *state = *state + 1;
            depart->subState = 0;
        }
        TradeSequenceDepart_Draw(depart);
        return FALSE;
    }
    return TRUE;
}

void TradeSequenceDepart_Draw(TradeSequenceDepart *depart) {
    if (depart->models != NULL) {
        G3X_Reset();
        TradeSequence_DrawModels(depart->models);
        RequestSwap3DBuffers(GX_SORTMODE_AUTO, GX_BUFFERMODE_Z);
    }
}

BOOL TradeSequenceDepart_SetupScreens(TradeSequenceDepart *depart, u32 *state) {
    TradeSequenceDepart_InitBgs(depart);
    TradeSequenceDepart_Init3D(depart);
    TradeSequenceDepart_CreateStreaks(depart);
    BeginNormalPaletteFade(0, 1, 1, RGB_BLACK, 8, 1, HEAP_ID_57);
    return TRUE;
}

BOOL TradeSequenceDepart_WaitBgZoom(TradeSequenceDepart *depart, u32 *state) {
    switch (*state) {
    case 0:
        if (IsPaletteFadeFinished()) {
            ov71_0224C0C4 = 0;
            (*state)++;
        }
        break;
    case 1:
        if (ov71_0224C0C4 < 30) {
            ov71_0224C0C4++;
        }
        if (depart->bgZoomFinished) {
            return TRUE;
        }
        break;
    }
    return FALSE;
}

BOOL TradeSequenceDepart_FlyOffTopScreen(TradeSequenceDepart *depart, u32 *state) {
    switch (*state) {
    case 0:
        depart->timer = 0;
        (*state)++;
        break;
    case 1:
        if (++depart->timer > 8) {
            TradeSequenceDepart_StartAlphaFade(depart->ball, 0, 31, 24);
            depart->flightTask = TradeSequenceDepart_StartFlight(depart);
            TradeSequence_SetModelAlpha(depart->ball, 0);
            TradeSequence_SetModelVisible(depart->ball, TRUE);
            (*state)++;
        }
        break;
    case 2:
        if (TradeSequenceDepart_IsFlightFlyingAway(depart->flightTask)) {
            (*state)++;
        }
        break;
    case 3:
        if (TradeSequenceDepart_IsFlightOutOfView(depart->flightTask)) {
            (*state)++;
        }
        break;
    case 4:
        TradeSequenceDepart_RequestScreenSwap(depart);
        return TRUE;
    }
    return FALSE;
}

BOOL TradeSequenceDepart_FlyThroughBottomScreen(TradeSequenceDepart *depart, u32 *state) {
    switch (*state) {
    case 0:
        TradeSequenceDepart_RestartFlightFromBelow(depart->flightTask);
        depart->timer = 0;
        (*state)++;
        break;
    case 1:
        if (++depart->timer > 2) {
            depart->timer = 0;
            TradeSequence_StopPulses(depart->pulses);
            (*state)++;
        }
        break;
    case 2:
        if (!depart->pulsesRunning) {
            TradeSequenceDepart_StartScroll(depart, &depart->scrollTask);
            (*state)++;
        }
        break;
    case 3:
        if (depart->timer < 40) {
            depart->timer++;
            if (depart->timer == 40) {
                PlaySE(SEQ_SE_DP_KOUKAN10);
            }
        }
        if (TradeSequenceDepart_IsFlightOutOfView(depart->flightTask)) {
            BeginNormalPaletteFade(0, 0, 0, RGB_WHITE, 4, 1, HEAP_ID_57);
            (*state)++;
        }
        break;
    case 4:
        if (IsPaletteFadeFinished()) {
            return TRUE;
        }
        break;
    }
    return FALSE;
}

void TradeSequenceDepart_InitBgs(TradeSequenceDepart *depart) {
    OSIntrMode intrMode;
    u32 srcOffset;
    void *buf;

    GfGfx_SetBanks(&sTradeSequenceDepartGraphicsBanks);
    reg_GX_POWCNT &= ~REG_GX_POWCNT_DSEL_MASK;
    SetBothScreensModesAndDisable(&sTradeSequenceDepartGraphicsModes);
    InitBgFromTemplate(depart->bgConfig, GF_BG_LYR_MAIN_2, &sTradeSequenceDepartBgTemplate_Layer2, GF_BG_TYPE_AFFINE);
    InitBgFromTemplate(depart->bgConfig, GF_BG_LYR_SUB_2, &sTradeSequenceDepartBgTemplate_Layer2, GF_BG_TYPE_AFFINE);
    intrMode = OS_DisableInterrupts();
    InitBgFromTemplate(depart->bgConfig, GF_BG_LYR_MAIN_3, &sTradeSequenceDepartBgTemplate_Layer3, GF_BG_TYPE_256x16PLTT);
    InitBgFromTemplate(depart->bgConfig, GF_BG_LYR_SUB_3, &sTradeSequenceDepartBgTemplate_Layer3, GF_BG_TYPE_256x16PLTT);
    OS_RestoreInterrupts(intrMode);
    GfGfxLoader_LoadCharData(NARC_a_0_8_9, 2, depart->bgConfig, GF_BG_LYR_MAIN_2, 0, 0, TRUE, HEAP_ID_57);
    GfGfxLoader_LoadCharData(NARC_a_0_8_9, 2, depart->bgConfig, GF_BG_LYR_SUB_2, 0, 0, TRUE, HEAP_ID_57);
    GfGfxLoader_LoadScrnData(NARC_a_0_8_9, 0, depart->bgConfig, GF_BG_LYR_MAIN_2, 0, 0, TRUE, HEAP_ID_57);
    GfGfxLoader_LoadScrnData(NARC_a_0_8_9, 0, depart->bgConfig, GF_BG_LYR_SUB_2, 0, 0, TRUE, HEAP_ID_57);
    srcOffset = TradeSequence_GetTimeOfDay(depart->ts);
    GfGfxLoader_GXLoadPal(NARC_a_0_8_9, 3, GF_PAL_LOCATION_MAIN_BG, GF_PAL_SLOT_0_OFFSET, 0x20, HEAP_ID_57);
    GfGfxLoader_GXLoadPal(NARC_a_0_8_9, 3, GF_PAL_LOCATION_SUB_BG, GF_PAL_SLOT_0_OFFSET, 0x20, HEAP_ID_57);
    srcOffset = sTradeSequenceDepartPaletteOffsets[srcOffset];
    GfGfxLoader_GXLoadPalWithSrcOffset(NARC_a_0_8_9, 3, GF_PAL_LOCATION_MAIN_BG, srcOffset, GF_PAL_SLOT_1_OFFSET, 0x40, HEAP_ID_57);
    GfGfxLoader_GXLoadPalWithSrcOffset(NARC_a_0_8_9, 3, GF_PAL_LOCATION_SUB_BG, srcOffset, GF_PAL_SLOT_1_OFFSET, 0x40, HEAP_ID_57);
    GfGfxLoader_LoadCharData(NARC_a_0_8_9, 5, depart->bgConfig, GF_BG_LYR_MAIN_3, 0, 0, TRUE, HEAP_ID_57);
    GfGfxLoader_LoadCharData(NARC_a_0_8_9, 5, depart->bgConfig, GF_BG_LYR_SUB_3, 0, 0, TRUE, HEAP_ID_57);
    GfGfxLoader_LoadScrnData(NARC_a_0_8_9, 4, depart->bgConfig, GF_BG_LYR_MAIN_3, 0, 0, TRUE, HEAP_ID_57);
    GfGfxLoader_LoadScrnData(NARC_a_0_8_9, 4, depart->bgConfig, GF_BG_LYR_SUB_3, 0, 0, TRUE, HEAP_ID_57);
    buf = Heap_Alloc(HEAP_ID_57, 0x60);
    if (buf != NULL) {
        MI_CpuClear32(buf, 0x60);
        DC_FlushRange(buf, 0x60);
        GX_BeginLoadBGExtPltt();
        GXS_BeginLoadBGExtPltt();
        GX_LoadBGExtPltt(buf, 0x6000, 0x60);
        GXS_LoadBGExtPltt(buf, 0x6000, 0x60);
        GX_EndLoadBGExtPltt();
        GXS_EndLoadBGExtPltt();
        Heap_Free(buf);
    }
    BgSetPosTextAndCommit(depart->bgConfig, GF_BG_LYR_SUB_2, BG_POS_OP_SET_Y, -256);
    ToggleBgLayer(GF_BG_LYR_SUB_3, GF_PLANE_TOGGLE_OFF);
    GfGfx_EngineATogglePlanes(GX_PLANEMASK_OBJ, GF_PLANE_TOGGLE_ON);
    GfGfx_EngineBTogglePlanes(GX_PLANEMASK_OBJ, GF_PLANE_TOGGLE_ON);
    BgSetPosTextAndCommit(depart->bgConfig, GF_BG_LYR_MAIN_3, BG_POS_OP_SET_Y, 0);
    depart->pulses = TradeSequence_CreatePulses(&depart->pulsesRunning, 1);
    depart->bgZoom = TradeSequence_StartBgZoom(depart->bgConfig, 0x480, FX32_ONE, 1, 0x128, 0, &depart->bgZoomFinished);
}

void TradeSequenceDepart_FreeBgs(TradeSequenceDepart *depart) {
    if (depart->pulsesRunning) {
        TradeSequence_DestroyPulses(depart->pulses);
    }
    FreeBgTilemapBuffer(depart->bgConfig, GF_BG_LYR_MAIN_2);
    FreeBgTilemapBuffer(depart->bgConfig, GF_BG_LYR_SUB_2);
    FreeBgTilemapBuffer(depart->bgConfig, GF_BG_LYR_MAIN_3);
    FreeBgTilemapBuffer(depart->bgConfig, GF_BG_LYR_SUB_3);
}

void TradeSequenceDepart_Init3D(TradeSequenceDepart *depart) {
    NNS_G3dInit();
    G3X_InitMtxStack();
    G3X_SetShading(GX_SHADING_TOON);
    G3X_AntiAlias(TRUE);
    G3X_AlphaTest(FALSE, 0);
    G3X_AlphaBlend(TRUE);
    G3X_EdgeMarking(TRUE);
    G3X_SetEdgeColorTable(sTradeSequenceDepartEdgeColors);
    G3X_SetClearColor(RGB(31, 31, 16), 0, 0x7FFF, 63, FALSE);
    G3_SwapBuffers(GX_SORTMODE_AUTO, GX_BUFFERMODE_W);
    G3_ViewPort(0, 0, 255, 191);
    GF_3DVramMan_InitFrameTexVramManager(1, TRUE);
    GF_3DVramMan_InitFramePlttVramManager(0x4000, TRUE);
    GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG0, GF_PLANE_TOGGLE_ON);
    G2_SetBG0Priority(0);
    depart->models = TradeSequence_CreateModels(1, 0, 0, 0);
    depart->ball = TradeSequence_LoadModel(depart->models, 0, NARC_a_0_8_9, 27, 0, -0x37800, 0, FALSE);
    ((CameraAngle *)(&ov71_0224C0C4 + 1))->x = 0xEBE0;
    ((CameraAngle *)(&ov71_0224C0C4 + 1))->y = 0;
    ((CameraAngle *)(&ov71_0224C0C4 + 1))->z = 0;
    TradeSequence_SetCameraAngle(depart->models, &sTradeSequenceDepartCameraAngle);
    TradeSequence_SetModelScale(depart->ball, 0x1C00);
}

void TradeSequenceDepart_Free3D(TradeSequenceDepart *depart) {
    if (depart->models != NULL) {
        TradeSequence_DestroyModels(depart->models);
    }
}

void TradeSequenceDepart_CreateStreaks(TradeSequenceDepart *depart) {
    NNSG2dImagePaletteProxy palette;
    NNSG2dImageProxy image;
    SpriteResourcesHeader header;
    int i, x, y;
    TradeSequenceDepart *p;

    TradeSequence_LoadCellAnim(&depart->cellAnim, NARC_a_0_8_9, 11, 12);
    NNS_G2dInitImagePaletteProxy(&palette);
    NNS_G2dInitImageProxy(&image);
    GfGfxLoader_LoadImageMapping(NARC_a_0_8_9, 13, TRUE, GF_BG_LYR_MAIN_0, 0, NNS_G2D_VRAM_TYPE_2DMAIN, 0, HEAP_ID_57, &image);
    GfGfxLoader_LoadImageMapping(NARC_a_0_8_9, 13, TRUE, GF_BG_LYR_MAIN_0, 0, NNS_G2D_VRAM_TYPE_2DSUB, 0, HEAP_ID_57, &image);
    GfGfxLoader_PartiallyLoadPalette(NARC_a_0_8_9, 14, NNS_G2D_VRAM_TYPE_2DMAIN, 0, HEAP_ID_57, &palette);
    GfGfxLoader_PartiallyLoadPalette(NARC_a_0_8_9, 14, NNS_G2D_VRAM_TYPE_2DSUB, 0, HEAP_ID_57, &palette);
    TradeSequence_InitSpriteHeader(&header, &depart->cellAnim, &image, &palette, 1);
    SetMTRNGSeed(0x35947D1);
    i = 0;
    p = depart;
    do {
        x = MTRandom() % 232 + 12;
        y = (int)(MTRandom() % 452) - 28;
        p->streaks[0].head = TradeSequence_CreateSprite(depart->ts, &header, x, y, 0, NNS_G2D_VRAM_TYPE_2DMAIN);
        y += 56;
        p->streaks[0].tail = TradeSequence_CreateSprite(depart->ts, &header, x, y, 0, NNS_G2D_VRAM_TYPE_2DMAIN);
        Sprite_SetAnimCtrlSeq(p->streaks[0].head, 0);
        Sprite_SetAnimCtrlSeq(p->streaks[0].tail, 1);
        Sprite_SetDrawFlag(p->streaks[0].head, FALSE);
        Sprite_SetDrawFlag(p->streaks[0].tail, FALSE);
        p = (TradeSequenceDepart *)((u8 *)p + sizeof(TradeSequenceDepartStreak)); // walks streaks[] through a shifted base pointer
    } while (++i < TRADE_SEQUENCE_DEPART_NUM_STREAKS);
}

void TradeSequenceDepart_DeleteStreaks(TradeSequenceDepart *depart) {
    s32 i;
    TradeSequenceDepart *p;

    i = 0;
    p = depart;
    do {
        if (p->streaks[0].head != NULL) {
            Sprite_Delete(p->streaks[0].head);
        }
        if (p->streaks[0].tail != NULL) {
            Sprite_Delete(p->streaks[0].tail);
        }
        i++;
        p = (TradeSequenceDepart *)((u8 *)p + sizeof(TradeSequenceDepartStreak)); // walks streaks[] through a shifted base pointer
    } while (i < TRADE_SEQUENCE_DEPART_NUM_STREAKS);
    TradeSequence_FreeCellAnim(&depart->cellAnim);
}

void TradeSequenceDepart_RequestScreenSwap(TradeSequenceDepart *depart) {
    depart->swapScreens = TRUE;
}

// Once requested (on the next V-wait): moves the layer 2 backgrounds, shows layer 3 on the sub engine instead of the main
// one and swaps the screens, so that the ball, which left the top screen, comes into view on the bottom one.
void TradeSequenceDepart_ScreenSwapTask(SysTask *task, void *data) {
    TradeSequenceDepart *depart = data;

    if (depart->swapScreens) {
        depart->swapScreens = FALSE;
        BgSetPosTextAndCommit(depart->bgConfig, GF_BG_LYR_MAIN_2, BG_POS_OP_SET_Y, -256);
        BgSetPosTextAndCommit(depart->bgConfig, GF_BG_LYR_SUB_2, BG_POS_OP_SET_Y, 0);
        ToggleBgLayer(GF_BG_LYR_MAIN_3, GF_PLANE_TOGGLE_OFF);
        ToggleBgLayer(GF_BG_LYR_SUB_3, GF_PLANE_TOGGLE_ON);
        reg_GX_POWCNT |= REG_GX_POWCNT_DSEL_MASK;
        SysTask_Destroy(task);
    }
}

void TradeSequenceDepart_StartStreakFall(TradeSequenceDepart *depart, SysTask **task, TradeSequenceDepartStreakFall *fall) {
    int i;

    fall->depart = depart;
    fall->task = task;
    fall->delay = 0;
    fall->revealed = 0;
    for (i = 0; i < TRADE_SEQUENCE_DEPART_NUM_STREAKS; i++) {
        fall->visible[i] = FALSE;
        fall->positions[i] = *Sprite_GetMatrixPtr(depart->streaks[i].head);
    }
    *fall->task = SysTask_CreateOnMainQueue(TradeSequenceDepart_StreakFallTask, fall, 0);
    if (*fall->task == NULL) {
        Heap_Free(fall);
    }
}

void TradeSequenceDepart_StreakFallTask(SysTask *task, void *data) {
    TradeSequenceDepartStreakFall *fall = data;
    int i;

    fall->delay++;
    if (fall->delay > 12) {
        fall->delay = 0;
        if (fall->revealed < TRADE_SEQUENCE_DEPART_NUM_STREAKS) {
            fall->visible[fall->revealed] = TRUE;
            fall->revealed++;
        }
    }

    for (i = 0; i < TRADE_SEQUENCE_DEPART_NUM_STREAKS; i++) {
        VecFx32 tail;

        fall->positions[i].y += FX32_CONST(33);
        if (fall->positions[i].y >= FX32_CONST(424)) {
            fall->positions[i].y -= FX32_CONST(452);
            if (fall->visible[i]) {
                Sprite_SetDrawFlag(fall->depart->streaks[i].head, TRUE);
                Sprite_SetDrawFlag(fall->depart->streaks[i].tail, TRUE);
            }
        }
        Sprite_SetMatrix(fall->depart->streaks[i].head, &fall->positions[i]);
        tail = fall->positions[i];
        tail.y += FX32_CONST(56);
        Sprite_SetMatrix(fall->depart->streaks[i].tail, &tail);
    }
}

void TradeSequenceDepart_StopStreakFall(SysTask *task) {
    if (task != NULL) {
        *((TradeSequenceDepartStreakFall *)SysTask_GetData(task))->task = NULL;
        SysTask_Destroy(task);
    }
}

void TradeSequenceDepart_StartScroll(TradeSequenceDepart *depart, SysTask **task) {
    TradeSequenceDepartScroll *scroll = Heap_Alloc(HEAP_ID_57, sizeof(TradeSequenceDepartScroll));
    if (scroll) {
        scroll->depart = depart;
        scroll->bgConfig = depart->bgConfig;
        scroll->mainY = Bg_GetYpos(scroll->bgConfig, GF_BG_LYR_MAIN_2) << FX32_SHIFT;
        scroll->subY = Bg_GetYpos(scroll->bgConfig, GF_BG_LYR_SUB_2) << FX32_SHIFT;
        scroll->speed = 0;
        scroll->distance = 0;
        scroll->task = task;
        scroll->fallTask = NULL;
        *task = SysTask_CreateOnVBlankQueue(TradeSequenceDepart_ScrollTask, scroll, 0);
        if (*task == NULL) {
            Heap_Free(scroll);
        }
    }
}

void TradeSequenceDepart_ScrollTask(SysTask *task, void *data) {
    TradeSequenceDepartScroll *scroll = data;

    if (scroll->distance < FX32_CONST(512)) {
        scroll->speed += 0x120;
        scroll->distance += scroll->speed;
        BgSetPosTextAndCommit(scroll->bgConfig, GF_BG_LYR_MAIN_2, BG_POS_OP_SET_Y, (scroll->mainY - scroll->distance) >> FX32_SHIFT);
        BgSetPosTextAndCommit(scroll->bgConfig, GF_BG_LYR_SUB_2, BG_POS_OP_SET_Y, (scroll->subY - scroll->distance) >> FX32_SHIFT);
        if (scroll->distance >= FX32_CONST(128) && scroll->fallTask == NULL) {
            TradeSequenceDepart_StartStreakFall(scroll->depart, &scroll->fallTask, &scroll->fall);
        }
    }
}

void TradeSequenceDepart_StopScroll(SysTask *task) {
    TradeSequenceDepartScroll *scroll;
    if (task != NULL) {
        scroll = SysTask_GetData(task);
        *scroll->task = NULL;
        TradeSequenceDepart_StopStreakFall(scroll->fallTask);
        Heap_Free(scroll);
        SysTask_Destroy(task);
    }
}

void TradeSequenceDepart_StartAlphaFade(TradeSequenceModel *model, int start, int target, int frames) {
    TradeSequenceDepartAlphaFade *fade = Heap_Alloc(HEAP_ID_57, sizeof(TradeSequenceDepartAlphaFade));
    if (fade != NULL) {
        fade->alpha = start << FX32_SHIFT;
        fade->target = target << FX32_SHIFT;
        fade->step = (fade->target - fade->alpha) / frames;
        fade->frames = frames;
        fade->model = model;
        TradeSequence_SetModelAlpha(model, start);
        SysTask_CreateOnMainQueue(TradeSequenceDepart_AlphaFadeTask, fade, 0);
    }
}

void TradeSequenceDepart_AlphaFadeTask(SysTask *task, void *data) {
    TradeSequenceDepartAlphaFade *fade = data;

    fade->frames--;
    if (fade->frames > 0) {
        fade->alpha += fade->step;
        TradeSequence_SetModelAlpha(fade->model, fade->alpha >> FX32_SHIFT);
        return;
    }
    TradeSequence_SetModelAlpha(fade->model, fade->target >> FX32_SHIFT);
    Heap_Free(fade);
    SysTask_Destroy(task);
}

SysTask *TradeSequenceDepart_StartFlight(TradeSequenceDepart *depart) {
    TradeSequenceDepartFlight *flight = &depart->flight;
    int i;

    TradeSequence_GetModelPosition(depart->ball, &flight->center);
    TradeSequence_GetModelRotation(depart->ball, &flight->rotation);
    flight->position = flight->center;
    flight->models = depart->models;
    flight->model = depart->ball;
    flight->height = flight->center.y;
    flight->speed = (-0x8000 - flight->center.y) / 30;
    flight->frames = 30;
    flight->radiusX = 0;
    flight->radiusZ = 0;
    flight->radiusXStep = 0;
    flight->radiusZStep = 0;
    flight->angle = 0;
    flight->angleSpeed = 0x4000;
    flight->angleSpeedStep = 0;
    flight->flyingAway = FALSE;
    for (i = 0; i < 3; i++) {
        flight->spins[i].speed = 0;
        flight->spins[i].step = 0;
        flight->spins[i].target = 0;
        flight->spins[i].frames = 0;
    }
    flight->outOfView = FALSE;
    TradeSequenceDepart_SetAngleSpeed(flight, 0xD00, 30);
    TradeSequenceDepart_SetRadius(flight, 0x29000, 0x14000, 30);
    return SysTask_CreateOnMainQueue(TradeSequenceDepart_FlightTask_Appear, flight, 0);
}

void TradeSequenceDepart_SetAngleSpeed(TradeSequenceDepartFlight *flight, int target, int frames) {
    if (frames != 0) {
        flight->angleSpeedStep = (target - flight->angleSpeed) / frames;
        flight->angleSpeed += flight->angleSpeedStep;
    } else {
        flight->angleSpeed = target;
    }
    flight->angleSpeedTarget = target;
    flight->angleSpeedFrames = frames;
}

void TradeSequenceDepart_SetRadius(TradeSequenceDepartFlight *flight, fx32 radiusX, fx32 radiusZ, int frames) {
    if (frames != 0) {
        flight->radiusXStep = (radiusX - flight->radiusX) / frames;
        flight->radiusZStep = (radiusZ - flight->radiusZ) / frames;
        flight->radiusX += flight->radiusXStep;
        flight->radiusZ += flight->radiusZStep;
        flight->radiusXTarget = radiusX;
        flight->radiusZTarget = radiusZ;
    }
    flight->radiusFrames = frames;
}

void TradeSequenceDepart_SetSpin(TradeSequenceDepartFlight *flight, int axis, int target, int frames) {
    if (frames != 0) {
        flight->spins[axis].step = (target - flight->spins[axis].speed) / frames;
    }
    flight->spins[axis].target = target;
    flight->spins[axis].frames = frames;
}

void TradeSequenceDepart_UpdateFlight(TradeSequenceDepartFlight *flight) {
    s64 product;
    s32 radius;
    s32 angle;
    int i;

    if (flight->angleSpeedFrames != 0) {
        flight->angleSpeedFrames--;
        if (flight->angleSpeedFrames != 0) {
            flight->angleSpeed += flight->angleSpeedStep;
        } else {
            flight->angleSpeed = flight->angleSpeedTarget;
        }
    }
    flight->angle += flight->angleSpeed;
    angle = (u16)flight->angle / 182; // to degrees
    if (angle >= 360) {
        angle -= 360;
    }
    if (flight->radiusFrames != 0) {
        flight->radiusX += flight->radiusXStep;
        flight->radiusZ += flight->radiusZStep;
        flight->radiusFrames--;
        if (flight->radiusFrames == 0) {
            flight->radiusX = flight->radiusXTarget;
            flight->radiusZ = flight->radiusZTarget;
        }
    }
    radius = flight->radiusX;
    product = GF_CosDegNoWrap(angle);
    product *= radius;
    flight->position.x = flight->center.x + (s32)((product + 0x800) >> FX32_SHIFT);
    radius = flight->radiusZ;
    product = GF_SinDegNoWrap(angle);
    product *= radius;
    flight->position.z = flight->center.z + (s32)((product + 0x800) >> FX32_SHIFT);
    flight->rotation.y += flight->angleSpeed;
    for (i = 0; i < 3; i++) {
        if (flight->spins[i].frames != 0) {
            flight->spins[i].frames--;
            if (flight->spins[i].frames == 0) {
                flight->spins[i].speed = flight->spins[i].target;
            } else {
                flight->spins[i].speed += flight->spins[i].step;
            }
        }
    }
    flight->rotation.x += flight->spins[0].speed;
    flight->rotation.y += flight->spins[1].speed;
    flight->rotation.z += flight->spins[2].speed;
}

void TradeSequenceDepart_FlightTask_Appear(SysTask *task, void *data) {
    TradeSequenceDepartFlight *flight = data;

    if (flight->frames != 0) {
        flight->height += flight->speed;
        flight->position.y = flight->height;
        flight->frames--;
        if (flight->frames <= 0) {
            flight->acceleration = 0x200;
            flight->speed = 0;
            TradeSequenceDepart_SetRadius(flight, 0x4000, 0x4000, 60);
            TradeSequenceDepart_SetSpin(flight, 0, -0x800, 30);
            flight->flyingAway = TRUE;
            SysTask_SetFunc(task, TradeSequenceDepart_FlightTask_FlyAway);
        }
    }
    TradeSequenceDepart_UpdateFlight(flight);
    TradeSequence_SetModelPosition(flight->model, &flight->position);
    TradeSequence_SetModelRotation(flight->model, &flight->rotation);
}

void TradeSequenceDepart_FlightTask_FlyAway(SysTask *task, void *data) {
#pragma unused(task)
    TradeSequenceDepartFlight *flight = data;

    if (!flight->outOfView) {
        flight->speed += flight->acceleration;
        flight->height += flight->speed;
        flight->position.y = flight->height;
        TradeSequenceDepart_UpdateFlight(flight);
        TradeSequence_SetModelPosition(flight->model, &flight->position);
        TradeSequence_SetModelRotation(flight->model, &flight->rotation);
        if (!TradeSequence_IsModelInView(flight->model)) {
            TradeSequence_SetModelVisible(flight->model, FALSE);
            flight->outOfView = TRUE;
        }
    }
}

BOOL TradeSequenceDepart_IsFlightOutOfView(SysTask *task) {
    return ((TradeSequenceDepartFlight *)SysTask_GetData(task))->outOfView;
}

BOOL TradeSequenceDepart_IsFlightFlyingAway(SysTask *task) {
    return ((TradeSequenceDepartFlight *)SysTask_GetData(task))->flyingAway;
}

void TradeSequenceDepart_RestartFlightFromBelow(SysTask *task) {
    CameraAngle angle;
    TradeSequenceDepartFlight *flight = SysTask_GetData(task);

    flight->outOfView = FALSE;
    angle.z = 0;
    angle.y = angle.z;
    angle.x = angle.y;
    TradeSequence_SetCameraPerspectiveType(flight->models, 1);
    TradeSequence_SetCameraAngle(flight->models, &angle);
    flight->speed = 0x58CC;
    flight->height = flight->speed - FX32_CONST(128);
    flight->frames = 20;
    flight->position.y = flight->height;
    TradeSequenceDepart_SetRadius(flight, 0, 0, 20);
    TradeSequenceDepart_SetSpin(flight, 1, -flight->angleSpeed, 30);
    TradeSequenceDepart_SetSpin(flight, 2, 0x100, 50);
    TradeSequence_SetModelScale(flight->model, FX32_CONST(2));
    TradeSequence_SetModelVisible(flight->model, TRUE);
    SysTask_SetFunc(task, TradeSequenceDepart_FlightTask_Rise);
}

void TradeSequenceDepart_FlightTask_Rise(SysTask *task, void *data) {
    TradeSequenceDepartFlight *flight = data;

    TradeSequenceDepart_UpdateFlight(flight);
    if (flight->frames != 0) {
        flight->height += flight->speed;
        flight->position.y = flight->height;
        flight->frames--;
        if (flight->frames == 0) {
            SysTask_SetFunc(task, TradeSequenceDepart_FlightTask_Hover);
        }
    }
    TradeSequence_SetModelPosition(flight->model, &flight->position);
    TradeSequence_SetModelRotation(flight->model, &flight->rotation);
}

void TradeSequenceDepart_FlightTask_Hover(SysTask *task, void *data) {
    TradeSequenceDepartFlight *flight = data;

    TradeSequenceDepart_UpdateFlight(flight);
    flight->frames++;
    if (flight->frames > 110) {
        flight->speed = (-0x3A800 - flight->height) / 53;
        flight->frames = 53;
        SysTask_SetFunc(task, TradeSequenceDepart_FlightTask_Sink);
    }
    TradeSequence_SetModelPosition(flight->model, &flight->position);
    TradeSequence_SetModelRotation(flight->model, &flight->rotation);
}

void TradeSequenceDepart_FlightTask_Sink(SysTask *task, void *data) {
    TradeSequenceDepartFlight *flight = data;

    TradeSequenceDepart_UpdateFlight(flight);
    if (flight->frames != 0) {
        flight->height += flight->speed;
        flight->position.y = flight->height;
        flight->frames--;
        if (flight->frames == 0) {
            SysTask_SetFunc(task, TradeSequenceDepart_FlightTask_Pause);
        }
    }
    TradeSequence_SetModelPosition(flight->model, &flight->position);
    TradeSequence_SetModelRotation(flight->model, &flight->rotation);
}

void TradeSequenceDepart_FlightTask_Pause(SysTask *task, void *data) {
    TradeSequenceDepartFlight *flight = data;

    TradeSequenceDepart_UpdateFlight(flight);
    flight->frames++;
    if (flight->frames > 1) {
        flight->speed = FX32_CONST(32);
        SysTask_SetFunc(task, TradeSequenceDepart_FlightTask_Leave);
    }
    TradeSequence_SetModelPosition(flight->model, &flight->position);
    TradeSequence_SetModelRotation(flight->model, &flight->rotation);
}

void TradeSequenceDepart_FlightTask_Leave(SysTask *task, void *data) {
    TradeSequenceDepartFlight *flight = data;

    if (!flight->outOfView) {
        TradeSequenceDepart_UpdateFlight(flight);
        flight->height += flight->speed;
        flight->position.y = flight->height;
        TradeSequence_SetModelPosition(flight->model, &flight->position);
        TradeSequence_SetModelRotation(flight->model, &flight->rotation);
        if (!TradeSequence_IsModelInView(flight->model)) {
            flight->outOfView = TRUE;
        }
    }
}

// Scene 2 of the trade animation: the ball model spirals across the screen, the screens swap as it leaves the top
// one, and it shrinks into the distance on the bottom one where a sprite animation plays; then the screen fades to
// white.

TradeSequenceRecede *TradeSequenceRecede_Init(TradeSequence *ts) {
    TradeSequenceRecede *recede = Heap_Alloc(HEAP_ID_57, sizeof(TradeSequenceRecede));
    if (recede != NULL) {
        recede->ts = ts;
        recede->subState = 0;
        recede->bgConfig = TradeSequence_GetBgConfig(ts);
        recede->models = NULL;
        TradeSequenceRecede_InitFlight(&recede->flight);
    }
    return recede;
}

void TradeSequenceRecede_Exit(TradeSequenceRecede *recede) {
    if (recede != NULL) {
        Main_SetVBlankIntrCB(NULL, NULL);
        TradeSequenceRecede_FreeBgs(recede);
        TradeSequenceRecede_Free3D(recede);
        TradeSequenceRecede_DeleteSprite(recede);
        TradeSequenceRecede_StopFlight(&recede->flight);
        Heap_Free(recede);
    }
}

BOOL TradeSequenceRecede_Main(TradeSequenceRecede *recede, u32 *state) {
    if (*state < TRADE_SEQUENCE_RECEDE_NUM_STATES) {
        if (sTradeSequenceRecedeStates[*state](recede, &recede->subState)) {
            *state = *state + 1;
            recede->subState = 0;
        }
        TradeSequenceRecede_Draw(recede);
        return FALSE;
    }
    return TRUE;
}

void TradeSequenceRecede_Draw(TradeSequenceRecede *recede) {
    if (recede->models != NULL) {
        G3X_Reset();
        TradeSequence_DrawModels(recede->models);
        RequestSwap3DBuffers(GX_SORTMODE_AUTO, GX_BUFFERMODE_Z);
    }
}

BOOL TradeSequenceRecede_SetupScreens(TradeSequenceRecede *recede, u32 *state) {
    TradeSequenceRecede_InitBgs(recede);
    TradeSequenceRecede_Init3D(recede);
    TradeSequenceRecede_CreateSprite(recede);
    recede->swapScreens = FALSE;
    SysTask_CreateOnVWaitQueue(TradeSequenceRecede_ScreenSwapTask, recede, 0);
    BeginNormalPaletteFade(0, 1, 1, RGB_WHITE, 3, 1, HEAP_ID_57);
    return TRUE;
}

BOOL TradeSequenceRecede_Run(TradeSequenceRecede *recede, u32 *state) {
    switch (*state) {
    case 0:
        recede->timer = 0;
        (*state)++;
        break;
    case 1:
        recede->timer++;
        if (recede->timer > 10) {
            PlaySE(SEQ_SE_DP_KOUKAN06);
            TradeSequenceRecede_StartFlight(recede, &recede->flight);
            TradeSequence_SetModelVisible(recede->ball, TRUE);
            (*state)++;
        }
        break;
    case 2:
        if (TradeSequenceRecede_IsFlightDone(&recede->flight)) {
            PlaySE(SEQ_SE_DP_KOUKAN07);
            Sprite_SetAnimCtrlSeq(recede->sprite, 3);
            Sprite_SetDrawFlag(recede->sprite, TRUE);
            (*state)++;
        }
        break;
    case 3:
        if (!Sprite_IsAnimated(recede->sprite)) {
            BeginNormalPaletteFade(0, 0, 0, RGB_WHITE, 16, 1, HEAP_ID_57);
            (*state)++;
        }
        break;
    case 4:
        if (IsPaletteFadeFinished()) {
            return TRUE;
        }
        break;
    }
    return FALSE;
}

void TradeSequenceRecede_InitBgs(TradeSequenceRecede *recede) {
    int scrnFile;
    int charFile;
    int palFile;
    int palOffset;

    GfGfx_SetBanks(&sTradeSequenceRecedeGraphicsBanks);
    reg_GX_POWCNT &= ~REG_GX_POWCNT_DSEL_MASK;
    SetBothScreensModesAndDisable(&sTradeSequenceRecedeGraphicsModes);
    InitBgFromTemplate(recede->bgConfig, GF_BG_LYR_MAIN_3, &sTradeSequenceRecedeBgTemplate, GF_BG_TYPE_TEXT);
    InitBgFromTemplate(recede->bgConfig, GF_BG_LYR_SUB_3, &sTradeSequenceRecedeBgTemplate, GF_BG_TYPE_TEXT);

    switch (TradeSequence_GetTimeOfDay(recede->ts)) {
    default:
    case 0:
        scrnFile = 15;
        charFile = 16;
        palFile = 17;
        palOffset = 0;
        break;
    case 1:
        scrnFile = 15;
        charFile = 16;
        palFile = 17;
        palOffset = 0x20;
        break;
    case 2:
        scrnFile = 15;
        charFile = 16;
        palFile = 17;
        palOffset = 0x40;
        break;
    case 3:
        scrnFile = 24;
        charFile = 25;
        palFile = 26;
        palOffset = 0;
        break;
    }
    GfGfxLoader_LoadCharData(NARC_a_0_8_9, charFile, recede->bgConfig, GF_BG_LYR_MAIN_3, 0, 0, TRUE, HEAP_ID_57);
    GfGfxLoader_LoadCharData(NARC_a_0_8_9, charFile, recede->bgConfig, GF_BG_LYR_SUB_3, 0, 0, TRUE, HEAP_ID_57);
    GfGfxLoader_LoadScrnData(NARC_a_0_8_9, scrnFile, recede->bgConfig, GF_BG_LYR_MAIN_3, 0, 0, TRUE, HEAP_ID_57);
    GfGfxLoader_LoadScrnData(NARC_a_0_8_9, scrnFile, recede->bgConfig, GF_BG_LYR_SUB_3, 0, 0, TRUE, HEAP_ID_57);
    GfGfxLoader_GXLoadPalWithSrcOffset(NARC_a_0_8_9, palFile, GF_PAL_LOCATION_MAIN_BG, palOffset, GF_PAL_SLOT_0_OFFSET, 0x20, HEAP_ID_57);
    GfGfxLoader_GXLoadPalWithSrcOffset(NARC_a_0_8_9, palFile, GF_PAL_LOCATION_SUB_BG, palOffset, GF_PAL_SLOT_0_OFFSET, 0x20, HEAP_ID_57);
    BgSetPosTextAndCommit(recede->bgConfig, GF_BG_LYR_MAIN_3, BG_POS_OP_SET_Y, 256);
    BgSetPosTextAndCommit(recede->bgConfig, GF_BG_LYR_SUB_3, BG_POS_OP_SET_Y, 0);
    GfGfx_EngineATogglePlanes(GX_PLANEMASK_OBJ, GF_PLANE_TOGGLE_ON);
    GfGfx_EngineBTogglePlanes(GX_PLANEMASK_OBJ, GF_PLANE_TOGGLE_ON);
}

void TradeSequenceRecede_FreeBgs(TradeSequenceRecede *recede) {
    FreeBgTilemapBuffer(recede->bgConfig, GF_BG_LYR_MAIN_3);
    FreeBgTilemapBuffer(recede->bgConfig, GF_BG_LYR_SUB_3);
}

void TradeSequenceRecede_Init3D(TradeSequenceRecede *recede) {
    CameraAngle angle;
    VecFx32 cameraPos;

    NNS_G3dInit();
    G3X_InitMtxStack();
    G3X_SetShading(GX_SHADING_TOON);
    G3X_AntiAlias(TRUE);
    G3X_AlphaTest(FALSE, 0);
    G3X_AlphaBlend(TRUE);
    G3X_EdgeMarking(TRUE);
    G3X_SetEdgeColorTable(sTradeSequenceRecedeEdgeColors);
    G3X_SetClearColor(RGB(31, 31, 16), 0, 0x7FFF, 63, FALSE);
    G3_SwapBuffers(GX_SORTMODE_AUTO, GX_BUFFERMODE_W);
    G3_ViewPort(0, 0, 255, 191);
    GF_3DVramMan_InitFrameTexVramManager(1, TRUE);
    GF_3DVramMan_InitFramePlttVramManager(0x4000, TRUE);
    GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG0, GF_PLANE_TOGGLE_ON);
    G2_SetBG0Priority(0);
    recede->models = TradeSequence_CreateModels(1, 0, 0, 0);
    recede->ball = TradeSequence_LoadModel(recede->models, 0, NARC_a_0_8_9, 27, 0, 0, 0, FALSE);
    angle.x = 0x18E3;
    angle.y = 0;
    angle.z = 0;
    TradeSequence_SetCameraAngle(recede->models, &angle);
    TradeSequence_SetCameraPerspectiveAngle(recede->models, 0x7D2);
    TradeSequence_GetCameraPos(recede->models, &cameraPos);
}

void TradeSequenceRecede_Free3D(TradeSequenceRecede *recede) {
    if (recede->models != NULL) {
        TradeSequence_DestroyModels(recede->models);
    }
}

void TradeSequenceRecede_CreateSprite(TradeSequenceRecede *recede) {
    NNSG2dImagePaletteProxy palette;
    NNSG2dImageProxy image;
    SpriteResourcesHeader header;

    TradeSequence_LoadCellAnim(&recede->cellAnim, NARC_a_0_8_9, 7, 8);
    NNS_G2dInitImagePaletteProxy(&palette);
    NNS_G2dInitImageProxy(&image);
    GfGfxLoader_LoadImageMapping(NARC_a_0_8_9, 9, TRUE, GF_BG_LYR_MAIN_0, 0, NNS_G2D_VRAM_TYPE_2DMAIN, 0, HEAP_ID_57, &image);
    GfGfxLoader_PartiallyLoadPalette(NARC_a_0_8_9, 10, NNS_G2D_VRAM_TYPE_2DMAIN, 0, HEAP_ID_57, &palette);
    TradeSequence_InitSpriteHeader(&header, &recede->cellAnim, &image, &palette, 0);
    recede->sprite = TradeSequence_CreateSprite(recede->ts, &header, 128, 96, 0, NNS_G2D_VRAM_TYPE_2DMAIN);
    Sprite_SetDrawFlag(recede->sprite, FALSE);
}

void TradeSequenceRecede_DeleteSprite(TradeSequenceRecede *recede) {
    if (recede->sprite != NULL) {
        Sprite_Delete(recede->sprite);
    }
    TradeSequence_FreeCellAnim(&recede->cellAnim);
}

void TradeSequenceRecede_InitFlight(TradeSequenceRecedeFlight *flight) {
    flight->task = NULL;
}

void TradeSequenceRecede_StopFlight(TradeSequenceRecedeFlight *flight) {
    if (flight->task != NULL) {
        SysTask_Destroy(flight->task);
        flight->task = NULL;
    }
}

void TradeSequenceRecede_StartFlight(TradeSequenceRecede *recede, TradeSequenceRecedeFlight *flight) {
    flight->recede = recede;
    flight->ball = recede->ball;
    flight->models = recede->models;
    flight->center.x = 0;
    flight->center.y = -0xAF200;
    flight->center.z = 0xE2000;
    flight->angle = -10128;
    flight->angleSpeed = -2736;
    flight->centerStepY = 0x134C4;
    flight->centerStepZ = -0xB2AA;
    flight->state = 0;
    flight->timer = 0;
    flight->elapsed = 0;
    flight->rotation.x = 0x2000;
    flight->rotation.y = -0x2000;
    flight->rotation.z = 0;
    TradeSequence_SetModelRotation(flight->ball, &flight->rotation);
    flight->scale = FX32_ONE;
    flight->radiusX = 0x12800;
    flight->radiusY = 0x18C00;
    flight->stepFrames = 0;
    TradeSequenceRecede_UpdateFlight(flight);
    TradeSequence_SetModelPosition(flight->ball, &flight->position);
    TradeSequence_GetModelRotation(flight->ball, &flight->rotation);
    flight->task = SysTask_CreateOnMainQueue(TradeSequenceRecede_FlightTask, flight, 0);
}

BOOL TradeSequenceRecede_IsFlightDone(TradeSequenceRecedeFlight *flight) {
    if (flight->task == NULL) {
        return TRUE;
    }
    return FALSE;
}

void TradeSequenceRecede_SetFlightTargets(TradeSequenceRecedeFlight *flight, fx32 radiusX, fx32 radiusY, fx32 scale, int frames) {
    if (frames) {
        flight->radiusXStep = (radiusX - flight->radiusX) / frames;
        flight->radiusYStep = (radiusY - flight->radiusY) / frames;
        flight->scaleStep = (scale - flight->scale) / frames;
        flight->stepFrames = frames;
    }
}

void TradeSequenceRecede_UpdateFlight(TradeSequenceRecedeFlight *flight) {
    int angle = (u16)flight->angle / 182; // to degrees
    if (angle >= 360) {
        angle -= 360;
    }
    flight->angle += flight->angleSpeed;
    if (flight->stepFrames) {
        flight->radiusX += flight->radiusXStep;
        flight->radiusY += flight->radiusYStep;
        flight->scale += flight->scaleStep;
        TradeSequence_SetModelScale(flight->ball, flight->scale);
        flight->stepFrames--;
    }
    flight->offset.x = FX_Mul(GF_CosDegNoWrap(angle), flight->radiusX);
    flight->offset.y = FX_Mul(GF_SinDegNoWrap(angle), flight->radiusY);
    flight->offset.z = 0;
    flight->rotation.x += 0x300;
    TradeSequence_SetModelRotation(flight->ball, &flight->rotation);
    VEC_Add(&flight->offset, &flight->center, &flight->position);
}

// Flies the ball into view, requests the screen swap once it leaves the top screen, then (after at least 60 frames)
// shrinks it into the distance and hides it.
void TradeSequenceRecede_FlightTask(SysTask *task, void *data) {
    TradeSequenceRecedeFlight *flight = data;

    if (flight->elapsed < 60) {
        flight->elapsed++;
    }
    switch (flight->state) {
    case 0:
        flight->center.y += flight->centerStepY;
        flight->center.z += flight->centerStepZ;
        TradeSequenceRecede_UpdateFlight(flight);
        TradeSequence_SetModelPosition(flight->ball, &flight->position);
        if (TradeSequence_IsModelInView(flight->ball)) {
            flight->state++;
        }
        break;
    case 1:
        if (!TradeSequence_IsModelInView(flight->ball)) {
            TradeSequenceRecede_RequestScreenSwap(flight->recede);
            flight->state++;
            return;
        }
        flight->center.y += flight->centerStepY;
        flight->center.z += flight->centerStepZ;
        TradeSequenceRecede_UpdateFlight(flight);
        TradeSequence_SetModelPosition(flight->ball, &flight->position);
        return;
    case 2:
        flight->center.y += flight->centerStepY;
        flight->center.z += flight->centerStepZ;
        TradeSequenceRecede_UpdateFlight(flight);
        TradeSequence_SetModelPosition(flight->ball, &flight->position);
        if (flight->elapsed >= 60) {
            TradeSequenceRecede_SetFlightTargets(flight, 0x800, 0x800, 0x200, 12);
            flight->timer = 0;
            flight->state++;
        }
        break;
    case 3:
        TradeSequenceRecede_UpdateFlight(flight);
        TradeSequence_SetModelPosition(flight->ball, &flight->position);
        flight->timer++;
        if (flight->timer >= 12) {
            TradeSequence_SetModelVisible(flight->ball, FALSE);
            flight->timer = 0;
            flight->state++;
        }
        break;
    case 4:
        flight->timer++;
        if (flight->timer > 8) {
            flight->task = NULL;
            SysTask_Destroy(task);
        }
        break;
    }
}

void TradeSequenceRecede_RequestScreenSwap(TradeSequenceRecede *recede) {
    recede->swapScreens = TRUE;
}

// Once requested (on the next V-wait): moves the layer 3 backgrounds, tilts the camera and swaps the screens.
void TradeSequenceRecede_ScreenSwapTask(SysTask *task, void *data) {
    TradeSequenceRecede *recede = data;
    CameraAngle delta;

    if (recede->swapScreens) {
        recede->swapScreens = FALSE;
        delta.x = 0x1150;
        delta.y = 0;
        delta.z = 0;
        BgSetPosTextAndCommit(recede->bgConfig, GF_BG_LYR_MAIN_3, BG_POS_OP_SET_Y, 0);
        BgSetPosTextAndCommit(recede->bgConfig, GF_BG_LYR_SUB_3, BG_POS_OP_SET_Y, 256);
        TradeSequence_AdjustCameraAngle(recede->models, &delta);
        reg_GX_POWCNT |= REG_GX_POWCNT_DSEL_MASK;
        SysTask_Destroy(task);
    }
}

// Scene 3 of the trade animation (trades only): the pictures of the sent and the received Pokemon slide past each
// other, then the screen fades to white.

TradeSequenceExchange *TradeSequenceExchange_Init(TradeSequence *ts) {
    TradeSequenceExchange *exchange = Heap_Alloc(HEAP_ID_57, sizeof(TradeSequenceExchange));
    if (exchange != NULL) {
        exchange->ts = ts;
        exchange->subState = 0;
        exchange->bgConfig = TradeSequence_GetBgConfig(ts);
    }
    return exchange;
}

void TradeSequenceExchange_Exit(TradeSequenceExchange *exchange) {
    if (exchange != NULL) {
        TradeSequenceExchange_FreeBgs(exchange);
        Heap_Free(exchange);
    }
}

BOOL TradeSequenceExchange_Main(TradeSequenceExchange *exchange, u32 *state) {
    if (*state < TRADE_SEQUENCE_EXCHANGE_NUM_STATES) {
        if (sTradeSequenceExchangeStates[*state](exchange, &exchange->subState)) {
            *state = *state + 1;
            exchange->subState = 0;
        }
        return FALSE;
    }
    return TRUE;
}

BOOL TradeSequenceExchange_SetupScreens(TradeSequenceExchange *exchange, u32 *state) {
    TradeSequenceExchange_InitBgs(exchange);
    BeginNormalPaletteFade(0, 1, 1, RGB_WHITE, 8, 1, HEAP_ID_57);
    return TRUE;
}

BOOL TradeSequenceExchange_Slide(TradeSequenceExchange *exchange, u32 *state) {
    switch (*state) {
    case 0:
        if (IsPaletteFadeFinished()) {
            exchange->sentY = FX32_CONST(-384);
            exchange->receivedY = FX32_CONST(80);
            exchange->sentStep = 0x5999;
            exchange->receivedStep = -0x2222;
            exchange->frames = 60;
            exchange->slideDone = FALSE;
            SysTask_CreateOnVBlankQueue(TradeSequenceExchange_SlideTask, exchange, 0);
            (*state)++;
        }
        break;
    case 1:
        if (exchange->slideDone) {
            PlaySE(SEQ_SE_DP_KOUKAN03);
            BeginNormalPaletteFade(0, 0, 0, RGB_WHITE, 16, 1, HEAP_ID_57);
            (*state)++;
        }
        break;
    case 2:
        if (IsPaletteFadeFinished()) {
            return TRUE;
        }
        break;
    }
    return FALSE;
}

void TradeSequenceExchange_SlideTask(SysTask *task, void *data) {
    TradeSequenceExchange *exchange = data;

    if (exchange->frames != 0) {
        exchange->frames--;
        exchange->sentY += exchange->sentStep;
        exchange->receivedY += exchange->receivedStep;
        TradeSequenceExchange_SetMonPicPositions(exchange->bgConfig, exchange->sentY >> FX32_SHIFT, exchange->receivedY >> FX32_SHIFT);
    } else {
        TradeSequenceExchange_SetMonPicPositions(exchange->bgConfig, -48, -48);
        exchange->slideDone = TRUE;
        SysTask_Destroy(task);
    }
}

void TradeSequenceExchange_InitBgs(TradeSequenceExchange *exchange) {
    GfGfx_SetBanks(&sTradeSequenceExchangeGraphicsBanks);
    reg_GX_POWCNT |= REG_GX_POWCNT_DSEL_MASK;
    SetBothScreensModesAndDisable(&sTradeSequenceExchangeGraphicsModes);
    InitBgFromTemplate(exchange->bgConfig, GF_BG_LYR_MAIN_1, &sTradeSequenceExchangeBgTemplate_SentMon, GF_BG_TYPE_TEXT);
    InitBgFromTemplate(exchange->bgConfig, GF_BG_LYR_SUB_1, &sTradeSequenceExchangeBgTemplate_SentMon, GF_BG_TYPE_TEXT);
    InitBgFromTemplate(exchange->bgConfig, GF_BG_LYR_MAIN_2, &sTradeSequenceExchangeBgTemplate_ReceivedMon, GF_BG_TYPE_TEXT);
    InitBgFromTemplate(exchange->bgConfig, GF_BG_LYR_MAIN_3, &sTradeSequenceExchangeBgTemplate_Background, GF_BG_TYPE_TEXT);
    InitBgFromTemplate(exchange->bgConfig, GF_BG_LYR_SUB_3, &sTradeSequenceExchangeBgTemplate_Background, GF_BG_TYPE_TEXT);
    GfGfxLoader_LoadCharData(NARC_a_0_8_9, 19, exchange->bgConfig, GF_BG_LYR_MAIN_3, 0, 0, TRUE, HEAP_ID_57);
    GfGfxLoader_LoadCharData(NARC_a_0_8_9, 19, exchange->bgConfig, GF_BG_LYR_SUB_3, 0, 0, TRUE, HEAP_ID_57);
    GfGfxLoader_LoadScrnData(NARC_a_0_8_9, 18, exchange->bgConfig, GF_BG_LYR_MAIN_3, 0, 0, TRUE, HEAP_ID_57);
    GfGfxLoader_LoadScrnData(NARC_a_0_8_9, 18, exchange->bgConfig, GF_BG_LYR_SUB_3, 0, 0, TRUE, HEAP_ID_57);
    GfGfxLoader_GXLoadPal(NARC_a_0_8_9, 20, GF_PAL_LOCATION_MAIN_BG, GF_PAL_SLOT_0_OFFSET, 0x20, HEAP_ID_57);
    GfGfxLoader_GXLoadPal(NARC_a_0_8_9, 20, GF_PAL_LOCATION_SUB_BG, GF_PAL_SLOT_0_OFFSET, 0x20, HEAP_ID_57);
    BG_FillCharDataRange(exchange->bgConfig, GF_BG_LYR_MAIN_1, 0, 1, 200);
    BG_FillCharDataRange(exchange->bgConfig, GF_BG_LYR_SUB_1, 0, 1, 200);
    BG_FillCharDataRange(exchange->bgConfig, GF_BG_LYR_MAIN_2, 0, 1, 200);
    TradeSequence_LoadMonPicToBg(exchange->ts, 0, GF_BG_LYR_MAIN_1, 14, FALSE);
    TradeSequence_LoadMonPicToBg(exchange->ts, 0, GF_BG_LYR_SUB_1, 14, FALSE);
    TradeSequence_LoadMonPicToBg(exchange->ts, 1, GF_BG_LYR_MAIN_2, 15, FALSE);
    FillBgTilemapRect(exchange->bgConfig, GF_BG_LYR_MAIN_1, 200, 0, 0, 32, 64, 0);
    FillBgTilemapRect(exchange->bgConfig, GF_BG_LYR_SUB_1, 200, 0, 0, 32, 64, 0);
    FillBgTilemapRect(exchange->bgConfig, GF_BG_LYR_MAIN_2, 200, 0, 0, 32, 64, 0);
    TradeSequence_LoadMonPicTilemap(exchange->ts, 0, GF_BG_LYR_MAIN_1, 14, 20, 0);
    TradeSequence_LoadMonPicTilemap(exchange->ts, 0, GF_BG_LYR_SUB_1, 14, 20, 0);
    TradeSequence_LoadMonPicTilemap(exchange->ts, 1, GF_BG_LYR_MAIN_2, 15, 2, 0);
    BgCommitTilemapBufferToVram(exchange->bgConfig, GF_BG_LYR_MAIN_1);
    BgCommitTilemapBufferToVram(exchange->bgConfig, GF_BG_LYR_SUB_1);
    BgCommitTilemapBufferToVram(exchange->bgConfig, GF_BG_LYR_MAIN_2);
    TradeSequenceExchange_SetMonPicPositions(exchange->bgConfig, 80, -384);
    GfGfx_EngineATogglePlanes(GX_PLANEMASK_OBJ, GF_PLANE_TOGGLE_ON);
    GfGfx_EngineBTogglePlanes(GX_PLANEMASK_OBJ, GF_PLANE_TOGGLE_ON);
}

void TradeSequenceExchange_FreeBgs(TradeSequenceExchange *exchange) {
    FreeBgTilemapBuffer(exchange->bgConfig, GF_BG_LYR_MAIN_3);
    FreeBgTilemapBuffer(exchange->bgConfig, GF_BG_LYR_SUB_3);
    FreeBgTilemapBuffer(exchange->bgConfig, GF_BG_LYR_MAIN_2);
    FreeBgTilemapBuffer(exchange->bgConfig, GF_BG_LYR_SUB_2);
    FreeBgTilemapBuffer(exchange->bgConfig, GF_BG_LYR_MAIN_1);
    FreeBgTilemapBuffer(exchange->bgConfig, GF_BG_LYR_SUB_1);
}

// The sent Pokemon is drawn on both screens (its sub-screen copy is 192 pixels further down), the received one on the
// main screen only.
void TradeSequenceExchange_SetMonPicPositions(BgConfig *bgConfig, u32 sentY, u32 receivedY) {
    u32 sent = sentY & 0x1FF;
    u32 received = receivedY & 0x1FF;
    BgSetPosTextAndCommit(bgConfig, GF_BG_LYR_MAIN_2, BG_POS_OP_SET_Y, received);
    BgSetPosTextAndCommit(bgConfig, GF_BG_LYR_MAIN_1, BG_POS_OP_SET_Y, sent);
    sent += 192;
    BgSetPosTextAndCommit(bgConfig, GF_BG_LYR_SUB_1, BG_POS_OP_SET_Y, sent);
}

// Scene 4 of the trade animation (trades and receiving): after a sprite animation the ball model comes out of the
// distance on the bottom screen, spirals toward the viewer, and the screens swap as it leaves the bottom one; then the
// screen fades to white.

TradeSequenceApproach *TradeSequenceApproach_Init(TradeSequence *ts) {
    TradeSequenceApproach *approach = Heap_Alloc(HEAP_ID_57, sizeof(TradeSequenceApproach));
    if (approach != NULL) {
        approach->ts = ts;
        approach->subState = 0;
        approach->bgConfig = TradeSequence_GetBgConfig(ts);
        approach->models = NULL;
        approach->swapScreens = FALSE;
        TradeSequenceApproach_InitFlight(&approach->flight);
        Main_SetVBlankIntrCB(TradeSequenceApproach_VBlankCallback, approach);
    }
    return approach;
}

void TradeSequenceApproach_Exit(TradeSequenceApproach *approach) {
    if (approach != NULL) {
        Main_SetVBlankIntrCB(NULL, NULL);
        TradeSequenceApproach_FreeBgs(approach);
        TradeSequenceApproach_Free3D(approach);
        TradeSequenceApproach_DeleteSprite(approach);
        TradeSequenceApproach_StopFlight(&approach->flight);
        Heap_Free(approach);
    }
}

BOOL TradeSequenceApproach_Main(TradeSequenceApproach *approach, u32 *state) {
    if (*state < TRADE_SEQUENCE_APPROACH_NUM_STATES) {
        if (sTradeSequenceApproachStates[*state](approach, &approach->subState)) {
            *state = *state + 1;
            approach->subState = 0;
        }
        return FALSE;
    }
    return TRUE;
}

BOOL TradeSequenceApproach_SetupScreens(TradeSequenceApproach *approach, u32 *state) {
    TradeSequenceApproach_InitBgs(approach);
    TradeSequenceApproach_Init3D(approach);
    TradeSequenceApproach_CreateSprite(approach);
    BeginNormalPaletteFade(0, 1, 1, RGB_WHITE, 8, 1, HEAP_ID_57);
    return TRUE;
}

BOOL TradeSequenceApproach_Run(TradeSequenceApproach *approach, u32 *state) {
    switch (*state) {
    case 0:
        if (IsPaletteFadeFinished()) {
            Sprite_SetAnimCtrlSeq(approach->sprite, 3);
            Sprite_SetDrawFlag(approach->sprite, TRUE);
            (*state)++;
        }
        break;
    case 1:
        if (!Sprite_IsAnimated(approach->sprite)) {
            PlaySE(SEQ_SE_DP_KOUKAN05);
            TradeSequenceApproach_StartFlight(approach, &approach->flight);
            TradeSequence_SetModelVisible(approach->ball, TRUE);
            (*state)++;
        }
        break;
    case 2:
        if (TradeSequenceApproach_IsFlightDone(&approach->flight)) {
            BeginNormalPaletteFade(0, 0, 0, RGB_WHITE, 16, 1, HEAP_ID_57);
            (*state)++;
        }
        break;
    case 3:
        if (IsPaletteFadeFinished()) {
            return TRUE;
        }
        break;
    }
    return FALSE;
}

void TradeSequenceApproach_InitBgs(TradeSequenceApproach *approach) {
    int scrnFile, charFile, palFile, palOffset;

    GfGfx_SetBanks(&sTradeSequenceApproachGraphicsBanks);
    reg_GX_POWCNT |= REG_GX_POWCNT_DSEL_MASK;
    SetBothScreensModesAndDisable(&sTradeSequenceApproachGraphicsModes);
    InitBgFromTemplate(approach->bgConfig, GF_BG_LYR_MAIN_3, &sTradeSequenceApproachBgTemplate, GF_BG_TYPE_TEXT);
    InitBgFromTemplate(approach->bgConfig, GF_BG_LYR_SUB_3, &sTradeSequenceApproachBgTemplate, GF_BG_TYPE_TEXT);
    switch (TradeSequence_GetTimeOfDay(approach->ts)) {
    default:
    case 0:
        scrnFile = 15;
        charFile = 16;
        palFile = 17;
        palOffset = 0;
        break;
    case 1:
        scrnFile = 15;
        charFile = 16;
        palFile = 17;
        palOffset = 32;
        break;
    case 2:
        scrnFile = 15;
        charFile = 16;
        palFile = 17;
        palOffset = 64;
        break;
    case 3:
        scrnFile = 24;
        charFile = 25;
        palFile = 26;
        palOffset = 0;
        break;
    }
    GfGfxLoader_LoadCharData(NARC_a_0_8_9, charFile, approach->bgConfig, GF_BG_LYR_MAIN_3, 0, 0, TRUE, HEAP_ID_57);
    GfGfxLoader_LoadCharData(NARC_a_0_8_9, charFile, approach->bgConfig, GF_BG_LYR_SUB_3, 0, 0, TRUE, HEAP_ID_57);
    GfGfxLoader_LoadScrnData(NARC_a_0_8_9, scrnFile, approach->bgConfig, GF_BG_LYR_MAIN_3, 0, 0, TRUE, HEAP_ID_57);
    GfGfxLoader_LoadScrnData(NARC_a_0_8_9, scrnFile, approach->bgConfig, GF_BG_LYR_SUB_3, 0, 0, TRUE, HEAP_ID_57);
    GfGfxLoader_GXLoadPalWithSrcOffset(NARC_a_0_8_9, palFile, GF_PAL_LOCATION_MAIN_BG, palOffset, GF_PAL_SLOT_0_OFFSET, 32, HEAP_ID_57);
    GfGfxLoader_GXLoadPalWithSrcOffset(NARC_a_0_8_9, palFile, GF_PAL_LOCATION_SUB_BG, palOffset, GF_PAL_SLOT_0_OFFSET, 32, HEAP_ID_57);
    BgSetPosTextAndCommit(approach->bgConfig, GF_BG_LYR_MAIN_3, BG_POS_OP_SET_Y, 0);
    BgSetPosTextAndCommit(approach->bgConfig, GF_BG_LYR_SUB_3, BG_POS_OP_SET_Y, 256);
    GfGfx_EngineATogglePlanes(GX_PLANEMASK_OBJ, GF_PLANE_TOGGLE_ON);
    GfGfx_EngineBTogglePlanes(GX_PLANEMASK_OBJ, GF_PLANE_TOGGLE_ON);
    WirelessStatusIcon_Create();
    WirelessStatusIcon_ShowOnBothScreens(1, HEAP_ID_56);
}

void TradeSequenceApproach_FreeBgs(TradeSequenceApproach *approach) {
    FreeBgTilemapBuffer(approach->bgConfig, GF_BG_LYR_MAIN_3);
    FreeBgTilemapBuffer(approach->bgConfig, GF_BG_LYR_SUB_3);
}

void TradeSequenceApproach_Init3D(TradeSequenceApproach *approach) {
    CameraAngle angle;
    VecFx32 cameraPos;

    NNS_G3dInit();
    G3X_InitMtxStack();
    G3X_SetShading(GX_SHADING_TOON);
    G3X_AntiAlias(TRUE);
    G3X_AlphaTest(FALSE, 0);
    G3X_AlphaBlend(TRUE);
    G3X_EdgeMarking(TRUE);
    G3X_SetEdgeColorTable(sTradeSequenceApproachEdgeColors);
    G3X_SetClearColor(RGB(31, 31, 16), 0, 0x7FFF, 63, FALSE);
    G3_SwapBuffers(GX_SORTMODE_AUTO, GX_BUFFERMODE_W);
    G3_ViewPort(0, 0, 255, 191);
    GF_3DVramMan_InitFrameTexVramManager(1, TRUE);
    GF_3DVramMan_InitFramePlttVramManager(0x4000, TRUE);
    GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG0, GF_PLANE_TOGGLE_ON);
    G2_SetBG0Priority(0);
    approach->models = TradeSequence_CreateModels(1, 0, 0, 0);
    approach->ball = TradeSequence_LoadModel(approach->models, 0, NARC_a_0_8_9, 27, 0, 0, 0, FALSE);
    angle.x = 0x18E3;
    angle.y = 0;
    angle.z = 0;
    TradeSequence_SetCameraAngle(approach->models, &angle);
    angle.x = 0x1150;
    TradeSequence_AdjustCameraAngle(approach->models, &angle);
    TradeSequence_SetCameraPerspectiveAngle(approach->models, 0x7D2);
    TradeSequence_GetCameraPos(approach->models, &cameraPos);
}

void TradeSequenceApproach_Free3D(TradeSequenceApproach *approach) {
    if (approach->models != NULL) {
        TradeSequence_DestroyModels(approach->models);
    }
}

void TradeSequenceApproach_CreateSprite(TradeSequenceApproach *approach) {
    NNSG2dImagePaletteProxy palette;
    NNSG2dImageProxy image;
    SpriteResourcesHeader header;

    TradeSequence_LoadCellAnim(&approach->cellAnim, NARC_a_0_8_9, 7, 8);
    NNS_G2dInitImagePaletteProxy(&palette);
    NNS_G2dInitImageProxy(&image);
    GfGfxLoader_LoadImageMapping(NARC_a_0_8_9, 9, TRUE, GF_BG_LYR_MAIN_0, 0, NNS_G2D_VRAM_TYPE_2DMAIN, 0, HEAP_ID_57, &image);
    GfGfxLoader_PartiallyLoadPalette(NARC_a_0_8_9, 10, NNS_G2D_VRAM_TYPE_2DMAIN, 0, HEAP_ID_57, &palette);
    TradeSequence_InitSpriteHeader(&header, &approach->cellAnim, &image, &palette, 1);
    approach->sprite = TradeSequence_CreateSprite(approach->ts, &header, 128, 96, 0, NNS_G2D_VRAM_TYPE_2DMAIN);
    Sprite_SetDrawFlag(approach->sprite, FALSE);
}

void TradeSequenceApproach_DeleteSprite(TradeSequenceApproach *approach) {
    if (approach->sprite != NULL) {
        Sprite_Delete(approach->sprite);
    }
    TradeSequence_FreeCellAnim(&approach->cellAnim);
}

void TradeSequenceApproach_InitFlight(TradeSequenceApproachFlight *flight) {
    flight->task = NULL;
}

void TradeSequenceApproach_StopFlight(TradeSequenceApproachFlight *flight) {
    if (flight->task != NULL) {
        SysTask_Destroy(flight->task);
        flight->task = NULL;
    }
}

void TradeSequenceApproach_StartFlight(TradeSequenceApproach *approach, TradeSequenceApproachFlight *flight) {
    flight->approach = approach;
    flight->ball = approach->ball;
    flight->models = approach->models;
    flight->center.x = 0;
    flight->center.y = 0x3D6C00;
    flight->center.z = -0x1BC000;
    flight->angle = -10128;
    flight->angleSpeed = -2736;
    flight->centerStepY = -0x10D3A;
    flight->centerStepZ = 0x936D;
    flight->state = 0;
    flight->timer = 0;
    flight->frames = 0;
    flight->rotation.x = 0x2000;
    flight->rotation.y = -0x2000;
    flight->rotation.z = 0;
    TradeSequence_SetModelRotation(flight->ball, &flight->rotation);
    flight->scale = 0x200;
    flight->radiusX = 0x800;
    flight->radiusY = 0x800;
    flight->stepFrames = 0;
    TradeSequenceApproach_SetFlightTargets(flight, 74 << 10, 99 << 10, FX32_ONE, 12);
    TradeSequenceApproach_UpdateFlight(flight);
    TradeSequence_SetModelPosition(flight->ball, &flight->position);
    TradeSequence_GetModelRotation(flight->ball, &flight->rotation);
    flight->task = SysTask_CreateOnMainQueue(TradeSequenceApproach_FlightTask, flight, 0);
}

BOOL TradeSequenceApproach_IsFlightDone(TradeSequenceApproachFlight *flight) {
    if (flight->task == NULL) {
        return TRUE;
    }
    return FALSE;
}

void TradeSequenceApproach_SetFlightTargets(TradeSequenceApproachFlight *flight, fx32 radiusX, fx32 radiusY, fx32 scale, int frames) {
    volatile int divisor = frames;

    if (frames != 0) {
        flight->radiusXStep = (radiusX - flight->radiusX) / divisor;
        flight->radiusYStep = (radiusY - flight->radiusY) / divisor;
        flight->scaleStep = (scale - flight->scale) / divisor;
        flight->radiusXTarget = radiusX;
        flight->radiusYTarget = radiusY;
        flight->scaleTarget = scale;
        flight->stepFrames = frames;
    }
}

void TradeSequenceApproach_UpdateFlight(TradeSequenceApproachFlight *flight) {
    s32 angle;

    if (sTradeSequenceBallSpirals[0] != 0) {
        fx32 radius;
        s64 product;

        angle = (u16)flight->angle / 182; // to degrees
        if (angle >= 360) {
            angle -= 360;
        }
        flight->angle -= flight->angleSpeed;

        if (flight->stepFrames != 0) {
            flight->stepFrames--;
            if (flight->stepFrames != 0) {
                flight->radiusX += flight->radiusXStep;
                flight->radiusY += flight->radiusYStep;
                flight->scale += flight->scaleStep;
            } else {
                flight->radiusX = flight->radiusXTarget;
                flight->radiusY = flight->radiusYTarget;
                flight->scale = flight->scaleTarget;
            }
            TradeSequence_SetModelScale(flight->ball, flight->scale);
        }

        radius = flight->radiusX;
        product = GF_CosDegNoWrap((u16)angle);
        product *= radius;
        flight->offset.x = (product + 0x800) >> FX32_SHIFT;
        radius = flight->radiusY;
        product = GF_SinDegNoWrap((u16)angle);
        product *= radius;
        flight->offset.y = (product + 0x800) >> FX32_SHIFT;
        flight->offset.z = 0;
        flight->rotation.x -= 0x300;
        TradeSequence_SetModelRotation(flight->ball, &flight->rotation);
        VEC_Add(&flight->offset, &flight->center, &flight->position);
    } else {
        flight->position = flight->center;
    }
}

// Lets the ball spiral in place for 12 frames, then moves the center until the ball leaves the bottom screen (screen
// swap) and, 70 frames after the start, hides it.
void TradeSequenceApproach_FlightTask(SysTask *task, void *data) {
    TradeSequenceApproachFlight *flight = data;

    if (flight->frames < 70) {
        flight->frames++;
    }

    switch (flight->state) {
    case 0:
        TradeSequenceApproach_UpdateFlight(flight);
        TradeSequence_SetModelPosition(flight->ball, &flight->position);
        flight->frames++;
        if (flight->frames >= 12) {
            flight->frames = 0;
            flight->state++;
        }
        break;
    case 1:
        if (!TradeSequence_IsModelInView(flight->ball)) {
            TradeSequenceApproach_RequestScreenSwap(flight->approach);
            flight->state++;
        } else {
            flight->center.y += flight->centerStepY;
            flight->center.z += flight->centerStepZ;
            TradeSequenceApproach_UpdateFlight(flight);
            TradeSequence_SetModelPosition(flight->ball, &flight->position);
        }
        break;
    case 2:
        flight->center.y += flight->centerStepY;
        flight->center.z += flight->centerStepZ;
        TradeSequenceApproach_UpdateFlight(flight);
        TradeSequence_SetModelPosition(flight->ball, &flight->position);
        if (flight->frames >= 70) {
            TradeSequence_SetModelVisible(flight->ball, FALSE);
            flight->timer = 0;
            flight->state++;
        }
        break;
    case 3:
        flight->timer++;
        if (flight->timer > 8) {
            flight->task = NULL;
            SysTask_Destroy(task);
        }
        break;
    }
}

void TradeSequenceApproach_RequestScreenSwap(TradeSequenceApproach *approach) {
    approach->swapScreens = TRUE;
}

// Draws the models; once requested, also moves the layer 3 backgrounds, tilts the camera back and swaps the screens.
void TradeSequenceApproach_VBlankCallback(void *data) {
    TradeSequenceApproach *approach = data;
    CameraAngle delta;

    if (approach->models) {
        G3X_Reset();
        TradeSequence_DrawModels(approach->models);
        RequestSwap3DBuffers(GX_SORTMODE_AUTO, GX_BUFFERMODE_Z);
    }
    if (approach->swapScreens) {
        delta.x = 0xEEB0;
        delta.y = 0;
        delta.z = 0;
        BgSetPosTextAndCommit(approach->bgConfig, GF_BG_LYR_MAIN_3, BG_POS_OP_SET_Y, 256);
        BgSetPosTextAndCommit(approach->bgConfig, GF_BG_LYR_SUB_3, BG_POS_OP_SET_Y, 0);
        TradeSequence_AdjustCameraAngle(approach->models, &delta);
        reg_GX_POWCNT &= ~REG_GX_POWCNT_DSEL_MASK;
        approach->swapScreens = FALSE;
    }
}

// Scene 5 of the trade animation (trades and receiving): the ball model, spinning, comes down onto the top screen
// while the backgrounds scroll into place, fades out over reversed pulses, then the background zooms out and the
// screen fades to black.

TradeSequenceArrive *TradeSequenceArrive_Init(TradeSequence *ts) {
    TradeSequenceArrive *arrive = Heap_Alloc(HEAP_ID_57, sizeof(TradeSequenceArrive));
    if (arrive != NULL) {
        arrive->ts = ts;
        arrive->subState = 0;
        arrive->bgConfig = TradeSequence_GetBgConfig(ts);
        arrive->models = NULL;
        arrive->pulses = NULL;
        arrive->pulsesRunning = FALSE;
        arrive->bgZoom = NULL;
        arrive->lowerTask = NULL;
        arrive->scrollTask = NULL;
        arrive->alphaFadeTask = NULL;
    }
    return arrive;
}

void TradeSequenceArrive_Exit(TradeSequenceArrive *arrive) {
    if (arrive != NULL) {
        TradeSequenceArrive_FreeBgs(arrive);
        TradeSequenceArrive_Free3D(arrive);
        if (arrive->lowerTask != NULL) {
            TradeSequenceArrive_StopLower(arrive->lowerTask);
        }
        if (arrive->scrollTask != NULL) {
            TradeSequenceArrive_StopScroll(arrive->scrollTask);
        }
        if (arrive->alphaFadeTask != NULL) {
            TradeSequenceArrive_StopAlphaFade(arrive->alphaFadeTask);
        }
        Heap_Free(arrive);
    }
}

BOOL TradeSequenceArrive_Main(TradeSequenceArrive *arrive, u32 *state) {
    if (*state < TRADE_SEQUENCE_ARRIVE_NUM_STATES) {
        if (sTradeSequenceArriveStates[*state](arrive, &arrive->subState)) {
            *state = *state + 1;
            arrive->subState = 0;
        }
        TradeSequenceArrive_Draw(arrive);
        return FALSE;
    }
    return TRUE;
}

// Spins the ball and draws the models.
void TradeSequenceArrive_Draw(TradeSequenceArrive *arrive) {
    if (arrive->models != NULL) {
        arrive->rotation.y += 0x1000;
        TradeSequence_SetModelRotation(arrive->ball, &arrive->rotation);
        G3X_Reset();
        TradeSequence_DrawModels(arrive->models);
        RequestSwap3DBuffers(GX_SORTMODE_AUTO, GX_BUFFERMODE_Z);
    }
}

BOOL TradeSequenceArrive_SetupScreens(TradeSequenceArrive *arrive, u32 *state) {
    TradeSequenceArrive_InitBgs(arrive);
    TradeSequenceArrive_Init3D(arrive);
    BeginNormalPaletteFade(0, 1, 1, RGB_WHITE, 8, 1, HEAP_ID_57);
    return TRUE;
}

BOOL TradeSequenceArrive_LowerAndFade(TradeSequenceArrive *arrive, u32 *state) {
    switch (*state) {
    case 0:
        if (IsPaletteFadeFinished()) {
            arrive->timer = 0;
            TradeSequenceArrive_StartLower(arrive, &arrive->lowerTask);
            (*state)++;
        }
        break;
    case 1:
        arrive->timer++;
        if (arrive->timer > 8) {
            TradeSequenceArrive_StartScroll(arrive, &arrive->scrollTask);
            (*state)++;
        }
        break;
    case 2:
        if (arrive->scrollTask == NULL && arrive->lowerTask == NULL) {
            (*state)++;
        }
        break;
    case 3:
        arrive->pulses = TradeSequence_CreatePulses(&arrive->pulsesRunning, -1);
        arrive->timer = 0;
        (*state)++;
        break;
    case 4:
        arrive->timer++;
        if (arrive->timer > 30) {
            TradeSequenceArrive_StartAlphaFade(arrive->ball, 31, 0, 40, &arrive->alphaFadeTask);
            (*state)++;
        }
        break;
    case 5:
        if (arrive->alphaFadeTask == NULL) {
            TradeSequence_StopPulses(arrive->pulses);
            (*state)++;
        }
        break;
    case 6:
        if (!arrive->pulsesRunning) {
            return TRUE;
        }
        break;
    }
    return FALSE;
}

BOOL TradeSequenceArrive_ZoomOut(TradeSequenceArrive *arrive, u32 *state) {
    switch (*state) {
    case 0:
        arrive->bgZoom = TradeSequence_StartBgZoom(arrive->bgConfig, FX32_ONE, 0x400, 1, 0x128, 1, &arrive->bgZoomFinished);
        arrive->timer = 0;
        (*state)++;
        break;
    case 1:
        arrive->timer++;
        if (arrive->timer > 13) {
            BeginNormalPaletteFade(0, 0, 0, RGB_BLACK, 8, 1, HEAP_ID_57);
            (*state)++;
        }
        break;
    case 2:
        if (IsPaletteFadeFinished() && arrive->bgZoomFinished) {
            return TRUE;
        }
        break;
    default:
        break;
    }
    return FALSE;
}

void TradeSequenceArrive_InitBgs(TradeSequenceArrive *arrive) {
    OSIntrMode intrMode;
    void *buf;
    u32 timeOfDay;
    u16 srcOffset;

    GfGfx_SetBanks(&sTradeSequenceArriveGraphicsBanks);
    reg_GX_POWCNT |= REG_GX_POWCNT_DSEL_MASK;
    SetBothScreensModesAndDisable(&sTradeSequenceArriveGraphicsModes);
    InitBgFromTemplate(arrive->bgConfig, GF_BG_LYR_MAIN_2, &sTradeSequenceArriveBgTemplate_Layer2, GF_BG_TYPE_AFFINE);
    InitBgFromTemplate(arrive->bgConfig, GF_BG_LYR_SUB_2, &sTradeSequenceArriveBgTemplate_Layer2, GF_BG_TYPE_AFFINE);
    intrMode = OS_DisableInterrupts();
    InitBgFromTemplate(arrive->bgConfig, GF_BG_LYR_MAIN_3, &sTradeSequenceArriveBgTemplate_Layer3, GF_BG_TYPE_256x16PLTT);
    InitBgFromTemplate(arrive->bgConfig, GF_BG_LYR_SUB_3, &sTradeSequenceArriveBgTemplate_Layer3, GF_BG_TYPE_256x16PLTT);
    OS_RestoreInterrupts(intrMode);
    GfGfxLoader_LoadCharData(NARC_a_0_8_9, 2, arrive->bgConfig, GF_BG_LYR_MAIN_2, 0, 0, TRUE, HEAP_ID_57);
    GfGfxLoader_LoadCharData(NARC_a_0_8_9, 2, arrive->bgConfig, GF_BG_LYR_SUB_2, 0, 0, TRUE, HEAP_ID_57);
    GfGfxLoader_LoadScrnData(NARC_a_0_8_9, 1, arrive->bgConfig, GF_BG_LYR_MAIN_2, 0, 0, TRUE, HEAP_ID_57);
    GfGfxLoader_LoadScrnData(NARC_a_0_8_9, 1, arrive->bgConfig, GF_BG_LYR_SUB_2, 0, 0, TRUE, HEAP_ID_57);
    timeOfDay = TradeSequence_GetTimeOfDay(arrive->ts);
    GfGfxLoader_GXLoadPal(NARC_a_0_8_9, 3, GF_PAL_LOCATION_MAIN_BG, GF_PAL_SLOT_0_OFFSET, 0x20, HEAP_ID_57);
    GfGfxLoader_GXLoadPal(NARC_a_0_8_9, 3, GF_PAL_LOCATION_SUB_BG, GF_PAL_SLOT_0_OFFSET, 0x20, HEAP_ID_57);
    srcOffset = sTradeSequenceArrivePaletteOffsets[timeOfDay];
    GfGfxLoader_GXLoadPalWithSrcOffset(NARC_a_0_8_9, 3, GF_PAL_LOCATION_MAIN_BG, srcOffset, GF_PAL_SLOT_1_OFFSET, 0x40, HEAP_ID_57);
    GfGfxLoader_GXLoadPalWithSrcOffset(NARC_a_0_8_9, 3, GF_PAL_LOCATION_SUB_BG, srcOffset, GF_PAL_SLOT_1_OFFSET, 0x40, HEAP_ID_57);
    GfGfxLoader_LoadCharData(NARC_a_0_8_9, 5, arrive->bgConfig, GF_BG_LYR_MAIN_3, 0, 0, TRUE, HEAP_ID_57);
    GfGfxLoader_LoadCharData(NARC_a_0_8_9, 5, arrive->bgConfig, GF_BG_LYR_SUB_3, 0, 0, TRUE, HEAP_ID_57);
    GfGfxLoader_LoadScrnData(NARC_a_0_8_9, 4, arrive->bgConfig, GF_BG_LYR_MAIN_3, 0, 0, TRUE, HEAP_ID_57);
    GfGfxLoader_LoadScrnData(NARC_a_0_8_9, 4, arrive->bgConfig, GF_BG_LYR_SUB_3, 0, 0, TRUE, HEAP_ID_57);
    buf = Heap_Alloc(HEAP_ID_57, 0x60);
    if (buf != NULL) {
        MI_CpuClear32(buf, 0x60);
        DC_FlushRange(buf, 0x60);
        GX_BeginLoadBGExtPltt();
        GXS_BeginLoadBGExtPltt();
        GX_LoadBGExtPltt(buf, 0x6000, 0x60);
        GXS_LoadBGExtPltt(buf, 0x6000, 0x60);
        GX_EndLoadBGExtPltt();
        GXS_EndLoadBGExtPltt();
        Heap_Free(buf);
    }
    BgSetPosTextAndCommit(arrive->bgConfig, GF_BG_LYR_MAIN_2, BG_POS_OP_SET_Y, -384);
    BgSetPosTextAndCommit(arrive->bgConfig, GF_BG_LYR_SUB_2, BG_POS_OP_SET_Y, -192);
    BgSetPosTextAndCommit(arrive->bgConfig, GF_BG_LYR_MAIN_3, BG_POS_OP_SET_Y, 67);
    ToggleBgLayer(GF_BG_LYR_SUB_3, GF_PLANE_TOGGLE_OFF);
    GfGfx_EngineATogglePlanes(GX_PLANEMASK_OBJ, GF_PLANE_TOGGLE_ON);
    GfGfx_EngineBTogglePlanes(GX_PLANEMASK_OBJ, GF_PLANE_TOGGLE_ON);
}

void TradeSequenceArrive_FreeBgs(TradeSequenceArrive *arrive) {
    if (arrive->pulsesRunning) {
        TradeSequence_DestroyPulses(arrive->pulses);
    }
    FreeBgTilemapBuffer(arrive->bgConfig, GF_BG_LYR_MAIN_2);
    FreeBgTilemapBuffer(arrive->bgConfig, GF_BG_LYR_SUB_2);
    FreeBgTilemapBuffer(arrive->bgConfig, GF_BG_LYR_MAIN_3);
    FreeBgTilemapBuffer(arrive->bgConfig, GF_BG_LYR_SUB_3);
}
