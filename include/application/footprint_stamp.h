#ifndef POKEHEARTGOLD_APPLICATION_FOOTPRINT_STAMP_H
#define POKEHEARTGOLD_APPLICATION_FOOTPRINT_STAMP_H

// Wi-Fi Plaza Footprint Stamp (OVY_89): the players stamp the footprints of their party Pokemon onto a shared
// 3D board. A stamp sits still for a moment, then plays the effect of its Pokemon's nature; stamps it touches
// take over the same effect, and a long enough chain moves the camera on every player's screen.

#include "bg_window.h"
#include "camera.h"
#include "gf_3d_vramman.h"
#include "message_format.h"
#include "msgdata.h"
#include "obj_char_transfer.h"
#include "overlay_manager.h"
#include "palette.h"
#include "save.h"
#include "sprite_system.h"
#include "unk_02013534.h"
#include "yes_no_prompt.h"

#define FOOTPRINT_STAMP_PARTY_SLOTS   6
#define FOOTPRINT_STAMP_PLAYERS_MAX   8
#define FOOTPRINT_STAMP_STAMPS_MAX    128
#define FOOTPRINT_STAMP_TEXTURES_MAX  32
#define FOOTPRINT_STAMP_INK_MAX       0x2000
#define FOOTPRINT_STAMP_INK_PER_STAMP 0x555

// FootprintStamp_GetTouchedPartySlot results besides a party slot
#define FOOTPRINT_STAMP_TOUCH_EXIT 0xFE
#define FOOTPRINT_STAMP_TOUCH_NONE 0xFF

// Effect of a stamp (from its Pokemon's nature) and the matching camera move of a completed chain.
enum FootprintStampEffect {
    FOOTPRINT_STAMP_EFFECT_NONE,
    FOOTPRINT_STAMP_EFFECT_VANISH,   // fades out while growing; camera zooms in
    FOOTPRINT_STAMP_EFFECT_SPLIT,    // splits into four; camera rolls
    FOOTPRINT_STAMP_EFFECT_ZIGZAG,   // turns, then zigzags away; camera turns left
    FOOTPRINT_STAMP_EFFECT_WAVE,     // turns, then waves away; camera turns right
    FOOTPRINT_STAMP_EFFECT_RISE,     // rises leaving a trail; camera pitches up
    FOOTPRINT_STAMP_EFFECT_FALL,     // sways downwards; camera sways
    FOOTPRINT_STAMP_EFFECT_GROW,     // grows; camera zooms out
    FOOTPRINT_STAMP_EFFECT_SPREAD_X, // two copies spread sideways; camera shakes sideways
    FOOTPRINT_STAMP_EFFECT_SPREAD_Y, // two copies spread up and down; camera shakes up and down
    FOOTPRINT_STAMP_EFFECT_STRETCH,  // stretches; camera pitches down
};

// Launch args (the lobby's overlay template for OVY_89).
typedef struct FootprintStampArgs {
    void *lobby; // ov45 Wi-Fi Plaza lobby state
    u8 unk04;    // selects the board model, palettes and ink color column
} FootprintStampArgs;

// A party Pokemon as shown in the party bar, and the 12-byte stamp record sent to the other players.
typedef struct FootprintStampMon {
    u16 color; // ink color
    u16 species;
    u32 personality;
    u8 form;
    u8 x; // touch position of the stamp
    u8 y;
    u8 padding;
} FootprintStampMon;

// Same layout as ModelResource / ModelInstance in src/model_animation.c (no header declares them).
typedef struct FootprintStampModel {
    NNSG3dResFileHeader *file;
    NNSG3dResMdlSet *set;
    NNSG3dResMdl *model;
    NNSG3dResTex *texture;
} FootprintStampModel;

typedef struct FootprintStampModelInstance {
    NNSG3dRenderObj render;
    VecFx32 position;
    VecFx32 scale;
    BOOL visible;
    u16 rotation[3];
} FootprintStampModelInstance;

// Board-space bounding box of a stamp (FootprintStamp_GetBounds).
typedef struct FootprintStampBounds {
    fx32 left;
    fx32 right;
    fx32 bottom;
    fx32 top;
} FootprintStampBounds;

// Per-effect state of a stamp (FootprintStampObj.effect).
typedef struct FootprintStampIdle {
    int timer;
    FootprintStampBounds bounds;
} FootprintStampIdle;

