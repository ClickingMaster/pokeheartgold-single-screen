#include "frontier/frontier_map_params.h"

#include "global.h"

#include "constants/sndseq.h"

#include "frontier/frontier_map.h"
#include "msgdata/msg.naix"

#include "bg_window.h"
#include "filesystem.h"
#include "gf_gfx_planes.h"
#include "heap.h"

// Per-map parameter table of the frontier maps (sFrontierMapParams, indexed by map id) and the map effect callbacks it
// points to.

typedef void (*FrontierMapEffectFunc)(void **args, void **effect);

typedef struct FrontierMapParams {
    u32 bgMode;
    FrontierMapEffectFunc initEffect;
    FrontierMapEffectFunc exitEffect;
    u16 scriptId;
    u16 msgBank;
    u16 bgm;
    u16 bgSize;
    u16 narc;
    u16 bg3Screen;
    u16 bg3Char;
    u16 bg3Palette;
    u16 bg2Screen;
    u16 bg2Char;
    u16 bg2Palette;
    u8 scrollMode;
    u8 bg2Scrolls;
    u8 padding[4];
} FrontierMapParams;

static void FrontierMapEffect_InitAlloc(void **args, void **effect);
static void FrontierMapEffect_ExitFree(void **args, void **effect);
static void FrontierMapEffect_InitBgAnim(void **args, void **effect);
static void FrontierMapEffect_ExitBgAnim(void **args, void **effect);
static void FrontierMapEffect_InitArcadeNone(void **args, void **effect);
static void FrontierMapEffect_ExitArcadeNone(void **args, void **effect);
static void FrontierMapEffect_InitArcadeBlend(void **args, void **effect);
static void FrontierMapEffect_ExitArcadeBlend(void **args, void **effect);
static void FrontierMapEffect_InitFactoryBgPriority(void **args, void **effect);
static void FrontierMapEffect_ExitFactoryBgPriority(void **args, void **effect);
static void FrontierMapEffect_InitFactoryBlend(void **args, void **effect);
static void FrontierMapEffect_ExitFactoryBlend(void **args, void **effect);
static void FrontierMapEffect_InitTileBlink(void **args, void **effect);
static void FrontierMapEffect_ExitTileBlink(void **args, void **effect);
static void FrontierMapEffect_InitCastleBlend(void **args, void **effect);
static void FrontierMapEffect_ExitCastleBlend(void **args, void **effect);

