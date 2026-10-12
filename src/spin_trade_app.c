#include "global.h"

#include "constants/game_stats.h"
#include "constants/sndseq.h"

#include "msgdata/msg.naix"
#include "msgdata/msg/msg_0206.h"

#include "bag.h"
#include "bg_window.h"
#include "camera.h"
#include "field_bgm.h"
#include "filesystem.h"
#include "font.h"
#include "spin_trade_link.h"
#include "game_stats.h"
#include "gf_3d_vramman.h"
#include "gf_gfx_planes.h"
#include "heap.h"
#include "math_util.h"
#include "message_format.h"
#include "msgdata.h"
#include "field/field_effect_manager.h"
#include "overlay_manager.h"
#include "palette.h"
#include "party.h"
#include "player_data.h"
#include "pokemon.h"
#include "render_window.h"
#include "sound_system.h"
#include "sprite_system.h"
#include "sys_task_api.h"
#include "system.h"
#include "text.h"
#include "trainer_memo.h"
#include "sound_playback.h"
#include "unk_02009D48.h"
#include "unk_0200FA24.h"
#include "touchpad.h"
#include "swap_3d_buffers.h"
#include "communication_player_state.h"
#include "unk_02035900.h"
#include "link_synchronization.h"
#include "comm_state.h"
#include "wireless_status_icon.h"
#include "render_model.h"
#include "render_animation.h"
#include "vram_transfer_manager.h"

// Main loop states (index into ov85_021EA800)
#define SPIN_TRADE_STATE_START               0
#define SPIN_TRADE_STATE_WAIT_FADE_IN        1
#define SPIN_TRADE_STATE_WAIT_MEMBERS        2
#define SPIN_TRADE_STATE_SEND_START          3
#define SPIN_TRADE_STATE_WAIT_CONNECTIONS    4
#define SPIN_TRADE_STATE_SEND_PLAYER_COUNT   5
#define SPIN_TRADE_STATE_SEND_SLOTS          6
#define SPIN_TRADE_STATE_SEND_READY          7
#define SPIN_TRADE_STATE_SEND_JOIN           8
#define SPIN_TRADE_STATE_WAIT_READY          9
#define SPIN_TRADE_STATE_START_SYNC          10
#define SPIN_TRADE_STATE_WAIT_SYNC           11
#define SPIN_TRADE_STATE_SEND_TRADE_SLOT     12
#define SPIN_TRADE_STATE_SEND_PARTY          13
#define SPIN_TRADE_STATE_CHECK_EGGS          14
#define SPIN_TRADE_STATE_WAIT_CONFIRM        15
#define SPIN_TRADE_STATE_SETUP_DROP          16
#define SPIN_TRADE_STATE_DROP_EGGS           17
#define SPIN_TRADE_STATE_WAIT_EGGS_LANDED    18
#define SPIN_TRADE_STATE_WAIT_BEFORE_SPIN    19
#define SPIN_TRADE_STATE_SEND_SPIN_START     20
#define SPIN_TRADE_STATE_WAIT_SPIN_START     21
#define SPIN_TRADE_STATE_START_SPIN_SYNC     22
#define SPIN_TRADE_STATE_WAIT_SPIN_SYNC      23
#define SPIN_TRADE_STATE_START_COUNTDOWN     24
#define SPIN_TRADE_STATE_CLEAR_NAMES         25
#define SPIN_TRADE_STATE_BEGIN_SPIN          26
#define SPIN_TRADE_STATE_SPIN_HOST           27
#define SPIN_TRADE_STATE_SPIN_CLIENT         28
#define SPIN_TRADE_STATE_STOP_SPIN           29
#define SPIN_TRADE_STATE_ROTATE_TO_OWN_EGG   30
#define SPIN_TRADE_STATE_ALIGN_TO_SLOT       31
#define SPIN_TRADE_STATE_START_RESULT_SYNC   32
#define SPIN_TRADE_STATE_WAIT_RESULT_SYNC    33
#define SPIN_TRADE_STATE_SEND_RESULT         34
#define SPIN_TRADE_STATE_RECEIVE_RESULT      35
#define SPIN_TRADE_STATE_SHOW_NAMES          36
#define SPIN_TRADE_STATE_SHOW_RECEIVED_EGG   37
#define SPIN_TRADE_STATE_WAIT_EGG_FANFARE    38
#define SPIN_TRADE_STATE_GIVE_BONUS_BERRY    39
#define SPIN_TRADE_STATE_WAIT_BERRY_FANFARE  40
#define SPIN_TRADE_STATE_SHOW_BAG_FULL       41
#define SPIN_TRADE_STATE_WAIT                42
#define SPIN_TRADE_STATE_START_SAVE          43
#define SPIN_TRADE_STATE_WAIT_SAVE_SYNC      44
#define SPIN_TRADE_STATE_SAVE                45
#define SPIN_TRADE_STATE_CANCEL              46
#define SPIN_TRADE_STATE_WAIT_CANCEL_MESSAGE 47
#define SPIN_TRADE_STATE_START_EXIT_SYNC     48
#define SPIN_TRADE_STATE_WAIT_EXIT_SYNC      49
#define SPIN_TRADE_STATE_WAIT_DISCONNECT     50
#define SPIN_TRADE_STATE_FADE_OUT            51
#define SPIN_TRADE_STATE_WAIT_FADE_OUT       52
#define SPIN_TRADE_STATE_EXIT                53

// Link commands sent through SpinTradeLink_SendCommand
#define SPIN_TRADE_CMD_STATUS     8
#define SPIN_TRADE_CMD_JOIN       9
#define SPIN_TRADE_CMD_JUMP       10
#define SPIN_TRADE_CMD_SPIN       11
#define SPIN_TRADE_CMD_SLOT       12
#define SPIN_TRADE_CMD_COUNT      13
#define SPIN_TRADE_CMD_TRADE_SLOT 14
#define SPIN_TRADE_CMD_RESULT     15
#define SPIN_TRADE_CMD_CANCEL     16
#define SPIN_TRADE_CMD_CONFIRM    17

#define SPIN_TRADE_LINK_SYNC_NO 0xCA

#define SPIN_TRADE_MAX_PLAYERS 5
#define SPIN_TRADE_ANGLE_FULL  FX32_CONST(360)

// View of the link menu owned by frontier_menu_communication.c (SpinTradeArgs.menu)
typedef struct SpinTradeLinkMenu {
    u8 unk_00[0x3C];
    u32 unk_3C;            // 0x3C
    u16 receivedStatus;    // 0x40, SPIN_TRADE_CMD_STATUS bits received from the host
    u16 memberMask;        // 0x42, netIds of the joined members
    u16 unk_44;            // 0x44
    s16 partyReceivedMask; // 0x46
    u16 cancelled;         // 0x48
    u16 confirmMask;       // 0x4A
} SpinTradeLinkMenu;

// 0x24-byte animation object handled by render_animation.c (RenderAnimation_LoadFromNarc ...)
typedef struct SpinTradeRenderAnim {
    u8 unk_00[0x24];
} SpinTradeRenderAnim;

// Egg sitting on the turntable
typedef struct SpinTradeEgg {
    BOOL active;               // 0x00
    BOOL visible;              // 0x04
    BOOL dropped;              // 0x08
    int netId;                 // 0x0C, owner
    u32 slot;                  // 0x10
    u32 order;                 // 0x14
    u32 unk_18;                // 0x18
    fx32 angle;                // 0x1C
    fx32 angleOffset;          // 0x20
    u16 rotation[3];           // 0x24
    u16 unk_2A;                // 0x2A
    fx32 rotationFx[3];        // 0x2C
    VecFx32 scale;             // 0x38
    VecFx32 position;          // 0x44
    VecFx32 offset;            // 0x50, height above the table
    NNSG3dRenderObj renderObj; // 0x5C
} SpinTradeEgg;                // size 0xB0

// Shadow under an egg, shrinks while the egg is in the air
typedef struct SpinTradeShadow {
    BOOL active;               // 0x00
    u16 rotation[3];           // 0x04
    u16 unk_0A;                // 0x0A
    VecFx32 scale;             // 0x0C
    VecFx32 position;          // 0x18
    NNSG3dRenderObj renderObj; // 0x24
    SpinTradeEgg *egg;         // 0x78
} SpinTradeShadow;             // size 0x7C

// Animated marker shown above an egg
typedef struct SpinTradeMarker {
    BOOL active;               // 0x00
    VecFx32 position;          // 0x04
    NNSG3dRenderObj renderObj; // 0x10
    SpinTradeEgg *egg;         // 0x64
} SpinTradeMarker;             // size 0x68

typedef struct SpinTradeObjects {
    UnkOv01_021FFECC_sub eggModel;             // 0x000
    UnkOv01_021FFECC_sub shadowModel;          // 0x014
    UnkOv01_021FFECC_sub markerModels[5];      // 0x028
    SpinTradeRenderAnim markerAnims[5];        // 0x08C
    SpinTradeEgg eggs[SPIN_TRADE_MAX_PLAYERS]; // 0x140
    SpinTradeShadow shadows[5];                // 0x4B0
    SpinTradeMarker markers[5];                // 0x71C
} SpinTradeObjects;                            // size 0x924

// The turntable machine model
typedef struct SpinTradeMachine {
    VecFx32 position;           // 0x00
    VecFx32 drawPosition;       // 0x0C
    VecFx32 scale;              // 0x18
    VecFx32 shakeOffset;        // 0x24
    VecFx32 jumpOffset;         // 0x30
    fx32 angle;                 // 0x3C
    fx32 baseAngle;             // 0x40
    fx32 angleOffset;           // 0x44
    fx32 speed;                 // 0x48
    u16 rotation[3];            // 0x4C
    u16 unk_52;                 // 0x52
    UnkOv01_021FFECC_sub model; // 0x54
    NNSG3dRenderObj renderObj;  // 0x68
} SpinTradeMachine;             // size 0xBC

typedef struct SpinTradeBounce {
    BOOL active;       // 0x00
    int state;         // 0x04
    int frame;         // 0x08
    BOOL bounced;      // 0x0C
    fx32 velocity;     // 0x10
    fx32 accel;        // 0x14
    u32 unk_18;        // 0x18
    SpinTradeEgg *egg; // 0x1C
} SpinTradeBounce;

typedef struct SpinTradeSpin {
    BOOL active;       // 0x00
    int state;         // 0x04
    u32 unk_08;        // 0x08
    fx32 step;         // 0x0C
    SpinTradeEgg *egg; // 0x10
} SpinTradeSpin;

typedef struct SpinTradeWobble {
    BOOL active;       // 0x00
    int state;         // 0x04
    fx32 amplitude;    // 0x08
    fx32 speed;        // 0x0C
    fx32 phase;        // 0x10
    SpinTradeEgg *egg; // 0x14
} SpinTradeWobble;

// The machine jumps when the button is touched; landing makes all eggs bounce
typedef struct SpinTradeJump {
    int state;   // 0x00
    BOOL landed; // 0x04
    BOOL active; // 0x08
    int counter; // 0x0C
    fx32 shake;  // 0x10
} SpinTradeJump;

typedef struct SpinTradeButtonFlash {
    u16 state;  // 0x00
    u16 active; // 0x02
    s32 timer;  // 0x04
} SpinTradeButtonFlash;

typedef struct SpinTradeHover {
    int state;         // 0x00
    int index;         // 0x04
    int timer;         // 0x08
    SpinTradeEgg *egg; // 0x0C
} SpinTradeHover;

typedef struct SpinTradeText {
    int nameWindowCount;      // 0x00
    MsgData *msgData;         // 0x04
    MessageFormat *msgFormat; // 0x08
    Window msgWindow;         // 0x0C
    Window nameWindows[5];    // 0x1C
    String *string;           // 0x6C
} SpinTradeText;

typedef struct SpinTradeCamera {
    fx32 distance;     // 0x00
    int perspective;   // 0x04
    VecFx32 target;    // 0x08
    CameraAngle angle; // 0x14
    Camera *camera;    // 0x1C
} SpinTradeCamera;

typedef struct SpinTradeTouchSamples {
    u16 count;
    TPData samples[8];
} SpinTradeTouchSamples;

typedef struct SpinTradeTouch {
    s32 x;                         // 0x00
    s32 y;                         // 0x04
    BOOL pressed;                  // 0x08
    u32 held;                      // 0x0C
    SpinTradeTouchSamples samples; // 0x10
} SpinTradeTouch;

// Order in which the eggs are dropped onto the table (kept in the scratch buffer)
typedef struct SpinTradeDrop {
    int count;     // 0x00
    u32 slotAngle; // 0x04
    u8 unk_08[8];  // 0x08
    int index;     // 0x10
    int total;     // 0x14
} SpinTradeDrop;

typedef struct SpinTradeMember {
    u16 slot;
    u16 netId;
} SpinTradeMember;

typedef struct SpinTradeSpinData {
    u16 unk_0;
    u16 frame;
    s16 speed;
    u16 angle;
} SpinTradeSpinData;

typedef struct SpinTradeApp {
    int state;                                       // 0x000
    s32 saveState;                                   // 0x004
    int frame;                                       // 0x008, spin duration
    int timer;                                       // 0x00C
    u32 unk_010;                                     // 0x010
    int sendCount;                                   // 0x014
    BOOL unk_018;                                    // 0x018
    int bgm;                                         // 0x01C
    u32 bonusMask;                                   // 0x020, owners of the eggs in a bonus area
    Party *party;                                    // 0x024
    SpinTradeEgg *landedEgg;                         // 0x028
    int mySlot;                                      // 0x02C
    int playerCount;                                 // 0x030
    BOOL remoteJump;                                 // 0x034
    BOOL sendJump;                                   // 0x038
    BOOL spinReceived;                               // 0x03C
    BOOL resultReceived;                             // 0x040
    u32 tradeSlots[SPIN_TRADE_MAX_PLAYERS];          // 0x044, party slot offered by each netId
    SpinTradeSpinData spinData;                      // 0x058
    u16 result[8];                                   // 0x060
    SpinTradeMember members[SPIN_TRADE_MAX_PLAYERS]; // 0x070
    PlayerProfile *profiles[SPIN_TRADE_MAX_PLAYERS]; // 0x084
    String *names[SPIN_TRADE_MAX_PLAYERS];           // 0x098
    u8 scratch[0x20];                                // 0x0AC
    SpinTradeArgs *args;                                 // 0x0CC
    SpinTradeLinkMenu *menu;                         // 0x0D0
    SpinTradeMachine machine;                        // 0x0D4
    SpinTradeObjects objects;                        // 0x190
    SpinTradeBounce bounces[5];                      // 0xAB4
    SpinTradeSpin spins[5];                          // 0xB54
    SpinTradeWobble wobbles[5];                      // 0xBB8
    SpinTradeJump jump;                              // 0xC30
    SpinTradeButtonFlash buttonFlash;                // 0xC44
    SpinTradeHover hovers[5];                        // 0xC4C
    SpinTradeText text;                              // 0xC9C
    SpinTradeCamera camera;                          // 0xD0C
    SpinTradeTouch touch;                            // 0xD2C
    NARC *narc;                                      // 0xD80
    BgConfig *bgConfig;                              // 0xD84
    NNSG2dScreenData *screenData;                    // 0xD88
    NNSG2dCharacterData *charData;                   // 0xD8C
    NNSG2dPaletteData *plttData;                     // 0xD90
    SpriteSystem *spriteSystem;                      // 0xD94
    SpriteManager *spriteManager;                    // 0xD98
    PaletteData *paletteData;                        // 0xD9C
    GXRgb edgeColors[8];                             // 0xDA0
    SysTask *bounceTask;                             // 0xDB0
    SysTask *spinTask;                               // 0xDB4
    SysTask *wobbleTask;                             // 0xDB8
    SysTask *jumpTask;                               // 0xDBC
    SysTask *buttonFlashTask;                        // 0xDC0
    SysTask *hoverTask;                              // 0xDC4
    WaitingIcon *waitingIcon;                        // 0xDC8
} SpinTradeApp;                                      // size 0xDCC

typedef struct SpinTradeEggDropTask {
    int state;            // 0x00
    u32 unk_04;           // 0x04
    BOOL landingSePlayed; // 0x08
    fx32 speed;           // 0x0C
    SpinTradeEgg *egg;    // 0x10
    SpinTradeApp *app;    // 0x14
} SpinTradeEggDropTask;

typedef struct SpinTradeCountdownTask {
    int state;             // 0x00
    int timer;             // 0x04
    u32 unk_08;            // 0x08
    int soundIndex;        // 0x0C
    SpinTradeApp *app;     // 0x10
    ManagedSprite *sprite; // 0x14
} SpinTradeCountdownTask;