typedef struct FootprintStampVanish {
    int alpha;
    fx32 scale;
    u8 state;
    u8 polygonId;
    u16 padding;
    FootprintStampBounds bounds;
} FootprintStampVanish;

typedef struct FootprintStampSplit {
    FootprintStampModelInstance pieces[3];
    u16 timer;
    u8 state;
    u8 padding;
    FootprintStampBounds bounds[4];
} FootprintStampSplit;

typedef struct FootprintStampZigzag {
    int state;
    int timer;
    int step;
    FootprintStampBounds bounds;
} FootprintStampZigzag;

typedef struct FootprintStampWave {
    int state;
    fx32 angle;
    int timer;
    fx32 baseY;
    FootprintStampBounds bounds;
} FootprintStampWave;

typedef struct FootprintStampRise {
    FootprintStampModelInstance trail[3];
    u16 timer;
    u8 trailIndex;
    u8 stop;
    u8 polygonId;
    u8 state;
    u8 padding[2];
    FootprintStampBounds bounds;
} FootprintStampRise;

typedef struct FootprintStampFall {
    int state;
    fx32 angle;
    fx32 baseX;
    FootprintStampBounds bounds;
} FootprintStampFall;

typedef struct FootprintStampGrow {
    fx32 scale;
    u16 timer;
    u8 state;
    u8 padding;
    FootprintStampBounds bounds;
} FootprintStampGrow;

typedef struct FootprintStampSpread {
    FootprintStampModelInstance copies[2];
    fx32 angle;
    u16 alpha;
    u8 polygonId;
    u8 state;
    FootprintStampBounds bounds[3];
} FootprintStampSpread;

typedef struct FootprintStampStretch {
    fx32 scale;
    u16 timer;
    u8 state;
    u8 padding;
    FootprintStampBounds bounds;
} FootprintStampStretch;

// One stamp on the board (0x244 bytes).
typedef struct FootprintStampObj {
    FootprintStampMon mon;
    FootprintStampModel model;
    FootprintStampModelInstance instance;

    union {
        FootprintStampIdle idle;
        FootprintStampVanish vanish;
        FootprintStampSplit split;
        FootprintStampZigzag zigzag;
        FootprintStampWave wave;
        FootprintStampRise rise;
        FootprintStampFall fall;
        FootprintStampGrow grow;
        FootprintStampSpread spread;
        FootprintStampStretch stretch;
        u8 raw[0x1AC];
    } effect;

    u32 ready : 4;          // set once the stamp is visible; only ready stamps take over an effect
    u32 textureChanged : 4; // the footprint texture was redrawn and is queued for upload
    u32 effectId : 8;       // running enum FootprintStampEffect
    u32 pendingEffect : 8;
    u32 chain : 8; // FootprintStampBoard.chains index, 0xFF = none
} FootprintStampObj;

typedef struct FootprintStampBoard FootprintStampBoard;

typedef struct FootprintStampEffectFuncs {
    BOOL (*update)(FootprintStampBoard *board, FootprintStampObj *stamp);
    void (*draw)(FootprintStampBoard *board, FootprintStampObj *stamp);
    BOOL (*hit)(FootprintStampBoard *board, FootprintStampObj *stamp, FootprintStampObj *other);
} FootprintStampEffectFuncs;

// Camera moves played when a chain completes (FootprintStampBoard.cameraEffect).
typedef struct FootprintStampZoom {
    fx32 distance;
    s16 timer;
    u8 state;
} FootprintStampZoom;

typedef struct FootprintStampRoll {
    VecFx32 up;
    u8 state;
    u8 phase;
    u8 count;
} FootprintStampRoll;

typedef struct FootprintStampTurn {
    CameraAngle angle;
    s32 current;
    s16 timer;
    u8 state;
} FootprintStampTurn;

typedef struct FootprintStampSway {
    CameraAngle angle;
    fx32 phase;
    u8 count;
    u8 state;
} FootprintStampSway;

typedef struct FootprintStampShake {
    VecFx32 target;
    VecFx32 position;
    fx32 phase;
    u8 count;
    u8 state;
} FootprintStampShake;

typedef struct FootprintStampFlash {
    u8 state;
    u8 count;
    s16 brightness;
} FootprintStampFlash;

