#include "global.h"

#include "constants/sndseq.h"

#include "application/vs_recorder/vs_recorder_internal.h"

#include "bg_window.h"
#include "filesystem.h"
#include "font_sprite.h"
#include "frontier_data.h"
#include "gf_gfx_loader.h"
#include "gf_gfx_planes.h"
#include "heap.h"
#include "math_util.h"
#include "orbit_cursor.h"
#include "palette.h"
#include "save_vars_flags.h"
#include "sound_playback.h"
#include "sprite_system.h"
#include "sys_vars.h"
#include "touchscreen.h"

// VS Recorder Frontier record screen (VS_RECORDER_SCREEN_FRONTIER): the five Battle Frontier prints spinning in around
// the player, the Battle Point count and the exit button. Starts with the saved video screen's exit button cleanup.

typedef struct VsRecorderIntSet5 {
    int values[5];
} VsRecorderIntSet5;

extern const VsRecorderIntSet5 ov40_02245174; // print palette slots
extern const VsRecorderIntSet5 ov40_02245188; // facility icon char files (NARC_a_1_7_9)
extern const VsRecorderIntSet5 ov40_0224519C; // facility icon palette files (NARC_a_1_7_9)
extern const VsRecorderIntSet5 ov40_022451B0; // print animation per facility
extern const s16 _02245CC0[];                 // print positions (x, y) after the spin

void VsRecorder_FreeExitButton(VsRecorderWork *work) {
    VsRecorderSavedVideo *savedVideo = work->screenWork;
    VsRecorder_FreeTextSprite(&savedVideo->exitButton);
    Sprite_DeleteAndFreeResources(savedVideo->exitButton.sprite);
    VsRecorder_FreeButtonGfx(work);
}

void VsRecorder_PlaceFrontierExitButton(VsRecorderWork *work, BOOL left) {
    VsRecorderFrontierRecord *record = work->screenWork;
    if (left == FALSE) {
        ManagedSprite_SetPositionXY(record->exitButton.sprite, 128, 232);
    } else {
        ManagedSprite_SetPositionXY(record->exitButton.sprite, 80, 232);
    }
    TextOBJ_SetPosition(record->exitButton.text, 36, -8);
}

void VsRecorder_LoadFrontierButtonGfx(VsRecorderWork *work, u32 vram) {
    NARC *narc = work->narc;
    SpriteSystem *spriteSystem = work->spriteSystem;
    SpriteManager *spriteManager = work->spriteManager;

    SpriteSystem_LoadCharResObjFromOpenNarc(spriteSystem, spriteManager, narc, 0x32, FALSE, vram, 11924);
    SpriteSystem_LoadCellResObjFromOpenNarc(spriteSystem, spriteManager, narc, 0x1C, FALSE, 11924);
    SpriteSystem_LoadAnimResObjFromOpenNarc(spriteSystem, spriteManager, narc, 0x1D, FALSE, 11924);
}

void VsRecorder_CreateFrontierButtons(VsRecorderWork *work) {
    VsRecorderFrontierRecord *record = work->screenWork;
    VsRecorder_LoadFrontierButtonGfx(work, NNS_G2D_VRAM_TYPE_2DSUB);
    record->exitButton.sprite = VsRecorder_NewButtonSprite(work, NNS_G2D_VRAM_TYPE_2DSUB);
    VsRecorder_CreateTextSprite(&record->exitButton, (VsRecorderGfx *)&work->narc, NNS_G2D_VRAM_TYPE_2DSUB);
    VsRecorder_SetTextSpriteMsg(&record->exitButton, (VsRecorderGfx *)&work->narc, 3); // EXIT
    ManagedSprite_SetPositionXY(record->exitButton.sprite, 128, 232);
    TextOBJ_SetPosition(record->exitButton.text, 36, -8);
    TextOBJ_SetSpritesDrawFlag(record->exitButton.text, TRUE);
    record->buttonSprite = VsRecorder_NewButtonSprite(work, NNS_G2D_VRAM_TYPE_2DSUB);
    ManagedSprite_SetPositionXY(record->buttonSprite, 220, 224);
    ManagedSprite_SetAnim(record->buttonSprite, 2);
    ManagedSprite_SetDrawFlag(record->buttonSprite, FALSE);
    VsRecorder_PlaceFrontierExitButton(work, TRUE);
}

