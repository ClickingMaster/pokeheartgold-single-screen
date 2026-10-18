#include "global.h"

#include "constants/mmodel.h"
#include "constants/sndseq.h"
#include "constants/species.h"

#include "char_tiles.h"
#include "follow_mon.h"
#include "gf_gfx_loader.h"
#include "legendary_cinematic.h"
#include "legendary_cinematic_internal.h"
#include "screen_fade.h"
#include "sound.h"
#include "sound_playback.h"
#include "sound_system.h"

// Arceus script commands: an opcode followed by its arguments. A command returns one of these.
enum ArceusScriptResult {
    ARCEUS_SCRIPT_WAIT,     // run the same command again next frame
    ARCEUS_SCRIPT_CONTINUE, // go to the next command now
    ARCEUS_SCRIPT_YIELD,    // go to the next command next frame
};

enum ArceusScriptCmdId {
    ARCEUS_CMD_WAIT,
    ARCEUS_CMD_FADE_IN_FROM_BLACK,
    ARCEUS_CMD_FADE_OUT_TO_BLACK,
    ARCEUS_CMD_FADE_IN_FROM_WHITE,
    ARCEUS_CMD_FADE_OUT_TO_WHITE,
    ARCEUS_CMD_WAIT_FADE,
    ARCEUS_CMD_LOAD_BG,
    ARCEUS_CMD_TOGGLE_BG,
    ARCEUS_CMD_SET_BG_PRIORITY,
    ARCEUS_CMD_SET_BLEND_PLANE,
    ARCEUS_CMD_START_BLEND,
    ARCEUS_CMD_RESET_BLEND,
    ARCEUS_CMD_SAVE_BGM,
    ARCEUS_CMD_STOP_BGM,
    ARCEUS_CMD_PLAY_BGM,
    ARCEUS_CMD_SET_SPRITE_VISIBLE,
    ARCEUS_CMD_SET_SPRITE_POSITION,
    ARCEUS_CMD_SET_SPRITE_PRIORITY,
    ARCEUS_CMD_NOP,
    ARCEUS_CMD_END,
};

typedef int (*ArceusScriptCmd)(LegendaryCinematic *cinematic, const int *cmd);

static void ArceusCinematic_VBlank(ArceusCinematic *arceus);
static void ArceusCinematic_SetBanks(void);
static void ArceusCinematic_InitBgs(ArceusCinematic *arceus);
static void ArceusCinematic_FreeBgs(ArceusCinematic *arceus);
static void ArceusCinematic_InitSprites(LegendaryCinematic *cinematic);
static void ArceusCinematic_DeleteSprite(ArceusCinematic *arceus, int index);
static void ArceusCinematic_FreeSprites(ArceusCinematic *arceus);
static void ArceusCinematic_UpdateSprites(ArceusCinematic *arceus);
static void ArceusCinematic_LoadObjChar(void *src, u32 offset, u32 size);
static void ArceusCinematic_LoadObjPltt(void *src, u32 offset, u32 size);
static int ArceusCinematic_GetOverworldModel(int species, int form, int gender);
static void ArceusCinematic_LoadArceusSprite(LegendaryCinematic *cinematic);
static void ArceusCinematic_LoadBgPalette(ArceusCinematic *arceus, int bgId, int fileId);
static void ArceusCinematic_TransferBgPalettes(ArceusCinematic *arceus);

static int ArceusCinematic_Init(LegendaryCinematic *cinematic);
static int ArceusCinematic_Exit(LegendaryCinematic *cinematic);
static int ArceusCinematic_WaitFade(LegendaryCinematic *cinematic);
static int ArceusCinematic_Blend(LegendaryCinematic *cinematic);
static int ArceusCinematic_RunScript(LegendaryCinematic *cinematic);
static int ArceusScriptCmd_Wait(LegendaryCinematic *cinematic, const int *cmd);
static int ArceusScriptCmd_FadeInFromBlack(LegendaryCinematic *cinematic, const int *cmd);
static int ArceusScriptCmd_FadeOutToBlack(LegendaryCinematic *cinematic, const int *cmd);
static int ArceusScriptCmd_FadeInFromWhite(LegendaryCinematic *cinematic, const int *cmd);
static int ArceusScriptCmd_FadeOutToWhite(LegendaryCinematic *cinematic, const int *cmd);
static int ArceusScriptCmd_WaitFade(LegendaryCinematic *cinematic, const int *cmd);
static int ArceusScriptCmd_LoadBg(LegendaryCinematic *cinematic, const int *cmd);
static int ArceusScriptCmd_ToggleBg(LegendaryCinematic *cinematic, const int *cmd);
static int ArceusScriptCmd_SetBgPriority(LegendaryCinematic *cinematic, const int *cmd);
static int ArceusScriptCmd_SetBlendPlane(LegendaryCinematic *cinematic, const int *cmd);
static int ArceusScriptCmd_StartBlend(LegendaryCinematic *cinematic, const int *cmd);
static int ArceusScriptCmd_ResetBlend(LegendaryCinematic *cinematic, const int *cmd);
static int ArceusScriptCmd_SaveBgm(LegendaryCinematic *cinematic, const int *cmd);
static int ArceusScriptCmd_StopBgm(LegendaryCinematic *cinematic, const int *cmd);
static int ArceusScriptCmd_PlayBgm(LegendaryCinematic *cinematic, const int *cmd);
static int ArceusScriptCmd_SetSpriteVisible(LegendaryCinematic *cinematic, const int *cmd);
static int ArceusScriptCmd_SetSpritePosition(LegendaryCinematic *cinematic, const int *cmd);
static int ArceusScriptCmd_SetSpritePriority(LegendaryCinematic *cinematic, const int *cmd);
static int ArceusScriptCmd_Nop(LegendaryCinematic *cinematic, const int *cmd);

static const GraphicsModes sArceusCinematicGraphicsModes = {
    .dispMode = GX_DISPMODE_GRAPHICS,
    .bgMode = GX_BGMODE_0,
    .subMode = GX_BGMODE_0,
    ._2d3dMode = GX_BG0_AS_2D,
};

static int (*const sScriptCinematicSubs_Arceus[5])(LegendaryCinematic *) = {
    ArceusCinematic_Init,
    ArceusCinematic_Exit,
    ArceusCinematic_WaitFade,
    ArceusCinematic_Blend,
    ArceusCinematic_RunScript,
};

static const OamCharTransferParam sArceusCinematicOamCharTransferParam = {
    .maxTasks = 0x400,
    .sizeMain = 0x20000,
    .sizeSub = 0x4000,
    .charModeMain = GX_OBJVRAMMODE_CHAR_1D_32K,
    .charModeSub = GX_OBJVRAMMODE_CHAR_1D_32K,
};

static const SpriteResourceCountsListUnion sArceusCinematicSpriteResourceCounts = { 1, 1, 1, 1, 0, 0 };

static const BgTemplate sArceusCinematicBgTemplate1 = {
    .x = 0,
    .y = 0,
    .bufferSize = 0x800,
    .baseTile = 0,
    .size = GF_BG_SCR_SIZE_256x256,
    .colorMode = GX_BG_COLORMODE_256,
    .screenBase = GX_BG_SCRBASE_0xf000,
    .charBase = GX_BG_CHARBASE_0x20000,
    .bgExtPltt = GX_BG_EXTPLTT_01,
    .priority = 1,
    .areaOver = GX_BG_AREAOVER_XLU,
    .mosaic = FALSE,
};

static const BgTemplate sArceusCinematicBgTemplate2 = {
    .x = 0,
    .y = 0,
    .bufferSize = 0x800,
    .baseTile = 0,
    .size = GF_BG_SCR_SIZE_256x256,
    .colorMode = GX_BG_COLORMODE_256,
    .screenBase = GX_BG_SCRBASE_0xe800,
    .charBase = GX_BG_CHARBASE_0x30000,
    .bgExtPltt = GX_BG_EXTPLTT_23,
    .priority = 2,
    .areaOver = GX_BG_AREAOVER_XLU,
    .mosaic = FALSE,
};

static const BgTemplate sArceusCinematicBgTemplate0 = {
    .x = 0,
    .y = 0,
    .bufferSize = 0x800,
    .baseTile = 0,
    .size = GF_BG_SCR_SIZE_256x256,
    .colorMode = GX_BG_COLORMODE_256,
    .screenBase = GX_BG_SCRBASE_0xf800,
    .charBase = GX_BG_CHARBASE_0x10000,
    .bgExtPltt = GX_BG_EXTPLTT_01,
    .priority = 0,
    .areaOver = GX_BG_AREAOVER_XLU,
    .mosaic = FALSE,
};

static const OamManagerParam sArceusCinematicOamManagerParam = {
    .fromOBJmain = 0,
    .numOBJmain = 128,
    .fromAffineMain = 0,
    .numAffineMain = 32,
    .fromOBJsub = 0,
    .numOBJsub = 128,
    .fromAffineSub = 0,
    .numAffineSub = 32,
};

static const GraphicsBanks sArceusCinematicGraphicsBanks = {
    .bg = GX_VRAM_BG_256_BC,
    .bgextpltt = GX_VRAM_BGEXTPLTT_0123_E,
    .subbg = GX_VRAM_SUB_BG_32_H,
    .subbgextpltt = GX_VRAM_SUB_BGEXTPLTT_NONE,
    .obj = GX_VRAM_OBJ_128_A,
    .objextpltt = GX_VRAM_OBJEXTPLTT_NONE,
    .subobj = GX_VRAM_SUB_OBJ_16_I,
    .subobjextpltt = GX_VRAM_SUB_OBJEXTPLTT_NONE,
    .tex = GX_VRAM_TEX_NONE,
    .texpltt = GX_VRAM_TEXPLTT_NONE,
};