typedef struct FootprintStampChain {
    u8 size; // stamps that took over the chain's effect
    u8 refs; // stamps on the board that belong to the chain
    u8 padding[2];
} FootprintStampChain;

// The stamp board (0x744 bytes, FootprintStamp.board).
struct FootprintStampBoard {
    FootprintStampZoom zoomIn;
    FootprintStampRoll roll;
    FootprintStampTurn turnLeft;
    FootprintStampTurn turnRight;
    FootprintStampTurn pitchUp;
    FootprintStampSway sway;
    FootprintStampZoom zoomOut;
    FootprintStampShake shakeX;
    FootprintStampShake shakeY;
    FootprintStampTurn pitchDown;
    FootprintStampFlash flash;
    FootprintStampObj *stamps[FOOTPRINT_STAMP_STAMPS_MAX];
    FootprintStampObj *sorted[FOOTPRINT_STAMP_STAMPS_MAX];       // scratch of FootprintStampBoard_ResolveChains
    NNSG3dResTex *pendingTextures[FOOTPRINT_STAMP_TEXTURES_MAX]; // uploaded in the next VBlank
    u32 polygonIds[2];
    FootprintStampChain chains[FOOTPRINT_STAMP_STAMPS_MAX];
    u8 cameraEffect;
    u8 pendingCameraEffect;
    u8 flashDone;
    u8 playerCount;
    u8 brightnessChanged;
    u8 unk741;
    s16 brightness;
};

typedef struct FootprintStampTextSprite {
    TextOBJ *textObj;
    UnkStruct_02021AC8 charTransfer;
    u16 width;
    u16 unk12;
} FootprintStampTextSprite;

// Filter for the lobby's link commands: only other players' commands, and only while playing.
typedef struct FootprintStampLinkFilter {
    BOOL active;
    u32 myId;
} FootprintStampLinkFilter;

// App state (0x19E4 bytes).
typedef struct FootprintStamp {
    FootprintStampArgs *args;
    SaveData *saveData;
    BgConfig *bgConfig;
    PaletteData *plttData;
    UnkStruct_02013534 *fontSystem;
    GF3DVramMan *vramMan3d;
    SysTask *task;
    SpriteSystem *spriteSystem;
    SpriteManager *spriteManager;
    YesNoPrompt *yesNoPrompt;
    u8 yesNoActive;
    u8 closeTimer;
    u8 unk2A[2];
    MessageFormat *msgFormat;
    MsgData *msgData;
    Window playerNameWindows[FOOTPRINT_STAMP_PLAYERS_MAX];
    Window msgWindow;
    String *string;
    u8 printerId;
    u8 unkC9[3];
    Camera *camera;
    fx32 viewWidth;
    fx32 viewHeight;
    FootprintStampModel boardModel;
    FootprintStampModelInstance boardInstance;
    NARC *narc;
    NARC *footprintNarc;
    FootprintStampLinkFilter linkFilter;
    u32 playerIds[FOOTPRINT_STAMP_PLAYERS_MAX];
    int nameTimer;
    FootprintStampBoard board;
    FootprintStampMon party[FOOTPRINT_STAMP_PARTY_SLOTS];
    u8 selectedSlot;
    u8 cursorPlttSlot;
    u8 unk922[2];
    ManagedSprite *inkSprites[FOOTPRINT_STAMP_PARTY_SLOTS];
    ManagedSprite *slotSprites[FOOTPRINT_STAMP_PARTY_SLOTS];
    ManagedSprite *footprintSprites[FOOTPRINT_STAMP_PARTY_SLOTS];
    ManagedSprite *myCursorSprite;
    ManagedSprite *playerFootprintSprites[FOOTPRINT_STAMP_PLAYERS_MAX];
    ManagedSprite *selectSprites[3];
    u16 playerSpecies[FOOTPRINT_STAMP_PLAYERS_MAX];
    u16 playerColors[FOOTPRINT_STAMP_PLAYERS_MAX];
    int phase;
    u16 subTilemap[0x400]; // the player list tilemap, copied into BG 6 when a player joins
    u8 unk11C0[0x800];
    FootprintStampTextSprite exitText;
    fx32 ink;
    fx32 inkShown;
    fx32 inkStep;
    void *unk19E0;
} FootprintStamp;