void VsRecorder_FreeFrontierButtons(VsRecorderWork *work) {
    VsRecorderFrontierRecord *record = work->screenWork;
    VsRecorder_FreeTextSprite(&record->exitButton);
    Sprite_DeleteAndFreeResources(record->exitButton.sprite);
    Sprite_DeleteAndFreeResources(record->buttonSprite);
    VsRecorder_FreeButtonGfx(work);
}

#pragma force_active on

void VsRecorder_LoadFrontierGfx(VsRecorderWork *work) {
    VsRecorderIntSet5 palFileIds;
    VsRecorderIntSet5 charFileIds;
    BgConfig *bgConfig = work->bgConfig;
    NARC *narc = work->narc;
    SpriteSystem *spriteSystem = work->spriteSystem;
    SpriteManager *spriteManager = work->spriteManager;
    PaletteData *plttData = work->plttData;
    int i;
    int palFileId;
    int charFileId;

    GfGfxLoader_LoadCharDataFromOpenNarc(narc, 62, bgConfig, GF_BG_LYR_SUB_2, 0, 0, FALSE, HEAP_ID_109);
    GfGfxLoader_LoadScrnDataFromOpenNarc(narc, 68, bgConfig, GF_BG_LYR_SUB_2, 0, 0, FALSE, HEAP_ID_109);
    SpriteSystem_LoadPaletteBufferFromOpenNarc(plttData, PLTTBUF_SUB_OBJ, spriteSystem, spriteManager, narc, VsRecorder_GetSchemeFileId(work, 0), FALSE, 1, NNS_G2D_VRAM_TYPE_2DSUB, 29292);
    SpriteSystem_LoadCharResObjFromOpenNarc(spriteSystem, spriteManager, narc, 66, FALSE, NNS_G2D_VRAM_TYPE_2DSUB, 29292);
    SpriteSystem_LoadCellResObjFromOpenNarc(spriteSystem, spriteManager, narc, 71, FALSE, 29292);
    SpriteSystem_LoadAnimResObjFromOpenNarc(spriteSystem, spriteManager, narc, 72, FALSE, 29292);
    SpriteSystem_LoadPaletteBufferFromOpenNarc(plttData, PLTTBUF_MAIN_OBJ, spriteSystem, spriteManager, narc, VsRecorder_GetSchemeFileId(work, 1), FALSE, 6, NNS_G2D_VRAM_TYPE_2DMAIN, 28282);
    SpriteSystem_LoadCharResObjFromOpenNarc(spriteSystem, spriteManager, narc, 64, FALSE, NNS_G2D_VRAM_TYPE_2DMAIN, 28282);
    SpriteSystem_LoadCellResObjFromOpenNarc(spriteSystem, spriteManager, narc, 38, FALSE, 28282);
    SpriteSystem_LoadAnimResObjFromOpenNarc(spriteSystem, spriteManager, narc, 39, FALSE, 28282);

    // facility icons: Battle Castle (3) from the app's NARC, the others from NARC_a_1_7_9
    palFileIds = ov40_0224519C;
    charFileIds = ov40_02245188;
    for (i = 0; i < 5; i++) {
        if (i == 3) {
            SpriteSystem_LoadPaletteBufferFromOpenNarc(plttData, PLTTBUF_MAIN_OBJ, spriteSystem, spriteManager, narc, 92, FALSE, 1, NNS_G2D_VRAM_TYPE_2DMAIN, 18181 + i);
            SpriteSystem_LoadCharResObjFromOpenNarc(spriteSystem, spriteManager, narc, 91, FALSE, NNS_G2D_VRAM_TYPE_2DMAIN, 18181 + i);
        } else {
            SpriteSystem_LoadPaletteBuffer(plttData, PLTTBUF_MAIN_OBJ, spriteSystem, spriteManager, NARC_a_1_7_9, palFileIds.values[i], FALSE, 1, NNS_G2D_VRAM_TYPE_2DMAIN, 18181 + i);
            SpriteSystem_LoadCharResObjAtEndWithHardwareMappingType(spriteSystem, spriteManager, NARC_a_1_7_9, charFileIds.values[i], FALSE, NNS_G2D_VRAM_TYPE_2DMAIN, 18181 + i);
        }
    }
    SpriteSystem_LoadCellResObj(spriteSystem, spriteManager, NARC_a_1_7_9, 9, FALSE, 18181);
    SpriteSystem_LoadAnimResObj(spriteSystem, spriteManager, NARC_a_1_7_9, 10, FALSE, 18181);
    SpriteSystem_LoadCellResObjFromOpenNarc(spriteSystem, spriteManager, narc, 89, FALSE, 18182);
    SpriteSystem_LoadAnimResObjFromOpenNarc(spriteSystem, spriteManager, narc, 90, FALSE, 18182);

    // player icon by gender (i = female)
    palFileId = 13;
    charFileId = 14;
    i = FrontierCard_GetGender(work->videoOwners[0]);
    spriteSystem = work->spriteSystem;
    spriteManager = work->spriteManager;
    plttData = work->plttData;
    if (i) {
        palFileId = 15;
        charFileId = 16;
    }
    SpriteSystem_LoadPaletteBuffer(plttData, PLTTBUF_MAIN_OBJ, spriteSystem, spriteManager, NARC_a_1_7_9, palFileId, FALSE, 1, NNS_G2D_VRAM_TYPE_2DMAIN, 165535);
    SpriteSystem_LoadCellResObj(spriteSystem, spriteManager, NARC_a_1_7_9, 9, FALSE, 165535);
    SpriteSystem_LoadAnimResObj(spriteSystem, spriteManager, NARC_a_1_7_9, 10, FALSE, 165535);
    SpriteSystem_LoadCharResObjAtEndWithHardwareMappingType(spriteSystem, spriteManager, NARC_a_1_7_9, charFileId, FALSE, NNS_G2D_VRAM_TYPE_2DMAIN, 165535);
}