typedef struct SpinTradeCountdownSounds {
    u32 sound[4];
} SpinTradeCountdownSounds;

typedef struct SpinTradeFadeTask {
    SpinTradeApp *app; // 0x00
    int fadeOut;       // 0x04
    int level;         // 0x08
} SpinTradeFadeTask;

typedef struct SpinTradeAngleRange {
    u16 low;
    u16 high;
} SpinTradeAngleRange;

typedef struct SpinTradeBonusAreas {
    u32 count;
    const SpinTradeAngleRange *ranges;
} SpinTradeBonusAreas;

extern const s32 ov85_021EA4F0[];
extern const WindowTemplate ov85_021EA4FC;
extern int (*const ov85_021EA51C[])(SpinTradeEggDropTask *task);
extern const SpinTradeCountdownSounds ov85_021EA528;
extern const GraphicsModes ov85_021EA538;
extern const int ov85_021EA558[];
extern const OamCharTransferParam ov85_021EA56C;
extern const int ov85_021EA580[];
extern const SpriteResourceCountsListUnion ov85_021EA594;
extern const WindowTemplate *const ov85_021EA5AC[];
extern const int ov85_021EA5C4[];
extern const s32 ov85_021EA5DC[];
extern const BgTemplate ov85_021EA60C;
extern const BgTemplate ov85_021EA628;
extern const BgTemplate ov85_021EA644;
extern const BgTemplate ov85_021EA660;
extern const BgTemplate ov85_021EA67C;
extern const OamManagerParam ov85_021EA6B8;
extern const GraphicsBanks ov85_021EA700;
extern u32 ov85_021EA728[][2];
extern u32 ov85_021EA72C[][2];
extern const SpinTradeBonusAreas ov85_021EA758[];
extern const u16 ov85_021EA788[][5];
extern const u16 ov85_021EA7C4[][5];
extern int (*const ov85_021EA800[])(SpinTradeApp *app);
extern const int _021EAA80[];

// Not declared by the owners' headers
void sub_0201A728(int a0);
void sub_0201A738(int a0);
int GetUserFrameGfx2PaletteMember(int frame);
BOOL sub_02036FD8(int command, void *data, int size);
void CommState_SetMaxEntries(int count);
void Comm_InitSyncedSave(s32 *state);
BOOL Comm_SyncedSave(SaveData *saveData, int a1, s32 *state);
int GF_TouchpadUpdate(SpinTradeTouchSamples *out, int mode, u32 threshold);
void *WirelessStatusIcon_LoadPaletteFile(enum HeapID heapId);
BOOL SpinTradeLink_SendCommand(SpinTradeLinkMenu *menu, int command, void *data, int size);
Party *SpinTradeLink_GetReceivedParty(SpinTradeLinkMenu *menu, int netId);
void RenderModel_LoadFromNarc(UnkOv01_021FFECC_sub *model, int index, NARC *narc, int fileId, enum HeapID heapId, BOOL atEnd);
void RenderModel_BindTexturesOnVBlank(UnkOv01_021FFECC_sub *model);
void RenderAnimation_LoadFromNarc(SpinTradeRenderAnim *anim, int unused, NARC *narc, int fileId, enum HeapID heapId, BOOL atEnd);
void RenderAnimation_FreeFile(SpinTradeRenderAnim *anim);
void RenderAnimation_AllocObjectForModel(SpinTradeRenderAnim *anim, UnkOv01_021FFECC_sub *model, enum HeapID heapId);
void RenderAnimation_InitObjectForModel(SpinTradeRenderAnim *anim, UnkOv01_021FFECC_sub *model);
BOOL RenderAnimation_Advance(SpinTradeRenderAnim *anim, fx32 step, BOOL loop);
void RenderObj_InitWithAnimation(NNSG3dRenderObj *obj, UnkOv01_021FFECC_sub *model, SpinTradeRenderAnim *anim);
void RenderObj_DrawRotated(NNSG3dRenderObj *obj, const VecFx32 *position, const VecFx32 *scale, const u16 *angles);

BOOL SpinTradeApp_Init(OverlayManager *man, int *state);
BOOL SpinTradeApp_Exit(OverlayManager *man, int *state);
BOOL SpinTradeApp_Main(OverlayManager *man, int *state);
void SpinTrade_ClearLinkFlags(SpinTradeApp *app);
BOOL SpinTrade_State_Start(SpinTradeApp *app);
BOOL SpinTrade_State_WaitFadeIn(SpinTradeApp *app);
BOOL SpinTrade_State_WaitMembers(SpinTradeApp *app);
BOOL SpinTrade_State_SendStart(SpinTradeApp *app);
BOOL SpinTrade_State_WaitConnections(SpinTradeApp *app);
BOOL SpinTrade_State_SendPlayerCount(SpinTradeApp *app);
BOOL SpinTrade_State_SendSlots(SpinTradeApp *app);
BOOL SpinTrade_State_SendReady(SpinTradeApp *app);
BOOL SpinTrade_State_SendJoin(SpinTradeApp *app);
BOOL SpinTrade_State_WaitReady(SpinTradeApp *app);
BOOL SpinTrade_State_StartSync(SpinTradeApp *app);
BOOL SpinTrade_State_WaitSync(SpinTradeApp *app);
BOOL SpinTrade_State_SendTradeSlot(SpinTradeApp *app);
BOOL SpinTrade_State_SendParty(SpinTradeApp *app);
BOOL SpinTrade_State_CheckEggs(SpinTradeApp *app);
BOOL SpinTrade_State_WaitConfirm(SpinTradeApp *app);
BOOL SpinTrade_State_SetupDrop(SpinTradeApp *app);
BOOL SpinTrade_State_DropEggs(SpinTradeApp *app);
BOOL SpinTrade_State_WaitEggsLanded(SpinTradeApp *app);
BOOL SpinTrade_State_WaitBeforeSpin(SpinTradeApp *app);
BOOL SpinTrade_State_SendSpinStart(SpinTradeApp *app);
BOOL SpinTrade_State_WaitSpinStart(SpinTradeApp *app);
BOOL SpinTrade_State_StartSpinSync(SpinTradeApp *app);
BOOL SpinTrade_State_WaitSpinSync(SpinTradeApp *app);
BOOL SpinTrade_State_StartCountdown(SpinTradeApp *app);
BOOL SpinTrade_State_ClearNames(SpinTradeApp *app);
BOOL SpinTrade_State_BeginSpin(SpinTradeApp *app);
BOOL SpinTrade_State_SpinHost(SpinTradeApp *app);
BOOL SpinTrade_State_SpinClient(SpinTradeApp *app);
BOOL SpinTrade_State_StopSpin(SpinTradeApp *app);
BOOL SpinTrade_State_RotateToOwnEgg(SpinTradeApp *app);
BOOL SpinTrade_State_AlignToSlot(SpinTradeApp *app);
BOOL SpinTrade_State_StartResultSync(SpinTradeApp *app);
BOOL SpinTrade_State_WaitResultSync(SpinTradeApp *app);
BOOL SpinTrade_State_SendResult(SpinTradeApp *app);
BOOL SpinTrade_State_ReceiveResult(SpinTradeApp *app);
BOOL SpinTrade_State_ShowNames(SpinTradeApp *app);
BOOL SpinTrade_State_ShowReceivedEgg(SpinTradeApp *app);
BOOL SpinTrade_State_WaitEggFanfare(SpinTradeApp *app);
BOOL SpinTrade_State_GiveBonusBerry(SpinTradeApp *app);
BOOL SpinTrade_State_WaitBerryFanfare(SpinTradeApp *app);
BOOL SpinTrade_State_ShowBagFull(SpinTradeApp *app);
BOOL SpinTrade_State_Wait(SpinTradeApp *app);
BOOL SpinTrade_State_StartSave(SpinTradeApp *app);
BOOL SpinTrade_State_WaitSaveSync(SpinTradeApp *app);
BOOL SpinTrade_State_Save(SpinTradeApp *app);
BOOL SpinTrade_State_Cancel(SpinTradeApp *app);
BOOL SpinTrade_State_WaitCancelMessage(SpinTradeApp *app);
BOOL SpinTrade_State_StartExitSync(SpinTradeApp *app);
BOOL SpinTrade_State_WaitExitSync(SpinTradeApp *app);
BOOL SpinTrade_State_WaitDisconnect(SpinTradeApp *app);
BOOL SpinTrade_State_FadeOut(SpinTradeApp *app);
BOOL SpinTrade_State_WaitFadeOut(SpinTradeApp *app);
int SpinTrade_State_Exit(SpinTradeApp *app);
void SpinTrade_VBlankCallback(void *data);
void SpinTrade_InitGraphics(SpinTradeApp *app);
void SpinTrade_FreeGraphics(SpinTradeApp *app);
void SpinTrade_Draw(SpinTradeApp *app);
void SpinTrade_SetGraphicsBanks(SpinTradeApp *app);
void SpinTrade_SetupPlanes(SpinTradeApp *app);
void SpinTrade_DisableDisplay(SpinTradeApp *app);
void SpinTrade_Init3D(SpinTradeApp *app);
void SpinTrade_InitBgLayers(SpinTradeApp *app);
void SpinTrade_LoadBgGraphics(SpinTradeApp *app);
void SpinTrade_FreeBgLayers(SpinTradeApp *app);
void SpinTrade_LoadButtonTilemap(SpinTradeApp *app, u32 member);
void SpinTrade_SetButtonPressed(SpinTradeApp *app);
void SpinTrade_SetButtonReleased(SpinTradeApp *app);
void SpinTrade_SetButtonHalfPressed(SpinTradeApp *app);
void SpinTrade_InitPaletteData(SpinTradeApp *app);
void SpinTrade_FreePaletteData(SpinTradeApp *app);
void SpinTrade_SetSubBgFadeLevel(SpinTradeApp *app, u32 level);
void SpinTrade_InitSpriteSystem(SpinTradeApp *app);
void SpinTrade_LoadSpriteResources(SpinTradeApp *app);
void SpinTrade_FreeSpriteSystem(SpinTradeApp *app);
ManagedSprite *SpinTrade_CreateSprite(SpinTradeApp *app, const ManagedSpriteTemplate *template);
void SpinTrade_InitText(SpinTradeApp *app);
void SpinTrade_FreeText(SpinTradeApp *app);
void SpinTrade_PrintMessage(SpinTradeApp *app, int msgNo);
void SpinTrade_PrintPlayerMessage(SpinTradeApp *app, int msgNo, PlayerProfile *profile);
void SpinTrade_PrintItemMessage(SpinTradeApp *app, int msgNo, int item);
void SpinTrade_ClearMessage(SpinTradeApp *app);
void SpinTrade_AddNameWindows(SpinTradeApp *app, int count);
void SpinTrade_RemoveNameWindows(SpinTradeApp *app);
void SpinTrade_PrintNameWindow(SpinTradeApp *app, String *name, int index, u32 color);
void SpinTrade_PrintPlayerName(SpinTradeApp *app, String *name, int index, u32 netId);
void SpinTrade_ClearNameWindow(SpinTradeApp *app, int index);
void SpinTrade_InitCamera(SpinTradeApp *app);
void SpinTrade_FreeCamera(SpinTradeApp *app);
void SpinTrade_UpdateTouch(SpinTradeApp *app);
BOOL SpinTrade_IsInButtonArea(SpinTradeTouch *touch);
BOOL SpinTrade_IsButtonTouched(SpinTradeApp *app);
void SpinTrade_InitMachine(SpinTradeApp *app);
void SpinTrade_FreeMachine(SpinTradeApp *app);
void SpinTrade_UpdateMachinePosition(SpinTradeApp *app);
void SpinTrade_DrawMachine(SpinTradeApp *app);
void SpinTradeMachine_Rotate(SpinTradeMachine *machine, fx32 delta);
void SpinTrade_LoadObjects(SpinTradeApp *app);
void SpinTrade_FreeObjects(SpinTradeApp *app);
void SpinTrade_LoadObjectModels(SpinTradeApp *app);
void SpinTrade_FreeObjectModels(SpinTradeApp *app);
void SpinTrade_ShowEgg(SpinTradeApp *app, SpinTradeEgg *egg);
void SpinTrade_UpdateEggPositions(SpinTradeApp *app);
void SpinTrade_DrawEggs(SpinTradeApp *app);
void SpinTrade_RotateEggs(SpinTradeApp *app, fx32 delta);
void SpinTradeEgg_UpdatePosition(SpinTradeEgg *egg, const VecFx32 *machineOffset);
SpinTradeEgg *SpinTrade_FindEggNearestSlot(SpinTradeApp *app);
void SpinTrade_ShowShadow(SpinTradeApp *app, SpinTradeEgg *egg);
void SpinTradeShadow_Update(SpinTradeApp *app, SpinTradeShadow *shadow);
void SpinTrade_ShowMarker(SpinTradeApp *app, SpinTradeEgg *egg);
void SpinTradeMarker_Update(SpinTradeApp *app, SpinTradeMarker *marker);
void SpinTrade_StartEggDrop(SpinTradeApp *app, u32 netId, int slot, int order, int angle, u32 angleOffset, SpinTradeEgg *egg);
void SpinTrade_EggDropTask(SysTask *task, void *data);
int SpinTradeEggDrop_Init(SpinTradeEggDropTask *task);
int SpinTradeEggDrop_Fall(SpinTradeEggDropTask *task);
int SpinTradeEggDrop_Land(SpinTradeEggDropTask *task);
void SpinTradeBounce_Update(SpinTradeApp *app, SpinTradeBounce *bounce);
void SpinTrade_BounceTask(SysTask *task, void *data);
void SpinTrade_StartBounceTask(SpinTradeApp *app);
void SpinTrade_StopBounceTask(SpinTradeApp *app);
void SpinTrade_AddBounce(SpinTradeApp *app, SpinTradeEgg *egg);
void SpinTrade_BounceAllEggs(SpinTradeApp *app);
BOOL SpinTrade_IsAnyEggBouncing(SpinTradeApp *app);
void SpinTradeSpin_Update(SpinTradeSpin *spin);
void SpinTrade_SpinTask(SysTask *task, void *data);
void SpinTrade_StartSpinTask(SpinTradeApp *app);
void SpinTrade_StopSpinTask(SpinTradeApp *app);
void SpinTrade_AddSpin(SpinTradeApp *app, SpinTradeEgg *egg);
void SpinTradeWobble_Update(SpinTradeWobble *wobble);
void SpinTrade_WobbleTask(SysTask *task, void *data);
void SpinTrade_StartWobbleTask(SpinTradeApp *app);
void SpinTrade_StopWobbleTask(SpinTradeApp *app);
void SpinTrade_AddWobble(SpinTradeApp *app, SpinTradeEgg *egg);
void SpinTradeJump_Update(SpinTradeApp *app, SpinTradeJump *jump);
void SpinTrade_JumpTask(SysTask *task, void *data);
void SpinTrade_StartJumpTask(SpinTradeApp *app);
void SpinTrade_StopJumpTask(SpinTradeApp *app);
void SpinTrade_StartJump(SpinTradeApp *app);
BOOL SpinTrade_HasJumpLanded(SpinTradeApp *app);
BOOL SpinTrade_IsJumping(SpinTradeApp *app);
void SpinTrade_CountdownTask(SysTask *task, void *data);
SysTask *SpinTrade_StartCountdown(SpinTradeApp *app);
void SpinTradeButtonFlash_Update(SpinTradeApp *app, SpinTradeButtonFlash *flash);
void SpinTrade_ButtonFlashTask(SysTask *task, void *data);
void SpinTrade_StartButtonFlashTask(SpinTradeApp *app);
void SpinTrade_StopButtonFlashTask(SpinTradeApp *app);
u16 SpinTrade_IsButtonFlashing(SpinTradeApp *app);
void SpinTrade_StartButtonFlash(SpinTradeApp *app);
void SpinTradeHover_Update(SpinTradeApp *app, SpinTradeHover *hover);
void SpinTrade_HoverTask(SysTask *task, void *data);
void SpinTrade_StartHoverTask(SpinTradeApp *app);
void SpinTrade_StopHoverTask(SpinTradeApp *app);
void SpinTrade_AddHover(SpinTradeApp *app, SpinTradeEgg *egg);
void SpinTrade_HoverBonusEggs(SpinTradeApp *app);
void SpinTrade_SubBgFadeTask(SysTask *task, void *data);
void SpinTrade_StartSubBgFade(SpinTradeApp *app, int fadeOut);
void SpinTrade_AddAngle(fx32 *angle, fx32 delta);
void SpinTrade_RotateAll(SpinTradeApp *app, fx32 delta);
BOOL SpinTrade_TickSpinTimer(SpinTradeApp *app);
void *SpinTrade_LoadNarcMember(SpinTradeApp *app, u32 member, int atStart);
void SpinTrade_ReceiveJump(SpinTradeApp *app, u8 jump);
void SpinTrade_ReceiveSpinData(SpinTradeApp *app, const u16 *data);
void *SpinTrade_ClearScratch(SpinTradeApp *app, u32 size);
void *SpinTrade_GetScratch(SpinTradeApp *app);
BOOL SpinTrade_HasReceivedStatus(SpinTradeApp *app, u32 status);
BOOL SpinTrade_HaveAllMembersJoined(SpinTradeApp *app);
int SpinTrade_CountMembers(SpinTradeApp *app);
void SpinTrade_ReceiveMemberSlot(SpinTradeApp *app, SpinTradeMember *member);
void SpinTrade_ReceivePlayerCount(SpinTradeApp *app, u32 count);
BOOL SpinTrade_IsMemberPresent(SpinTradeApp *app, u32 netId);
void SpinTrade_ReceiveTradedMon(SpinTradeApp *app, int netId);
int SpinTrade_CountReceivedParties(SpinTradeApp *app);
void SpinTrade_ReceiveTradeSlot(SpinTradeApp *app, int netId, u32 slot);
void SpinTrade_ReceiveResult(SpinTradeApp *app, const u16 *src);
BOOL SpinTrade_IsInBonusArea(SpinTradeApp *app, fx32 angle, int playerCount);
BOOL SpinTrade_PartyHasBadEgg(Party *party);
BOOL SpinTrade_AnyPartyHasBadEgg(SpinTradeApp *app);
int SpinTrade_CountConfirmed(SpinTradeApp *app);
BOOL SpinTrade_IsCancelled(SpinTradeApp *app);