// app (footprint_stamp.c)
BOOL FootprintStamp_Init(OverlayManager *man, int *state);
BOOL FootprintStamp_Main(OverlayManager *man, int *state);
BOOL FootprintStamp_Exit(OverlayManager *man, int *state);
void FootprintStamp_MainTask(SysTask *task, void *data);
void FootprintStamp_VBlank(void *data);
void FootprintStamp_InitBgs(BgConfig *bgConfig);
void FootprintStamp_FreeBgs(BgConfig *bgConfig);
void FootprintStamp_LoadBgGfx(FootprintStamp *work, NARC *narc);
void FootprintStamp_LoadPartySpriteGfx(FootprintStamp *work, NARC *narc);
void FootprintStamp_CreatePartySprites(FootprintStamp *work);
void FootprintStamp_FreePartySprites(FootprintStamp *work);
void FootprintStamp_LoadPlayerSpriteGfx(FootprintStamp *work, NARC *narc);

// footprint_stamp_sprites.c
void FootprintStamp_CreatePlayerSprites(FootprintStamp *work);

// screens, party, players, ink, stamp objects (footprint_stamp_board.c)
void FootprintStamp_FreePlayerSprites(FootprintStamp *work);
void FootprintStamp_ShowParty(FootprintStamp *work);
BOOL FootprintStamp_LoadFootprintIcon(int species, int form, ManagedSprite *sprite, NARC *narc, NARC *footprintNarc, int screen, void *unk19E0);
void FootprintStamp_CreateCamera(FootprintStamp *work);
void FootprintStamp_FreeCamera(FootprintStamp *work);
void FootprintStamp_LoadBoardModel(FootprintStamp *work, NARC *narc);
void FootprintStamp_FreeBoardModel(FootprintStamp *work);
void FootprintStamp_Draw3D(FootprintStamp *work);
void FootprintStamp_CreateWindows(FootprintStamp *work);
void FootprintStamp_FreeWindows(FootprintStamp *work);
GF3DVramMan *FootprintStamp_Create3DVramMan(enum HeapID heapId);
void FootprintStamp_Setup3D(void);
void FootprintStamp_Free3DVramMan(GF3DVramMan *vramMan);
void FootprintStamp_InitLinkFilter(FootprintStamp *work);
void FootprintStampMon_InitDummy(void *unused, FootprintStampMon *mon);
FootprintStampLinkFilter *FootprintStamp_GetLinkFilter(FootprintStamp *work);
void FootprintStamp_LoadParty(u32 inkColumn, SaveData *saveData, FootprintStampMon *party);
BOOL FootprintStamp_PlaceStamp(FootprintStamp *work, FootprintStampMon *mon, u32 playerId);
int FootprintStamp_UpdatePlayers(FootprintStamp *work);
void FootprintStamp_ShowPlayerName(FootprintStamp *work, u32 playerId);
void FootprintStamp_ClearPlayerName(FootprintStamp *work, u32 index);
void FootprintStamp_StartSelectEffect(FootprintStamp *work, int slot);
void FootprintStamp_HighlightPartySlot(FootprintStamp *work, int slot);
void FootprintStamp_UpdateSelectEffects(FootprintStamp *work);
void FootprintStamp_CreateTextSprite(FootprintStamp *work, FootprintStampTextSprite *text, String *string, int font, int color, int priority, int plttResId, int x, int y, BOOL centered);
void FootprintStamp_FreeTextSprite(FootprintStampTextSprite *text);
void FootprintStamp_GetTextSize(String *string, int font, int *width, int *tiles);
void FootprintStamp_UpdateInkGauge(FootprintStamp *work);
BOOL FootprintStamp_UseInk(FootprintStamp *work, fx32 amount);
void ov89_0225A468(FootprintStamp *work);
void FootprintStampBoard_Init(FootprintStampBoard *board, void *unk19E0);
void FootprintStampBoard_Free(FootprintStampBoard *board);
BOOL FootprintStampBoard_AddStamp(FootprintStamp *work, FootprintStampBoard *board, const FootprintStampMon *mon, NARC *narc, NARC *footprintNarc, void *unk19E0);
void FootprintStampBoard_Update(FootprintStampBoard *board, Camera *camera, int phase, u32 inkColumn);
void FootprintStampBoard_Draw(FootprintStampBoard *board);
void FootprintStampBoard_ResolveChains(FootprintStampBoard *board);
void FootprintStampBoard_VBlank(FootprintStampBoard *board, int phase);
FootprintStampObj *FootprintStampBoard_CreateStamp(FootprintStamp *work, FootprintStampBoard *board, const FootprintStampMon *mon, NARC *narc, NARC *footprintNarc, void *unk19E0);
BOOL FootprintStamp_LoadStampModel(FootprintStampBoard *board, FootprintStampModel *model, NARC *narc, NARC *footprintNarc, FootprintStampMon *mon, void *unk19E0);
BOOL FootprintStampBoard_QueueTexture(FootprintStampBoard *board, NNSG3dResTex *texture);
void FootprintStampBoard_UnqueueTexture(FootprintStampBoard *board, NNSG3dResTex *texture);
BOOL FootprintStamp_AllocTextureVram(NNSG3dResTex *texture);