#pragma force_active reset

// converts count colours from start on to grey
void VsRecorder_GrayscalePalette(u16 *buf, int start, int count) {
    int i;
    for (i = 0; i < start; i++) {
        buf++;
    }
    for (; i < start + count; i++) {
        int color = *buf;
        int gray = ((color & 0x1F) * 76 + ((color >> 5) & 0x1F) * 151 + ((color >> 10) & 0x1F) * 29) >> 8;
        *buf = (u16)((gray << 10) | (gray << 5) | gray);
        buf++;
    }
}

#pragma force_active on

void VsRecorder_GrayFrontierPrints(VsRecorderWork *work) {
    PaletteData *plttData = work->plttData;
    SaveVarsFlags *varsFlags = Save_VarsFlags_Get(work->saveData);
    int progress[5];
    VsRecorderIntSet5 slots;
    int i;

    progress[0] = Save_VarsFlags_GetBattleTowerPrintProgress(varsFlags);
    progress[1] = Save_VarsFlags_GetBattleFactoryPrintProgress(varsFlags);
    progress[2] = Save_VarsFlags_GetBattleArcadePrintProgress(varsFlags);
    progress[3] = Save_VarsFlags_GetBattleCastlePrintProgress(varsFlags);
    progress[4] = Save_VarsFlags_GetBattleHallPrintProgress(varsFlags);
    for (i = 0; i < 5; i++) {
        if (progress[i] != 0) {
            slots = ov40_02245174;
            if (progress[i] == 2 || progress[i] == 3) {
                VsRecorder_GrayscalePalette(PaletteData_GetFadedBuf(work->plttData, PLTTBUF_MAIN_OBJ), (u16)((slots.values[i] + 4) * 16), 16);
            }
        }
    }
    PaletteData_SetAutoTransparent(plttData, TRUE);
}