#pragma force_active on
BOOL SpinTradeApp_Init(OverlayManager *man, int *state) {
    SpinTradeArgs *args = OverlayManager_GetArgs(man);
    SpinTradeApp *app;
    int i, slot, myNetId;
    SpinTradeMachine *machine;

    CommState_SetErrorWatch(1, 1);
    Main_SetVBlankIntrCB(NULL, NULL);
    HBlankInterruptDisable();
    sub_0201A728(2);
    Heap_Create(HEAP_ID_3, HEAP_ID_102, 0x80000);
    app = OverlayManager_CreateAndGetData(man, sizeof(SpinTradeApp), HEAP_ID_102);
    memset(app, 0, sizeof(SpinTradeApp));
    args->field34 = app;
    app->args = args;
    app->menu = args->menu;
    app->party = SaveArray_Party_Get(app->args->input.saveData);
    app->narc = NARC_New(NARC_a_1_8_7, HEAP_ID_102);
    GF_CreateVramTransferManager(8, HEAP_ID_102);
    GF_TouchpadResetSampling();
    GF_TouchpadStartSampling(4);
    SpinTrade_InitGraphics(app);
    Main_SetVBlankIntrCB(SpinTrade_VBlankCallback, app);
    SpinTrade_InitMachine(app);
    SpinTrade_LoadObjects(app);

    // the machine faces this player's seat
    i = 0;
    slot = 0;
    myNetId = Comm_GetLocalNetId();
    machine = &app->machine;
    args = app->args;
    for (; i < SPIN_TRADE_MAX_PLAYERS; i++) {
        if (args->unk0[3] & (1 << i)) {
            if (i == myNetId) {
                break;
            }
            slot++;
        }
    }
    machine->baseAngle = ov85_021EA788[args->unk0[2]][slot] << FX32_SHIFT;
    app->bgm = GF_GetCurrentPlayingBGM();
    SpinTrade_StartBounceTask(app);
    SpinTrade_StartSpinTask(app);
    SpinTrade_StartWobbleTask(app);
    SpinTrade_StartJumpTask(app);
    SpinTrade_StartButtonFlashTask(app);
    SpinTrade_StartHoverTask(app);
    BeginNormalPaletteFade(0, 1, 1, RGB_BLACK, 8, 1, HEAP_ID_102);
    return TRUE;
}

BOOL SpinTradeApp_Exit(OverlayManager *man, int *state) {
    SpinTradeApp *app = OverlayManager_GetData(man);

    GF_ASSERT(GF_TouchpadStopSampling() == 1);
    SpinTrade_StopBounceTask(app);
    SpinTrade_StopSpinTask(app);
    SpinTrade_StopWobbleTask(app);
    SpinTrade_StopJumpTask(app);
    SpinTrade_StopButtonFlashTask(app);
    SpinTrade_StopHoverTask(app);
    SpinTrade_FreeMachine(app);
    SpinTrade_FreeObjects(app);
    SpinTrade_FreeGraphics(app);
    Main_SetVBlankIntrCB(NULL, NULL);
    GF_DestroyVramTransferManager();
    NARC_Delete(app->narc);
    OverlayManager_FreeData(man);
    Heap_Destroy(HEAP_ID_102);
    sub_0201A738(2);
    return TRUE;
}

BOOL SpinTradeApp_Main(OverlayManager *man, int *state) {
    SpinTradeApp *app = OverlayManager_GetData(man);
    int ret;

    SpinTrade_UpdateTouch(app);
    do {
        ret = ov85_021EA800[app->state](app);
    } while (ret == 1);
    if (ret == 2) {
        return TRUE;
    }
    SpinTrade_UpdateMachinePosition(app);
    SpinTrade_UpdateEggPositions(app);
    SpinTrade_Draw(app);
    SpinTrade_ClearLinkFlags(app);
    return FALSE;
}
#pragma force_active reset

void SpinTrade_ClearLinkFlags(SpinTradeApp *app) {
    app->remoteJump = FALSE;
    app->spinReceived = FALSE;
}

BOOL SpinTrade_State_Start(SpinTradeApp *app) {
    SpinTrade_PrintMessage(app, msg_0206_00000);
    app->state = SPIN_TRADE_STATE_WAIT_FADE_IN;
    return FALSE;
}

#pragma force_active on
BOOL SpinTrade_State_WaitFadeIn(SpinTradeApp *app) {
    if (IsPaletteFadeFinished()) {
        if (!Comm_GetLocalNetId()) {
            app->state = SPIN_TRADE_STATE_WAIT_MEMBERS;
        } else {
            app->state = SPIN_TRADE_STATE_SEND_JOIN;
        }
        return TRUE;
    }
    return FALSE;
}
#pragma force_active reset

BOOL SpinTrade_State_WaitMembers(SpinTradeApp *app) {
    if (SpinTrade_HaveAllMembersJoined(app)) {
        app->state = SPIN_TRADE_STATE_SEND_START;
        return TRUE;
    }
    return FALSE;
}

BOOL SpinTrade_State_SendStart(SpinTradeApp *app) {
    u16 status = 1;
    if (SpinTradeLink_SendCommand(app->menu, SPIN_TRADE_CMD_STATUS, &status, 2) == 1) {
        app->state = SPIN_TRADE_STATE_WAIT_CONNECTIONS;
        return TRUE;
    }
    return FALSE;
}

BOOL SpinTrade_State_WaitConnections(SpinTradeApp *app) {
    int expected = SpinTrade_CountMembers(app) + 1;
    if (expected != Comm_CountConnected()) {
        return FALSE;
    }
    app->state = SPIN_TRADE_STATE_SEND_PLAYER_COUNT;
    return TRUE;
}

BOOL SpinTrade_State_SendPlayerCount(SpinTradeApp *app) {
    app->playerCount = SpinTrade_CountMembers(app) + 1;
    if (SpinTradeLink_SendCommand(app->menu, SPIN_TRADE_CMD_COUNT, &app->playerCount, 4) == 1) {
        app->sendCount = 0;
        app->state = SPIN_TRADE_STATE_SEND_SLOTS;
    }
    return FALSE;
}

// Tells every member its seat: slot 0 is the host, the others follow in netId order
BOOL SpinTrade_State_SendSlots(SpinTradeApp *app) {
    u16 values[2];
    int netId;
    int slot;

    if (app->sendCount == 0) {
        values[1] = 0;
        values[0] = 0;
        if (SpinTradeLink_SendCommand(app->menu, SPIN_TRADE_CMD_SLOT, values, 4) == 1) {
            app->sendCount++;
        }
        return FALSE;
    }

    netId = 1;
    slot = 1;
    for (; netId < SPIN_TRADE_MAX_PLAYERS; netId++) {
        if ((1 << netId) & app->menu->memberMask) {
            if (slot >= app->sendCount) {
                values[1] = netId;
                values[0] = app->sendCount;
                if (SpinTradeLink_SendCommand(app->menu, SPIN_TRADE_CMD_SLOT, values, 4) == 1) {
                    app->sendCount++;
                }
                break;
            }
            slot++;
        }
    }
    if (app->sendCount >= app->playerCount) {
        app->state = SPIN_TRADE_STATE_SEND_READY;
    }
    return FALSE;
}

BOOL SpinTrade_State_SendReady(SpinTradeApp *app) {
    u16 status = 8;
    if (SpinTradeLink_SendCommand(app->menu, SPIN_TRADE_CMD_STATUS, &status, 2) == 1) {
        app->state = SPIN_TRADE_STATE_START_SYNC;
    }
    return FALSE;
}

BOOL SpinTrade_State_SendJoin(SpinTradeApp *app) {
    if (SpinTradeLink_SendCommand(app->menu, SPIN_TRADE_CMD_JOIN, NULL, 0) == 1) {
        app->unk_010 = 0;
        app->state = SPIN_TRADE_STATE_WAIT_READY;
        return FALSE;
    }
    return FALSE;
}

BOOL SpinTrade_State_WaitReady(SpinTradeApp *app) {
    if (SpinTrade_HasReceivedStatus(app, 8) == 1) {
        GF_ASSERT(app->playerCount >= 2);
        GF_ASSERT(app->mySlot != 0);
        app->state = SPIN_TRADE_STATE_START_SYNC;
    }
    return FALSE;
}

BOOL SpinTrade_State_StartSync(SpinTradeApp *app) {
    LinkSync_StartSync(SPIN_TRADE_LINK_SYNC_NO);
    app->state = SPIN_TRADE_STATE_WAIT_SYNC;
    return FALSE;
}

BOOL SpinTrade_State_WaitSync(SpinTradeApp *app) {
    if (LinkSync_IsSynced(SPIN_TRADE_LINK_SYNC_NO)) {
        app->state = SPIN_TRADE_STATE_SEND_TRADE_SLOT;
    }
    return FALSE;
}

BOOL SpinTrade_State_SendTradeSlot(SpinTradeApp *app) {
    if (SpinTradeLink_SendCommand(app->menu, SPIN_TRADE_CMD_TRADE_SLOT, &app->args->unk0[1], 4)) {
        app->state = SPIN_TRADE_STATE_SEND_PARTY;
    }
    return FALSE;
}

BOOL SpinTrade_State_SendParty(SpinTradeApp *app) {
    if (sub_02036FD8(0x83, app->party, sizeof(PartyCore))) {
        app->state = SPIN_TRADE_STATE_CHECK_EGGS;
    }
    return FALSE;
}

#pragma force_active on
BOOL SpinTrade_State_CheckEggs(SpinTradeApp *app) {
    if (app->playerCount == SpinTrade_CountReceivedParties(app)) {
        if (SpinTrade_AnyPartyHasBadEgg(app) == 1) {
            SpinTradeLink_SendCommand(app->menu, SPIN_TRADE_CMD_CANCEL, NULL, 0);
            app->state = SPIN_TRADE_STATE_CANCEL;
        } else {
            SpinTradeLink_SendCommand(app->menu, SPIN_TRADE_CMD_CONFIRM, NULL, 0);
            app->state = SPIN_TRADE_STATE_WAIT_CONFIRM;
        }
    }
    return FALSE;
}
#pragma force_active reset

BOOL SpinTrade_State_WaitConfirm(SpinTradeApp *app) {
    if (SpinTrade_IsCancelled(app) == 1) {
        app->state = SPIN_TRADE_STATE_CANCEL;
    } else if (app->playerCount == SpinTrade_CountConfirmed(app)) {
        app->state = SPIN_TRADE_STATE_SETUP_DROP;
    }
    return FALSE;
}

#pragma force_active on
BOOL SpinTrade_State_SetupDrop(SpinTradeApp *app) {
    SpinTradeDrop *drop = SpinTrade_ClearScratch(app, sizeof(SpinTradeDrop));
    u32 playerCount;
    int netId;

    drop->index = app->mySlot;
    playerCount = app->playerCount;
    drop->total = playerCount;
    drop->slotAngle = ov85_021EA788[playerCount][app->mySlot];
    for (netId = 0; netId < 8; netId++) {
        if (SpinTrade_IsMemberPresent(app, netId)) {
            app->profiles[netId] = CommPlayerInfo_GetProfile(netId);
            PlayerName_FlatToString(app->profiles[netId], app->names[netId]);
        }
    }
    app->machine.baseAngle = drop->slotAngle << FX32_SHIFT;
    app->state = SPIN_TRADE_STATE_DROP_EGGS;
    Sound_SetSceneAndPlayBGM(15, SEQ_GS_GURUGURU, 1);
    SpinTrade_ClearMessage(app);
    return TRUE;
}

// Drops one egg every 15 frames, starting at this player's seat
BOOL SpinTrade_State_DropEggs(SpinTradeApp *app) {
    SpinTradeDrop *drop;

    if (--app->timer > 0) {
        return FALSE;
    }
    app->timer = 15;
    drop = SpinTrade_GetScratch(app);
    SpinTrade_StartEggDrop(app, app->members[drop->index].netId, drop->index, drop->count, ov85_021EA7C4[drop->total][drop->index], drop->slotAngle, &app->objects.eggs[drop->index]);
    drop->index++;
    drop->index %= drop->total;
    if (++drop->count == drop->total) {
        app->timer = 0;
        app->state = SPIN_TRADE_STATE_WAIT_EGGS_LANDED;
    }
    return FALSE;
}
#pragma force_active reset

BOOL SpinTrade_State_WaitEggsLanded(SpinTradeApp *app) {
    int i = 0;
    int landed = 0;
    int count = app->playerCount;
    SpinTradeObjects *objects = &app->objects;

    do {
        if (objects->eggs[i].dropped == 1) {
            landed++;
        }
        i++;
    } while (i < count);
    if (landed == count) {
        app->state = SPIN_TRADE_STATE_WAIT_BEFORE_SPIN;
        return TRUE;
    }
    return FALSE;
}

BOOL SpinTrade_State_WaitBeforeSpin(SpinTradeApp *app) {
    if (++app->timer > 30) {
        app->timer = 0;
        if (Comm_GetLocalNetId() == 0) {
            app->state = SPIN_TRADE_STATE_SEND_SPIN_START;
        } else {
            app->state = SPIN_TRADE_STATE_WAIT_SPIN_START;
        }
    }
    return FALSE;
}

BOOL SpinTrade_State_SendSpinStart(SpinTradeApp *app) {
    u16 status = 4;
    if (SpinTradeLink_SendCommand(app->menu, SPIN_TRADE_CMD_STATUS, &status, 2) == 1) {
        app->state = SPIN_TRADE_STATE_START_SPIN_SYNC;
    }
    return FALSE;
}

BOOL SpinTrade_State_WaitSpinStart(SpinTradeApp *app) {
    if (SpinTrade_HasReceivedStatus(app, 4) == 1) {
        app->state = SPIN_TRADE_STATE_START_SPIN_SYNC;
        return TRUE;
    }
    return FALSE;
}

BOOL SpinTrade_State_StartSpinSync(SpinTradeApp *app) {
    LinkSync_StartSync(SPIN_TRADE_LINK_SYNC_NO);
    app->state = SPIN_TRADE_STATE_WAIT_SPIN_SYNC;
    return FALSE;
}

BOOL SpinTrade_State_WaitSpinSync(SpinTradeApp *app) {
    if (LinkSync_IsSynced(SPIN_TRADE_LINK_SYNC_NO)) {
        app->state = SPIN_TRADE_STATE_START_COUNTDOWN;
    }
    return FALSE;
}

BOOL SpinTrade_State_StartCountdown(SpinTradeApp *app) {
    SpinTrade_StartSubBgFade(app, 1);
    SpinTrade_StartCountdown(app);
    app->state = SPIN_TRADE_STATE_CLEAR_NAMES;
    return FALSE;
}