static const FrontierMapParams sFrontierMapParams[] = {
    [0] = { .bgMode = GX_BGMODE_0, .initEffect = FrontierMapEffect_InitAlloc,             .exitEffect = FrontierMapEffect_ExitFree,              .scriptId = 4,  .msgBank = NARC_msg_msg_0035_bin,          .bgm = SEQ_GS_BATTLETOWER, .bgSize = GF_BG_SCR_SIZE_512x256, .narc = NARC_a_1_8_3, .bg3Screen = 1,   .bg3Char = 0,   .bg3Palette = 148, .bg2Screen = 3,                  .bg2Char = 0,                  .bg2Palette = 148,                .scrollMode = 0, .bg2Scrolls = 1 },
    [1] = { .bgMode = GX_BGMODE_0, .initEffect = NULL,                                    .exitEffect = NULL,                                    .scriptId = 0,  .msgBank = NARC_msg_msg_0035_bin,          .bgm = SEQ_GS_BATTLETOWER, .bgSize = GF_BG_SCR_SIZE_512x256, .narc = NARC_a_1_8_3, .bg3Screen = 2,   .bg3Char = 0,   .bg3Palette = 148, .bg2Screen = FRONTIER_MAP_NO_ID, .bg2Char = FRONTIER_MAP_NO_ID, .bg2Palette = FRONTIER_MAP_NO_ID, .scrollMode = 0, .bg2Scrolls = 1 },
    [2] = { .bgMode = GX_BGMODE_0, .initEffect = NULL,                                    .exitEffect = NULL,                                    .scriptId = 5,  .msgBank = NARC_msg_msg_0036_bin,          .bgm = SEQ_GS_BATTLETOWER, .bgSize = GF_BG_SCR_SIZE_512x256, .narc = NARC_a_1_8_3, .bg3Screen = 1,   .bg3Char = 0,   .bg3Palette = 148, .bg2Screen = FRONTIER_MAP_NO_ID, .bg2Char = FRONTIER_MAP_NO_ID, .bg2Palette = FRONTIER_MAP_NO_ID, .scrollMode = 0, .bg2Scrolls = 1 },
    [3] = { .bgMode = GX_BGMODE_5, .initEffect = FrontierMapEffect_InitFactoryBgPriority, .exitEffect = FrontierMapEffect_ExitFactoryBgPriority, .scriptId = 1,  .msgBank = NARC_msg_msg_0195_bin,          .bgm = SEQ_GS_BF_FACTORY,  .bgSize = GF_BG_SCR_SIZE_512x512, .narc = NARC_a_1_8_3, .bg3Screen = 2,   .bg3Char = 0,   .bg3Palette = 148, .bg2Screen = 3,                  .bg2Char = 0,                  .bg2Palette = 148,                .scrollMode = 1, .bg2Scrolls = 0 },
    [4] = { .bgMode = GX_BGMODE_5, .initEffect = FrontierMapEffect_InitFactoryBlend,      .exitEffect = FrontierMapEffect_ExitFactoryBlend,      .scriptId = 1,  .msgBank = NARC_msg_msg_0195_bin,          .bgm = SEQ_GS_BF_FACTORY,  .bgSize = GF_BG_SCR_SIZE_256x256, .narc = NARC_a_1_8_3, .bg3Screen = 1,   .bg3Char = 0,   .bg3Palette = 148, .bg2Screen = FRONTIER_MAP_NO_ID, .bg2Char = FRONTIER_MAP_NO_ID, .bg2Palette = FRONTIER_MAP_NO_ID, .scrollMode = 1, .bg2Scrolls = 1 },
    [5] = { .bgMode = GX_BGMODE_5, .initEffect = NULL,                                    .exitEffect = NULL,                                    .scriptId = 6,  .msgBank = NARC_msg_msg_0096_D31R0201_bin, .bgm = SEQ_GS_BATTLETOWER, .bgSize = GF_BG_SCR_SIZE_512x512, .narc = NARC_a_1_8_3, .bg3Screen = 54,  .bg3Char = 49,  .bg3Palette = 160, .bg2Screen = 55,                 .bg2Char = 49,                 .bg2Palette = 160,                .scrollMode = 1, .bg2Scrolls = 1 },
    [6] = { .bgMode = GX_BGMODE_5, .initEffect = NULL,                                    .exitEffect = NULL,                                    .scriptId = 7,  .msgBank = NARC_msg_msg_0096_D31R0201_bin, .bgm = SEQ_GS_BATTLETOWER, .bgSize = GF_BG_SCR_SIZE_512x512, .narc = NARC_a_1_8_3, .bg3Screen = 51,  .bg3Char = 49,  .bg3Palette = 160, .bg2Screen = 52,                 .bg2Char = 49,                 .bg2Palette = 160,                .scrollMode = 1, .bg2Scrolls = 1 },
    [7] = { .bgMode = GX_BGMODE_5, .initEffect = NULL,                                    .exitEffect = NULL,                                    .scriptId = 8,  .msgBank = NARC_msg_msg_0097_bin,          .bgm = SEQ_GS_BATTLETOWER, .bgSize = GF_BG_SCR_SIZE_256x256, .narc = NARC_a_1_8_3, .bg3Screen = 53,  .bg3Char = 49,  .bg3Palette = 160, .bg2Screen = FRONTIER_MAP_NO_ID, .bg2Char = FRONTIER_MAP_NO_ID, .bg2Palette = FRONTIER_MAP_NO_ID, .scrollMode = 1, .bg2Scrolls = 1 },
    [8] = { .bgMode = GX_BGMODE_5, .initEffect = NULL,                                    .exitEffect = NULL,                                    .scriptId = 9,  .msgBank = NARC_msg_msg_0098_bin,          .bgm = SEQ_GS_BATTLETOWER, .bgSize = GF_BG_SCR_SIZE_256x256, .narc = NARC_a_1_8_3, .bg3Screen = 50,  .bg3Char = 49,  .bg3Palette = 160, .bg2Screen = FRONTIER_MAP_NO_ID, .bg2Char = FRONTIER_MAP_NO_ID, .bg2Palette = FRONTIER_MAP_NO_ID, .scrollMode = 1, .bg2Scrolls = 1 },
    [9] = { .bgMode = GX_BGMODE_5, .initEffect = NULL,                                    .exitEffect = NULL,                                    .scriptId = 3,  .msgBank = NARC_msg_msg_0441_bin,          .bgm = SEQ_GS_BF_STAGE,    .bgSize = GF_BG_SCR_SIZE_256x256, .narc = NARC_a_1_8_3, .bg3Screen = 17,  .bg3Char = 16,  .bg3Palette = 151, .bg2Screen = FRONTIER_MAP_NO_ID, .bg2Char = FRONTIER_MAP_NO_ID, .bg2Palette = FRONTIER_MAP_NO_ID, .scrollMode = 1, .bg2Scrolls = 1 },
    [10] = { .bgMode = GX_BGMODE_5, .initEffect = FrontierMapEffect_InitBgAnim,            .exitEffect = FrontierMapEffect_ExitBgAnim,            .scriptId = 3,  .msgBank = NARC_msg_msg_0441_bin,          .bgm = SEQ_GS_BF_STAGE,    .bgSize = GF_BG_SCR_SIZE_512x512, .narc = NARC_a_1_8_3, .bg3Screen = 20,  .bg3Char = 14,  .bg3Palette = 150, .bg2Screen = 22,                 .bg2Char = 19,                 .bg2Palette = 152,                .scrollMode = 1, .bg2Scrolls = 1 },
    [11] = { .bgMode = GX_BGMODE_5, .initEffect = NULL,                                    .exitEffect = NULL,                                    .scriptId = 0,  .msgBank = NARC_msg_msg_0032_bin,          .bgm = SEQ_GS_BF_CASTLE,   .bgSize = GF_BG_SCR_SIZE_512x512, .narc = NARC_a_1_8_3, .bg3Screen = 26,  .bg3Char = 25,  .bg3Palette = 154, .bg2Screen = FRONTIER_MAP_NO_ID, .bg2Char = FRONTIER_MAP_NO_ID, .bg2Palette = FRONTIER_MAP_NO_ID, .scrollMode = 1, .bg2Scrolls = 1 },
    [12] = { .bgMode = GX_BGMODE_5, .initEffect = FrontierMapEffect_InitCastleBlend,       .exitEffect = FrontierMapEffect_ExitCastleBlend,       .scriptId = 0,  .msgBank = NARC_msg_msg_0032_bin,          .bgm = SEQ_GS_BF_CASTLE,   .bgSize = GF_BG_SCR_SIZE_256x256, .narc = NARC_a_1_8_3, .bg3Screen = 30,  .bg3Char = 29,  .bg3Palette = 159, .bg2Screen = 33,                 .bg2Char = 29,                 .bg2Palette = 159,                .scrollMode = 1, .bg2Scrolls = 1 },
    [13] = { .bgMode = GX_BGMODE_5, .initEffect = NULL,                                    .exitEffect = NULL,                                    .scriptId = 0,  .msgBank = NARC_msg_msg_0032_bin,          .bgm = SEQ_GS_BF_CASTLE,   .bgSize = GF_BG_SCR_SIZE_256x256, .narc = NARC_a_1_8_3, .bg3Screen = 28,  .bg3Char = 27,  .bg3Palette = 155, .bg2Screen = FRONTIER_MAP_NO_ID, .bg2Char = FRONTIER_MAP_NO_ID, .bg2Palette = FRONTIER_MAP_NO_ID, .scrollMode = 1, .bg2Scrolls = 1 },
    [14] = { .bgMode = GX_BGMODE_5, .initEffect = FrontierMapEffect_InitTileBlink,         .exitEffect = FrontierMapEffect_ExitTileBlink,         .scriptId = 10, .msgBank = NARC_msg_msg_0018_bin,          .bgm = SEQ_GS_BATTLETOWER, .bgSize = GF_BG_SCR_SIZE_256x256, .narc = NARC_a_1_8_3, .bg3Screen = 132, .bg3Char = 131, .bg3Palette = 191, .bg2Screen = FRONTIER_MAP_NO_ID, .bg2Char = FRONTIER_MAP_NO_ID, .bg2Palette = FRONTIER_MAP_NO_ID, .scrollMode = 1, .bg2Scrolls = 1 },
    [15] = { .bgMode = GX_BGMODE_5, .initEffect = FrontierMapEffect_InitArcadeNone,        .exitEffect = FrontierMapEffect_ExitArcadeNone,        .scriptId = 2,  .msgBank = NARC_msg_msg_0426_bin,          .bgm = SEQ_GS_BF_ROULETTE, .bgSize = GF_BG_SCR_SIZE_256x256, .narc = NARC_a_1_8_3, .bg3Screen = 59,  .bg3Char = 60,  .bg3Palette = 162, .bg2Screen = FRONTIER_MAP_NO_ID, .bg2Char = FRONTIER_MAP_NO_ID, .bg2Palette = FRONTIER_MAP_NO_ID, .scrollMode = 1, .bg2Scrolls = 1 },
    [16] = { .bgMode = GX_BGMODE_5, .initEffect = FrontierMapEffect_InitArcadeBlend,       .exitEffect = FrontierMapEffect_ExitArcadeBlend,       .scriptId = 2,  .msgBank = NARC_msg_msg_0426_bin,          .bgm = SEQ_GS_BF_ROULETTE, .bgSize = GF_BG_SCR_SIZE_512x512, .narc = NARC_a_1_8_3, .bg3Screen = 56,  .bg3Char = 58,  .bg3Palette = 161, .bg2Screen = 61,                 .bg2Char = 93,                 .bg2Palette = 188,                .scrollMode = 1, .bg2Scrolls = 1 },
};