static const ManagedSpriteTemplate sArceusCinematicSpriteTemplate = {
    .x = 0,
    .y = 0,
    .z = 0,
    .animation = 0,
    .drawPriority = 0,
    .pal = 0,
    .vram = NNS_G2D_VRAM_TYPE_2DMAIN,
    .resIdList = { 51433, 51433, 51433, 51433, -1, -1 },
    .bgPriority = 0,
    .vramTransfer = 0,
};

static const ArceusScriptCmd sArceusCinematicScriptCmds[] = {
    [ARCEUS_CMD_WAIT] = ArceusScriptCmd_Wait,
    [ARCEUS_CMD_FADE_IN_FROM_BLACK] = ArceusScriptCmd_FadeInFromBlack,
    [ARCEUS_CMD_FADE_OUT_TO_BLACK] = ArceusScriptCmd_FadeOutToBlack,
    [ARCEUS_CMD_FADE_IN_FROM_WHITE] = ArceusScriptCmd_FadeInFromWhite,
    [ARCEUS_CMD_FADE_OUT_TO_WHITE] = ArceusScriptCmd_FadeOutToWhite,
    [ARCEUS_CMD_WAIT_FADE] = ArceusScriptCmd_WaitFade,
    [ARCEUS_CMD_LOAD_BG] = ArceusScriptCmd_LoadBg,
    [ARCEUS_CMD_TOGGLE_BG] = ArceusScriptCmd_ToggleBg,
    [ARCEUS_CMD_SET_BG_PRIORITY] = ArceusScriptCmd_SetBgPriority,
    [ARCEUS_CMD_SET_BLEND_PLANE] = ArceusScriptCmd_SetBlendPlane,
    [ARCEUS_CMD_START_BLEND] = ArceusScriptCmd_StartBlend,
    [ARCEUS_CMD_RESET_BLEND] = ArceusScriptCmd_ResetBlend,
    [ARCEUS_CMD_SAVE_BGM] = ArceusScriptCmd_SaveBgm,
    [ARCEUS_CMD_STOP_BGM] = ArceusScriptCmd_StopBgm,
    [ARCEUS_CMD_PLAY_BGM] = ArceusScriptCmd_PlayBgm,
    [ARCEUS_CMD_SET_SPRITE_VISIBLE] = ArceusScriptCmd_SetSpriteVisible,
    [ARCEUS_CMD_SET_SPRITE_POSITION] = ArceusScriptCmd_SetSpritePosition,
    [ARCEUS_CMD_SET_SPRITE_PRIORITY] = ArceusScriptCmd_SetSpritePriority,
    [ARCEUS_CMD_NOP] = ArceusScriptCmd_Nop,
};

// Length of each command, opcode included.
static const int sArceusCinematicScriptCmdSizes[] = {
    [ARCEUS_CMD_WAIT] = 2,
    [ARCEUS_CMD_FADE_IN_FROM_BLACK] = 3,
    [ARCEUS_CMD_FADE_OUT_TO_BLACK] = 3,
    [ARCEUS_CMD_FADE_IN_FROM_WHITE] = 3,
    [ARCEUS_CMD_FADE_OUT_TO_WHITE] = 3,
    [ARCEUS_CMD_WAIT_FADE] = 1,
    [ARCEUS_CMD_LOAD_BG] = 3,
    [ARCEUS_CMD_TOGGLE_BG] = 3,
    [ARCEUS_CMD_SET_BG_PRIORITY] = 3,
    [ARCEUS_CMD_SET_BLEND_PLANE] = 4,
    [ARCEUS_CMD_START_BLEND] = 3,
    [ARCEUS_CMD_RESET_BLEND] = 1,
    [ARCEUS_CMD_SAVE_BGM] = 1,
    [ARCEUS_CMD_STOP_BGM] = 1,
    [ARCEUS_CMD_PLAY_BGM] = 2,
    [ARCEUS_CMD_SET_SPRITE_VISIBLE] = 2,
    [ARCEUS_CMD_SET_SPRITE_POSITION] = 3,
    [ARCEUS_CMD_SET_SPRITE_PRIORITY] = 2,
    [ARCEUS_CMD_NOP] = 2,
    [ARCEUS_CMD_END] = 1,
};

#define ARCEUS_WAIT(frames)                             ARCEUS_CMD_WAIT, (frames)
#define ARCEUS_FADE_IN_FROM_BLACK(steps, framesPerStep) ARCEUS_CMD_FADE_IN_FROM_BLACK, (steps), (framesPerStep)
#define ARCEUS_FADE_OUT_TO_BLACK(steps, framesPerStep)  ARCEUS_CMD_FADE_OUT_TO_BLACK, (steps), (framesPerStep)
#define ARCEUS_FADE_IN_FROM_WHITE(steps, framesPerStep) ARCEUS_CMD_FADE_IN_FROM_WHITE, (steps), (framesPerStep)
#define ARCEUS_FADE_OUT_TO_WHITE(steps, framesPerStep)  ARCEUS_CMD_FADE_OUT_TO_WHITE, (steps), (framesPerStep)
#define ARCEUS_WAIT_FADE()                              ARCEUS_CMD_WAIT_FADE
#define ARCEUS_LOAD_BG(layer, file)                     ARCEUS_CMD_LOAD_BG, (layer), (file)
#define ARCEUS_TOGGLE_BG(layer, toggle)                 ARCEUS_CMD_TOGGLE_BG, (layer), (toggle)
#define ARCEUS_SET_BG_PRIORITY(layer, priority)         ARCEUS_CMD_SET_BG_PRIORITY, (layer), (priority)
#define ARCEUS_SET_BLEND_PLANE(target, planes, ev)      ARCEUS_CMD_SET_BLEND_PLANE, (target), (planes), (ev)
#define ARCEUS_START_BLEND(ev, frames)                  ARCEUS_CMD_START_BLEND, (ev), (frames)
#define ARCEUS_RESET_BLEND()                            ARCEUS_CMD_RESET_BLEND
#define ARCEUS_SAVE_BGM()                               ARCEUS_CMD_SAVE_BGM
#define ARCEUS_STOP_BGM()                               ARCEUS_CMD_STOP_BGM
#define ARCEUS_PLAY_BGM(seqNo)                          ARCEUS_CMD_PLAY_BGM, (seqNo)
#define ARCEUS_SET_SPRITE_VISIBLE(visible)              ARCEUS_CMD_SET_SPRITE_VISIBLE, (visible)
#define ARCEUS_SET_SPRITE_POSITION(x, y)                ARCEUS_CMD_SET_SPRITE_POSITION, (x), (y)
#define ARCEUS_SET_SPRITE_PRIORITY(priority)            ARCEUS_CMD_SET_SPRITE_PRIORITY, (priority)
#define ARCEUS_NOP(unused)                              ARCEUS_CMD_NOP, (unused)
#define ARCEUS_END()                                    ARCEUS_CMD_END