#pragma force_active on
BOOL SpinTrade_State_ClearNames(SpinTradeApp *app) {
    int i;
    if (++app->timer < 95) {
        return FALSE;
    }
    for (i = 0; i < app->playerCount; i++) {
        SpinTrade_ClearNameWindow(app, i);
    }
    app->timer = 0;
    app->state = SPIN_TRADE_STATE_BEGIN_SPIN;
    return FALSE;
}
#pragma force_active reset

BOOL SpinTrade_State_BeginSpin(SpinTradeApp *app) {
    Comm_GetLocalNetId();
    app->state = SPIN_TRADE_STATE_SPIN_HOST;
    return FALSE;
}

#pragma force_active on
// The host accelerates the table for 300 frames, then slows it down; the clients follow its SPIN packets
BOOL SpinTrade_State_SpinHost(SpinTradeApp *app) {
    fx32 speed;
    u16 data[4];
    SpinTradeMachine *machine;

    if (SpinTrade_TickSpinTimer(app) == 1 && !SpinTrade_IsJumping(app)) {
        app->state = SPIN_TRADE_STATE_STOP_SPIN;
        return TRUE;
    }
    if (app->remoteJump && !SpinTrade_IsJumping(app)) {
        SpinTrade_StartJump(app);
    }
    if (!SpinTrade_IsButtonFlashing(app) && SpinTrade_IsButtonTouched(app) == 1) {
        if (!SpinTrade_IsJumping(app)) {
            SpinTrade_StartJump(app);
            app->sendJump = TRUE;
        }
        SpinTrade_StartButtonFlash(app);
    }
    if (app->sendJump) {
        if (SpinTradeLink_SendCommand(app->args->menu, SPIN_TRADE_CMD_JUMP, &app->sendJump, 1) == 1) {
            app->sendJump = FALSE;
        }
    }
    machine = &app->machine;
    if (app->frame < 300) {
        machine->speed += 191;
        if (machine->speed >= FX32_CONST(14)) {
            machine->speed = FX32_CONST(14);
        }
    } else {
        machine->speed -= 191;
        if (machine->speed < FX32_CONST(4)) {
            machine->speed = FX32_CONST(4);
        }
    }
    speed = machine->speed;
    if (SpinTrade_HasJumpLanded(app) == 1) {
        SpinTradeMachine_Rotate(&app->machine, speed * 2);
    } else {
        SpinTrade_RotateAll(app, speed);
    }
    data[1] = app->frame;
    data[2] = app->machine.speed / FX32_ONE;
    data[3] = app->machine.angle / FX32_ONE;
    SpinTradeLink_SendCommand(app->menu, SPIN_TRADE_CMD_SPIN, data, 8);
    return FALSE;
}
#pragma force_active reset

BOOL SpinTrade_State_SpinClient(SpinTradeApp *app) {
    if (app->spinReceived == 1) {
        app->machine.angle = app->spinData.angle << FX32_SHIFT;
        app->machine.speed = app->spinData.speed << FX32_SHIFT;
        app->frame = app->spinData.frame;
    }
    if (SpinTrade_TickSpinTimer(app) == TRUE && SpinTrade_IsJumping(app) == FALSE) {
        app->state = SPIN_TRADE_STATE_STOP_SPIN;
        return TRUE;
    }
    if (app->remoteJump != 0 && SpinTrade_IsJumping(app) == FALSE) {
        SpinTrade_StartJump(app);
    }
    if (SpinTrade_IsButtonFlashing(app) == 0 && SpinTrade_IsButtonTouched(app) == TRUE) {
        if (SpinTrade_IsJumping(app) == FALSE) {
            SpinTrade_StartJump(app);
            app->sendJump = TRUE;
        }
        SpinTrade_StartButtonFlash(app);
    }
    if (app->sendJump != 0) {
        SpinTradeArgs *args = app->args;
        if (SpinTradeLink_SendCommand(args->menu, SPIN_TRADE_CMD_JUMP, &app->sendJump, 1) == TRUE) {
            app->sendJump = FALSE;
        }
    }
    if (SpinTrade_HasJumpLanded(app) == TRUE) {
        SpinTradeMachine_Rotate(&app->machine, app->machine.speed * 2);
    } else {
        SpinTrade_RotateAll(app, app->machine.speed);
    }
    return FALSE;
}

BOOL SpinTrade_State_StopSpin(SpinTradeApp *app) {
    SpinTrade_StartSubBgFade(app, 0);
    app->landedEgg = SpinTrade_FindEggNearestSlot(app);
    if (app->landedEgg->netId == Comm_GetLocalNetId()) {
        app->state = SPIN_TRADE_STATE_ROTATE_TO_OWN_EGG;
    } else {
        app->state = SPIN_TRADE_STATE_ALIGN_TO_SLOT;
    }
    PlaySE(SEQ_SE_DP_PINPON);
    return TRUE;
}

// Nobody can receive their own egg: keep turning until another egg is in front
BOOL SpinTrade_State_RotateToOwnEgg(SpinTradeApp *app) {
    SpinTrade_RotateAll(app, FX32_CONST(4));
    app->landedEgg = SpinTrade_FindEggNearestSlot(app);
    if (app->landedEgg->netId != Comm_GetLocalNetId()) {
        app->state = SPIN_TRADE_STATE_ALIGN_TO_SLOT;
    }
    return FALSE;
}

#pragma force_active on
BOOL SpinTrade_State_AlignToSlot(SpinTradeApp *app) {
    fx32 angle;
    fx32 target;
    fx32 step;
    fx32 distance;
    int slot = app->mySlot;
    int count = app->playerCount;

    step = FX32_CONST(4);
    angle = app->landedEgg->angle;
    target = ov85_021EA7C4[count][slot] << FX32_SHIFT;
    distance = target - angle;
    if (distance < 0) {
        distance = target + SPIN_TRADE_ANGLE_FULL - angle;
    }
    if (distance <= FX32_CONST(16)) {
        step >>= 1;
    }
    if (distance < step) {
        step = distance;
    }
    SpinTradeMachine_Rotate(&app->machine, step);
    SpinTrade_RotateEggs(app, step);
    SpinTrade_AddAngle(&angle, step);
    if (angle / FX32_ONE == target / FX32_ONE) {
        app->state = SPIN_TRADE_STATE_START_RESULT_SYNC;
    }
    return FALSE;
}
#pragma force_active reset

BOOL SpinTrade_State_StartResultSync(SpinTradeApp *app) {
    LinkSync_StartSync(SPIN_TRADE_LINK_SYNC_NO);
    app->state = SPIN_TRADE_STATE_WAIT_RESULT_SYNC;
    return FALSE;
}

BOOL SpinTrade_State_WaitResultSync(SpinTradeApp *app) {
    if (LinkSync_IsSynced(SPIN_TRADE_LINK_SYNC_NO)) {
        if (Comm_GetLocalNetId() == 0) {
            app->state = SPIN_TRADE_STATE_SEND_RESULT;
        } else {
            app->state = SPIN_TRADE_STATE_RECEIVE_RESULT;
        }
    }
    return FALSE;
}

// result: [0] table angle, [1] bonus mask, [2 + slot] egg angles
BOOL SpinTrade_State_SendResult(SpinTradeApp *app) {
    s32 i;
    u16 data[8];
    SpinTradeEgg *egg;

    data[0] = app->machine.angle / FX32_ONE;
    app->bonusMask = 0;
    i = 0;
    if (app->playerCount > 0) {
        egg = app->objects.eggs;
        do {
            GF_ASSERT(egg->active);
            data[egg->slot + 2] = egg->angle / FX32_ONE;
            if (SpinTrade_IsInBonusArea(app, egg->angle, app->playerCount) == 1) {
                app->bonusMask |= 1 << egg->netId;
            }
            i++;
            egg++;
        } while (i < app->playerCount);
    }
    data[1] = app->bonusMask;
    if (SpinTradeLink_SendCommand(app->menu, SPIN_TRADE_CMD_RESULT, data, 16) == 1) {
        app->state = SPIN_TRADE_STATE_SHOW_NAMES;
    }
    return FALSE;
}

BOOL SpinTrade_State_ReceiveResult(SpinTradeApp *app) {
    s32 i;
    SpinTradeEgg *egg;
    u16 *result;

    if (app->resultReceived == 1) {
        app->machine.angle = app->result[0] << FX32_SHIFT;
        result = app->result;
        i = 0;
        if (app->playerCount > 0) {
            egg = app->objects.eggs;
            do {
                GF_ASSERT(egg->active);
                egg->angle = result[egg->slot + 2] << FX32_SHIFT;
                i++;
                egg++;
            } while (i < app->playerCount);
        }
        app->bonusMask = result[1];
        SpinTrade_UpdateMachinePosition(app);
        SpinTrade_UpdateEggPositions(app);
        app->landedEgg = SpinTrade_FindEggNearestSlot(app);
        app->state = SPIN_TRADE_STATE_SHOW_NAMES;
    }
    return FALSE;
}

BOOL SpinTrade_State_ShowNames(SpinTradeApp *app) {
    s32 i;
    s32 slot = app->landedEgg->slot;

    for (i = 0; i < app->playerCount; i++) {
        u32 netId = app->objects.eggs[slot].netId;
        SpinTrade_PrintPlayerName(app, app->names[netId], i, netId);
        slot = (slot + 1) % app->playerCount;
    }
    app->timer = 0;
    app->state = SPIN_TRADE_STATE_SHOW_RECEIVED_EGG;
    return FALSE;
}

BOOL SpinTrade_State_ShowReceivedEgg(SpinTradeApp *app) {
    s32 i;

    app->timer++;
    if (app->timer < 120) {
        return FALSE;
    }
    app->timer = 0;
    for (i = 0; i < app->playerCount; i++) {
        SpinTrade_ClearNameWindow(app, i);
    }
    SpinTrade_PrintPlayerMessage(app, msg_0206_00002, app->profiles[app->landedEgg->netId]);
    PlayFanfare(SEQ_ME_ITEM);
    app->state = SPIN_TRADE_STATE_WAIT_EGG_FANFARE;
    return FALSE;
}

BOOL SpinTrade_State_WaitEggFanfare(SpinTradeApp *app) {
    if (app->timer < 120) {
        app->timer++;
    } else if (!IsFanfarePlaying()) {
        app->timer = 0;
        app->state = SPIN_TRADE_STATE_GIVE_BONUS_BERRY;
    }
    return FALSE;
}

#pragma force_active on
// An egg that stopped in a bonus area also gives its receiver a random Berry
BOOL SpinTrade_State_GiveBonusBerry(SpinTradeApp *app) {
    u32 item, high;
    int playerCount;
    int result;

    SpinTrade_HoverBonusEggs(app);
    if (!(app->bonusMask & (1 << app->landedEgg->netId))) {
        app->state = SPIN_TRADE_STATE_START_SAVE;
        return FALSE;
    }
    playerCount = app->playerCount;
    item = ov85_021EA728[playerCount][0];
    high = ov85_021EA72C[playerCount][0];
    item += LCRandom() % (high - item + 1);
    if (item < ov85_021EA728[playerCount][0]) {
        item = ov85_021EA728[playerCount][0];
    } else if (item > high) {
        item = high;
    }
    SpinTrade_PrintItemMessage(app, msg_0206_00003, item);
    result = Bag_AddItem(Save_Bag_Get(app->args->input.saveData), (u16)item, 1, HEAP_ID_102);
    PlayFanfare(SEQ_ME_ITEM);
    if (result == 1) {
        app->state = SPIN_TRADE_STATE_WAIT_BERRY_FANFARE;
    } else {
        app->state = SPIN_TRADE_STATE_SHOW_BAG_FULL;
    }
    return FALSE;
}
#pragma force_active reset

BOOL SpinTrade_State_WaitBerryFanfare(SpinTradeApp *app) {
    if (app->timer < 120) {
        app->timer++;
    } else if (!IsFanfarePlaying()) {
        app->timer = 0;
        app->state = SPIN_TRADE_STATE_START_SAVE;
    }
    return FALSE;
}

BOOL SpinTrade_State_ShowBagFull(SpinTradeApp *app) {
    if (app->timer < 120) {
        app->timer++;
    } else if (!IsFanfarePlaying()) {
        app->timer = 0;
        app->state = SPIN_TRADE_STATE_WAIT_BERRY_FANFARE;
        SpinTrade_PrintMessage(app, msg_0206_00004);
    }
    return FALSE;
}

BOOL SpinTrade_State_Wait(SpinTradeApp *app) {
    if (app->timer < 120) {
        app->timer++;
    } else {
        app->timer = 0;
        app->state = SPIN_TRADE_STATE_START_SAVE;
    }
    return FALSE;
}

BOOL SpinTrade_State_StartSave(SpinTradeApp *app) {
    GameStats_Inc(app->args->input.gameStats, GAME_STAT_EGGS_SPUN);
    GameStats_AddScore(app->args->input.gameStats, SCORE_EVENT_SPIN_TRADE);
    SpinTrade_PrintMessage(app, msg_0206_00011);
    LinkSync_StartSync(SPIN_TRADE_LINK_SYNC_NO);
    gSystem.softResetDisabled = TRUE;
    app->state = SPIN_TRADE_STATE_WAIT_SAVE_SYNC;
    return FALSE;
}

BOOL SpinTrade_State_WaitSaveSync(SpinTradeApp *app) {
    if (LinkSync_IsSynced(SPIN_TRADE_LINK_SYNC_NO)) {
        SpinTrade_ReceiveTradedMon(app, app->landedEgg->netId);
        Comm_InitSyncedSave(&app->saveState);
        app->waitingIcon = WaitingIcon_New(&app->text.msgWindow, 10);
        app->state = SPIN_TRADE_STATE_SAVE;
    }
    return FALSE;
}

BOOL SpinTrade_State_Save(SpinTradeApp *app) {
    if (Comm_SyncedSave(app->args->input.saveData, 2, &app->saveState)) {
        gSystem.softResetDisabled = FALSE;
        WaitingIcon_Delete(app->waitingIcon);
        app->state = SPIN_TRADE_STATE_START_EXIT_SYNC;
    }
    return FALSE;
}

BOOL SpinTrade_State_Cancel(SpinTradeApp *app) {
    SpinTrade_PrintMessage(app, msg_0206_00012);
    app->state = SPIN_TRADE_STATE_WAIT_CANCEL_MESSAGE;
    return FALSE;
}

BOOL SpinTrade_State_WaitCancelMessage(SpinTradeApp *app) {
    if (++app->timer >= 120) {
        app->timer = 0;
        app->state = SPIN_TRADE_STATE_START_EXIT_SYNC;
        return TRUE;
    }
    return FALSE;
}

BOOL SpinTrade_State_StartExitSync(SpinTradeApp *app) {
    if (app->unk_018 == 0) {
        LinkSync_StartSync(SPIN_TRADE_LINK_SYNC_NO);
    }
    SpinTrade_PrintMessage(app, msg_0206_00010);
    app->state = SPIN_TRADE_STATE_WAIT_EXIT_SYNC;
    return FALSE;
}

#pragma force_active on
BOOL SpinTrade_State_WaitExitSync(SpinTradeApp *app) {
    if (app->unk_018 == 0) {
        if (LinkSync_IsSynced(SPIN_TRADE_LINK_SYNC_NO)) {
            CommState_SetErrorWatch(0, 0);
            CommState_SetMaxEntries(1);
            CommState_ResumeAlternateSearch();
            app->state = SPIN_TRADE_STATE_WAIT_DISCONNECT;
        }
    } else {
        app->timer++;
        if (app->timer >= 120) {
            app->timer = 0;
            if (app->menu->unk_3C == 0) {
                CommState_SetErrorWatch(0, 0);
                CommState_SetMaxEntries(1);
                CommState_ResumeAlternateSearch();
            }
            app->state = SPIN_TRADE_STATE_WAIT_DISCONNECT;
        }
    }
    return FALSE;
}
#pragma force_active reset

BOOL SpinTrade_State_WaitDisconnect(SpinTradeApp *app) {
    if (Comm_CountConnected() <= 1) {
        app->state = SPIN_TRADE_STATE_FADE_OUT;
        return TRUE;
    }
    return FALSE;
}