void VsRecorder_CreateFrontierSprites(VsRecorderWork *work) {
    int zeros = 0;
    VsRecorderFrontierRecord *record = work->screenWork;
    int digits[5];
    int progress[5];
    ManagedSpriteTemplate template;
    VsRecorderIntSet5 printAnims;
    int started;
    int i;
    int div;
    int value;
    SaveVarsFlags *varsFlags;

    template.x = 0;
    template.y = 256;
    template.z = 0;
    template.animation = 0;
    template.drawPriority = 10;
    template.vram = NNS_G2D_VRAM_TYPE_2DMAIN;
    template.pal = 0;
    template.bgPriority = 0;
    template.vramTransfer = 0;
    template.resIdList[GF_GFX_RES_TYPE_MCEL] = -1;
    template.resIdList[GF_GFX_RES_TYPE_MANM] = -1;
    template.resIdList[GF_GFX_RES_TYPE_CHAR] = 165535;
    template.resIdList[GF_GFX_RES_TYPE_PLTT] = 165535;
    template.resIdList[GF_GFX_RES_TYPE_CELL] = 165535;
    template.resIdList[GF_GFX_RES_TYPE_ANIM] = 165535;
    for (i = 0; i < 5; i++) {
        record->playerIcons[i] = SpriteSystem_NewSprite(work->spriteSystem, work->spriteManager, &template);
        ManagedSprite_SetAnim(record->playerIcons[i], 1);
        ManagedSprite_TickFrame(record->playerIcons[i]);
        ManagedSprite_SetAffineScale(record->playerIcons[i], record->scales[i], record->scales[i]);
    }
    for (i = 0; i < 5; i++) {
        template.resIdList[GF_GFX_RES_TYPE_CHAR] = 18181 + i;
        template.resIdList[GF_GFX_RES_TYPE_PLTT] = 18181 + i;
        if (i == 3) {
            template.resIdList[GF_GFX_RES_TYPE_CELL] = 18182;
            template.resIdList[GF_GFX_RES_TYPE_ANIM] = 18182;
        } else {
            template.resIdList[GF_GFX_RES_TYPE_CELL] = 18181;
            template.resIdList[GF_GFX_RES_TYPE_ANIM] = 18181;
        }
        record->facilityIcons[i] = SpriteSystem_NewSprite(work->spriteSystem, work->spriteManager, &template);
        if (i == 3) {
            ManagedSprite_SetAnim(record->facilityIcons[i], 0);
        } else {
            ManagedSprite_SetAnim(record->facilityIcons[i], 1);
        }
        ManagedSprite_TickFrame(record->facilityIcons[i]);
        ManagedSprite_SetAffineScale(record->facilityIcons[i], record->scales[i], record->scales[i]);
        ManagedSprite_SetPaletteOverride(record->facilityIcons[i], i + 10);
    }
    template.resIdList[GF_GFX_RES_TYPE_CHAR] = 28282;
    template.resIdList[GF_GFX_RES_TYPE_PLTT] = 28282;
    template.resIdList[GF_GFX_RES_TYPE_CELL] = 28282;
    template.resIdList[GF_GFX_RES_TYPE_ANIM] = 28282;
    template.resIdList[GF_GFX_RES_TYPE_MCEL] = -1;
    template.resIdList[GF_GFX_RES_TYPE_MANM] = -1;
    template.drawPriority = 20;
    varsFlags = Save_VarsFlags_Get(work->saveData);
    progress[0] = Save_VarsFlags_GetBattleTowerPrintProgress(varsFlags);
    progress[1] = Save_VarsFlags_GetBattleFactoryPrintProgress(varsFlags);
    progress[2] = Save_VarsFlags_GetBattleArcadePrintProgress(varsFlags);
    progress[3] = Save_VarsFlags_GetBattleCastlePrintProgress(varsFlags);
    progress[4] = Save_VarsFlags_GetBattleHallPrintProgress(varsFlags);
    for (i = 0; i < 5; i++) {
        record->prints[i] = SpriteSystem_NewSprite(work->spriteSystem, work->spriteManager, &template);
        if (progress[i] == 0 || progress[i] == 1) {
            // no print yet: empty frame, facility and player icons hidden
            ManagedSprite_SetAnim(record->prints[i], 5);
            ManagedSprite_SetPaletteOverride(record->prints[i], 9);
            ManagedSprite_SetDrawFlag(record->facilityIcons[i], FALSE);
            ManagedSprite_SetDrawFlag(record->playerIcons[i], FALSE);
        } else {
            printAnims = ov40_022451B0;
            ManagedSprite_SetAnim(record->prints[i], printAnims.values[i]);
            ManagedSprite_SetPaletteOverride(record->prints[i], printAnims.values[i] + 4);
        }
        ManagedSprite_TickFrame(record->prints[i]);
        ManagedSprite_SetAffineScale(record->prints[i], record->scales[i], record->scales[i]);
    }

    // Battle Points: four digits without leading zeros, then "BP"
    template.resIdList[GF_GFX_RES_TYPE_CHAR] = 29292;
    template.resIdList[GF_GFX_RES_TYPE_PLTT] = 29292;
    template.resIdList[GF_GFX_RES_TYPE_CELL] = 29292;
    template.resIdList[GF_GFX_RES_TYPE_ANIM] = 29292;
    template.resIdList[GF_GFX_RES_TYPE_MCEL] = -1;
    template.resIdList[GF_GFX_RES_TYPE_MANM] = -1;
    template.vram = NNS_G2D_VRAM_TYPE_2DSUB;
    started = FALSE;
    template.drawPriority = 0;
    value = record->battlePoints;
    div = 1000;
    for (i = 0; i < 4; i++) {
        digits[i] = value / div;
        value = value % div;
        div = div / 10;
        if (digits[i] == 0 && started == FALSE) {
            zeros++;
        } else {
            started = TRUE;
        }
    }
    digits[4] = 10;
    for (i = 0; i < 5; i++) {
        record->digits[i] = SpriteSystem_NewSprite(work->spriteSystem, work->spriteManager, &template);
        if (digits[i] == 0 && i < zeros) {
            ManagedSprite_SetDrawFlag(record->digits[i], FALSE);
        }
        ManagedSprite_SetAnim(record->digits[i], digits[i]);
        VsRecorder_SetSpritePos(record->digits[i], 88 + i * 16, 72);
        ManagedSprite_TickFrame(record->digits[i]);
    }
    ManagedSprite_SetDrawFlag(record->digits[3], TRUE);
    ManagedSprite_SetDrawFlag(record->digits[4], TRUE);
    for (i = 0; i < 5; i++) {
        ManagedSprite_SetAffineOverwriteMode(record->playerIcons[i], 2);
        ManagedSprite_SetAffineOverwriteMode(record->facilityIcons[i], 2);
        ManagedSprite_SetAffineOverwriteMode(record->prints[i], 2);
    }
}

