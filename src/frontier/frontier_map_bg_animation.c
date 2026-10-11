#include "global.h"

#include "frontier/frontier_map.h"

#include "bg_window.h"
#include "filesystem.h"
#include "gf_gfx_loader.h"
#include "heap.h"
#include "palette.h"
#include "sys_task_api.h"

// Map effect of some frontier maps: BG3 flips between two tile/screen sets every 13 frames and BG palettes 6-8
// cycle through three palettes every 9 frames.

typedef struct FrontierMapBgTileAnim {
    SysTask *task;
    BgConfig *bgConfig;
    u32 screens[2][0x1000 / 4];
    u32 chars[2][0x10000 / 4];
    u8 timer;
    u8 index;
} FrontierMapBgTileAnim;

typedef struct FrontierMapPaletteAnim {
    SysTask *task;
    PaletteData *paletteData;
    u16 palettes[3][16];
    u8 paletteIndex;
    u8 timer;
} FrontierMapPaletteAnim;

struct FrontierMapBgAnim {
    FrontierMapBgTileAnim *tileAnim;
    FrontierMapPaletteAnim *paletteAnim;
    u32 unused;
};

extern const u16 ov80_0223DB2C[2];

FrontierMapBgTileAnim *FrontierMapBgAnim_CreateTileAnim(BgConfig *bgConfig);
void FrontierMapBgAnim_DestroyTileAnim(FrontierMapBgTileAnim *tileAnim);
void FrontierMapBgAnim_TileTask(SysTask *task, void *data);
FrontierMapPaletteAnim *FrontierMapBgAnim_CreatePaletteAnim(PaletteData *paletteData);
void FrontierMapBgAnim_DestroyPaletteAnim(FrontierMapPaletteAnim *paletteAnim);
void FrontierMapBgAnim_PaletteTask(SysTask *task, void *data);

FrontierMapBgAnim *FrontierMapBgAnim_Create(BgConfig *bgConfig, PaletteData *paletteData) {
    FrontierMapBgAnim *anim = Heap_Alloc(HEAP_ID_101, sizeof(FrontierMapBgAnim));

    MI_CpuFill8(anim, 0, sizeof(FrontierMapBgAnim));
    anim->tileAnim = FrontierMapBgAnim_CreateTileAnim(bgConfig);
    anim->paletteAnim = FrontierMapBgAnim_CreatePaletteAnim(paletteData);
    G2_SetBlendAlpha(GX_BLEND_PLANEMASK_BG2, GX_BLEND_PLANEMASK_BG0 | GX_BLEND_PLANEMASK_BG1 | GX_BLEND_PLANEMASK_BG2 | GX_BLEND_PLANEMASK_BG3 | GX_BLEND_PLANEMASK_OBJ, 8, 9);
    return anim;
}

void FrontierMapBgAnim_Free(FrontierMapBgAnim *anim) {
    FrontierMapBgAnim_DestroyTileAnim(anim->tileAnim);
    FrontierMapBgAnim_DestroyPaletteAnim(anim->paletteAnim);
    Heap_Free(anim);
}