u32 FrontierMap_GetParam(u32 mapId, u32 field) {
    switch (field) {
    case 0:
        return sFrontierMapParams[mapId].bgMode;
    case 1:
        return sFrontierMapParams[mapId].scriptId;
    case 2:
        return sFrontierMapParams[mapId].msgBank;
    case 3:
        return sFrontierMapParams[mapId].bgm;
    case 4:
        return sFrontierMapParams[mapId].bgSize;
    case 5:
        return sFrontierMapParams[mapId].narc;
    case 6:
        return sFrontierMapParams[mapId].bg3Screen;
    case 7:
        return sFrontierMapParams[mapId].bg3Char;
    case 8:
        return sFrontierMapParams[mapId].bg3Palette;
    case 9:
        return sFrontierMapParams[mapId].bg2Screen;
    case 10:
        return sFrontierMapParams[mapId].bg2Char;
    case 11:
        return sFrontierMapParams[mapId].bg2Palette;
    case 12:
        return sFrontierMapParams[mapId].scrollMode;
    case 13:
        return sFrontierMapParams[mapId].bg2Scrolls;
    default:
        GF_AssertFail();
        return 0;
    }
}

void FrontierMap_InitEffect(void *arg, void *effect, u32 mapId) {
    FrontierMapEffectFunc func = sFrontierMapParams[mapId].initEffect;
    if (func != NULL) {
        func(arg, effect);
    }
}