#pragma force_active on
BOOL SpinTrade_State_FadeOut(SpinTradeApp *app) {
    int bgm;

    BeginNormalPaletteFade(2, 0, 0, RGB_BLACK, 8, 1, HEAP_ID_102);
    bgm = app->bgm;
    if (bgm != GF_GetCurrentPlayingBGM()) {
        sub_02055198(NULL, bgm);
    }
    app->state = SPIN_TRADE_STATE_WAIT_FADE_OUT;
    return FALSE;
}
#pragma force_active reset

BOOL SpinTrade_State_WaitFadeOut(SpinTradeApp *app) {
    if (IsPaletteFadeFinished()) {
        app->state = SPIN_TRADE_STATE_EXIT;
        return TRUE;
    }
    return FALSE;
}

int SpinTrade_State_Exit(SpinTradeApp *app) {
    return 2;
}

void SpinTrade_VBlankCallback(void *data) {
    SpinTradeApp *app = data;

    GF_RunVramTransferTasks();
    SpriteSystem_TransferOam();
    PaletteData_PushTransparentBuffers(app->paletteData);
    DoScheduledBgGpuUpdates(app->bgConfig);
}

#pragma force_active on
void SpinTrade_InitGraphics(SpinTradeApp *app) {
    SpinTrade_SetGraphicsBanks(app);
    SpinTrade_Init3D(app);
    SpinTrade_InitCamera(app);
    SpinTrade_InitBgLayers(app);
    SpinTrade_InitPaletteData(app);
    SpinTrade_InitSpriteSystem(app);
    SpinTrade_LoadBgGraphics(app);
    SpinTrade_InitText(app);
    SpinTrade_LoadSpriteResources(app);
    SpinTrade_SetupPlanes(app);
}
#pragma force_active reset

void SpinTrade_FreeGraphics(SpinTradeApp *app) {
    SpinTrade_DisableDisplay(app);
    SpinTrade_FreeSpriteSystem(app);
    SpinTrade_FreeText(app);
    SpinTrade_FreeBgLayers(app);
    SpinTrade_FreePaletteData(app);
    SpinTrade_FreeCamera(app);
}

void SpinTrade_Draw(SpinTradeApp *app) {
    Thunk_G3X_Reset();
    Camera_PushLookAtToNNSGlb();
    NNS_G3dGlbLightVector(GX_LIGHTID_0, 0, -FX16_ONE, 0);
    NNS_G3dGlbLightColor(GX_LIGHTID_0, GX_RGB(31, 31, 31));
    NNS_G3dGlbMaterialColorDiffAmb(GX_RGB(31, 31, 31), GX_RGB(31, 31, 31), FALSE);
    NNS_G3dGlbMaterialColorSpecEmi(GX_RGB(31, 31, 31), GX_RGB(31, 31, 31), FALSE);
    SpinTrade_DrawMachine(app);
    SpinTrade_DrawEggs(app);
    RequestSwap3DBuffers(GX_SORTMODE_AUTO, GX_BUFFERMODE_Z);
    SpriteSystem_DrawSprites(app->spriteManager);
}

void SpinTrade_SetGraphicsBanks(SpinTradeApp *app) {
    GraphicsBanks banks = ov85_021EA700;
    GfGfx_SetBanks(&banks);
}

void SpinTrade_SetupPlanes(SpinTradeApp *app) {
    GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG0, TRUE);
    GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG1, TRUE);
    GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG2, TRUE);
    GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG3, TRUE);
    GfGfx_EngineATogglePlanes(GX_PLANEMASK_OBJ, TRUE);
    GfGfx_EngineBTogglePlanes(GX_PLANEMASK_BG0, FALSE);
    GfGfx_EngineBTogglePlanes(GX_PLANEMASK_BG1, FALSE);
    GfGfx_EngineBTogglePlanes(GX_PLANEMASK_BG2, TRUE);
    GfGfx_EngineBTogglePlanes(GX_PLANEMASK_BG3, TRUE);
    GfGfx_EngineBTogglePlanes(GX_PLANEMASK_OBJ, TRUE);
    G2_SetBG0Priority(1);
    G2_SetBG1Priority(0);
    G2_SetBG2Priority(2);
    G2_SetBG3Priority(3);
    G2S_SetBG0Priority(0);
    G2S_SetBG1Priority(1);
    G2S_SetBG2Priority(2);
    G2S_SetBG3Priority(3);
    G2_SetBlendAlpha(GX_BLEND_PLANEMASK_BG2, GX_BLEND_PLANEMASK_BG3 | GX_BLEND_PLANEMASK_BD, 11, 10);
}

void SpinTrade_DisableDisplay(SpinTradeApp *app) {
    GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG0, FALSE);
    GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG1, FALSE);
    GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG2, FALSE);
    GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG3, FALSE);
    GfGfx_EngineATogglePlanes(GX_PLANEMASK_OBJ, FALSE);
    GfGfx_EngineBTogglePlanes(GX_PLANEMASK_BG0, FALSE);
    GfGfx_EngineBTogglePlanes(GX_PLANEMASK_BG1, FALSE);
    GfGfx_EngineBTogglePlanes(GX_PLANEMASK_BG2, FALSE);
    GfGfx_EngineBTogglePlanes(GX_PLANEMASK_BG3, FALSE);
    GfGfx_EngineBTogglePlanes(GX_PLANEMASK_OBJ, FALSE);
}

void SpinTrade_Init3D(SpinTradeApp *app) {
    int i;

    NNS_G3dInit();
    G3X_InitMtxStack();
    G3X_SetShading(GX_SHADING_TOON);
    G3X_AntiAlias(TRUE);
    G3X_AlphaTest(FALSE, 0);
    G3X_AlphaBlend(TRUE);
    G3X_EdgeMarking(TRUE);
    for (i = 0; i < 8; i++) {
        app->edgeColors[i] = GX_RGB(4, 4, 4);
    }
    G3X_SetEdgeColorTable(app->edgeColors);
    G3X_SetClearColor(GX_RGB(31, 31, 31), 0, 0x7FFF, 0x3F, FALSE);
    G3_SwapBuffers(GX_SORTMODE_AUTO, GX_BUFFERMODE_W);
    G3_ViewPort(0, 0, 255, 191);
    GF_3DVramMan_InitFrameTexVramManager(1, TRUE);
    GF_3DVramMan_InitFramePlttVramManager(0x4000, TRUE);
}

void SpinTrade_InitBgLayers(SpinTradeApp *app) {
    app->bgConfig = BgConfig_Alloc(HEAP_ID_102);
    reg_GX_POWCNT |= 0x8000; // main engine on the bottom screen
    {
        GraphicsModes modes = ov85_021EA538;
        SetBothScreensModesAndDisable(&modes);
    }
    {
        BgTemplate template = ov85_021EA60C;
        InitBgFromTemplate(app->bgConfig, GF_BG_LYR_MAIN_1, &template, GF_BG_TYPE_TEXT);
    }
    BgClearTilemapBufferAndCommit(app->bgConfig, GF_BG_LYR_MAIN_1);
    BG_ClearCharDataRange(GF_BG_LYR_MAIN_1, 0x20, 0, HEAP_ID_102);
    {
        BgTemplate template = ov85_021EA628;
        InitBgFromTemplate(app->bgConfig, GF_BG_LYR_MAIN_2, &template, GF_BG_TYPE_TEXT);
    }
    BgClearTilemapBufferAndCommit(app->bgConfig, GF_BG_LYR_MAIN_2);
    {
        BgTemplate template = ov85_021EA644;
        InitBgFromTemplate(app->bgConfig, GF_BG_LYR_MAIN_3, &template, GF_BG_TYPE_TEXT);
    }
    BgClearTilemapBufferAndCommit(app->bgConfig, GF_BG_LYR_MAIN_3);
    {
        BgTemplate template = ov85_021EA660;
        InitBgFromTemplate(app->bgConfig, GF_BG_LYR_SUB_2, &template, GF_BG_TYPE_TEXT);
    }
    BgClearTilemapBufferAndCommit(app->bgConfig, GF_BG_LYR_SUB_2);
    {
        BgTemplate template = ov85_021EA67C;
        InitBgFromTemplate(app->bgConfig, GF_BG_LYR_SUB_3, &template, GF_BG_TYPE_TEXT);
    }
    BgClearTilemapBufferAndCommit(app->bgConfig, GF_BG_LYR_SUB_3);
}

void SpinTrade_LoadBgGraphics(SpinTradeApp *app) {
    void *buf;

    buf = SpinTrade_LoadNarcMember(app, 17, FALSE);
    NNS_G2dGetUnpackedPaletteData(buf, &app->plttData);
    PaletteData_LoadPalette(app->paletteData, app->plttData->pRawData, PLTTBUF_MAIN_BG, 0, 0x40);
    Heap_Free(buf);

    buf = SpinTrade_LoadNarcMember(app, 20, FALSE);
    NNS_G2dGetUnpackedPaletteData(buf, &app->plttData);
    PaletteData_LoadPalette(app->paletteData, app->plttData->pRawData, PLTTBUF_SUB_BG, 0, 0x40);
    Heap_Free(buf);

    PaletteData_BlendPalettes(app->paletteData, PLTTBUF_SUB_BG, 0xFFFF, 8, RGB_BLACK);

    buf = SpinTrade_LoadNarcMember(app, 16, FALSE);
    NNS_G2dGetUnpackedCharacterData(buf, &app->charData);
    BG_LoadCharTilesData(app->bgConfig, GF_BG_LYR_MAIN_3, app->charData->pRawData, app->charData->szByte, 0);
    Heap_Free(buf);

    buf = SpinTrade_LoadNarcMember(app, 22, FALSE);
    NNS_G2dGetUnpackedCharacterData(buf, &app->charData);
    BG_LoadCharTilesData(app->bgConfig, GF_BG_LYR_SUB_2, app->charData->pRawData, app->charData->szByte, 0);
    Heap_Free(buf);

    buf = SpinTrade_LoadNarcMember(app, 21, FALSE);
    NNS_G2dGetUnpackedCharacterData(buf, &app->charData);
    BG_LoadCharTilesData(app->bgConfig, GF_BG_LYR_SUB_3, app->charData->pRawData, app->charData->szByte, 0);
    Heap_Free(buf);

    buf = SpinTrade_LoadNarcMember(app, 19, FALSE);
    NNS_G2dGetUnpackedScreenData(buf, &app->screenData);
    BG_LoadScreenTilemapData(app->bgConfig, GF_BG_LYR_MAIN_2, app->screenData->rawData, app->screenData->szByte);
    BgCommitTilemapBufferToVram(app->bgConfig, GF_BG_LYR_MAIN_2);
    Heap_Free(buf);

    buf = SpinTrade_LoadNarcMember(app, 18, FALSE);
    NNS_G2dGetUnpackedScreenData(buf, &app->screenData);
    BG_LoadScreenTilemapData(app->bgConfig, GF_BG_LYR_MAIN_3, app->screenData->rawData, app->screenData->szByte);
    BgCommitTilemapBufferToVram(app->bgConfig, GF_BG_LYR_MAIN_3);
    Heap_Free(buf);

    buf = SpinTrade_LoadNarcMember(app, 24, FALSE);
    NNS_G2dGetUnpackedScreenData(buf, &app->screenData);
    BG_LoadScreenTilemapData(app->bgConfig, GF_BG_LYR_SUB_2, app->screenData->rawData, app->screenData->szByte);
    BgCommitTilemapBufferToVram(app->bgConfig, GF_BG_LYR_SUB_2);
    Heap_Free(buf);

    buf = SpinTrade_LoadNarcMember(app, 23, FALSE);
    NNS_G2dGetUnpackedScreenData(buf, &app->screenData);
    BG_LoadScreenTilemapData(app->bgConfig, GF_BG_LYR_SUB_3, app->screenData->rawData, app->screenData->szByte);
    BgCommitTilemapBufferToVram(app->bgConfig, GF_BG_LYR_SUB_3);
    Heap_Free(buf);
}

void SpinTrade_FreeBgLayers(SpinTradeApp *app) {
    FreeBgTilemapBuffer(app->bgConfig, GF_BG_LYR_MAIN_1);
    FreeBgTilemapBuffer(app->bgConfig, GF_BG_LYR_MAIN_2);
    FreeBgTilemapBuffer(app->bgConfig, GF_BG_LYR_MAIN_3);
    FreeBgTilemapBuffer(app->bgConfig, GF_BG_LYR_SUB_2);
    FreeBgTilemapBuffer(app->bgConfig, GF_BG_LYR_SUB_3);
    Heap_Free(app->bgConfig);
}

void SpinTrade_LoadButtonTilemap(SpinTradeApp *app, u32 member) {
    void *buf = SpinTrade_LoadNarcMember(app, member, FALSE);
    NNS_G2dGetUnpackedScreenData(buf, &app->screenData);
    BG_LoadScreenTilemapData(app->bgConfig, GF_BG_LYR_SUB_2, app->screenData->rawData, app->screenData->szByte);
    BgCommitTilemapBufferToVram(app->bgConfig, GF_BG_LYR_SUB_2);
    Heap_Free(buf);
}

void SpinTrade_SetButtonPressed(SpinTradeApp *app) {
    SpinTrade_LoadButtonTilemap(app, 26);
}

void SpinTrade_SetButtonReleased(SpinTradeApp *app) {
    SpinTrade_LoadButtonTilemap(app, 24);
}

void SpinTrade_SetButtonHalfPressed(SpinTradeApp *app) {
    SpinTrade_LoadButtonTilemap(app, 25);
}

void SpinTrade_InitPaletteData(SpinTradeApp *app) {
    app->paletteData = PaletteData_Init(HEAP_ID_102);
    PaletteData_SetAutoTransparent(app->paletteData, TRUE);
    PaletteData_AllocBuffers(app->paletteData, PLTTBUF_MAIN_BG, 0x200, HEAP_ID_102);
    PaletteData_AllocBuffers(app->paletteData, PLTTBUF_MAIN_OBJ, 0x200, HEAP_ID_102);
    PaletteData_AllocBuffers(app->paletteData, PLTTBUF_SUB_BG, 0x200, HEAP_ID_102);
    PaletteData_AllocBuffers(app->paletteData, PLTTBUF_SUB_OBJ, 0x200, HEAP_ID_102);
}

void SpinTrade_FreePaletteData(SpinTradeApp *app) {
    PaletteData_FreeBuffers(app->paletteData, PLTTBUF_MAIN_BG);
    PaletteData_FreeBuffers(app->paletteData, PLTTBUF_MAIN_OBJ);
    PaletteData_FreeBuffers(app->paletteData, PLTTBUF_SUB_BG);
    PaletteData_FreeBuffers(app->paletteData, PLTTBUF_SUB_OBJ);
    PaletteData_Free(app->paletteData);
}

void SpinTrade_SetSubBgFadeLevel(SpinTradeApp *app, u32 level) {
    PaletteData_BlendPalettes(app->paletteData, PLTTBUF_SUB_BG, 0xFFFF, level, RGB_BLACK);
}

void SpinTrade_InitSpriteSystem(SpinTradeApp *app) {
    OamManagerParam oamParam = ov85_021EA6B8;
    OamCharTransferParam transferParam = ov85_021EA56C;
    SpriteResourceCountsListUnion counts;

    app->spriteSystem = SpriteSystem_Alloc(HEAP_ID_102);
    SpriteSystem_Init(app->spriteSystem, &oamParam, &transferParam, 0x20);
    counts = ov85_021EA594;
    app->spriteManager = SpriteManager_New(app->spriteSystem);
    if (!SpriteSystem_InitSprites(app->spriteSystem, app->spriteManager, 0xFF)) {
        GF_AssertFail();
    }
    if (!SpriteSystem_InitManagerWithCapacities(app->spriteSystem, app->spriteManager, &counts)) {
        GF_AssertFail();
    }
}

void SpinTrade_LoadSpriteResources(SpinTradeApp *app) {
    SpriteSystem *spriteSystem = app->spriteSystem;
    SpriteManager *spriteManager = app->spriteManager;
    PaletteData *paletteData = app->paletteData;
    NARC *narc;
    void *raw;
    NNSG2dPaletteData *plttData;

    G2dRenderer_SetPlttTransferReservedRegion(NNS_G2D_VRAM_TYPE_2DMAIN);
    narc = NARC_New(NARC_a_2_1_5, HEAP_ID_102);
    SpriteSystem_LoadCharResObjFromOpenNarc(spriteSystem, spriteManager, narc, 16, FALSE, NNS_G2D_VRAM_TYPE_2DMAIN, 4);
    SpriteSystem_LoadPaletteBufferFromOpenNarc(paletteData, PLTTBUF_MAIN_OBJ, spriteSystem, spriteManager, narc, 15, FALSE, 1, NNS_G2D_VRAM_TYPE_2DMAIN, 5);
    SpriteSystem_LoadCellResObjFromOpenNarc(spriteSystem, spriteManager, narc, 17, FALSE, 6);
    SpriteSystem_LoadAnimResObjFromOpenNarc(spriteSystem, spriteManager, narc, 18, FALSE, 7);
    NARC_Delete(narc);
    WirelessStatusIcon_Create();
    raw = WirelessStatusIcon_LoadPaletteFile(HEAP_ID_102);
    NNS_G2dGetUnpackedPaletteData(raw, &plttData);
    PaletteData_LoadPalette(paletteData, plttData->pRawData, PLTTBUF_MAIN_OBJ, 14 * 16, 0x20);
    Heap_Free(raw);
}