#pragma force_active reset

void VsRecorder_FreeFrontierSprites(VsRecorderWork *work) {
    VsRecorderFrontierRecord *record = work->screenWork;
    int i;

    for (i = 0; i < 5; i++) {
        SpriteManager_UnloadCharObjById(work->spriteManager, 18181 + i);
        SpriteManager_UnloadPlttObjById(work->spriteManager, 18181 + i);
        Sprite_DeleteAndFreeResources(record->facilityIcons[i]);
    }
    SpriteManager_UnloadCellObjById(work->spriteManager, 18181);
    SpriteManager_UnloadAnimObjById(work->spriteManager, 18181);
    SpriteManager_UnloadCellObjById(work->spriteManager, 18182);
    SpriteManager_UnloadAnimObjById(work->spriteManager, 18182);

    for (i = 0; i < 5; i++) {
        Sprite_DeleteAndFreeResources(record->playerIcons[i]);
    }
    SpriteManager_UnloadCharObjById(work->spriteManager, 165535);
    SpriteManager_UnloadPlttObjById(work->spriteManager, 165535);
    SpriteManager_UnloadCellObjById(work->spriteManager, 165535);
    SpriteManager_UnloadAnimObjById(work->spriteManager, 165535);

    for (i = 0; i < 5; i++) {
        Sprite_DeleteAndFreeResources(record->prints[i]);
    }
    SpriteManager_UnloadCharObjById(work->spriteManager, 28282);
    SpriteManager_UnloadPlttObjById(work->spriteManager, 28282);
    SpriteManager_UnloadCellObjById(work->spriteManager, 28282);
    SpriteManager_UnloadAnimObjById(work->spriteManager, 28282);

    for (i = 0; i < 5; i++) {
        Sprite_DeleteAndFreeResources(record->digits[i]);
    }
    SpriteManager_UnloadCharObjById(work->spriteManager, 29292);
    SpriteManager_UnloadPlttObjById(work->spriteManager, 29292);
    SpriteManager_UnloadCellObjById(work->spriteManager, 29292);
    SpriteManager_UnloadAnimObjById(work->spriteManager, 29292);
}