void FrontierMap_ExitEffect(void *arg, void *effect, u32 mapId) {
    FrontierMapEffectFunc func = sFrontierMapParams[mapId].exitEffect;
    if (func != NULL) {
        func(arg, effect);
    }
}

static void FrontierMapEffect_InitAlloc(void **args, void **effect) {
    *effect = Heap_Alloc(HEAP_ID_101, 0x20);
}

static void FrontierMapEffect_ExitFree(void **args, void **effect) {
    Heap_Free(*effect);
}

static void FrontierMapEffect_InitBgAnim(void **args, void **effect) {
    *effect = FrontierMapBgAnim_Create(args[0], args[1]);
    SetBgPriority(GF_BG_LYR_MAIN_0, 1);
    SetBgPriority(GF_BG_LYR_MAIN_2, 2);
    SetBgPriority(GF_BG_LYR_MAIN_3, 3);
    GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG2, GF_PLANE_TOGGLE_OFF);
}

static void FrontierMapEffect_ExitBgAnim(void **args, void **effect) {
    FrontierMapBgAnim_Free(*effect);
}

static void FrontierMapEffect_InitArcadeNone(void **args, void **effect) {
}

static void FrontierMapEffect_ExitArcadeNone(void **args, void **effect) {
}

static void FrontierMapEffect_InitArcadeBlend(void **args, void **effect) {
    SetBgPriority(GF_BG_LYR_MAIN_0, 1);
    SetBgPriority(GF_BG_LYR_MAIN_2, 2);
    SetBgPriority(GF_BG_LYR_MAIN_3, 3);
    G2_SetBlendAlpha(GX_BLEND_PLANEMASK_BG0, GX_BLEND_PLANEMASK_BG2 | GX_BLEND_PLANEMASK_BG3 | GX_BLEND_PLANEMASK_OBJ | GX_BLEND_PLANEMASK_BD, 20, 20);
    GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG2, GF_PLANE_TOGGLE_OFF);
}