void SpinTrade_FreeSpriteSystem(SpinTradeApp *app) {
    SpriteSystem_FreeResourcesAndManager(app->spriteSystem, app->spriteManager);
    SpriteSystem_Free(app->spriteSystem);
}

ManagedSprite *SpinTrade_CreateSprite(SpinTradeApp *app, const ManagedSpriteTemplate *template) {
    return SpriteSystem_NewSprite(app->spriteSystem, app->spriteManager, template);
}

#pragma force_active on
void SpinTrade_InitText(SpinTradeApp *app) {
    SpinTradeText *text = &app->text;
    int i;

    LoadUserFrameGfx1(app->bgConfig, GF_BG_LYR_MAIN_1, 1, 15, 0, HEAP_ID_102);
    LoadUserFrameGfx2(app->bgConfig, GF_BG_LYR_MAIN_1, 10, 14, app->args->input.frame, HEAP_ID_102);
    PaletteData_LoadNarc(app->paletteData, NARC_a_0_3_8, GetUserFrameGfx2PaletteMember(app->args->input.frame), HEAP_ID_102, PLTTBUF_MAIN_BG, 0x20, 14 * 16);
    PaletteData_LoadNarc(app->paletteData, NARC_graphic_font, 8, HEAP_ID_102, PLTTBUF_MAIN_BG, 0x20, 15 * 16);
    text->msgData = NewMsgDataFromNarc(MSGDATA_LOAD_DIRECT, NARC_msgdata_msg, NARC_msg_msg_0206_bin, HEAP_ID_102);
    text->msgFormat = MessageFormat_New(HEAP_ID_102);
    AddWindow(app->bgConfig, &text->msgWindow, &ov85_021EA4FC);
    text->string = String_New(256, HEAP_ID_102);
    for (i = 0; i < SPIN_TRADE_MAX_PLAYERS; i++) {
        app->names[i] = String_New(PLAYER_NAME_LENGTH + 1, HEAP_ID_102);
    }
    SpinTrade_AddNameWindows(app, app->args->unk0[2]);
}
#pragma force_active reset

void SpinTrade_FreeText(SpinTradeApp *app) {
    SpinTradeText *text = &app->text;
    Window *window = &text->msgWindow;
    int i;

    ClearWindowTilemapAndCopyToVram(window);
    RemoveWindow(window);
    SpinTrade_RemoveNameWindows(app);
    DestroyMsgData(text->msgData);
    MessageFormat_Delete(text->msgFormat);
    String_Delete(text->string);
    for (i = 0; i < SPIN_TRADE_MAX_PLAYERS; i++) {
        String_Delete(app->names[i]);
    }
}

void SpinTrade_PrintMessage(SpinTradeApp *app, int msgNo) {
    SpinTradeText *text = &app->text;
    Window *window = &text->msgWindow;

    FillWindowPixelBuffer(window, 15);
    DrawFrameAndWindow2(window, TRUE, 10, 14);
    FillWindowPixelBuffer(window, 15);
    ReadMsgDataIntoString(text->msgData, msgNo, text->string);
    AddTextPrinterParameterized(window, 1, text->string, 0, 0, TEXT_SPEED_NOTRANSFER, NULL);
    ScheduleWindowCopyToVram(window);
}

void SpinTrade_PrintPlayerMessage(SpinTradeApp *app, int msgNo, PlayerProfile *profile) {
    SpinTradeText *text = &app->text;
    Window *window = &text->msgWindow;
    String *template;

    BufferPlayersName(text->msgFormat, 1, CommPlayerInfo_GetProfile(Comm_GetLocalNetId()));
    BufferPlayersName(text->msgFormat, 2, profile);
    template = String_New(256, HEAP_ID_102);
    ReadMsgDataIntoString(text->msgData, msgNo, template);
    StringExpandPlaceholders(text->msgFormat, text->string, template);
    String_Delete(template);
    DrawFrameAndWindow2(window, TRUE, 10, 14);
    FillWindowPixelBuffer(window, 15);
    AddTextPrinterParameterized(window, 1, text->string, 0, 0, TEXT_SPEED_NOTRANSFER, NULL);
    ScheduleWindowCopyToVram(window);
}

void SpinTrade_PrintItemMessage(SpinTradeApp *app, int msgNo, int item) {
    SpinTradeText *text = &app->text;
    Window *window = &text->msgWindow;
    String *template;

    BufferItemName(text->msgFormat, 0, item);
    template = String_New(256, HEAP_ID_102);
    ReadMsgDataIntoString(text->msgData, msgNo, template);
    StringExpandPlaceholders(text->msgFormat, text->string, template);
    String_Delete(template);
    DrawFrameAndWindow2(window, TRUE, 10, 14);
    FillWindowPixelBuffer(window, 15);
    AddTextPrinterParameterized(window, 1, text->string, 0, 0, TEXT_SPEED_NOTRANSFER, NULL);
    ScheduleWindowCopyToVram(window);
}

void SpinTrade_ClearMessage(SpinTradeApp *app) {
    Window *window = &app->text.msgWindow;

    ClearFrameAndWindow2(window, TRUE);
    FillWindowPixelBuffer(window, 0);
    ScheduleWindowCopyToVram(window);
}

#pragma force_active on
void SpinTrade_AddNameWindows(SpinTradeApp *app, int count) {
    int i;
    SpinTradeText *text = &app->text;
    const WindowTemplate *templates = ov85_021EA5AC[count];

    text->nameWindowCount = count;
    for (i = 0; i < count; i++) {
        AddWindow(app->bgConfig, &text->nameWindows[i], &templates[i]);
    }
}

void SpinTrade_RemoveNameWindows(SpinTradeApp *app) {
    int i;
    SpinTradeText *text = &app->text;

    for (i = 0; i < text->nameWindowCount; i++) {
        ClearWindowTilemapAndCopyToVram(&text->nameWindows[i]);
        RemoveWindow(&text->nameWindows[i]);
    }
}
#pragma force_active reset

void SpinTrade_PrintNameWindow(SpinTradeApp *app, String *name, int index, u32 color) {
    Window *window = &app->text.nameWindows[index];
    s32 x;

    DrawFrameAndWindow1(window, TRUE, 1, 14);
    FillWindowPixelBuffer(window, 15);
    x = FontID_String_GetCenterAlignmentX(0, name, 0, 64);
    AddTextPrinterParameterizedWithColor(window, 0, name, x, 0, TEXT_SPEED_NOTRANSFER, color, NULL);
    ScheduleWindowCopyToVram(window);
}

// The local player's name is printed in a different color
void SpinTrade_PrintPlayerName(SpinTradeApp *app, String *name, int index, u32 netId) {
    u32 color = MAKE_TEXT_COLOR(1, 2, 0);
    if (netId == Comm_GetLocalNetId()) {
        color = MAKE_TEXT_COLOR(3, 4, 0);
    }
    SpinTrade_PrintNameWindow(app, name, index, color);
}

void SpinTrade_ClearNameWindow(SpinTradeApp *app, int index) {
    Window *window = &app->text.nameWindows[index];

    ClearFrameAndWindow2(window, TRUE);
    FillWindowPixelBuffer(window, 0);
    ScheduleWindowCopyToVram(window);
}

#pragma force_active on
void SpinTrade_InitCamera(SpinTradeApp *app) {
    SpinTradeCamera *camera = &app->camera;
    VecFx32 up;

    camera->camera = Camera_New(HEAP_ID_102);
    camera->target.x = 0;
    camera->target.y = 0;
    camera->target.z = 0;
    camera->angle.x = 0xE93F;
    camera->angle.y = 0;
    camera->angle.z = 0;
    app->camera.distance = FX32_CONST(323);
    camera->perspective = 0x444;
    Camera_Init_FromTargetDistanceAndAngle(&camera->target, camera->distance, &camera->angle, camera->perspective, 0, TRUE, camera->camera);
    up.x = 0;
    up.y = FX32_ONE;
    up.z = 0;
    Camera_SetLookAtCamUp(&up, camera->camera);
    Camera_SetStaticPtr(camera->camera);
}
#pragma force_active reset

void SpinTrade_FreeCamera(SpinTradeApp *app) {
    Camera_Delete(app->camera.camera);
}

#pragma force_active on
// Takes the newest valid touch sample of this frame
void SpinTrade_UpdateTouch(SpinTradeApp *app) {
    int i = 3;
    u32 held = 0;
    SpinTradeTouch *touch = &app->touch;

    GF_TouchpadUpdate(&touch->samples, 2, 0);
    do {
        if (touch->samples.samples[i].touch == 1) {
            touch->x = touch->samples.samples[i].x;
            touch->y = touch->samples.samples[i].y;
            held = 1;
        }
        i--;
    } while (i >= 0);
    touch->pressed = (touch->held ^ held) & held;
    touch->held = held;
}
#pragma force_active reset

BOOL SpinTrade_IsInButtonArea(SpinTradeTouch *touch) {
    s32 x = touch->x;
    s32 y = touch->y;
    if (y >= 32 && y <= 159 && x >= 64 && x <= 191) {
        return TRUE;
    }
    return FALSE;
}

BOOL SpinTrade_IsButtonTouched(SpinTradeApp *app) {
    SpinTradeTouch *touch = &app->touch;
    if (touch->pressed && SpinTrade_IsInButtonArea(touch) == 1) {
        return TRUE;
    }
    return FALSE;
}

#pragma force_active on
void SpinTrade_InitMachine(SpinTradeApp *app) {
    SpinTradeMachine *machine = &app->machine;
    int playerCount = app->args->unk0[2];

    RenderModel_LoadFromNarc(&machine->model, 0, app->narc, ov85_021EA5C4[playerCount], HEAP_ID_102, FALSE);
    RenderModel_BindTexturesOnVBlank(&machine->model);
    RenderObj_InitFromModel(&machine->renderObj, &machine->model);
    machine->angle = 0;
    machine->angleOffset = ov85_021EA5DC[playerCount];
    machine->scale.x = FX32_ONE;
    machine->scale.y = FX32_ONE;
    machine->scale.z = FX32_ONE;
    machine->rotation[0] = 0;
    machine->rotation[1] = 0;
    machine->rotation[2] = 0;
    machine->position.x = 0;
    machine->position.y = FX32_CONST(-36);
    machine->position.z = 0;
    machine->drawPosition = machine->position;
}
#pragma force_active reset

void SpinTrade_FreeMachine(SpinTradeApp *app) {
    RenderModel_Free(&app->machine.model);
}

#pragma force_active on
void SpinTrade_UpdateMachinePosition(SpinTradeApp *app) {
    SpinTradeMachine *machine = &app->machine;
    fx32 angle = machine->angle;

    SpinTrade_AddAngle(&angle, machine->baseAngle);
    SpinTrade_AddAngle(&angle, machine->angleOffset);
    machine->rotation[1] = (360 - angle / FX32_ONE) % 360;
    machine->drawPosition.x = machine->shakeOffset.x + (machine->position.x + machine->jumpOffset.x);
    machine->drawPosition.y = machine->shakeOffset.y + (machine->position.y + machine->jumpOffset.y);
    machine->drawPosition.z = machine->shakeOffset.z + (machine->position.z + machine->jumpOffset.z);
}
#pragma force_active reset

void SpinTrade_DrawMachine(SpinTradeApp *app) {
    SpinTradeMachine *machine = &app->machine;
    RenderObj_DrawRotated(&machine->renderObj, &machine->drawPosition, &machine->scale, machine->rotation);
}

void SpinTradeMachine_Rotate(SpinTradeMachine *machine, fx32 delta) {
    SpinTrade_AddAngle(&machine->angle, delta);
}

void SpinTrade_LoadObjects(SpinTradeApp *app) {
    SpinTrade_LoadObjectModels(app);
}

void SpinTrade_FreeObjects(SpinTradeApp *app) {
    SpinTrade_FreeObjectModels(app);
}

#pragma force_active on
void SpinTrade_LoadObjectModels(SpinTradeApp *app) {
    volatile int i; // volatile: matches the original loop codegen
    const int *modelIds;
    const int *animIds;
    UnkOv01_021FFECC_sub *model;
    SpinTradeRenderAnim *anim;
    UnkOv01_021FFECC_sub *animModel;
    SpinTradeObjects *objects = &app->objects;

    RenderModel_LoadFromNarc(&objects->eggModel, 0, app->narc, 0, HEAP_ID_102, FALSE);
    RenderModel_BindTexturesOnVBlank(&objects->eggModel);
    RenderModel_LoadFromNarc(&objects->shadowModel, 0, app->narc, 5, HEAP_ID_102, FALSE);
    RenderModel_BindTexturesOnVBlank(&objects->shadowModel);
    model = objects->markerModels;
    modelIds = ov85_021EA558;
    animIds = ov85_021EA580;
    i = 0;
    anim = objects->markerAnims;
    animModel = model;
    do {
        int next;
        RenderModel_LoadFromNarc(model, 0, app->narc, *modelIds, HEAP_ID_102, FALSE);
        RenderModel_BindTexturesOnVBlank(model);
        RenderAnimation_LoadFromNarc(anim, 0, app->narc, *animIds, HEAP_ID_102, FALSE);
        RenderAnimation_AllocObjectForModel(anim, animModel, HEAP_ID_102);
        RenderAnimation_InitObjectForModel(anim, animModel);
        next = i + 1;
        model++;
        i = next;
        anim++;
        modelIds++;
        animModel++;
        animIds++;
    } while (i < 5);
}

void SpinTrade_FreeObjectModels(SpinTradeApp *app) {
    int i;
    SpinTradeObjects *objects = &app->objects;

    RenderModel_Free(&objects->eggModel);
    RenderModel_Free(&objects->shadowModel);
    for (i = 0; i < 5; i++) {
        RenderModel_Free(&objects->markerModels[i]);
        RenderAnimation_FreeFile(&objects->markerAnims[i]);
    }
}
#pragma force_active reset

void SpinTrade_ShowEgg(SpinTradeApp *app, SpinTradeEgg *egg) {
    RenderObj_InitFromModel(&egg->renderObj, &app->objects.eggModel);
    egg->visible = TRUE;
    SpinTrade_ShowShadow(app, egg);
    SpinTrade_ShowMarker(app, egg);
}

#pragma force_active on
void SpinTrade_UpdateEggPositions(SpinTradeApp *app) {
    int i = 0;
    int count = app->playerCount;
    SpinTradeObjects *objects = &app->objects;

    for (; i < count; i++) {
        if (objects->eggs[i].active) {
            SpinTradeEgg_UpdatePosition(&objects->eggs[i], &app->machine.jumpOffset);
        }
        if (objects->shadows[i].active) {
            SpinTradeShadow_Update(app, &objects->shadows[i]);
        }
        if (objects->markers[i].active) {
            SpinTradeMarker_Update(app, &objects->markers[i]);
        }
    }
}
#pragma force_active reset

void SpinTrade_DrawEggs(SpinTradeApp *app) {
    int count = app->playerCount;
    int i = 0;
    SpinTradeObjects *objects = &app->objects;
    SpinTradeEgg *egg = objects->eggs;
    SpinTradeShadow *shadow = objects->shadows;
    SpinTradeMarker *marker = objects->markers;

    if (count > 0) {
        do {
            if (egg->visible) {
                RenderObj_DrawRotated(&egg->renderObj, &egg->position, &egg->scale, egg->rotation);
            }
            if (shadow->active) {
                RenderObj_DrawRotated(&shadow->renderObj, &shadow->position, &shadow->scale, shadow->rotation);
            }
            if (marker->active) {
                RenderObj_DrawAt((int)&marker->renderObj, &marker->position);
            }
            i++;
            egg++;
            shadow++;
            marker++;
        } while (i < count);
    }
}