static const int sArceusCinematicScript[] = {
    ARCEUS_SAVE_BGM(),
    ARCEUS_LOAD_BG(GF_BG_LYR_MAIN_0, 58),
    ARCEUS_WAIT(60),
    ARCEUS_FADE_IN_FROM_BLACK(1, 1),
    ARCEUS_WAIT_FADE(),
    ARCEUS_WAIT(90),
    ARCEUS_FADE_OUT_TO_BLACK(15, 2),
    ARCEUS_WAIT_FADE(),
    ARCEUS_SET_SPRITE_VISIBLE(FALSE),
    ARCEUS_STOP_BGM(),
    ARCEUS_PLAY_BGM(SEQ_GS_E_ARCEUS),
    ARCEUS_WAIT(15),
    ARCEUS_LOAD_BG(GF_BG_LYR_MAIN_0, 1),
    ARCEUS_NOP(0),
    ARCEUS_FADE_IN_FROM_BLACK(1, 1),
    ARCEUS_WAIT_FADE(),
    ARCEUS_WAIT(15),
    ARCEUS_FADE_OUT_TO_BLACK(15, 2),
    ARCEUS_WAIT_FADE(),
    ARCEUS_LOAD_BG(GF_BG_LYR_MAIN_0, 2),
    ARCEUS_WAIT(40),
    ARCEUS_NOP(1),
    ARCEUS_FADE_IN_FROM_BLACK(1, 1),
    ARCEUS_WAIT_FADE(),
    ARCEUS_WAIT(15),
    ARCEUS_FADE_OUT_TO_BLACK(15, 2),
    ARCEUS_WAIT_FADE(),
    ARCEUS_LOAD_BG(GF_BG_LYR_MAIN_0, 3),
    ARCEUS_WAIT(35),
    ARCEUS_NOP(2),
    ARCEUS_FADE_IN_FROM_BLACK(1, 1),
    ARCEUS_WAIT_FADE(),
    ARCEUS_WAIT(15),
    ARCEUS_FADE_OUT_TO_BLACK(15, 2),
    ARCEUS_WAIT_FADE(),
    ARCEUS_LOAD_BG(GF_BG_LYR_MAIN_0, 6),
    ARCEUS_LOAD_BG(GF_BG_LYR_MAIN_1, 4),
    ARCEUS_SET_BG_PRIORITY(GF_BG_LYR_MAIN_0, 0),
    ARCEUS_SET_BG_PRIORITY(GF_BG_LYR_MAIN_1, 1),
    ARCEUS_WAIT(37),
    ARCEUS_NOP(10),
    ARCEUS_FADE_IN_FROM_BLACK(1, 1),
    ARCEUS_WAIT_FADE(),
    ARCEUS_WAIT(5),
    ARCEUS_SET_BLEND_PLANE(0, 1, 15),
    ARCEUS_SET_BLEND_PLANE(1, 2, 0),
    ARCEUS_START_BLEND(0, 20),
    ARCEUS_FADE_OUT_TO_BLACK(15, 2),
    ARCEUS_WAIT_FADE(),
    ARCEUS_RESET_BLEND(),
    ARCEUS_LOAD_BG(GF_BG_LYR_MAIN_0, 4),
    ARCEUS_LOAD_BG(GF_BG_LYR_MAIN_1, 7),
    ARCEUS_SET_BLEND_PLANE(0, 1, 15),
    ARCEUS_SET_BLEND_PLANE(1, 2, 0),
    ARCEUS_WAIT(35),
    ARCEUS_NOP(11),
    ARCEUS_FADE_IN_FROM_BLACK(1, 1),
    ARCEUS_WAIT_FADE(),
    ARCEUS_WAIT(5),
    ARCEUS_SET_BLEND_PLANE(0, 1, 15),
    ARCEUS_SET_BLEND_PLANE(1, 2, 0),
    ARCEUS_START_BLEND(0, 20),
    ARCEUS_FADE_OUT_TO_BLACK(20, 2),
    ARCEUS_WAIT_FADE(),
    ARCEUS_RESET_BLEND(),
    ARCEUS_WAIT(17),
    ARCEUS_LOAD_BG(GF_BG_LYR_MAIN_0, 52),
    ARCEUS_LOAD_BG(GF_BG_LYR_MAIN_1, 53),
    ARCEUS_SET_BG_PRIORITY(GF_BG_LYR_MAIN_0, 0),
    ARCEUS_SET_BG_PRIORITY(GF_BG_LYR_MAIN_1, 1),
    ARCEUS_SET_BLEND_PLANE(0, 1, 15),
    ARCEUS_SET_BLEND_PLANE(1, 2, 0),
    ARCEUS_NOP(20),
    ARCEUS_FADE_IN_FROM_BLACK(10, 1),
    ARCEUS_WAIT_FADE(),
    ARCEUS_WAIT(20),
    ARCEUS_START_BLEND(0, 30),
    ARCEUS_SET_BG_PRIORITY(GF_BG_LYR_MAIN_1, 0),
    ARCEUS_SET_BG_PRIORITY(GF_BG_LYR_MAIN_0, 1),
    ARCEUS_RESET_BLEND(),
    ARCEUS_LOAD_BG(GF_BG_LYR_MAIN_0, 54),
    ARCEUS_SET_BLEND_PLANE(0, 2, 15),
    ARCEUS_SET_BLEND_PLANE(1, 1, 0),
    ARCEUS_WAIT(20),
    ARCEUS_NOP(21),
    ARCEUS_START_BLEND(0, 30),
    ARCEUS_SET_BG_PRIORITY(GF_BG_LYR_MAIN_0, 0),
    ARCEUS_SET_BG_PRIORITY(GF_BG_LYR_MAIN_1, 1),
    ARCEUS_RESET_BLEND(),
    ARCEUS_LOAD_BG(GF_BG_LYR_MAIN_1, 55),
    ARCEUS_SET_BLEND_PLANE(0, 1, 15),
    ARCEUS_SET_BLEND_PLANE(1, 2, 0),
    ARCEUS_WAIT(20),
    ARCEUS_NOP(22),
    ARCEUS_START_BLEND(0, 30),
    ARCEUS_WAIT(17),
    ARCEUS_FADE_OUT_TO_BLACK(1, 1),
    ARCEUS_WAIT_FADE(),
    ARCEUS_RESET_BLEND(),
    ARCEUS_LOAD_BG(GF_BG_LYR_MAIN_0, 11),
    ARCEUS_LOAD_BG(GF_BG_LYR_MAIN_1, 12),
    ARCEUS_SET_BG_PRIORITY(GF_BG_LYR_MAIN_0, 0),
    ARCEUS_SET_BG_PRIORITY(GF_BG_LYR_MAIN_1, 1),
    ARCEUS_NOP(30),
    ARCEUS_FADE_IN_FROM_BLACK(10, 1),
    ARCEUS_WAIT(22),
    ARCEUS_SET_BG_PRIORITY(GF_BG_LYR_MAIN_0, 1),
    ARCEUS_SET_BG_PRIORITY(GF_BG_LYR_MAIN_1, 0),
    ARCEUS_NOP(31),
    ARCEUS_LOAD_BG(GF_BG_LYR_MAIN_0, 13),
    ARCEUS_WAIT(22),
    ARCEUS_SET_BG_PRIORITY(GF_BG_LYR_MAIN_0, 0),
    ARCEUS_SET_BG_PRIORITY(GF_BG_LYR_MAIN_1, 1),
    ARCEUS_NOP(32),
    ARCEUS_LOAD_BG(GF_BG_LYR_MAIN_1, 14),
    ARCEUS_WAIT(22),
    ARCEUS_SET_BG_PRIORITY(GF_BG_LYR_MAIN_0, 1),
    ARCEUS_SET_BG_PRIORITY(GF_BG_LYR_MAIN_1, 0),
    ARCEUS_NOP(33),
    ARCEUS_LOAD_BG(GF_BG_LYR_MAIN_0, 15),
    ARCEUS_WAIT(22),
    ARCEUS_SET_BG_PRIORITY(GF_BG_LYR_MAIN_0, 0),
    ARCEUS_SET_BG_PRIORITY(GF_BG_LYR_MAIN_1, 1),
    ARCEUS_NOP(34),
    ARCEUS_LOAD_BG(GF_BG_LYR_MAIN_1, 16),
    ARCEUS_WAIT(22),
    ARCEUS_SET_BG_PRIORITY(GF_BG_LYR_MAIN_0, 1),
    ARCEUS_SET_BG_PRIORITY(GF_BG_LYR_MAIN_1, 0),
    ARCEUS_NOP(35),
    ARCEUS_LOAD_BG(GF_BG_LYR_MAIN_0, 17),
    ARCEUS_WAIT(22),
    ARCEUS_SET_BG_PRIORITY(GF_BG_LYR_MAIN_0, 0),
    ARCEUS_SET_BG_PRIORITY(GF_BG_LYR_MAIN_1, 1),
    ARCEUS_NOP(36),
    ARCEUS_LOAD_BG(GF_BG_LYR_MAIN_1, 19),
    ARCEUS_WAIT(22),
    ARCEUS_SET_BG_PRIORITY(GF_BG_LYR_MAIN_0, 1),
    ARCEUS_SET_BG_PRIORITY(GF_BG_LYR_MAIN_1, 0),
    ARCEUS_NOP(37),
    ARCEUS_WAIT(22),
    ARCEUS_FADE_OUT_TO_WHITE(1, 1),
    ARCEUS_WAIT_FADE(),
    ARCEUS_LOAD_BG(GF_BG_LYR_MAIN_0, 36),
    ARCEUS_LOAD_BG(GF_BG_LYR_MAIN_1, 37),
    ARCEUS_LOAD_BG(GF_BG_LYR_MAIN_2, 38),
    ARCEUS_SET_BG_PRIORITY(GF_BG_LYR_MAIN_0, 0),
    ARCEUS_SET_BG_PRIORITY(GF_BG_LYR_MAIN_1, 1),
    ARCEUS_SET_BG_PRIORITY(GF_BG_LYR_MAIN_2, 2),
    ARCEUS_SET_BLEND_PLANE(0, 2, 15),
    ARCEUS_SET_BLEND_PLANE(1, 4, 0),
    ARCEUS_SET_SPRITE_VISIBLE(TRUE),
    ARCEUS_SET_SPRITE_POSITION(128, 80),
    ARCEUS_SET_SPRITE_PRIORITY(0),
    ARCEUS_NOP(40),
    ARCEUS_FADE_IN_FROM_WHITE(10, 1),
    ARCEUS_WAIT_FADE(),
    ARCEUS_WAIT(20),
    ARCEUS_START_BLEND(0, 30),
    ARCEUS_SET_BG_PRIORITY(GF_BG_LYR_MAIN_2, 0),
    ARCEUS_SET_BG_PRIORITY(GF_BG_LYR_MAIN_1, 1),
    ARCEUS_RESET_BLEND(),
    ARCEUS_LOAD_BG(GF_BG_LYR_MAIN_1, 39),
    ARCEUS_SET_BLEND_PLANE(0, 4, 15),
    ARCEUS_SET_BLEND_PLANE(1, 2, 0),
    ARCEUS_WAIT(20),
    ARCEUS_NOP(41),
    ARCEUS_START_BLEND(0, 30),
    ARCEUS_SET_BG_PRIORITY(GF_BG_LYR_MAIN_1, 0),
    ARCEUS_SET_BG_PRIORITY(GF_BG_LYR_MAIN_2, 1),
    ARCEUS_RESET_BLEND(),
    ARCEUS_LOAD_BG(GF_BG_LYR_MAIN_2, 40),
    ARCEUS_SET_BLEND_PLANE(0, 2, 15),
    ARCEUS_SET_BLEND_PLANE(1, 4, 0),
    ARCEUS_WAIT(20),
    ARCEUS_NOP(42),
    ARCEUS_START_BLEND(0, 30),
    ARCEUS_WAIT(17),
    ARCEUS_FADE_OUT_TO_WHITE(1, 1),
    ARCEUS_WAIT_FADE(),
    ARCEUS_RESET_BLEND(),
    ARCEUS_LOAD_BG(GF_BG_LYR_MAIN_1, 66),
    ARCEUS_LOAD_BG(GF_BG_LYR_MAIN_2, 58),
    ARCEUS_SET_BG_PRIORITY(GF_BG_LYR_MAIN_0, 0),
    ARCEUS_SET_BG_PRIORITY(GF_BG_LYR_MAIN_1, 1),
    ARCEUS_SET_BG_PRIORITY(GF_BG_LYR_MAIN_2, 2),
    ARCEUS_SET_BLEND_PLANE(0, 2, 15),
    ARCEUS_SET_BLEND_PLANE(1, 4, 0),
    ARCEUS_NOP(50),
    ARCEUS_FADE_IN_FROM_WHITE(1, 1),
    ARCEUS_WAIT_FADE(),
    ARCEUS_WAIT(10),
    ARCEUS_START_BLEND(0, 3),
    ARCEUS_SET_BG_PRIORITY(GF_BG_LYR_MAIN_2, 0),
    ARCEUS_SET_BG_PRIORITY(GF_BG_LYR_MAIN_1, 1),
    ARCEUS_RESET_BLEND(),
    ARCEUS_LOAD_BG(GF_BG_LYR_MAIN_1, 23),
    ARCEUS_SET_BLEND_PLANE(1, 4, 15),
    ARCEUS_SET_BLEND_PLANE(0, 2, 0),
    ARCEUS_NOP(51),
    ARCEUS_START_BLEND(0, 2),
    ARCEUS_SET_BG_PRIORITY(GF_BG_LYR_MAIN_1, 0),
    ARCEUS_SET_BG_PRIORITY(GF_BG_LYR_MAIN_2, 1),
    ARCEUS_RESET_BLEND(),
    ARCEUS_WAIT(10),
    ARCEUS_START_BLEND(0, 5),
    ARCEUS_SET_BG_PRIORITY(GF_BG_LYR_MAIN_2, 0),
    ARCEUS_SET_BG_PRIORITY(GF_BG_LYR_MAIN_1, 1),
    ARCEUS_RESET_BLEND(),
    ARCEUS_LOAD_BG(GF_BG_LYR_MAIN_1, 22),
    ARCEUS_SET_BLEND_PLANE(1, 4, 15),
    ARCEUS_SET_BLEND_PLANE(0, 2, 0),
    ARCEUS_NOP(52),
    ARCEUS_START_BLEND(0, 2),
    ARCEUS_SET_BG_PRIORITY(GF_BG_LYR_MAIN_1, 0),
    ARCEUS_SET_BG_PRIORITY(GF_BG_LYR_MAIN_2, 1),
    ARCEUS_RESET_BLEND(),
    ARCEUS_WAIT(10),
    ARCEUS_START_BLEND(0, 5),
    ARCEUS_SET_BG_PRIORITY(GF_BG_LYR_MAIN_2, 0),
    ARCEUS_SET_BG_PRIORITY(GF_BG_LYR_MAIN_1, 1),
    ARCEUS_RESET_BLEND(),
    ARCEUS_LOAD_BG(GF_BG_LYR_MAIN_1, 25),
    ARCEUS_SET_BLEND_PLANE(1, 4, 15),
    ARCEUS_SET_BLEND_PLANE(0, 2, 0),
    ARCEUS_NOP(53),
    ARCEUS_START_BLEND(0, 2),
    ARCEUS_SET_BG_PRIORITY(GF_BG_LYR_MAIN_1, 0),
    ARCEUS_SET_BG_PRIORITY(GF_BG_LYR_MAIN_2, 1),
    ARCEUS_RESET_BLEND(),
    ARCEUS_WAIT(10),
    ARCEUS_START_BLEND(0, 5),
    ARCEUS_SET_BG_PRIORITY(GF_BG_LYR_MAIN_2, 0),
    ARCEUS_SET_BG_PRIORITY(GF_BG_LYR_MAIN_1, 1),
    ARCEUS_RESET_BLEND(),
    ARCEUS_LOAD_BG(GF_BG_LYR_MAIN_1, 24),
    ARCEUS_SET_BLEND_PLANE(1, 4, 15),
    ARCEUS_SET_BLEND_PLANE(0, 2, 0),
    ARCEUS_NOP(54),
    ARCEUS_START_BLEND(0, 2),
    ARCEUS_SET_BG_PRIORITY(GF_BG_LYR_MAIN_1, 0),
    ARCEUS_SET_BG_PRIORITY(GF_BG_LYR_MAIN_2, 1),
    ARCEUS_RESET_BLEND(),
    ARCEUS_WAIT(10),
    ARCEUS_START_BLEND(0, 5),
    ARCEUS_SET_BG_PRIORITY(GF_BG_LYR_MAIN_2, 0),
    ARCEUS_SET_BG_PRIORITY(GF_BG_LYR_MAIN_1, 1),
    ARCEUS_RESET_BLEND(),
    ARCEUS_LOAD_BG(GF_BG_LYR_MAIN_1, 21),
    ARCEUS_SET_BLEND_PLANE(1, 4, 15),
    ARCEUS_SET_BLEND_PLANE(0, 2, 0),
    ARCEUS_NOP(55),
    ARCEUS_START_BLEND(0, 2),
    ARCEUS_SET_BG_PRIORITY(GF_BG_LYR_MAIN_1, 0),
    ARCEUS_SET_BG_PRIORITY(GF_BG_LYR_MAIN_2, 1),
    ARCEUS_RESET_BLEND(),
    ARCEUS_WAIT(10),
    ARCEUS_START_BLEND(0, 5),
    ARCEUS_SET_BG_PRIORITY(GF_BG_LYR_MAIN_2, 0),
    ARCEUS_SET_BG_PRIORITY(GF_BG_LYR_MAIN_1, 1),
    ARCEUS_RESET_BLEND(),
    ARCEUS_LOAD_BG(GF_BG_LYR_MAIN_1, 27),
    ARCEUS_SET_BLEND_PLANE(1, 4, 15),
    ARCEUS_SET_BLEND_PLANE(0, 2, 0),
    ARCEUS_NOP(56),
    ARCEUS_START_BLEND(0, 2),
    ARCEUS_SET_BG_PRIORITY(GF_BG_LYR_MAIN_1, 0),
    ARCEUS_SET_BG_PRIORITY(GF_BG_LYR_MAIN_2, 1),
    ARCEUS_RESET_BLEND(),
    ARCEUS_WAIT(10),
    ARCEUS_START_BLEND(0, 5),
    ARCEUS_SET_BG_PRIORITY(GF_BG_LYR_MAIN_2, 0),
    ARCEUS_SET_BG_PRIORITY(GF_BG_LYR_MAIN_1, 1),
    ARCEUS_RESET_BLEND(),
    ARCEUS_LOAD_BG(GF_BG_LYR_MAIN_1, 26),
    ARCEUS_SET_BLEND_PLANE(1, 4, 15),
    ARCEUS_SET_BLEND_PLANE(0, 2, 0),
    ARCEUS_NOP(57),
    ARCEUS_START_BLEND(0, 2),
    ARCEUS_SET_BG_PRIORITY(GF_BG_LYR_MAIN_1, 0),
    ARCEUS_SET_BG_PRIORITY(GF_BG_LYR_MAIN_2, 1),
    ARCEUS_RESET_BLEND(),
    ARCEUS_WAIT(10),
    ARCEUS_START_BLEND(0, 5),
    ARCEUS_SET_BG_PRIORITY(GF_BG_LYR_MAIN_2, 0),
    ARCEUS_SET_BG_PRIORITY(GF_BG_LYR_MAIN_1, 1),
    ARCEUS_RESET_BLEND(),
    ARCEUS_LOAD_BG(GF_BG_LYR_MAIN_1, 41),
    ARCEUS_SET_BG_PRIORITY(GF_BG_LYR_MAIN_0, 0),
    ARCEUS_SET_BG_PRIORITY(GF_BG_LYR_MAIN_2, 1),
    ARCEUS_SET_BG_PRIORITY(GF_BG_LYR_MAIN_1, 2),
    ARCEUS_SET_BLEND_PLANE(1, 4, 15),
    ARCEUS_SET_BLEND_PLANE(0, 2, 0),
    ARCEUS_NOP(60),
    ARCEUS_START_BLEND(0, 2),
    ARCEUS_SET_BG_PRIORITY(GF_BG_LYR_MAIN_1, 1),
    ARCEUS_SET_BG_PRIORITY(GF_BG_LYR_MAIN_2, 2),
    ARCEUS_RESET_BLEND(),
    ARCEUS_LOAD_BG(GF_BG_LYR_MAIN_2, 42),
    ARCEUS_SET_BLEND_PLANE(0, 2, 15),
    ARCEUS_SET_BLEND_PLANE(1, 4, 0),
    ARCEUS_WAIT(20),
    ARCEUS_NOP(61),
    ARCEUS_START_BLEND(0, 30),
    ARCEUS_SET_BG_PRIORITY(GF_BG_LYR_MAIN_2, 1),
    ARCEUS_SET_BG_PRIORITY(GF_BG_LYR_MAIN_1, 2),
    ARCEUS_RESET_BLEND(),
    ARCEUS_LOAD_BG(GF_BG_LYR_MAIN_1, 43),
    ARCEUS_SET_BLEND_PLANE(0, 4, 15),
    ARCEUS_SET_BLEND_PLANE(1, 2, 0),
    ARCEUS_WAIT(20),
    ARCEUS_NOP(62),
    ARCEUS_START_BLEND(0, 30),
    ARCEUS_SET_BG_PRIORITY(GF_BG_LYR_MAIN_1, 1),
    ARCEUS_SET_BG_PRIORITY(GF_BG_LYR_MAIN_2, 2),
    ARCEUS_RESET_BLEND(),
    ARCEUS_LOAD_BG(GF_BG_LYR_MAIN_2, 44),
    ARCEUS_SET_BLEND_PLANE(0, 2, 15),
    ARCEUS_SET_BLEND_PLANE(1, 4, 0),
    ARCEUS_WAIT(20),
    ARCEUS_NOP(63),
    ARCEUS_START_BLEND(0, 30),
    ARCEUS_RESET_BLEND(),
    ARCEUS_SET_BG_PRIORITY(GF_BG_LYR_MAIN_2, 1),
    ARCEUS_SET_BG_PRIORITY(GF_BG_LYR_MAIN_1, 2),
    ARCEUS_LOAD_BG(GF_BG_LYR_MAIN_1, 59),
    ARCEUS_WAIT(20),
    ARCEUS_START_BLEND(0, 2),
    ARCEUS_SET_BG_PRIORITY(GF_BG_LYR_MAIN_1, 1),
    ARCEUS_SET_BG_PRIORITY(GF_BG_LYR_MAIN_2, 2),
    ARCEUS_LOAD_BG(GF_BG_LYR_MAIN_2, 45),
    ARCEUS_SET_BLEND_PLANE(0, 2, 15),
    ARCEUS_SET_BLEND_PLANE(1, 4, 0),
    ARCEUS_NOP(70),
    ARCEUS_START_BLEND(0, 22),
    ARCEUS_SET_BG_PRIORITY(GF_BG_LYR_MAIN_2, 1),
    ARCEUS_SET_BG_PRIORITY(GF_BG_LYR_MAIN_1, 2),
    ARCEUS_RESET_BLEND(),
    ARCEUS_LOAD_BG(GF_BG_LYR_MAIN_1, 48),
    ARCEUS_SET_BLEND_PLANE(0, 4, 15),
    ARCEUS_SET_BLEND_PLANE(1, 2, 0),
    ARCEUS_NOP(71),
    ARCEUS_START_BLEND(0, 22),
    ARCEUS_SET_BG_PRIORITY(GF_BG_LYR_MAIN_1, 1),
    ARCEUS_SET_BG_PRIORITY(GF_BG_LYR_MAIN_2, 2),
    ARCEUS_RESET_BLEND(),
    ARCEUS_LOAD_BG(GF_BG_LYR_MAIN_2, 65),
    ARCEUS_SET_BLEND_PLANE(0, 2, 15),
    ARCEUS_SET_BLEND_PLANE(1, 4, 0),
    ARCEUS_NOP(72),
    ARCEUS_START_BLEND(0, 22),
    ARCEUS_SET_BG_PRIORITY(GF_BG_LYR_MAIN_2, 1),
    ARCEUS_SET_BG_PRIORITY(GF_BG_LYR_MAIN_1, 2),
    ARCEUS_RESET_BLEND(),
    ARCEUS_LOAD_BG(GF_BG_LYR_MAIN_1, 46),
    ARCEUS_SET_BLEND_PLANE(0, 4, 15),
    ARCEUS_SET_BLEND_PLANE(1, 2, 0),
    ARCEUS_NOP(73),
    ARCEUS_START_BLEND(0, 22),
    ARCEUS_SET_BG_PRIORITY(GF_BG_LYR_MAIN_1, 1),
    ARCEUS_SET_BG_PRIORITY(GF_BG_LYR_MAIN_2, 2),
    ARCEUS_RESET_BLEND(),
    ARCEUS_LOAD_BG(GF_BG_LYR_MAIN_2, 64),
    ARCEUS_SET_BLEND_PLANE(0, 2, 15),
    ARCEUS_SET_BLEND_PLANE(1, 4, 0),
    ARCEUS_NOP(74),
    ARCEUS_START_BLEND(0, 22),
    ARCEUS_SET_BG_PRIORITY(GF_BG_LYR_MAIN_2, 1),
    ARCEUS_SET_BG_PRIORITY(GF_BG_LYR_MAIN_1, 2),
    ARCEUS_RESET_BLEND(),
    ARCEUS_LOAD_BG(GF_BG_LYR_MAIN_1, 63),
    ARCEUS_SET_BLEND_PLANE(0, 4, 15),
    ARCEUS_SET_BLEND_PLANE(1, 2, 0),
    ARCEUS_NOP(75),
    ARCEUS_START_BLEND(0, 22),
    ARCEUS_SET_BG_PRIORITY(GF_BG_LYR_MAIN_1, 1),
    ARCEUS_SET_BG_PRIORITY(GF_BG_LYR_MAIN_2, 2),
    ARCEUS_RESET_BLEND(),
    ARCEUS_LOAD_BG(GF_BG_LYR_MAIN_2, 49),
    ARCEUS_SET_BLEND_PLANE(0, 2, 15),
    ARCEUS_SET_BLEND_PLANE(1, 4, 0),
    ARCEUS_NOP(76),
    ARCEUS_START_BLEND(0, 21),
    ARCEUS_SET_BG_PRIORITY(GF_BG_LYR_MAIN_2, 1),
    ARCEUS_SET_BG_PRIORITY(GF_BG_LYR_MAIN_1, 2),
    ARCEUS_RESET_BLEND(),
    ARCEUS_LOAD_BG(GF_BG_LYR_MAIN_1, 59),
    ARCEUS_SET_BLEND_PLANE(1, 4, 15),
    ARCEUS_SET_BLEND_PLANE(0, 2, 0),
    ARCEUS_WAIT(15),
    ARCEUS_START_BLEND(0, 2),
    ARCEUS_SET_BG_PRIORITY(GF_BG_LYR_MAIN_1, 1),
    ARCEUS_SET_BG_PRIORITY(GF_BG_LYR_MAIN_2, 2),
    ARCEUS_RESET_BLEND(),
    ARCEUS_LOAD_BG(GF_BG_LYR_MAIN_2, 28),
    ARCEUS_SET_BLEND_PLANE(0, 2, 15),
    ARCEUS_SET_BLEND_PLANE(1, 4, 0),
    ARCEUS_NOP(80),
    ARCEUS_START_BLEND(0, 5),
    ARCEUS_SET_BG_PRIORITY(GF_BG_LYR_MAIN_2, 1),
    ARCEUS_SET_BG_PRIORITY(GF_BG_LYR_MAIN_1, 2),
    ARCEUS_RESET_BLEND(),
    ARCEUS_SET_BLEND_PLANE(1, 4, 15),
    ARCEUS_SET_BLEND_PLANE(0, 2, 0),
    ARCEUS_WAIT(10),
    ARCEUS_START_BLEND(0, 30),
    ARCEUS_SET_BG_PRIORITY(GF_BG_LYR_MAIN_1, 1),
    ARCEUS_SET_BG_PRIORITY(GF_BG_LYR_MAIN_2, 2),
    ARCEUS_LOAD_BG(GF_BG_LYR_MAIN_2, 50),
    ARCEUS_SET_BLEND_PLANE(0, 2, 15),
    ARCEUS_SET_BLEND_PLANE(1, 4, 0),
    ARCEUS_NOP(81),
    ARCEUS_START_BLEND(0, 5),
    ARCEUS_SET_BG_PRIORITY(GF_BG_LYR_MAIN_2, 1),
    ARCEUS_SET_BG_PRIORITY(GF_BG_LYR_MAIN_1, 2),
    ARCEUS_RESET_BLEND(),
    ARCEUS_SET_BLEND_PLANE(1, 4, 15),
    ARCEUS_SET_BLEND_PLANE(0, 2, 0),
    ARCEUS_WAIT(9),
    ARCEUS_START_BLEND(0, 30),
    ARCEUS_SET_BG_PRIORITY(GF_BG_LYR_MAIN_1, 1),
    ARCEUS_SET_BG_PRIORITY(GF_BG_LYR_MAIN_2, 2),
    ARCEUS_LOAD_BG(GF_BG_LYR_MAIN_2, 57),
    ARCEUS_SET_BLEND_PLANE(0, 2, 15),
    ARCEUS_SET_BLEND_PLANE(1, 4, 0),
    ARCEUS_NOP(82),
    ARCEUS_START_BLEND(0, 5),
    ARCEUS_SET_BG_PRIORITY(GF_BG_LYR_MAIN_2, 1),
    ARCEUS_SET_BG_PRIORITY(GF_BG_LYR_MAIN_1, 2),
    ARCEUS_RESET_BLEND(),
    ARCEUS_SET_BLEND_PLANE(1, 4, 15),
    ARCEUS_SET_BLEND_PLANE(0, 2, 0),
    ARCEUS_WAIT(9),
    ARCEUS_START_BLEND(0, 30),
    ARCEUS_SET_BG_PRIORITY(GF_BG_LYR_MAIN_1, 1),
    ARCEUS_SET_BG_PRIORITY(GF_BG_LYR_MAIN_2, 2),
    ARCEUS_LOAD_BG(GF_BG_LYR_MAIN_2, 51),
    ARCEUS_SET_BLEND_PLANE(0, 2, 15),
    ARCEUS_SET_BLEND_PLANE(1, 4, 0),
    ARCEUS_NOP(83),
    ARCEUS_START_BLEND(0, 5),
    ARCEUS_SET_BG_PRIORITY(GF_BG_LYR_MAIN_2, 1),
    ARCEUS_SET_BG_PRIORITY(GF_BG_LYR_MAIN_1, 2),
    ARCEUS_RESET_BLEND(),
    ARCEUS_SET_BLEND_PLANE(1, 4, 15),
    ARCEUS_SET_BLEND_PLANE(0, 2, 0),
    ARCEUS_WAIT(9),
    ARCEUS_FADE_OUT_TO_BLACK(30, 1),
    ARCEUS_WAIT_FADE(),
    ARCEUS_SET_BG_PRIORITY(GF_BG_LYR_MAIN_1, 1),
    ARCEUS_SET_BG_PRIORITY(GF_BG_LYR_MAIN_2, 2),
    ARCEUS_LOAD_BG(GF_BG_LYR_MAIN_0, 4),
    ARCEUS_SET_SPRITE_VISIBLE(FALSE),
    ARCEUS_NOP(90),
    ARCEUS_FADE_IN_FROM_BLACK(3, 1),
    ARCEUS_WAIT_FADE(),
    ARCEUS_WAIT(26),
    ARCEUS_FADE_OUT_TO_BLACK(10, 1),
    ARCEUS_WAIT_FADE(),
    ARCEUS_LOAD_BG(GF_BG_LYR_MAIN_0, 67),
    ARCEUS_FADE_IN_FROM_BLACK(3, 1),
    ARCEUS_WAIT_FADE(),
    ARCEUS_WAIT(26),
    ARCEUS_FADE_OUT_TO_BLACK(10, 1),
    ARCEUS_WAIT_FADE(),
    ARCEUS_LOAD_BG(GF_BG_LYR_MAIN_0, 29),
    ARCEUS_FADE_IN_FROM_BLACK(3, 1),
    ARCEUS_WAIT_FADE(),
    ARCEUS_WAIT(26),
    ARCEUS_FADE_OUT_TO_BLACK(10, 1),
    ARCEUS_WAIT_FADE(),
    ARCEUS_LOAD_BG(GF_BG_LYR_MAIN_0, 36),
    ARCEUS_LOAD_BG(GF_BG_LYR_MAIN_1, 56),
    ARCEUS_LOAD_BG(GF_BG_LYR_MAIN_2, 58),
    ARCEUS_TOGGLE_BG(GF_BG_LYR_MAIN_0, GF_PLANE_TOGGLE_OFF),
    ARCEUS_NOP(91),
    ARCEUS_FADE_IN_FROM_BLACK(10, 1),
    ARCEUS_WAIT_FADE(),
    ARCEUS_WAIT(40),
    ARCEUS_NOP(92),
    ARCEUS_TOGGLE_BG(GF_BG_LYR_MAIN_0, GF_PLANE_TOGGLE_ON),
    ARCEUS_SET_BLEND_PLANE(0, 2, 15),
    ARCEUS_SET_BLEND_PLANE(1, 4, 0),
    ARCEUS_WAIT(10),
    ARCEUS_NOP(93),
    ARCEUS_START_BLEND(0, 30),
    ARCEUS_SET_BG_PRIORITY(GF_BG_LYR_MAIN_2, 1),
    ARCEUS_SET_BG_PRIORITY(GF_BG_LYR_MAIN_1, 2),
    ARCEUS_RESET_BLEND(),
    ARCEUS_LOAD_BG(GF_BG_LYR_MAIN_1, 61),
    ARCEUS_WAIT(55),
    ARCEUS_NOP(94),
    ARCEUS_SET_BG_PRIORITY(GF_BG_LYR_MAIN_1, 0),
    ARCEUS_SET_BG_PRIORITY(GF_BG_LYR_MAIN_0, 1),
    ARCEUS_SET_BG_PRIORITY(GF_BG_LYR_MAIN_2, 2),
    ARCEUS_SET_BLEND_PLANE(0, 1, 15),
    ARCEUS_SET_BLEND_PLANE(1, 4, 0),
    ARCEUS_WAIT(90),
    ARCEUS_START_BLEND(0, 30),
    ARCEUS_SET_BG_PRIORITY(GF_BG_LYR_MAIN_1, 0),
    ARCEUS_SET_BG_PRIORITY(GF_BG_LYR_MAIN_2, 1),
    ARCEUS_SET_BG_PRIORITY(GF_BG_LYR_MAIN_0, 2),
    ARCEUS_RESET_BLEND(),
    ARCEUS_LOAD_BG(GF_BG_LYR_MAIN_0, 62),
    ARCEUS_WAIT(60),
    ARCEUS_SET_BG_PRIORITY(GF_BG_LYR_MAIN_0, 0),
    ARCEUS_SET_BG_PRIORITY(GF_BG_LYR_MAIN_2, 1),
    ARCEUS_SET_BG_PRIORITY(GF_BG_LYR_MAIN_1, 2),
    ARCEUS_NOP(95),
    ARCEUS_FADE_OUT_TO_BLACK(60, 1),
    ARCEUS_WAIT_FADE(),
    ARCEUS_STOP_BGM(),
    ARCEUS_WAIT(1),
    ARCEUS_END(),
};