// the prints spin in on a circle while growing to full size; TRUE while spinning
BOOL VsRecorder_SpinInFrontierSprites(VsRecorderWork *work) {
    VsRecorderFrontierRecord *record = work->screenWork;
    s16 x, y;
    int i;

    for (i = 0; i < 5; i++) {
        record->angles[i] += 20;
        record->angles[i] %= 360;
    }
    VsRecorder_StepBlend(&record->eva, &record->evb, FALSE, 0);
    for (i = 0; i < 5; i++) {
        if (record->angles[0] != 0) {
            record->scales[i] += 0.05f;
        } else {
            ManagedSprite_SetAffineOverwriteMode(record->playerIcons[i], 0);
            ManagedSprite_SetAffineOverwriteMode(record->facilityIcons[i], 0);
            ManagedSprite_SetAffineOverwriteMode(record->prints[i], 0);
            record->scales[i] = 1.0f;
        }
        {
            fx32 dx = GF_SinDeg((u16)record->angles[i]) * record->radii[i];
            fx32 dy = GF_CosDeg((u16)record->angles[i]) * record->radii[i];
            dy = 0x6A000 - dy;
            dx += 0x80000;
            ManagedSprite_SetPositonFxXY(record->prints[i], dx, dy);
        }
        ManagedSprite_GetPositionXY(record->prints[i], &x, &y);
        ManagedSprite_SetPositionXY(record->playerIcons[i], x - 32, y - 2);
        ManagedSprite_SetPositionXY(record->facilityIcons[i], x + 16, y - 2);
        ManagedSprite_SetAffineScale(record->prints[i], record->scales[i], record->scales[i]);
        ManagedSprite_SetAffineScale(record->playerIcons[i], record->scales[i], record->scales[i]);
        ManagedSprite_SetAffineScale(record->facilityIcons[i], record->scales[i], record->scales[i]);
    }
    return record->angles[0] != 0;
}

// the reverse: the prints spin out while shrinking and are hidden at the end; TRUE while spinning
BOOL VsRecorder_SpinOutFrontierSprites(VsRecorderWork *work) {
    VsRecorderFrontierRecord *record = work->screenWork;
    s16 x, y;
    int i;

    for (i = 0; i < 5; i++) {
        record->angles[i] += 20;
        record->angles[i] %= 360;
    }
    if (record->angles[0] == 20) {
        for (i = 0; i < 5; i++) {
            ManagedSprite_SetAffineOverwriteMode(record->playerIcons[i], 2);
            ManagedSprite_SetAffineOverwriteMode(record->facilityIcons[i], 2);
            ManagedSprite_SetAffineOverwriteMode(record->prints[i], 2);
        }
    }
    VsRecorder_StepBlend(&record->eva, &record->evb, TRUE, 0);
    for (i = 0; i < 5; i++) {
        if (record->angles[0] != 0) {
            record->scales[i] -= 0.05f;
        } else {
            ManagedSprite_SetDrawFlag(record->prints[i], FALSE);
            ManagedSprite_SetDrawFlag(record->playerIcons[i], FALSE);
            ManagedSprite_SetDrawFlag(record->facilityIcons[i], FALSE);
        }
        {
            fx32 dx = GF_SinDeg((u16)record->angles[i]) * record->radii[i];
            fx32 dy;
            dx = 0x80000 - dx;
            dy = GF_CosDeg((u16)record->angles[i]) * record->radii[i];
            dy = 0x6A000 - dy;
            ManagedSprite_SetPositonFxXY(record->prints[i], dx, dy);
        }
        ManagedSprite_GetPositionXY(record->prints[i], &x, &y);
        ManagedSprite_SetPositionXY(record->playerIcons[i], x - 32, y - 2);
        ManagedSprite_SetPositionXY(record->facilityIcons[i], x + 16, y - 2);
        ManagedSprite_SetAffineScale(record->prints[i], record->scales[i], record->scales[i]);
        ManagedSprite_SetAffineScale(record->playerIcons[i], record->scales[i], record->scales[i]);
        ManagedSprite_SetAffineScale(record->facilityIcons[i], record->scales[i], record->scales[i]);
    }
    return record->angles[0] != 0;
}

void VsRecorder_PlaceFrontierSprites(VsRecorderWork *work) {
    const s16 *pos = _02245CC0;
    VsRecorderFrontierRecord *record = work->screenWork;
    s16 x;
    s16 y;
    int i;
    for (i = 0; i < 5; i++) {
        ManagedSprite_SetPositonFxXY(record->prints[i], pos[0] << FX32_SHIFT, pos[1] << FX32_SHIFT);
        ManagedSprite_GetPositionXY(record->prints[i], &x, &y);
        ManagedSprite_SetPositionXY(record->playerIcons[i], x - 32, y - 2);
        ManagedSprite_SetPositionXY(record->facilityIcons[i], x + 16, y - 2);
        pos += 2;
    }
}

#pragma force_active on