void SpinTrade_RotateEggs(SpinTradeApp *app, fx32 delta) {
    int count = app->playerCount;
    int i = 0;
    SpinTradeObjects *objects = &app->objects;

    for (; i < count; i++) {
        if (objects->eggs[i].active) {
            SpinTrade_AddAngle(&objects->eggs[i].angle, delta);
        }
    }
}

// Eggs sit on a circle of radius 22 around the table and follow its jump offset
void SpinTradeEgg_UpdatePosition(SpinTradeEgg *egg, const VecFx32 *machineOffset) {
    fx32 angle = egg->angle;
    u16 degrees;

    SpinTrade_AddAngle(&angle, egg->angleOffset);
    degrees = angle / FX32_ONE;
    egg->position.x = egg->offset.x + machineOffset->x + GF_CosDegNoWrap(degrees) * 22;
    egg->position.y = egg->offset.y - FX32_CONST(6) + machineOffset->y;
    egg->position.z = egg->offset.z + machineOffset->z + GF_SinDegNoWrap(degrees) * 22;
    egg->rotation[0] = egg->rotationFx[0] / FX32_ONE;
    egg->rotation[1] = egg->rotationFx[1] / FX32_ONE;
    egg->rotation[2] = egg->rotationFx[2] / FX32_ONE;
}

#pragma force_active on
// The egg that stopped closest (turning forward) to this player's seat
SpinTradeEgg *SpinTrade_FindEggNearestSlot(SpinTradeApp *app) {
    int distances[SPIN_TRADE_MAX_PLAYERS];
    int order[SPIN_TRADE_MAX_PLAYERS];
    int j;
    int keyDistance;
    int key;
    int i;
    fx32 target;
    fx32 distance;
    int count;
    SpinTradeObjects *objects;

    count = app->playerCount;
    objects = &app->objects;
    i = 0;
    target = ov85_021EA7C4[count][app->mySlot] << FX32_SHIFT;
    do {
        order[i] = i;
        distances[i] = 0xFFFF;
        i++;
    } while (i < count);
    for (i = 0; i < count; i++) {
        distance = target - objects->eggs[i].angle;
        if (distance < 0) {
            distance = target + SPIN_TRADE_ANGLE_FULL - objects->eggs[i].angle;
        }
        distances[i] = distance / FX32_ONE;
    }
    // insertion sort by distance
    for (i = 1; i < count; i++) {
        key = order[i];
        keyDistance = distances[key];
        for (j = i - 1; j >= 0 && distances[order[j]] > keyDistance; j--) {
            order[j + 1] = order[j];
        }
        order[j + 1] = key;
    }
    return &objects->eggs[order[0]];
}
#pragma force_active reset

void SpinTrade_ShowShadow(SpinTradeApp *app, SpinTradeEgg *egg) {
    SpinTradeObjects *objects = &app->objects;
    SpinTradeShadow *shadow = &objects->shadows[egg->slot];

    shadow->active = TRUE;
    shadow->egg = egg;
    RenderObj_InitFromModel(&shadow->renderObj, &objects->shadowModel);
    SpinTradeShadow_Update(app, shadow);
}

void SpinTradeShadow_Update(SpinTradeApp *app, SpinTradeShadow *shadow) {
    s32 shrink;

    shadow->rotation[0] = 0;
    shadow->rotation[1] = 0;
    shadow->rotation[2] = 0;
    shadow->position = shadow->egg->position;
    shadow->position.x -= shadow->egg->offset.x;
    shadow->position.y -= shadow->egg->offset.y;
    shadow->position.z -= shadow->egg->offset.z;
    shadow->position.y += FX32_CONST(2);
    shrink = shadow->egg->offset.y;
    if (shrink < 0) {
        shrink = -shrink;
    }
    shrink /= 128;
    if (shrink > FX32_HALF) {
        shrink = FX32_HALF;
    }
    shadow->scale.x = FX32_ONE - shrink;
    shadow->scale.y = FX32_ONE - shrink;
    shadow->scale.z = FX32_ONE - shrink;
}

void SpinTrade_ShowMarker(SpinTradeApp *app, SpinTradeEgg *egg) {
    SpinTradeObjects *objects = &app->objects;
    SpinTradeMarker *marker = &objects->markers[egg->slot];

    marker->active = TRUE;
    marker->egg = egg;
    RenderObj_InitWithAnimation(&marker->renderObj, &objects->markerModels[egg->slot], &objects->markerAnims[egg->slot]);
}

void SpinTradeMarker_Update(SpinTradeApp *app, SpinTradeMarker *marker) {
    marker->position = marker->egg->position;
    marker->position.y += FX32_CONST(20);
    RenderAnimation_Advance(&app->objects.markerAnims[marker->egg->slot], FX32_ONE, TRUE);
}

void SpinTrade_StartEggDrop(SpinTradeApp *app, u32 netId, int slot, int order, int angle, u32 angleOffset, SpinTradeEgg *egg) {
    SpinTradeEggDropTask *task;

    egg->netId = netId;
    egg->slot = slot;
    egg->order = order;
    egg->unk_18 = slot;
    egg->angle = angle << FX32_SHIFT;
    egg->angleOffset = angleOffset << FX32_SHIFT;
    egg->active = TRUE;
    egg->dropped = FALSE;
    task = Heap_AllocAtEnd(HEAP_ID_102, sizeof(SpinTradeEggDropTask));
    memset(task, 0, sizeof(SpinTradeEggDropTask));
    task->app = app;
    task->egg = egg;
    SysTask_CreateOnMainQueue(SpinTrade_EggDropTask, task, 0);
}

void SpinTrade_EggDropTask(SysTask *sysTask, void *data) {
    SpinTradeEggDropTask *task = data;
    int ret;

    do {
        ret = ov85_021EA51C[task->state](task);
    } while (ret == 1);
    if (ret == 2) {
        task->egg->dropped = TRUE;
        Heap_Free(task);
        SysTask_Destroy(sysTask);
    }
}

int SpinTradeEggDrop_Init(SpinTradeEggDropTask *task) {
    SpinTradeEgg *egg = task->egg;

    egg->scale.x = FX32_ONE;
    egg->scale.y = FX32_ONE;
    egg->scale.z = FX32_ONE;
    egg->offset.y = FX32_CONST(192);
    SpinTrade_ShowEgg(task->app, egg);
    task->speed = FX32_CONST(8);
    task->state = 1;
    return 0;
}

int SpinTradeEggDrop_Fall(SpinTradeEggDropTask *task) {
    SpinTradeEgg *egg = task->egg;

    egg->offset.y -= task->speed;
    if (task->landingSePlayed == FALSE && egg->offset.y <= FX32_CONST(104)) {
        task->landingSePlayed = TRUE;
        PlaySE(SEQ_SE_DP_FW104);
    }
    if (egg->offset.y <= FX32_CONST(32)) {
        egg->offset.y = FX32_CONST(32);
        task->state = 2;
    }
    return 0;
}

#pragma force_active on
int SpinTradeEggDrop_Land(SpinTradeEggDropTask *task) {
    SpinTradeEgg *egg = task->egg;

    if (task->speed > FX32_ONE) {
        task->speed -= FX32_ONE;
        if (task->speed < FX32_ONE) {
            task->speed = FX32_ONE;
        }
    }
    egg->offset.y -= task->speed;
    if (egg->offset.y <= 0) {
        egg->offset.y = 0;
        SpinTrade_PrintPlayerName(task->app, task->app->names[egg->netId], egg->order, egg->netId);
        return 2;
    }
    return 0;
}
#pragma force_active reset

void SpinTradeBounce_Update(SpinTradeApp *app, SpinTradeBounce *bounce) {
    SpinTradeEgg *egg = bounce->egg;

    switch (bounce->state) {
    case 0:
        SpinTrade_AddSpin(app, egg);
        bounce->velocity = FX32_CONST(4);
        bounce->accel = -0x444;
        bounce->state++;
    case 1:
        egg->offset.y += bounce->velocity;
        bounce->velocity += bounce->accel;
        bounce->frame++;
        if (bounce->frame >= 15) {
            bounce->frame = 0;
            bounce->velocity = 0;
            if (bounce->bounced == FALSE) {
                bounce->bounced = TRUE;
                return;
            }
            egg->offset.y = 0;
            SpinTrade_AddWobble(app, egg);
            memset(bounce, 0, sizeof(SpinTradeBounce));
        }
        break;
    }
}

void SpinTrade_BounceTask(SysTask *task, void *data) {
    SpinTradeApp *app = data;
    s32 i;
    SpinTradeBounce *bounce = app->bounces;

    for (i = 0; i < 5; i++, bounce++) {
        if (bounce->active) {
            SpinTradeBounce_Update(app, bounce);
        }
    }
}

#pragma force_active on
void SpinTrade_StartBounceTask(SpinTradeApp *app) {
    memset(app->bounces, 0, sizeof(app->bounces));
    app->bounceTask = SysTask_CreateOnMainQueue(SpinTrade_BounceTask, app, 256);
    GF_ASSERT(app->bounceTask != NULL);
}
#pragma force_active reset

void SpinTrade_StopBounceTask(SpinTradeApp *app) {
    SysTask_Destroy(app->bounceTask);
}

void SpinTrade_AddBounce(SpinTradeApp *app, SpinTradeEgg *egg) {
    s32 i;
    SpinTradeBounce *bounce = app->bounces;

    for (i = 0; i < 5; i++, bounce++) {
        if (bounce->active == FALSE) {
            bounce->active = TRUE;
            bounce->egg = egg;
            return;
        }
    }
    GF_AssertFail();
}

#pragma force_active on
void SpinTrade_BounceAllEggs(SpinTradeApp *app) {
    SpinTradeObjects *objects = &app->objects;
    int count = app->playerCount;
    int i = 0;

    do {
        if (objects->eggs[i].active) {
            SpinTrade_AddBounce(app, &objects->eggs[i]);
        }
        i++;
    } while (i < count);
}
#pragma force_active reset

BOOL SpinTrade_IsAnyEggBouncing(SpinTradeApp *app) {
    s32 i;
    SpinTradeBounce *bounce = app->bounces;

    for (i = 0; i < 5; i++, bounce++) {
        if (bounce->active) {
            return TRUE;
        }
    }
    return FALSE;
}

#pragma force_active on
void SpinTradeSpin_Update(SpinTradeSpin *spin) {
    SpinTradeEgg *egg = spin->egg;

    switch (spin->state) {
    case 0:
        spin->step = FX32_CONST(24);
        egg->rotationFx[1] = 0;
        spin->state++;
    case 1:
        SpinTrade_AddAngle(&egg->rotationFx[1], spin->step);
        if (egg->rotationFx[1] < FX32_CONST(4)) {
            spin->step = (u32)spin->step >> 1;
            if (spin->step < FX32_CONST(4)) {
                egg->rotationFx[1] = 0;
                memset(spin, 0, sizeof(SpinTradeSpin));
            }
        }
        break;
    }
}
#pragma force_active reset

void SpinTrade_SpinTask(SysTask *task, void *data) {
    SpinTradeApp *app = data;
    s32 i;
    SpinTradeSpin *spin = app->spins;

    for (i = 0; i < 5; i++, spin++) {
        if (spin->active) {
            SpinTradeSpin_Update(spin);
        }
    }
}

#pragma force_active on
void SpinTrade_StartSpinTask(SpinTradeApp *app) {
    memset(app->spins, 0, sizeof(app->spins));
    app->spinTask = SysTask_CreateOnMainQueue(SpinTrade_SpinTask, app, 257);
    if (app->spinTask == NULL) {
        GF_AssertFail();
    }
}
#pragma force_active reset

void SpinTrade_StopSpinTask(SpinTradeApp *app) {
    SysTask_Destroy(app->spinTask);
}

#pragma force_active on
// Restarts the spin of an egg that is already spinning
void SpinTrade_AddSpin(SpinTradeApp *app, SpinTradeEgg *egg) {
    int i;
    SpinTradeSpin *scan;
    SpinTradeSpin *spin;

    spin = app->spins;
    scan = spin;
    for (i = 0; i < 5; i++, scan++) {
        if (scan->active == TRUE && scan->egg == egg) {
            scan->state = 0;
            return;
        }
    }
    for (i = 0; i < 5; i++, spin++) {
        if (spin->active == FALSE) {
            spin->active = TRUE;
            spin->egg = egg;
            return;
        }
    }
    GF_AssertFail();
}

// Rocks the egg from side to side with a decaying amplitude
void SpinTradeWobble_Update(SpinTradeWobble *wobble) {
    SpinTradeEgg *egg = wobble->egg;
    fx32 oldPhase;

    switch (wobble->state) {
    case 0:
        egg->rotationFx[2] = 0;
        wobble->phase = 0;
        wobble->amplitude = FX32_CONST(30);
        wobble->speed = FX32_CONST(20);
        wobble->state++;
    case 1: {
        int sine = GF_SinDegNoWrap((u16)(wobble->phase / FX32_ONE));
        int amplitude = wobble->amplitude / FX32_ONE;
        int value = amplitude * sine;
        egg->rotationFx[2] = 0;
        SpinTrade_AddAngle(&egg->rotationFx[2], value);
    }
        oldPhase = wobble->phase;
        SpinTrade_AddAngle(&wobble->phase, wobble->speed);
        if (wobble->phase < oldPhase) {
            wobble->amplitude = (u32)wobble->amplitude >> 1;
            if (wobble->amplitude == 0) {
                egg->rotationFx[2] = 0;
                memset(wobble, 0, sizeof(SpinTradeWobble));
            }
        }
        break;
    }
}
#pragma force_active reset

void SpinTrade_WobbleTask(SysTask *task, void *data) {
    SpinTradeApp *app = data;
    s32 i;
    SpinTradeWobble *wobble = app->wobbles;

    for (i = 0; i < 5; i++, wobble++) {
        if (wobble->active) {
            SpinTradeWobble_Update(wobble);
        }
    }
}

#pragma force_active on
void SpinTrade_StartWobbleTask(SpinTradeApp *app) {
    memset(app->wobbles, 0, sizeof(app->wobbles));
    app->wobbleTask = SysTask_CreateOnMainQueue(SpinTrade_WobbleTask, app, 258);
    if (app->wobbleTask == NULL) {
        GF_AssertFail();
    }
}
#pragma force_active reset

void SpinTrade_StopWobbleTask(SpinTradeApp *app) {
    SysTask_Destroy(app->wobbleTask);
}

#pragma force_active on
// Restarts the wobble of an egg that is already wobbling
void SpinTrade_AddWobble(SpinTradeApp *app, SpinTradeEgg *egg) {
    int i = 0;
    SpinTradeWobble *scan;
    SpinTradeWobble *wobble;

    wobble = app->wobbles;
    scan = wobble;
    do {
        if (scan->active == TRUE && scan->egg == egg) {
            scan->state = 0;
            return;
        }
        i++;
        scan++;
    } while (i < 5);
    i = 0;
    do {
        if (wobble->active == FALSE) {
            wobble->active = TRUE;
            wobble->egg = egg;
            return;
        }
        i++;
        wobble++;
    } while (i < 5);
    GF_ASSERT(FALSE);
}

// The machine dips, jumps back up (all eggs bounce on landing), shakes and waits for the eggs to settle
void SpinTradeJump_Update(SpinTradeApp *app, SpinTradeJump *jump) {
    SpinTradeMachine *machine = &app->machine;

    switch (jump->state) {
    case 1:
        jump->shake = FX32_CONST(4);
        jump->counter = 0;
        jump->state++;
    case 2:
        machine->jumpOffset.y -= FX32_ONE;
        if (machine->jumpOffset.y <= FX32_CONST(-8)) {
            jump->state++;
        }
        break;
    case 3:
        machine->jumpOffset.y += FX32_CONST(2);
        if (machine->jumpOffset.y >= 0) {
            machine->jumpOffset.y = 0;
            SpinTrade_BounceAllEggs(app);
            jump->landed = TRUE;
            jump->state++;
            PlaySE(SEQ_SE_DP_DANSA5);
        }
        break;
    case 4:
        machine->shakeOffset.y = jump->shake;
        jump->shake = -jump->shake;
        jump->counter++;
        if (!(jump->counter & 3)) {
            if (jump->shake <= FX32_ONE) {
                machine->shakeOffset.y = 0;
                jump->state++;
            } else {
                jump->shake >>= 1;
            }
        }
        break;
    case 5:
        if (!SpinTrade_IsAnyEggBouncing(app)) {
            jump->active = FALSE;
            jump->landed = FALSE;
            jump->state = 0;
            PlaySE(SEQ_SE_DP_SUTYA);
        }
        break;
    }
}
#pragma force_active reset