int ScriptCinematic_Arceus(void *data) {
    LegendaryCinematic *cinematic = data;
    ArceusCinematic *arceus;
    cinematic->state = sScriptCinematicSubs_Arceus[cinematic->state](cinematic);
    if (cinematic->state == 5) {
        return FALSE;
    }
    arceus = cinematic->arceus;
    ArceusCinematic_UpdateSprites(arceus);
    SpriteSystem_DrawSprites(arceus->spriteManager);
    return TRUE;
}

static void ArceusCinematic_VBlank(ArceusCinematic *arceus) {
    ArceusCinematic_TransferBgPalettes(arceus);
    DoScheduledBgGpuUpdates(arceus->bgConfig);
    SpriteSystem_TransferOam();
    OS_SetIrqCheckFlag(OS_IE_V_BLANK);
}

static void ArceusCinematic_SetBanks(void) {
    GfGfx_SetBanks(&sArceusCinematicGraphicsBanks);
}

static void ArceusCinematic_InitBgs(ArceusCinematic *arceus) {
    GraphicsModes modes;
    BgTemplate bg0;
    BgTemplate bg1;
    BgTemplate bg2;

    arceus->bgConfig = BgConfig_Alloc(HEAP_ID_153);
    modes = sArceusCinematicGraphicsModes;
    SetBothScreensModesAndDisable(&modes);
    bg0 = sArceusCinematicBgTemplate0;
    InitBgFromTemplate(arceus->bgConfig, GF_BG_LYR_MAIN_0, &bg0, GF_BG_TYPE_TEXT);
    BgClearTilemapBufferAndCommit(arceus->bgConfig, GF_BG_LYR_MAIN_0);
    BG_ClearCharDataRange(GF_BG_LYR_MAIN_0, 64, 0, HEAP_ID_153);
    bg1 = sArceusCinematicBgTemplate1;
    InitBgFromTemplate(arceus->bgConfig, GF_BG_LYR_MAIN_1, &bg1, GF_BG_TYPE_TEXT);
    BgClearTilemapBufferAndCommit(arceus->bgConfig, GF_BG_LYR_MAIN_1);
    BG_ClearCharDataRange(GF_BG_LYR_MAIN_1, 64, 0, HEAP_ID_153);
    bg2 = sArceusCinematicBgTemplate2;
    InitBgFromTemplate(arceus->bgConfig, GF_BG_LYR_MAIN_2, &bg2, GF_BG_TYPE_TEXT);
    BgClearTilemapBufferAndCommit(arceus->bgConfig, GF_BG_LYR_MAIN_2);
    BG_ClearCharDataRange(GF_BG_LYR_MAIN_2, 64, 0, HEAP_ID_153);
    BG_SetMaskColor(GF_BG_LYR_MAIN_0, 0);
    BG_SetMaskColor(GF_BG_LYR_SUB_0, 0);
}