BOOL VsRecorder_FrontierEnter(VsRecorderWork *work) {
    VsRecorderFrontierRecord *record;
    int i;

    if (work->step == 0) {
        record = Heap_Alloc(HEAP_ID_109, sizeof(VsRecorderFrontierRecord));
        MI_CpuFill8(record, 0, sizeof(VsRecorderFrontierRecord));
        work->screenWork = record;
        record->fade = 0;
        for (i = 0; i < 5; i++) {
            if (i == 2 || i == 3) {
                record->radii[i] = 52;
            } else {
                record->radii[i] = 64;
            }
            record->angles[i] = i * 72;
            record->scales[i] = 0.2f;
        }
        BgClearTilemapBufferAndCommit(work->bgConfig, GF_BG_LYR_MAIN_3);
        BgClearTilemapBufferAndCommit(work->bgConfig, GF_BG_LYR_MAIN_2);
        BgClearTilemapBufferAndCommit(work->bgConfig, GF_BG_LYR_SUB_3);
        BgClearTilemapBufferAndCommit(work->bgConfig, GF_BG_LYR_SUB_2);
        VsRecorder_InitBgBlend(&record->eva, &record->evb, 0);
        VsRecorder_ApplyBlend(&record->eva, &record->evb, 0, 0, 4, 2, 0);
        PlaySE(SEQ_SE_PL_BREC20);
        work->step++;
    } else {
        record = work->screenWork;
    }
    switch (work->step) {
    case 1:
        if (VsRecorder_StepFade(&record->fade, 1)) {
            record->battlePoints = FrontierData_BattlePointAction(Save_FrontierData_Get(work->saveData), 0, 0);
            work->step++;
        }
        PaletteData_BlendPalettes(work->plttData, PLTTBUF_MAIN_OBJ, 12, (u8)record->fade, (u16)work->blendColor);
        PaletteData_BlendPalettes(work->plttData, PLTTBUF_SUB_OBJ, 12, (u8)record->fade, (u16)work->blendColor);
        PaletteData_BlendPalettes(work->plttData, PLTTBUF_SUB_BG, 2, (u8)record->fade, (u16)work->blendColor);
        break;
    case 2:
        VsRecorder_SetVBlankPaused(work, TRUE);
        VsRecorder_FreeMenuRows(work);
        VsRecorder_SetVBlankPaused(work, FALSE);
        VsRecorder_StartFlipScreens(work, 0);
        OrbitCursor_SetVisible(work->orbitCursors[0], FALSE);
        VsRecorder_ResetBgPriorities();
        SetBgPriority(GF_BG_LYR_SUB_2, 2);
        VsRecorder_LoadFrontierGfx(work);
        PaletteData_BlendPalettes(work->plttData, PLTTBUF_SUB_OBJ, 28, (u8)record->fade, (u16)work->blendColor);
        work->step++;
        break;
    case 3:
        if (VsRecorder_IsFlipScreensDone(work)) {
            VsRecorder_SetVBlankPaused(work, TRUE);
            VsRecorder_CreateFrontierButtons(work);
            VsRecorder_CreateFrontierSprites(work);
            VsRecorder_GrayFrontierPrints(work);
            VsRecorder_SetVBlankPaused(work, FALSE);
            GfGfx_EngineBTogglePlanes(GX_PLANEMASK_BG3, GF_PLANE_TOGGLE_ON);
            GfGfx_EngineBTogglePlanes(GX_PLANEMASK_BG2, GF_PLANE_TOGGLE_ON);
            work->step++;
        }
        break;
    case 4: {
        BOOL spinning = VsRecorder_SpinInFrontierSprites(work);
        VsRecorder_StepBlend(&record->eva, &record->evb, FALSE, 0);
        VsRecorder_StepFade(&record->fade, 0);
        if (spinning == FALSE) {
            work->step++;
            VsRecorder_PlaceFrontierSprites(work);
        }
        PaletteData_BlendPalettes(work->plttData, PLTTBUF_SUB_OBJ, 28, (u8)record->fade, (u16)work->blendColor);
        break;
    }
    default:
        VsRecorder_SetSubState(work, 1);
        break;
    }
    return FALSE;
}

#pragma force_active reset

BOOL VsRecorder_FrontierMain(VsRecorderWork *work) {
    u32 x;
    u32 y;
    if (System_GetTouchNewCoords(&x, &y)) {
        if (x > 80 && x < 176 && y > 152 && y < 176) {
            PlaySE(SEQ_SE_PL_BREC57);
            VsRecorder_SetSubState(work, 2);
        }
    }
    return FALSE;
}