void SpinTrade_JumpTask(SysTask *task, void *data) {
    SpinTradeApp *app = data;
    SpinTradeJump_Update(app, &app->jump);
}

#pragma force_active on
void SpinTrade_StartJumpTask(SpinTradeApp *app) {
    memset(&app->jump, 0, sizeof(app->jump));
    app->jumpTask = SysTask_CreateOnMainQueue(SpinTrade_JumpTask, app, 255);
    GF_ASSERT(app->jumpTask);
}
#pragma force_active reset

void SpinTrade_StopJumpTask(SpinTradeApp *app) {
    SysTask_Destroy(app->jumpTask);
}

void SpinTrade_StartJump(SpinTradeApp *app) {
    SpinTradeJump *jump = &app->jump;
    if (jump->landed) {
        GF_AssertFail();
    }
    jump->active = TRUE;
    jump->state = 1;
}

BOOL SpinTrade_HasJumpLanded(SpinTradeApp *app) {
    return app->jump.landed;
}

BOOL SpinTrade_IsJumping(SpinTradeApp *app) {
    return app->jump.active;
}

// "3, 2, 1, go" sprite animation before the table starts turning
void SpinTrade_CountdownTask(SysTask *task, void *data) {
    SpinTradeCountdownTask *countdown = data;

    switch (countdown->state) {
    case 0:
        if (countdown->soundIndex < 4) {
            SpinTradeCountdownSounds sounds = ov85_021EA528;
            countdown->timer--;
            if (countdown->timer <= 0) {
                countdown->timer = 30;
                PlaySE(sounds.sound[countdown->soundIndex]);
                countdown->soundIndex++;
            }
        }
        ManagedSprite_TickNFrames(countdown->sprite, FX32_CONST(1.5));
        if (!ManagedSprite_IsAnimated(countdown->sprite)) {
            countdown->state++;
        }
        break;
    case 1:
        Sprite_DeleteAndFreeResources(countdown->sprite);
        Heap_Free(countdown);
        SysTask_Destroy(task);
        break;
    }
}

#pragma force_active on
SysTask *SpinTrade_StartCountdown(SpinTradeApp *app) {
    SpinTradeCountdownTask *countdown;
    ManagedSpriteTemplate template;
    SysTask *task;

    countdown = Heap_AllocAtEnd(HEAP_ID_102, sizeof(SpinTradeCountdownTask));
    memset(countdown, 0, sizeof(SpinTradeCountdownTask));
    template.x = 128;
    template.y = 100;
    template.z = 0;
    template.animation = 0;
    template.vram = NNS_G2D_VRAM_TYPE_2DMAIN;
    template.resIdList[0] = 4;
    template.resIdList[1] = 5;
    template.drawPriority = 0;
    template.pal = 0;
    template.bgPriority = 0;
    template.vramTransfer = 0;
    template.resIdList[2] = 6;
    template.resIdList[3] = 7;
    template.resIdList[4] = -1;
    template.resIdList[5] = -1;
    countdown->sprite = SpinTrade_CreateSprite(app, &template);
    ManagedSprite_TickFrame(countdown->sprite);
    countdown->app = app;
    task = SysTask_CreateOnMainQueue(SpinTrade_CountdownTask, countdown, 0);
    GF_ASSERT(task != NULL);
    return task;
}
#pragma force_active reset

void SpinTradeButtonFlash_Update(SpinTradeApp *app, SpinTradeButtonFlash *flash) {
    switch (flash->state) {
    case 1:
        SpinTrade_SetButtonPressed(app);
        PlaySE(SEQ_SE_DP_BUTTON9);
        flash->timer = 0;
        flash->state++;
    case 2:
        flash->timer++;
        if (flash->timer >= 2) {
            flash->timer = 0;
            flash->state++;
            SpinTrade_SetButtonHalfPressed(app);
        }
        break;
    case 3:
        flash->timer++;
        if (flash->timer >= 4) {
            SpinTrade_SetButtonPressed(app);
            flash->timer = 0;
            flash->state++;
        }
        break;
    case 4:
        flash->timer++;
        if (flash->timer >= 2) {
            SpinTrade_SetButtonReleased(app);
            flash->active = 0;
            flash->state = 0;
        }
        break;
    }
}

void SpinTrade_ButtonFlashTask(SysTask *task, void *data) {
    SpinTradeApp *app = data;
    SpinTradeButtonFlash_Update(app, &app->buttonFlash);
}

#pragma force_active on
void SpinTrade_StartButtonFlashTask(SpinTradeApp *app) {
    memset(&app->buttonFlash, 0, sizeof(app->buttonFlash));
    app->buttonFlashTask = SysTask_CreateOnMainQueue(SpinTrade_ButtonFlashTask, app, 259);
    if (app->buttonFlashTask == NULL) {
        GF_AssertFail();
    }
}
#pragma force_active reset

void SpinTrade_StopButtonFlashTask(SpinTradeApp *app) {
    SysTask_Destroy(app->buttonFlashTask);
}

u16 SpinTrade_IsButtonFlashing(SpinTradeApp *app) {
    return app->buttonFlash.active;
}

void SpinTrade_StartButtonFlash(SpinTradeApp *app) {
    SpinTradeButtonFlash *flash = &app->buttonFlash;
    if (flash->active != 0) {
        GF_AssertFail();
    }
    flash->active = 1;
    flash->state = 1;
}

#pragma force_active on
void SpinTradeHover_Update(SpinTradeApp *app, SpinTradeHover *hover) {
    switch (hover->state) {
    case 0:
        break;
    case 1:
        hover->egg->offset.y = _021EAA80[hover->index] << FX32_SHIFT;
        hover->index++;
        if (hover->index >= 16) {
            hover->index = 0;
            hover->state++;
        }
        break;
    case 2:
        hover->timer++;
        if (hover->timer >= 15) {
            hover->timer = 0;
            hover->state = 1;
        }
        break;
    }
}
#pragma force_active reset

void SpinTrade_HoverTask(SysTask *task, void *data) {
    SpinTradeApp *app = data;
    s32 i;
    SpinTradeHover *hover = app->hovers;

    for (i = 0; i < 5; i++, hover++) {
        SpinTradeHover_Update(app, hover);
    }
}

void SpinTrade_StartHoverTask(SpinTradeApp *app) {
    memset(app->hovers, 0, sizeof(app->hovers));
    app->hoverTask = SysTask_CreateOnMainQueue(SpinTrade_HoverTask, app, 260);
    if (app->hoverTask == NULL) {
        GF_AssertFail();
    }
}

void SpinTrade_StopHoverTask(SpinTradeApp *app) {
    SysTask_Destroy(app->hoverTask);
}

void SpinTrade_AddHover(SpinTradeApp *app, SpinTradeEgg *egg) {
    SpinTradeHover *hover;

    hover = &app->hovers[egg->netId];
    if (egg->netId < 5) {
    } else {
        GF_AssertFail();
    }
    if (hover->state != 0) {
        GF_AssertFail();
    }
    hover->state = 1;
    hover->egg = egg;
}

#pragma force_active on
// Eggs that stopped in a bonus area float up and down
void SpinTrade_HoverBonusEggs(SpinTradeApp *app) {
    u32 mask = app->bonusMask;
    int count = app->playerCount;
    int i = 0;
    SpinTradeObjects *objects = &app->objects;

    for (; i < count; i++) {
        if (objects->eggs[i].active && ((1u << objects->eggs[i].netId) & mask)) {
            SpinTrade_AddHover(app, &objects->eggs[i]);
        }
    }
}
#pragma force_active reset

void SpinTrade_SubBgFadeTask(SysTask *task, void *data) {
    SpinTradeFadeTask *fade = data;
    BOOL done = FALSE;

    if (fade->fadeOut == 1) {
        fade->level--;
        if (fade->level <= 0) {
            fade->level = 0;
            done = TRUE;
        }
    } else {
        fade->level++;
        if (fade->level >= 8) {
            fade->level = 8;
            done = TRUE;
        }
    }
    SpinTrade_SetSubBgFadeLevel(fade->app, fade->level);
    if (done == TRUE) {
        Heap_Free(fade);
        SysTask_Destroy(task);
    }
}

void SpinTrade_StartSubBgFade(SpinTradeApp *app, int fadeOut) {
    s32 startLevels[2];
    SpinTradeFadeTask *fade;

    startLevels[0] = ov85_021EA4F0[7];
    startLevels[1] = ov85_021EA4F0[8];
    fade = Heap_AllocAtEnd(HEAP_ID_102, sizeof(SpinTradeFadeTask));
    fade->app = app;
    fade->fadeOut = fadeOut;
    fade->level = startLevels[fadeOut];
    if (SysTask_CreateOnMainQueue(SpinTrade_SubBgFadeTask, fade, 0) == NULL) {
        GF_AssertFail();
    }
}

void SpinTrade_AddAngle(fx32 *angle, fx32 delta) {
    *angle += delta;
    while (*angle < 0) {
        *angle += SPIN_TRADE_ANGLE_FULL;
    }
    *angle %= SPIN_TRADE_ANGLE_FULL;
}

void SpinTrade_RotateAll(SpinTradeApp *app, fx32 delta) {
    SpinTradeMachine_Rotate(&app->machine, delta);
    SpinTrade_RotateEggs(app, delta);
}

BOOL SpinTrade_TickSpinTimer(SpinTradeApp *app) {
    app->frame++;
    if (app->frame < 600) {
        return FALSE;
    }
    app->frame = 600;
    return TRUE;
}

void *SpinTrade_LoadNarcMember(SpinTradeApp *app, u32 member, int atStart) {
    void *buf;
    u32 size = NARC_GetMemberSize(app->narc, member);
    if (atStart == 1) {
        buf = Heap_Alloc(HEAP_ID_102, size);
    } else {
        buf = Heap_AllocAtEnd(HEAP_ID_102, size);
    }
    NARC_ReadWholeMember(app->narc, member, buf);
    return buf;
}

// Link receive callbacks (registered by frontier_menu_communication.c)
void SpinTrade_ReceiveJump(SpinTradeApp *app, u8 jump) {
    if (jump != 0) {
        app->remoteJump = jump;
    }
}

void SpinTrade_ReceiveSpinData(SpinTradeApp *app, const u16 *data) {
    app->spinReceived = TRUE;
    app->spinData.unk_0 = data[0];
    app->spinData.frame = data[1];
    app->spinData.speed = data[2];
    app->spinData.angle = data[3];
}

void *SpinTrade_ClearScratch(SpinTradeApp *app, u32 size) {
    if (size >= sizeof(app->scratch)) {
        GF_AssertFail();
    }
    memset(app->scratch, 0, sizeof(app->scratch));
    return app->scratch;
}

void *SpinTrade_GetScratch(SpinTradeApp *app) {
    return app->scratch;
}

BOOL SpinTrade_HasReceivedStatus(SpinTradeApp *app, u32 status) {
    SpinTradeLinkMenu *menu = app->menu;
    if ((menu->receivedStatus & status) != 0) {
        return TRUE;
    }
    return FALSE;
}

BOOL SpinTrade_HaveAllMembersJoined(SpinTradeApp *app) {
    SpinTradeLinkMenu *menu = app->menu;
    SpinTradeArgs *args = app->args;
    u32 count = 0;
    s32 needed = args->unk0[2] - 1;
    u32 mask = menu->memberMask;

    while (mask != 0) {
        count += mask & 1;
        mask >>= 1;
    }
    if ((s32)count >= needed) {
        return TRUE;
    }
    return FALSE;
}

int SpinTrade_CountMembers(SpinTradeApp *app) {
    SpinTradeLinkMenu *menu = app->menu;
    u32 count = 0;
    u32 mask = menu->memberMask;

    while (mask != 0) {
        count += mask & 1;
        mask >>= 1;
    }
    return count;
}

#pragma force_active on
void SpinTrade_ReceiveMemberSlot(SpinTradeApp *app, SpinTradeMember *member) {
    volatile SpinTradeMember *received = member; // volatile: every field read is a separate load
    int index = received->slot;
    app->members[index].slot = received->slot;
    app->members[index].netId = received->netId;
    if (received->netId == Comm_GetLocalNetId()) {
        app->mySlot = received->slot;
    }
}
#pragma force_active reset

void SpinTrade_ReceivePlayerCount(SpinTradeApp *app, u32 count) {
    app->playerCount = count;
}

BOOL SpinTrade_IsMemberPresent(SpinTradeApp *app, u32 netId) {
    if (netId != 0) {
        if ((app->menu->memberMask & (1 << netId)) == 0) {
            return FALSE;
        }
    }
    return TRUE;
}

// Replaces the offered party slot with the Pokemon (egg) offered by netId
void SpinTrade_ReceiveTradedMon(SpinTradeApp *app, int netId) {
    Party *party = app->party;
    Party *otherParty = SpinTradeLink_GetReceivedParty(app->menu, netId);
    int otherSlot = app->tradeSlots[netId];
    Pokemon *dest = Party_GetMonByIndex(party, app->args->unk0[1]);
    Pokemon *mon = Party_GetMonByIndex(otherParty, otherSlot);

    MonSetTrainerMemo(mon, CommPlayerInfo_GetProfile(Comm_GetLocalNetId()), 5, 0, HEAP_ID_FIELD2);
    CopyPokemonToPokemon(mon, dest);
}

int SpinTrade_CountReceivedParties(SpinTradeApp *app) {
    SpinTradeLinkMenu *menu = app->menu;
    u32 count = 0;
    u32 mask = menu->partyReceivedMask;

    while (mask != 0) {
        count += mask & 1;
        mask >>= 1;
    }
    return count;
}

void SpinTrade_ReceiveTradeSlot(SpinTradeApp *app, int netId, u32 slot) {
    app->tradeSlots[netId] = slot;
}

void SpinTrade_ReceiveResult(SpinTradeApp *app, const u16 *src) {
    u32 i;
    u16 *dst;

    dst = app->result;
    i = 8;
    do {
        *dst = *src++;
        dst++;
    } while (--i != 0);
    app->resultReceived = TRUE;
}

#pragma force_active on
BOOL SpinTrade_IsInBonusArea(SpinTradeApp *app, fx32 angle, int playerCount) {
    s32 position;
    u32 a, count;
    s32 b;
    const SpinTradeAngleRange *range;

    SpinTrade_AddAngle(&angle, -app->machine.angle);
    position = angle;
    SpinTrade_AddAngle(&position, FX32_CONST(-4));
    a = position / FX32_ONE;
    position = angle;
    SpinTrade_AddAngle(&position, FX32_CONST(5));
    b = position / FX32_ONE;
    {
        const SpinTradeBonusAreas *areas = &ov85_021EA758[playerCount];
        count = areas->count;
        range = areas->ranges;
    }
    while (count) {
        if ((a >= range->low && a <= range->high) || ((u32)b >= range->low && (u32)b <= range->high)) {
            return TRUE;
        }
        range++;
        count--;
    }
    return FALSE;
}
#pragma force_active reset

BOOL SpinTrade_PartyHasBadEgg(Party *party) {
    Pokemon *mon;
    int i;
    int count = Party_GetCount(party);

    for (i = 0; i < count; i++) {
        mon = Party_GetMonByIndex(party, i);
        if (GetMonData(mon, MON_DATA_IS_EGG, NULL) == 0) {
            continue;
        }
        if (GetMonData(mon, MON_DATA_CHECKSUM_FAILED, NULL) == 0) {
            continue;
        }
        return TRUE;
    }
    return FALSE;
}

BOOL SpinTrade_AnyPartyHasBadEgg(SpinTradeApp *app) {
    int i;

    if (SpinTrade_PartyHasBadEgg(app->party) == 1) {
        return TRUE;
    }
    for (i = 0; i < SPIN_TRADE_MAX_PLAYERS; i++) {
        if (SpinTrade_IsMemberPresent(app, i)) {
            if (SpinTrade_PartyHasBadEgg(SpinTradeLink_GetReceivedParty(app->menu, i)) == 1) {
                return TRUE;
            }
        }
    }
    return FALSE;
}

int SpinTrade_CountConfirmed(SpinTradeApp *app) {
    SpinTradeLinkMenu *menu = app->menu;
    u32 count = 0;
    u32 mask = menu->confirmMask;

    while (mask != 0) {
        count += mask & 1;
        mask >>= 1;
    }
    return count;
}

BOOL SpinTrade_IsCancelled(SpinTradeApp *app) {
    SpinTradeLinkMenu *menu = app->menu;
    if (menu->cancelled != 0) {
        return TRUE;
    }
    return FALSE;
}