static void ArceusCinematic_FreeBgs(ArceusCinematic *arceus) {
    FreeBgTilemapBuffer(arceus->bgConfig, GF_BG_LYR_MAIN_2);
    FreeBgTilemapBuffer(arceus->bgConfig, GF_BG_LYR_MAIN_1);
    FreeBgTilemapBuffer(arceus->bgConfig, GF_BG_LYR_MAIN_0);
    Heap_Free(arceus->bgConfig);
}

static void ArceusCinematic_InitSprites(LegendaryCinematic *cinematic) {
    ArceusCinematic *arceus = cinematic->arceus;
    OamManagerParam oamManagerParam;
    OamCharTransferParam oamCharTransferParam;
    SpriteResourceCountsListUnion resourceCounts;

    arceus->spriteSystem = SpriteSystem_Alloc(HEAP_ID_153);
    arceus->spriteManager = SpriteManager_New(arceus->spriteSystem);
    oamManagerParam = sArceusCinematicOamManagerParam;
    oamCharTransferParam = sArceusCinematicOamCharTransferParam;
    SpriteSystem_Init(arceus->spriteSystem, &oamManagerParam, &oamCharTransferParam, 32);
    resourceCounts = sArceusCinematicSpriteResourceCounts;
    SpriteSystem_InitSprites(arceus->spriteSystem, arceus->spriteManager, 1);
    SpriteSystem_InitManagerWithCapacities(arceus->spriteSystem, arceus->spriteManager, &resourceCounts);
    G2dRenderer_SetSubSurfaceCoords(SpriteSystem_GetRenderer(arceus->spriteSystem), 0, FX32_CONST(1024));
    SpriteSystem_LoadCellResObjFromOpenNarc(arceus->spriteSystem, arceus->spriteManager, arceus->narc, 205, TRUE, 51433);
    SpriteSystem_LoadAnimResObjFromOpenNarc(arceus->spriteSystem, arceus->spriteManager, arceus->narc, 206, TRUE, 51433);
    SpriteSystem_LoadPlttResObjFromOpenNarc(arceus->spriteSystem, arceus->spriteManager, arceus->narc, 207, FALSE, 1, NNS_G2D_VRAM_TYPE_2DMAIN, 51433);
    SpriteSystem_LoadCharResObjFromOpenNarc(arceus->spriteSystem, arceus->spriteManager, arceus->narc, 204, TRUE, NNS_G2D_VRAM_TYPE_2DMAIN, 51433);
    arceus->sprites[0] = SpriteSystem_NewSprite(arceus->spriteSystem, arceus->spriteManager, &sArceusCinematicSpriteTemplate);
    ManagedSprite_SetDrawFlag(arceus->sprites[0], FALSE);
    ArceusCinematic_LoadArceusSprite(cinematic);
    GfGfx_EngineATogglePlanes(GX_PLANEMASK_OBJ, GF_PLANE_TOGGLE_ON);
}