// footprint_stamp_texture.c
void FootprintStamp_DrawFootprintTexture(NNSG3dResTex *texture, NARC *narc, NARC *footprintNarc, FootprintStampMon *mon, u32 unk19E0);

// effects, camera moves, player names, tables, link commands (footprint_stamp_effects.c)
void FootprintStamp_RotateFootprintTexture(NNSG3dResTex *texture, int direction);
void FootprintStamp_DeleteStamp(FootprintStampObj *stamp);
void FootprintStampBoard_RemoveStamp(FootprintStampBoard *board, FootprintStampObj *stamp, int slot);
void FootprintStamp_TouchToBoardPos(void *unused, int x, int y, fx32 *outX, fx32 *outY);
int FootprintStampBoard_AllocPolygonId(FootprintStampBoard *board);
void FootprintStampBoard_FreePolygonId(FootprintStampBoard *board, int id);
void FootprintStamp_GetBounds(FootprintStampModelInstance *instance, FootprintStampMon *mon, FootprintStampBounds *bounds, BOOL scaled);
BOOL FootprintStamp_BoundsOverlap(const FootprintStampBounds *a, const FootprintStampBounds *b);
void FootprintStampBoard_NewChain(FootprintStampBoard *board, FootprintStampObj *stamp);
BOOL FootprintStampBoard_AddToChain(FootprintStampBoard *board, int chain, int effect);
void FootprintStampBoard_ReleaseChain(FootprintStampBoard *board, int chain);
void FootprintStampBoard_StartCameraEffect(FootprintStampBoard *board);
BOOL FootprintStamp_SetEffect(FootprintStampObj *stamp, int effect);
u8 FootprintStamp_GetNatureEffect(FootprintStampObj *stamp);
BOOL FootprintStamp_UpdateIdle(FootprintStampBoard *board, FootprintStampObj *stamp);
BOOL FootprintStamp_HitIdle(void);
BOOL FootprintStamp_UpdateVanish(FootprintStampBoard *board, FootprintStampObj *stamp);
void FootprintStamp_DrawVanish(FootprintStampBoard *board, FootprintStampObj *stamp);
BOOL FootprintStamp_HitVanish(FootprintStampBoard *board, FootprintStampObj *stamp, FootprintStampObj *other);
BOOL FootprintStamp_UpdateSplit(FootprintStampBoard *board, FootprintStampObj *stamp);
void FootprintStamp_DrawSplit(FootprintStampBoard *board, FootprintStampObj *stamp);
BOOL FootprintStamp_HitSplit(FootprintStampBoard *board, FootprintStampObj *stamp, FootprintStampObj *other);
BOOL FootprintStamp_UpdateZigzag(FootprintStampBoard *board, FootprintStampObj *stamp);
BOOL FootprintStamp_HitZigzag(FootprintStampBoard *board, FootprintStampObj *stamp, FootprintStampObj *other);
BOOL FootprintStamp_UpdateWave(FootprintStampBoard *board, FootprintStampObj *stamp);
BOOL FootprintStamp_HitWave(FootprintStampBoard *board, FootprintStampObj *stamp, FootprintStampObj *other);
BOOL FootprintStamp_UpdateRise(FootprintStampBoard *board, FootprintStampObj *stamp);
void FootprintStamp_DrawRise(FootprintStampBoard *board, FootprintStampObj *stamp);
BOOL FootprintStamp_HitRise(FootprintStampBoard *board, FootprintStampObj *stamp, FootprintStampObj *other);
BOOL FootprintStamp_UpdateFall(FootprintStampBoard *board, FootprintStampObj *stamp);
BOOL FootprintStamp_HitFall(FootprintStampBoard *board, FootprintStampObj *stamp, FootprintStampObj *other);
BOOL FootprintStamp_UpdateGrow(FootprintStampBoard *board, FootprintStampObj *stamp);
BOOL FootprintStamp_HitGrow(FootprintStampBoard *board, FootprintStampObj *stamp, FootprintStampObj *other);
BOOL FootprintStamp_UpdateSpreadX(FootprintStampBoard *board, FootprintStampObj *stamp);
void FootprintStamp_DrawSpreadX(FootprintStampBoard *board, FootprintStampObj *stamp);
BOOL FootprintStamp_HitSpreadX(FootprintStampBoard *board, FootprintStampObj *stamp, FootprintStampObj *other);
BOOL FootprintStamp_UpdateSpreadY(FootprintStampBoard *board, FootprintStampObj *stamp);
void FootprintStamp_DrawSpreadY(FootprintStampBoard *board, FootprintStampObj *stamp);
BOOL FootprintStamp_HitSpreadY(FootprintStampBoard *board, FootprintStampObj *stamp, FootprintStampObj *other);
BOOL FootprintStamp_UpdateStretch(FootprintStampBoard *board, FootprintStampObj *stamp);
BOOL FootprintStamp_HitStretch(FootprintStampBoard *board, FootprintStampObj *stamp, FootprintStampObj *other);
BOOL FootprintStampBoard_Flash(FootprintStampBoard *board, FootprintStampFlash *flash, int phase, u32 inkColumn);
BOOL FootprintStampCamera_ZoomIn(FootprintStampBoard *unused, FootprintStampZoom *move, Camera *camera);
BOOL FootprintStampCamera_Roll(FootprintStampBoard *unused, FootprintStampBoard *board, Camera *camera);
BOOL FootprintStampCamera_TurnLeft(FootprintStampBoard *unused, FootprintStampBoard *board, Camera *camera);
BOOL FootprintStampCamera_TurnRight(FootprintStampBoard *unused, FootprintStampBoard *board, Camera *camera);
BOOL FootprintStampCamera_PitchUp(FootprintStampBoard *unused, FootprintStampBoard *board, Camera *camera);
BOOL FootprintStampCamera_Sway(FootprintStampBoard *unused, FootprintStampBoard *board, Camera *camera);
BOOL FootprintStampCamera_ZoomOut(FootprintStampBoard *unused, FootprintStampBoard *board, Camera *camera);
BOOL FootprintStampCamera_ShakeX(FootprintStampBoard *unused, FootprintStampBoard *board, Camera *camera);
BOOL FootprintStampCamera_ShakeY(FootprintStampBoard *unused, FootprintStampBoard *board, Camera *camera);
BOOL FootprintStampCamera_PitchDown(FootprintStampBoard *unused, FootprintStampBoard *board, Camera *camera);
void FootprintStamp_PrintPlayerName(MsgData *msgData, MessageFormat *msgFormat, Window *windows, void *lobby, u32 playerId);
void FootprintStamp_ClearPlayerNameWindow(Window *windows, u32 index);
u16 FootprintStamp_GetInkColor(u32 column, u32 otId);
u32 FootprintStamp_GetTouchedPartySlot(const FootprintStampMon *party, int current);
u32 FootprintStamp_GetFootprintType(u32 species, int form, u32 unk19E0);
u32 FootprintStamp_GetFootprintSize(u32 species, int form);
int FootprintStamp_RegisterLinkCommands(FootprintStamp *work);
BOOL FootprintStamp_IsFromOtherPlayer(u32 playerId, const FootprintStampLinkFilter *filter);
BOOL FootprintStamp_SendStamp(FootprintStampMon *mon);
void FootprintStamp_HandlePlayerJoined(u32 playerId, void *data, u32 unused, FootprintStamp *work);
void FootprintStamp_HandlePlayerLeft(u32 playerId, void *data, u32 unused, FootprintStamp *work);
void FootprintStamp_HandleStampReceived(u32 playerId, void *data, u32 unused, FootprintStamp *work);

#endif // POKEHEARTGOLD_APPLICATION_FOOTPRINT_STAMP_H
