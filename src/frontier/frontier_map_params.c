#include "global.h"

#include "frontier/frontier_map.h"

#include "bg_window.h"
#include "gf_gfx_planes.h"
#include "heap.h"

// Per-map parameter table of the frontier maps (ov80_0223B724, indexed by map id) and the map effect callbacks it
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

extern const FrontierMapParams ov80_0223B724[];

u32 FrontierMap_GetParam(u32 mapId, u32 field);
void FrontierMap_InitEffect(void *arg, void *effect, u32 mapId);
void FrontierMap_ExitEffect(void *arg, void *effect, u32 mapId);
void ov80_0222ADCC(void *unused, void **effect);
void ov80_0222ADDC(void *unused, void **effect);
void FrontierMapEffect_InitBgAnim(void **args, void **effect);
void FrontierMapEffect_ExitBgAnim(void *unused, void **effect);
void ov80_0222AE28(void);
void ov80_0222AE2C(void);
void ov80_0222AE30(void);
void ov80_0222AE68(void);
void ov80_0222AE6C(void);
void ov80_0222AE80(void);
void ov80_0222AE84(void);
void ov80_0222AE9C(void);
void FrontierMapEffect_InitTileBlink(void **args, void **effect);
void FrontierMapEffect_ExitTileBlink(void *unused, void **effect);
void ov80_0222AEBC(void);
void ov80_0222AEF4(void);

u32 FrontierMap_GetParam(u32 mapId, u32 field) {
    switch (field) {
    case 0:
        return ov80_0223B724[mapId].bgMode;
    case 1:
        return ov80_0223B724[mapId].scriptId;
    case 2:
        return ov80_0223B724[mapId].msgBank;
    case 3:
        return ov80_0223B724[mapId].bgm;
    case 4:
        return ov80_0223B724[mapId].bgSize;
    case 5:
        return ov80_0223B724[mapId].narc;
    case 6:
        return ov80_0223B724[mapId].bg3Screen;
    case 7:
        return ov80_0223B724[mapId].bg3Char;
    case 8:
        return ov80_0223B724[mapId].bg3Palette;
    case 9:
        return ov80_0223B724[mapId].bg2Screen;
    case 10:
        return ov80_0223B724[mapId].bg2Char;
    case 11:
        return ov80_0223B724[mapId].bg2Palette;
    case 12:
        return ov80_0223B724[mapId].scrollMode;
    case 13:
        return ov80_0223B724[mapId].bg2Scrolls;
    default:
        GF_AssertFail();
        return 0;
    }
}

void FrontierMap_InitEffect(void *arg, void *effect, u32 mapId) {
    FrontierMapEffectFunc func = ov80_0223B724[mapId].initEffect;
    if (func != NULL) {
        func(arg, effect);
    }
}

void FrontierMap_ExitEffect(void *arg, void *effect, u32 mapId) {
    FrontierMapEffectFunc func = ov80_0223B724[mapId].exitEffect;
    if (func != NULL) {
        func(arg, effect);
    }
}

void ov80_0222ADCC(void *unused, void **effect) {
    *effect = Heap_Alloc(HEAP_ID_101, 0x20);
}

void ov80_0222ADDC(void *unused, void **effect) {
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

void ov80_0222AE28(void) {
}

void ov80_0222AE2C(void) {
}

void ov80_0222AE30(void) {
    SetBgPriority(GF_BG_LYR_MAIN_0, 1);
    SetBgPriority(GF_BG_LYR_MAIN_2, 2);
    SetBgPriority(GF_BG_LYR_MAIN_3, 3);
    G2_SetBlendAlpha(GX_BLEND_PLANEMASK_BG0, GX_BLEND_PLANEMASK_BG2 | GX_BLEND_PLANEMASK_BG3 | GX_BLEND_PLANEMASK_OBJ | GX_BLEND_PLANEMASK_BD, 20, 20);
    GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG2, GF_PLANE_TOGGLE_OFF);
}

void ov80_0222AE68(void) {
}

void ov80_0222AE6C(void) {
    SetBgPriority(GF_BG_LYR_MAIN_2, 3);
    SetBgPriority(GF_BG_LYR_MAIN_3, 2);
}

void ov80_0222AE80(void) {
}

void ov80_0222AE84(void) {
    G2_SetBlendAlpha(GX_BLEND_PLANEMASK_BG0, GX_BLEND_PLANEMASK_BG2 | GX_BLEND_PLANEMASK_BG3 | GX_BLEND_PLANEMASK_OBJ | GX_BLEND_PLANEMASK_BD, 20, 20);
}

void ov80_0222AE9C(void) {
}

void FrontierMapEffect_InitTileBlink(void **args, void **effect) {
    *effect = FrontierMapTileBlink_Create(args[0]);
}

void FrontierMapEffect_ExitTileBlink(void *unused, void **effect) {
    FrontierMapTileBlink_Free(*effect);
}

void ov80_0222AEBC(void) {
    SetBgPriority(GF_BG_LYR_MAIN_1, 0);
    SetBgPriority(GF_BG_LYR_MAIN_0, 2);
    SetBgPriority(GF_BG_LYR_MAIN_2, 2);
    SetBgPriority(GF_BG_LYR_MAIN_3, 3);
    G2_SetBlendAlpha(GX_BLEND_PLANEMASK_BG0, GX_BLEND_PLANEMASK_BG2 | GX_BLEND_PLANEMASK_BG3 | GX_BLEND_PLANEMASK_OBJ | GX_BLEND_PLANEMASK_BD, 20, 20);
}

void ov80_0222AEF4(void) {
}