static void ArceusCinematic_DeleteSprite(ArceusCinematic *arceus, int index) {
    if (arceus->sprites[index] != NULL) {
        Sprite_DeleteAndFreeResources(arceus->sprites[index]);
        arceus->sprites[index] = NULL;
    }
}

static void ArceusCinematic_FreeSprites(ArceusCinematic *arceus) {
    ArceusCinematic_DeleteSprite(arceus, 0);
    SpriteSystem_FreeResourcesAndManager(arceus->spriteSystem, arceus->spriteManager);
    SpriteSystem_Free(arceus->spriteSystem);
}

static void ArceusCinematic_UpdateSprites(ArceusCinematic *arceus) {
    if (arceus->sprites[0] != NULL) {
        ManagedSprite_TickFrame(arceus->sprites[0]);
    }
}

static void ArceusCinematic_LoadObjChar(void *src, u32 offset, u32 size) {
    DC_FlushRange(src, size);
    GX_LoadOBJ(src, offset, size);
}

static void ArceusCinematic_LoadObjPltt(void *src, u32 offset, u32 size) {
    DC_FlushRange(src, size);
    GX_LoadOBJPltt(src, offset, size);
}

static int ArceusCinematic_GetOverworldModel(int species, int form, int gender) {
    int model;
    if (species <= 0 || species > NATIONAL_DEX_COUNT) {
        model = MMODEL_BABYGIRL1;
    } else {
        model = SpeciesToOverworldModelIndexOffset(species) + MMODEL_FOLLOWER_MON_BULBASAUR;
        if (OverworldModelLookupHasFemaleForm(species)) {
            if (gender == MON_FEMALE) {
                model++;
            }
        } else {
            if (form > OverworldModelLookupFormCount(species)) {
                form = 0;
            }
            model += form;
        }
    }
    return model;
}

