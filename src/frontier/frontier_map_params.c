#include "global.h"

#include "frontier/frontier_map.h"

#include "bg_window.h"
#include "gf_gfx_planes.h"
#include "heap.h"

// Per-map parameter table of the frontier maps (sFrontierMapParams, indexed by map id) and the map effect callbacks it
// points to.

typedef void (*FrontierMapEffectFunc)(void *arg, void *effect);

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

extern const FrontierMapParams sFrontierMapParams[];

u32 FrontierMap_GetParam(u32 mapId, u32 field);
void FrontierMap_InitEffect(void *arg, void *effect, u32 mapId);
void FrontierMap_ExitEffect(void *arg, void *effect, u32 mapId);
void FrontierMapEffect_InitAlloc(void *unused, void **effect);
void FrontierMapEffect_ExitFree(void *unused, void **effect);
void FrontierMapEffect_InitBgAnim(void **args, void **effect);
void FrontierMapEffect_ExitBgAnim(void *unused, void **effect);
void FrontierMapEffect_InitArcadeNone(void);
void FrontierMapEffect_ExitArcadeNone(void);
void FrontierMapEffect_InitArcadeBlend(void);
void FrontierMapEffect_ExitArcadeBlend(void);
void FrontierMapEffect_InitFactoryBgPriority(void);
void FrontierMapEffect_ExitFactoryBgPriority(void);
void FrontierMapEffect_InitFactoryBlend(void);
void FrontierMapEffect_ExitFactoryBlend(void);
void FrontierMapEffect_InitTileBlink(void **args, void **effect);
void FrontierMapEffect_ExitTileBlink(void *unused, void **effect);
void FrontierMapEffect_InitCastleBlend(void);
void FrontierMapEffect_ExitCastleBlend(void);

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

void FrontierMapEffect_InitAlloc(void *unused, void **effect) {
    *effect = Heap_Alloc(HEAP_ID_101, 0x20);
}

void FrontierMapEffect_ExitFree(void *unused, void **effect) {
    Heap_Free(*effect);
}

void FrontierMapEffect_InitBgAnim(void **args, void **effect) {
    *effect = FrontierMapBgAnim_Create(args[0], args[1]);
    SetBgPriority(GF_BG_LYR_MAIN_0, 1);
    SetBgPriority(GF_BG_LYR_MAIN_2, 2);
    SetBgPriority(GF_BG_LYR_MAIN_3, 3);
    GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG2, GF_PLANE_TOGGLE_OFF);
}

void FrontierMapEffect_ExitBgAnim(void *unused, void **effect) {
    FrontierMapBgAnim_Free(*effect);
}

void FrontierMapEffect_InitArcadeNone(void) {
}

void FrontierMapEffect_ExitArcadeNone(void) {
}

void FrontierMapEffect_InitArcadeBlend(void) {
    SetBgPriority(GF_BG_LYR_MAIN_0, 1);
    SetBgPriority(GF_BG_LYR_MAIN_2, 2);
    SetBgPriority(GF_BG_LYR_MAIN_3, 3);
    G2_SetBlendAlpha(GX_BLEND_PLANEMASK_BG0, GX_BLEND_PLANEMASK_BG2 | GX_BLEND_PLANEMASK_BG3 | GX_BLEND_PLANEMASK_OBJ | GX_BLEND_PLANEMASK_BD, 20, 20);
    GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG2, GF_PLANE_TOGGLE_OFF);
}

void FrontierMapEffect_ExitArcadeBlend(void) {
}

void FrontierMapEffect_InitFactoryBgPriority(void) {
    SetBgPriority(GF_BG_LYR_MAIN_2, 3);
    SetBgPriority(GF_BG_LYR_MAIN_3, 2);
}

void FrontierMapEffect_ExitFactoryBgPriority(void) {
}

void FrontierMapEffect_InitFactoryBlend(void) {
    G2_SetBlendAlpha(GX_BLEND_PLANEMASK_BG0, GX_BLEND_PLANEMASK_BG2 | GX_BLEND_PLANEMASK_BG3 | GX_BLEND_PLANEMASK_OBJ | GX_BLEND_PLANEMASK_BD, 20, 20);
}

void FrontierMapEffect_ExitFactoryBlend(void) {
}

void FrontierMapEffect_InitTileBlink(void **args, void **effect) {
    *effect = FrontierMapTileBlink_Create(args[0]);
}

void FrontierMapEffect_ExitTileBlink(void *unused, void **effect) {
    FrontierMapTileBlink_Free(*effect);
}

void FrontierMapEffect_InitCastleBlend(void) {
    SetBgPriority(GF_BG_LYR_MAIN_1, 0);
    SetBgPriority(GF_BG_LYR_MAIN_0, 2);
    SetBgPriority(GF_BG_LYR_MAIN_2, 2);
    SetBgPriority(GF_BG_LYR_MAIN_3, 3);
    G2_SetBlendAlpha(GX_BLEND_PLANEMASK_BG0, GX_BLEND_PLANEMASK_BG2 | GX_BLEND_PLANEMASK_BG3 | GX_BLEND_PLANEMASK_OBJ | GX_BLEND_PLANEMASK_BD, 20, 20);
}

void FrontierMapEffect_ExitCastleBlend(void) {
}