static void FrontierMapEffect_ExitArcadeBlend(void **args, void **effect) {
}

static void FrontierMapEffect_InitFactoryBgPriority(void **args, void **effect) {
    SetBgPriority(GF_BG_LYR_MAIN_2, 3);
    SetBgPriority(GF_BG_LYR_MAIN_3, 2);
}

static void FrontierMapEffect_ExitFactoryBgPriority(void **args, void **effect) {
}

static void FrontierMapEffect_InitFactoryBlend(void **args, void **effect) {
    G2_SetBlendAlpha(GX_BLEND_PLANEMASK_BG0, GX_BLEND_PLANEMASK_BG2 | GX_BLEND_PLANEMASK_BG3 | GX_BLEND_PLANEMASK_OBJ | GX_BLEND_PLANEMASK_BD, 20, 20);
}

static void FrontierMapEffect_ExitFactoryBlend(void **args, void **effect) {
}

static void FrontierMapEffect_InitTileBlink(void **args, void **effect) {
    *effect = FrontierMapTileBlink_Create(args[0]);
}

static void FrontierMapEffect_ExitTileBlink(void **args, void **effect) {
    FrontierMapTileBlink_Free(*effect);
}

static void FrontierMapEffect_InitCastleBlend(void **args, void **effect) {
    SetBgPriority(GF_BG_LYR_MAIN_1, 0);
    SetBgPriority(GF_BG_LYR_MAIN_0, 2);
    SetBgPriority(GF_BG_LYR_MAIN_2, 2);
    SetBgPriority(GF_BG_LYR_MAIN_3, 3);
    G2_SetBlendAlpha(GX_BLEND_PLANEMASK_BG0, GX_BLEND_PLANEMASK_BG2 | GX_BLEND_PLANEMASK_BG3 | GX_BLEND_PLANEMASK_OBJ | GX_BLEND_PLANEMASK_BD, 20, 20);
}

static void FrontierMapEffect_ExitCastleBlend(void **args, void **effect) {
}