// The Arceus sprite uses the overworld texture of the party's lead Arceus (its form and shininess),
// or of a genderless normal Arceus when the lead is something else.
static void ArceusCinematic_LoadArceusSprite(LegendaryCinematic *cinematic) {
    ArceusCinematic *arceus = cinematic->arceus;
    Party *party = SaveArray_Party_Get(cinematic->args->saveData);
    u16 gender;
    u16 form;
    int shiny;
    NNSG3dResTex *tex;
    void *file;
    u8 *src;
    void *buffer;
    u32 dest;
    u32 i;

    if (!Party_GetCount(party)) {
        gender = MON_GENDERLESS;
        form = 0;
        shiny = FALSE;
    } else {
        Pokemon *mon = Party_GetMonByIndex(party, 0);
        if (GetMonData(mon, MON_DATA_SPECIES, NULL) == SPECIES_ARCEUS) {
            gender = (u16)GetMonData(mon, MON_DATA_GENDER, NULL);
            form = (u16)GetMonData(mon, MON_DATA_FORM, NULL);
            shiny = MonIsShiny(mon);
        } else {
            gender = MON_GENDERLESS;
            form = 0;
            shiny = FALSE;
        }
    }
    file = AllocAndReadWholeNarcMemberByIdPair(NARC_data_mmodel_mmodel, ArceusCinematic_GetOverworldModel(SPECIES_ARCEUS, form, gender), HEAP_ID_153);
    tex = NNS_G3dGetTex(file);
    dest = NNS_G2dGetImageLocation(Sprite_GetImageProxy(arceus->sprites[0]->sprite), NNS_G2D_VRAM_TYPE_2DMAIN);
    src = (u8 *)tex + tex->texInfo.ofsTex;
    buffer = Heap_Alloc(HEAP_ID_153, 0x800);
    for (i = 0; i < 8; i++) {
        CharTiles_CopyRectFromBuffer(src, 8, 0, 0, 8, 8, buffer);
        ArceusCinematic_LoadObjChar(buffer, dest, 0x800);
        src += 0x800;
        dest += 0x800;
    }
    Heap_Free(buffer);
    dest = NNS_G2dGetImagePaletteLocation(Sprite_GetPaletteProxy(arceus->sprites[0]->sprite), NNS_G2D_VRAM_TYPE_2DMAIN);
    src = (u8 *)tex + tex->plttInfo.ofsPlttData;
    if (shiny == TRUE) {
        src += 0x20;
    }
    ArceusCinematic_LoadObjPltt(src, dest, 0x20);
    Heap_Free(file);
}

static int ArceusCinematic_Init(LegendaryCinematic *cinematic) {
    ArceusCinematic *arceus;
    Main_SetVBlankIntrCB(NULL, NULL);
    HBlankInterruptDisable();
    GfGfx_EngineASetPlanes(GX_PLANEMASK_NONE);
    GfGfx_EngineBSetPlanes(GX_PLANEMASK_NONE);
    G2_BlendNone();
    G2S_BlendNone();
    reg_GX_POWCNT |= REG_GX_POWCNT_DSEL_MASK;
    cinematic->arceus = Heap_Alloc(HEAP_ID_153, sizeof(ArceusCinematic));
    MI_CpuFill8(cinematic->arceus, 0, sizeof(ArceusCinematic));
    arceus = cinematic->arceus;
    arceus->scriptPos = sArceusCinematicScript;
    arceus->narc = NARC_New(NARC_a_2_4_5, HEAP_ID_153);
    ArceusCinematic_SetBanks();
    ArceusCinematic_InitBgs(arceus);
    ArceusCinematic_InitSprites(cinematic);
    Main_SetVBlankIntrCB((GFIntrCB)ArceusCinematic_VBlank, cinematic->arceus);
    return 4;
}

static int ArceusCinematic_Exit(LegendaryCinematic *cinematic) {
    ArceusCinematic *arceus = cinematic->arceus;
    Main_SetVBlankIntrCB(NULL, NULL);
    ArceusCinematic_FreeSprites(arceus);
    ArceusCinematic_FreeBgs(arceus);
    NARC_Delete(arceus->narc);
    Heap_Free(cinematic->arceus);
    return 5;
}

static int ArceusCinematic_WaitFade(LegendaryCinematic *cinematic) {
    if (IsPaletteFadeFinished() == TRUE) {
        return cinematic->nextState;
    }
    return 2;
}

// Moves the first blend target's EV from blendStartEv to blendTargetEv over blendDuration frames
// (the second target gets the complement to 15).
static int ArceusCinematic_Blend(LegendaryCinematic *cinematic) {
    struct {
        volatile int targetEv;
        volatile int frame;
    } local;
    ArceusCinematic *arceus = cinematic->arceus;
    int frame = cinematic->frame;
    int duration = arceus->blendDuration;
    int ev;

    local.frame = frame;
    if (frame == duration) {
        ev = arceus->blendTargetEv;
        G2_SetBlendAlpha(arceus->blendPlane1, arceus->blendPlane2, ev, abs(15 - ev));
        cinematic->frame = 0;
        return cinematic->nextState;
    } else {
        int targetEv = arceus->blendTargetEv;
        local.targetEv = targetEv;
        int startEv = arceus->blendStartEv;
        u32 delta = (abs(startEv - targetEv) << 8) / duration;
        delta = (local.frame * delta) >> 8;
        if (startEv > local.targetEv) {
            ev = startEv - delta;
        } else {
            ev = delta + startEv;
        }
        G2_SetBlendAlpha(arceus->blendPlane1, arceus->blendPlane2, ev, abs(15 - ev));
        cinematic->frame++;
        return 3;
    }
}

static int ArceusCinematic_RunScript(LegendaryCinematic *cinematic) {
    ArceusCinematic *arceus = cinematic->arceus;
    while (TRUE) {
        const int *cmd = arceus->scriptPos;
        int opcode = *cmd;
        int result;
        if (opcode == ARCEUS_CMD_END) {
            return 1;
        }
        result = sArceusCinematicScriptCmds[opcode](cinematic, cmd);
        if (result == ARCEUS_SCRIPT_WAIT) {
            break;
        }
        if (result == ARCEUS_SCRIPT_CONTINUE) {
            arceus->scriptPos += sArceusCinematicScriptCmdSizes[*arceus->scriptPos];
            continue;
        }
        if (result == ARCEUS_SCRIPT_YIELD) {
            arceus->scriptPos += sArceusCinematicScriptCmdSizes[*arceus->scriptPos];
            break;
        }
    }
    return cinematic->state;
}