#pragma force_active on
FrontierMapBgTileAnim *FrontierMapBgAnim_CreateTileAnim(BgConfig *bgConfig) {
    NNSG2dScreenData *screenData;
    NNSG2dCharacterData *charData;
    FrontierMapBgTileAnim *tileAnim;
    NARC *narc;
    int i;
    void *file;

    tileAnim = Heap_Alloc(HEAP_ID_101, sizeof(FrontierMapBgTileAnim));
    MI_CpuFill8(tileAnim, 0, sizeof(FrontierMapBgTileAnim));
    tileAnim->bgConfig = bgConfig;
    narc = NARC_New(NARC_a_1_8_3, HEAP_ID_101);
    for (i = 0; i < 2; i++) {
        file = GfGfxLoader_GetScrnDataFromOpenNarc(narc, ov80_0223DB2C[i], TRUE, &screenData, HEAP_ID_101);
        MIi_CpuCopy32(screenData->rawData, tileAnim->screens[i], 0x1000);
        Heap_Free(file);
    }
    NARC_Delete(narc);
    narc = NARC_New(NARC_a_1_8_3, HEAP_ID_101);
    file = GfGfxLoader_GetCharDataFromOpenNarc(narc, 14, TRUE, &charData, HEAP_ID_101);
    MIi_CpuCopy32(charData->pRawData, tileAnim->chars[0], charData->szByte);
    Heap_Free(file);
    file = GfGfxLoader_GetCharDataFromOpenNarc(narc, 15, TRUE, &charData, HEAP_ID_101);
    MIi_CpuCopy32(charData->pRawData, tileAnim->chars[1], charData->szByte);
    Heap_Free(file);
    NARC_Delete(narc);
    tileAnim->task = SysTask_CreateOnMainQueue(FrontierMapBgAnim_TileTask, tileAnim, 79500);
    return tileAnim;
}
#pragma force_active reset

void FrontierMapBgAnim_DestroyTileAnim(FrontierMapBgTileAnim *tileAnim) {
    SysTask_Destroy(tileAnim->task);
    Heap_Free(tileAnim);
}

void FrontierMapBgAnim_TileTask(SysTask *task, void *data) {
    FrontierMapBgTileAnim *tileAnim = data;

    if (tileAnim->timer < 12) {
        tileAnim->timer++;
        return;
    }
    tileAnim->timer = 0;
    BG_LoadCharTilesData(tileAnim->bgConfig, GF_BG_LYR_MAIN_3, tileAnim->chars[tileAnim->index], 0x10000, 0);
    BG_LoadScreenTilemapData(tileAnim->bgConfig, GF_BG_LYR_MAIN_3, tileAnim->screens[tileAnim->index], 0x1000);
    ScheduleBgTilemapBufferTransfer(tileAnim->bgConfig, GF_BG_LYR_MAIN_3);
    tileAnim->index ^= 1;
}

FrontierMapPaletteAnim *FrontierMapBgAnim_CreatePaletteAnim(PaletteData *paletteData) {
    FrontierMapPaletteAnim *paletteAnim = Heap_Alloc(HEAP_ID_101, sizeof(FrontierMapPaletteAnim));
    u16 *palettes;

    MI_CpuFill8(paletteAnim, 0, sizeof(FrontierMapPaletteAnim));
    paletteAnim->paletteData = paletteData;
    paletteAnim->paletteIndex = 2;
    palettes = PaletteData_GetUnfadedBuf(paletteData, PLTTBUF_MAIN_BG);
    MIi_CpuCopy16(palettes + 5 * 16, &paletteAnim->palettes[0][0], sizeof(paletteAnim->palettes));
    paletteAnim->task = SysTask_CreateOnMainQueue(FrontierMapBgAnim_PaletteTask, paletteAnim, 79501);
    return paletteAnim;
}

void FrontierMapBgAnim_DestroyPaletteAnim(FrontierMapPaletteAnim *paletteAnim) {
    SysTask_Destroy(paletteAnim->task);
    Heap_Free(paletteAnim);
}

void FrontierMapBgAnim_PaletteTask(SysTask *task, void *data) {
    FrontierMapPaletteAnim *paletteAnim = data;

    if (paletteAnim->timer < 8) {
        paletteAnim->timer++;
        return;
    }
    paletteAnim->timer = 0;
    PaletteData_LoadPalette(paletteAnim->paletteData, paletteAnim->palettes[paletteAnim->paletteIndex], PLTTBUF_MAIN_BG, 6 * 16, 0x20);
    paletteAnim->paletteIndex++;
    if (paletteAnim->paletteIndex >= 3) {
        paletteAnim->paletteIndex = 0;
    }
}