static void ArceusCinematic_LoadBgPalette(ArceusCinematic *arceus, int bgId, int fileId) {
    NNSG2dPaletteData *palette;
    void *buffer;
    ArceusBgPalette *bgPalette;

    buffer = GfGfxLoader_GetPlttDataFromOpenNarc(arceus->narc, fileId + 136, &palette, HEAP_ID_153);
    bgPalette = &arceus->bgPalettes[bgId];
    bgPalette->size = palette->szByte;
    MIi_CpuCopy32(palette->pRawData, (u32 *)bgPalette->data, bgPalette->size);
    Heap_Free(buffer);
}

static void ArceusCinematic_TransferBgPalettes(ArceusCinematic *arceus) {
    u32 i;

    for (i = 0; i < ARCEUS_CINEMATIC_BG_PALETTES; i++) {
        if (arceus->bgPalettes[i].size != 0) {
            GX_BeginLoadBGExtPltt();
            GX_LoadBGExtPltt(arceus->bgPalettes[i].data, i * 0x2000, arceus->bgPalettes[i].size);
            GX_EndLoadBGExtPltt();
            arceus->bgPalettes[i].size = 0;
        }
    }
}

static int ArceusScriptCmd_Wait(LegendaryCinematic *cinematic, const int *cmd) {
    u32 frame;
    GF_ASSERT(cmd[0] == ARCEUS_CMD_WAIT);
    frame = cinematic->frame;
    if (frame == cmd[1]) {
        cinematic->frame = 0;
        return ARCEUS_SCRIPT_YIELD;
    }
    cinematic->frame = frame + 1;
    return ARCEUS_SCRIPT_WAIT;
}

static int ArceusScriptCmd_FadeInFromBlack(LegendaryCinematic *cinematic, const int *cmd) {
    GF_ASSERT(cmd[0] == ARCEUS_CMD_FADE_IN_FROM_BLACK);
    LegendaryCinematic_FadeInFromBlack(cinematic, cmd[1], cmd[2], 4);
    return ARCEUS_SCRIPT_CONTINUE;
}

static int ArceusScriptCmd_FadeOutToBlack(LegendaryCinematic *cinematic, const int *cmd) {
    GF_ASSERT(cmd[0] == ARCEUS_CMD_FADE_OUT_TO_BLACK);
    LegendaryCinematic_FadeOutToBlack(cinematic, cmd[1], cmd[2], 4);
    return ARCEUS_SCRIPT_CONTINUE;
}

static int ArceusScriptCmd_FadeInFromWhite(LegendaryCinematic *cinematic, const int *cmd) {
    GF_ASSERT(cmd[0] == ARCEUS_CMD_FADE_IN_FROM_WHITE);
    LegendaryCinematic_FadeInFromWhite(cinematic, cmd[1], cmd[2], 4);
    return ARCEUS_SCRIPT_CONTINUE;
}

static int ArceusScriptCmd_FadeOutToWhite(LegendaryCinematic *cinematic, const int *cmd) {
    GF_ASSERT(cmd[0] == ARCEUS_CMD_FADE_OUT_TO_WHITE);
    LegendaryCinematic_FadeOutToWhite(cinematic, cmd[1], cmd[2], 4);
    return ARCEUS_SCRIPT_CONTINUE;
}

static int ArceusScriptCmd_WaitFade(LegendaryCinematic *cinematic, const int *cmd) {
    GF_ASSERT(cmd[0] == ARCEUS_CMD_WAIT_FADE);
    cinematic->state = 2;
    cinematic->nextState = 4;
    return ARCEUS_SCRIPT_YIELD;
}

static int ArceusScriptCmd_LoadBg(LegendaryCinematic *cinematic, const int *cmd) {
    ArceusCinematic *arceus = cinematic->arceus;
    GF_ASSERT(cmd[0] == ARCEUS_CMD_LOAD_BG);
    GfGfxLoader_LoadCharDataFromOpenNarc(arceus->narc, cmd[2] + 68, arceus->bgConfig, (GFBgLayer)cmd[1], 0, 0, TRUE, HEAP_ID_153);
    ArceusCinematic_LoadBgPalette(arceus, cmd[1], cmd[2]);
    GfGfxLoader_LoadScrnDataFromOpenNarc(arceus->narc, cmd[2], arceus->bgConfig, (GFBgLayer)cmd[1], 0, 0, TRUE, HEAP_ID_153);
    return ARCEUS_SCRIPT_CONTINUE;
}

static int ArceusScriptCmd_ToggleBg(LegendaryCinematic *cinematic, const int *cmd) {
    GF_ASSERT(cmd[0] == ARCEUS_CMD_TOGGLE_BG);
    ToggleBgLayer((u8)cmd[1], (u8)cmd[2]);
    return ARCEUS_SCRIPT_CONTINUE;
}

static int ArceusScriptCmd_SetBgPriority(LegendaryCinematic *cinematic, const int *cmd) {
    GF_ASSERT(cmd[0] == ARCEUS_CMD_SET_BG_PRIORITY);
    SetBgPriority((u8)cmd[1], (u8)cmd[2]);
    return ARCEUS_SCRIPT_CONTINUE;
}

static int ArceusScriptCmd_SetBlendPlane(LegendaryCinematic *cinematic, const int *cmd) {
    ArceusCinematic *arceus = cinematic->arceus;
    GF_ASSERT(cmd[0] == ARCEUS_CMD_SET_BLEND_PLANE);
    if (cmd[1] == 0) {
        arceus->blendPlane1 = cmd[2];
        arceus->blendStartEv = cmd[3];
    } else {
        arceus->blendPlane2 = cmd[2];
        arceus->blendStartEv2 = cmd[3];
    }
    return ARCEUS_SCRIPT_CONTINUE;
}

static int ArceusScriptCmd_StartBlend(LegendaryCinematic *cinematic, const int *cmd) {
    ArceusCinematic *arceus = cinematic->arceus;
    GF_ASSERT(cmd[0] == ARCEUS_CMD_START_BLEND);
    arceus->blendTargetEv = cmd[1];
    arceus->blendDuration = cmd[2];
    cinematic->state = 3;
    cinematic->nextState = 4;
    return ARCEUS_SCRIPT_YIELD;
}

static int ArceusScriptCmd_ResetBlend(LegendaryCinematic *cinematic, const int *cmd) {
    ArceusCinematic *arceus = cinematic->arceus;
    GF_ASSERT(cmd[0] == ARCEUS_CMD_RESET_BLEND);
    G2_BlendNone();
    arceus->blendPlane1 = 0;
    arceus->blendStartEv = 0;
    arceus->blendPlane2 = 0;
    arceus->blendStartEv2 = 0;
    return ARCEUS_SCRIPT_CONTINUE;
}

static int ArceusScriptCmd_SaveBgm(LegendaryCinematic *cinematic, const int *cmd) {
    ArceusCinematic *arceus = cinematic->arceus;
    GF_ASSERT(cmd[0] == ARCEUS_CMD_SAVE_BGM);
    arceus->savedBgm = GF_GetCurrentPlayingBGM();
    return ARCEUS_SCRIPT_CONTINUE;
}

static int ArceusScriptCmd_StopBgm(LegendaryCinematic *cinematic, const int *cmd) {
    u16 bgm;
    GF_ASSERT(cmd[0] == ARCEUS_CMD_STOP_BGM);
    bgm = GF_GetCurrentPlayingBGM();
    StopBGM(bgm, 0);
    return ARCEUS_SCRIPT_CONTINUE;
}

static int ArceusScriptCmd_PlayBgm(LegendaryCinematic *cinematic, const int *cmd) {
    ArceusCinematic *arceus = cinematic->arceus;
    GF_ASSERT(cmd[0] == ARCEUS_CMD_PLAY_BGM);
    if (cmd[1] == -1) {
        PlayBGM(arceus->savedBgm);
    } else {
        PlayBGM((u16)cmd[1]);
    }
    return ARCEUS_SCRIPT_CONTINUE;
}

static int ArceusScriptCmd_SetSpriteVisible(LegendaryCinematic *cinematic, const int *cmd) {
    ArceusCinematic *arceus = cinematic->arceus;
    GF_ASSERT(cmd[0] == ARCEUS_CMD_SET_SPRITE_VISIBLE);
    ManagedSprite_SetDrawFlag(arceus->sprites[0], cmd[1]);
    return ARCEUS_SCRIPT_CONTINUE;
}

static int ArceusScriptCmd_SetSpritePosition(LegendaryCinematic *cinematic, const int *cmd) {
    ArceusCinematic *arceus = cinematic->arceus;
    GF_ASSERT(cmd[0] == ARCEUS_CMD_SET_SPRITE_POSITION);
    ManagedSprite_SetPositionXY(arceus->sprites[0], (s16)cmd[1], (s16)cmd[2]);
    return ARCEUS_SCRIPT_CONTINUE;
}

static int ArceusScriptCmd_SetSpritePriority(LegendaryCinematic *cinematic, const int *cmd) {
    ArceusCinematic *arceus = cinematic->arceus;
    GF_ASSERT(cmd[0] == ARCEUS_CMD_SET_SPRITE_PRIORITY);
    ManagedSprite_SetPriority(arceus->sprites[0], cmd[1]);
    return ARCEUS_SCRIPT_CONTINUE;
}

static int ArceusScriptCmd_Nop(LegendaryCinematic *cinematic, const int *cmd) {
    return ARCEUS_SCRIPT_CONTINUE;
}
