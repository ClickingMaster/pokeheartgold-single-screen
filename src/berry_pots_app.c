#include "berry_pots_app.h"

#include <nitro/rtc/ARM9/convert.h>

#include "global.h"

#include "data/resdat.naix"
#include "msgdata/msg.naix"
#include "msgdata/msg/msg_0248.h"

#include "bag.h"
#include "berry_pots_app_internal.h"
#include "font.h"
#include "game_stats.h"
#include "gf_gfx_loader.h"
#include "gf_gfx_planes.h"
#include "gf_rtc.h"
#include "launch_application.h"
#include "menu_input_state.h"
#include "berry_pots.h"
#include "render_window.h"
#include "save_misc_data.h"
#include "screen_fade.h"
#include "sound_system.h"
#include "sprite.h"
#include "sprite_system.h"
#include "system.h"
#include "text.h"
#include "touchscreen.h"
#include "sound_playback.h"
#include "oam_manager.h"
#include "touchpad.h"
#include "vram_transfer_manager.h"
#include "yes_no_prompt.h"

struct BerryPotsMenu {
    u32 numButtons;
    const u8 *msgId;
};

static void BerryPotsApp_InitData(BerryPotsAppData *data);
static void BerryPotsApp_FreeData(BerryPotsAppData *data);
static BOOL BerryPotsApp_Setup(BerryPotsAppData *data);
static u32 BerryPotsApp_MainSeq_FadeIn(BerryPotsAppData *data);
static BOOL BerryPotsApp_Teardown(BerryPotsAppData *data);
static u32 BerryPotsApp_MainSeq_FadeOut(BerryPotsAppData *data);
static u32 BerryPotsApp_MainSeq_SelectPot(BerryPotsAppData *data);
static u32 BerryPotsApp_MainSeq_EmptyPotMenu(BerryPotsAppData *data);
static u32 BerryPotsApp_MainSeq_MulchedPotMenu(BerryPotsAppData *data);
static u32 BerryPotsApp_MainSeq_WaterMenu(BerryPotsAppData *data);
static u32 BerryPotsApp_MainSeq_HarvestMenu(BerryPotsAppData *data);
static u32 BerryPotsApp_MainSeq_ItemUsedMessage(BerryPotsAppData *data);
static u32 BerryPotsApp_MainSeq_WaitMessage(BerryPotsAppData *data);
static u32 BerryPotsApp_GetStateAfterFadeIn(BerryPotsAppData *data);
static void BerryPotsApp_ApplyChosenItem(BerryPotsAppData *data);
static BOOL BerryPotsApp_WaitMessageInput(BerryPotsAppData *data);
static void BerryPotsApp_SetupResources(BerryPotsAppData *data);
static void BerryPotsApp_FreeResources(BerryPotsAppData *data);
static void BerryPotsApp_InitDisplay(BerryPotsAppData *data);
static BOOL BerryPotsApp_WateringAnim(BerryPotsAppData *data);
static BOOL BerryPotsApp_HarvestAnim(BerryPotsAppData *data);
static void BerryPotsApp_SetCursorVisible(BerryPotsAppData *data, BOOL visible);
static void BerryPotsApp_SetAnimsActive(BerryPotsAppData *data, BOOL active);
static void BerryPotsApp_PrintMessage(BerryPotsAppData *data, enum BerryPotsAppMessage action, u8 a2, BOOL a3);
static void BerryPotsApp_CreateMenu(BerryPotsAppData *data, u32 a1);
static int BerryPotsApp_HandleMenuInput(BerryPotsAppData *data);
static void BerryPotsApp_HarvestBerries(BerryPotsAppData *data);
static void BerryPotsApp_VBlankCB(BerryPotsAppData *data);
static void BerryPotsApp_SetGfxBanks(void);
static void BerryPotsApp_SetupBgConfig(BerryPotsAppData *data);
static void BerryPotsApp_FreeBgConfig(BerryPotsAppData *data);
static void BerryPotsApp_SetupNarc(BerryPotsAppData *data);
static void BerryPotsApp_FreeNarc(BerryPotsAppData *data);
static void BerryPotsApp_SetupText(BerryPotsAppData *data);
static void BerryPotsApp_FreeText(BerryPotsAppData *data);
static void BerryPotsApp_SetupWindows(BerryPotsAppData *data);
static void BerryPotsApp_FreeWindows(BerryPotsAppData *data);
static void BerryPotsApp_SetupListMenuItems(BerryPotsAppData *data);
static void BerryPotsApp_FreeListMenuItems(BerryPotsAppData *data);
static void BerryPotsApp_SetupSpriteRendererAndGfxHandler(BerryPotsAppData *data);
static void BerryPotsApp_FreeSpriteRendererAndGfxHandler(BerryPotsAppData *data);
static void BerryPotsApp_SetupSpriteSystem(BerryPotsAppData *data);
static void BerryPotsApp_FreeSpriteSystem(BerryPotsAppData *data);
static void BerryPotsApp_SetupSprites(BerryPotsAppData *data);
static void BerryPotsApp_FreeSprites(BerryPotsAppData *data);
static void BerryPotsApp_LoadPlantGfx(BerryPotsAppData *data, int index);
static void BerryPotsApp_LoadBerryIconGfx(BerryPotsAppData *data, u8 index);

static const u8 ov17_02203D60[3][4] = {
    {
     0,
     2,
     1,
     0,
     },
    {
     0,
     4,
     3,
     0,
     },
    {
     0,
     6,
     5,
     0,
     },
};
static const TouchscreenListMenuTemplate sMenuTemplate = {
    TRUE,
    TRUE,
    4,
    0,
    11,
    0,
    0,
    58,
    31,
    84,
};
static const ResdatIdList sBerryPotsResdatIdList = {
    .charRes = NARC_resdat_resdat_00000030_bin,
    .plttRes = NARC_resdat_resdat_00000031_bin,
    .cellRes = NARC_resdat_resdat_00000029_bin,
    .animRes = NARC_resdat_resdat_00000028_bin,
    .mcelRes = 0xFFFF,
    .manmRes = 0xFFFF,
    .headerId = NARC_resdat_resdat_00000079_bin,
};
static const GraphicsModes sBerryPotsGraphicsModes = {
    GX_DISPMODE_GRAPHICS,
    GX_BGMODE_0,
    GX_BGMODE_0,
    GX_BG0_AS_2D,
};
static const OamCharTransferParam sBerryPotsOamCharTransferParam = {
    0x8,
    0x10000,
    0x4000,
    GX_OBJVRAMMODE_CHAR_1D_32K,
    GX_OBJVRAMMODE_CHAR_1D_32K,
};
static const WindowTemplate sBerryPotsWindowTemplates[3] = {
    { 2, 25, 21, 6,  2, 9,  0x1A2 },
    { 0, 2,  19, 19, 4, 12, 0x0CA },
    { 0, 2,  19, 27, 4, 12, 0x136 },
};
static const SpriteResourceCountsListUnion sBerryPotsSpriteResourceCounts = {
    { 1, 1, 1, 1, 0, 0 }
};
static const BgTemplate sBerryPotsBgTemplate_Sub3 = {
    0,
    0,
    GF_BG_BUF_SIZE_256x256_4BPP,
    0x000,
    GF_BG_SCR_SIZE_256x256,
    GX_BG_COLORMODE_16,
    GX_BG_SCRBASE_0xf000,
    GX_BG_CHARBASE_0x00000,
    GX_BG_EXTPLTT_01,
    3,
    GX_BG_AREAOVER_XLU,
    0,
    FALSE,
};
static const BgTemplate sBerryPotsBgTemplate_Main0 = {
    0,
    0,
    GF_BG_BUF_SIZE_256x256_4BPP,
    0x000,
    GF_BG_SCR_SIZE_256x256,
    GX_BG_COLORMODE_16,
    GX_BG_SCRBASE_0xf800,
    GX_BG_CHARBASE_0x10000,
    GX_BG_EXTPLTT_01,
    0,
    GX_BG_AREAOVER_XLU,
    0,
    FALSE,
};
static const BgTemplate sBerryPotsBgTemplate_Main2 = {
    0,
    0,
    GF_BG_BUF_SIZE_256x256_4BPP,
    0,
    GF_BG_SCR_SIZE_256x256,
    GX_BG_COLORMODE_16,
    GX_BG_SCRBASE_0xf000,
    GX_BG_CHARBASE_0x10000,
    GX_BG_EXTPLTT_01,
    2,
    GX_BG_AREAOVER_XLU,
    0,
    FALSE,
};
static const BgTemplate sBerryPotsBgTemplate_Sub0 = {
    0,
    0,
    GF_BG_BUF_SIZE_256x256_4BPP,
    0,
    GF_BG_SCR_SIZE_256x256,
    GX_BG_COLORMODE_16,
    GX_BG_SCRBASE_0xf800,
    GX_BG_CHARBASE_0x10000,
    GX_BG_EXTPLTT_01,
    0,
    GX_BG_AREAOVER_XLU,
    0,
    FALSE,
};
static const BgTemplate sBerryPotsBgTemplate_Main3 = {
    0,
    0,
    GF_BG_BUF_SIZE_256x256_4BPP,
    0,
    GF_BG_SCR_SIZE_256x256,
    GX_BG_COLORMODE_16,
    GX_BG_SCRBASE_0xe800,
    GX_BG_CHARBASE_0x00000,
    GX_BG_EXTPLTT_01,
    3,
    GX_BG_AREAOVER_XLU,
    0,
    FALSE,
};
static const OamManagerParam sBerryPotsOamManagerParam = { 0, 128, 0, 32, 0, 128, 0, 32 };

static const u8 sMenu_Fertilize_Plant_Exit[] = { msg_0248_00004, msg_0248_00003, msg_0248_00001 };
static const u8 sMenu_Plant_Exit[] = { msg_0248_00003, msg_0248_00001 };
static const u8 sMenu_Water_Exit[] = { msg_0248_00002, msg_0248_00001 };
static const u8 sMenu_Take_Exit[] = { msg_0248_00005, msg_0248_00001 };

static const struct BerryPotsMenu sBerryPotsMenus[4] = {
    { NELEMS(sMenu_Fertilize_Plant_Exit), sMenu_Fertilize_Plant_Exit },
    { NELEMS(sMenu_Plant_Exit),           sMenu_Plant_Exit           },
    { NELEMS(sMenu_Water_Exit),           sMenu_Water_Exit           },
    { NELEMS(sMenu_Take_Exit),            sMenu_Take_Exit            },
};
static const UnmanagedSpriteTemplate sBerryPlantSpriteTemplate = { 0, 27, 96, 0, 5, 13, 0, NNS_G2D_VRAM_TYPE_2DMAIN, 1, 0, 0, 0 };
static const GraphicsBanks sBerryPotsGraphicsBanks = {
    GX_VRAM_BG_128_A,
    GX_VRAM_BGEXTPLTT_NONE,
    GX_VRAM_SUB_BG_128_C,
    GX_VRAM_SUB_BGEXTPLTT_NONE,
    GX_VRAM_OBJ_64_E,
    GX_VRAM_OBJEXTPLTT_NONE,
    GX_VRAM_SUB_OBJ_16_I,
    GX_VRAM_SUB_OBJEXTPLTT_NONE,
    GX_VRAM_TEX_NONE,
    GX_VRAM_TEXPLTT_NONE,
};
static const UnmanagedSpriteTemplate sBerryPotsSpriteTemplates[5] = {
    { 0, 224, 176, 0, 0, 15, 0, NNS_G2D_VRAM_TYPE_2DMAIN, 1, 0, 0, 0 },
    { 0, 27,  96,  0, 2, 0,  0, NNS_G2D_VRAM_TYPE_2DMAIN, 1, 0, 0, 0 },
    { 0, 31,  56,  0, 8, 1,  0, NNS_G2D_VRAM_TYPE_2DMAIN, 1, 0, 0, 0 },
    { 0, 27,  99,  0, 4, 14, 0, NNS_G2D_VRAM_TYPE_2DMAIN, 1, 0, 0, 0 },
    { 0, 27,  88,  0, 7, 12, 0, NNS_G2D_VRAM_TYPE_2DMAIN, 1, 0, 0, 0 }
};

void BerryPotsApp_ResetDisplay(void) {
    Main_SetVBlankIntrCB(NULL, NULL);
    HBlankInterruptDisable();

    GfGfx_DisableEngineAPlanes();
    GfGfx_DisableEngineBPlanes();

    GX_SetVisiblePlane(0);
    GXS_SetVisiblePlane(0);

    SetMasterBrightnessByColor(PM_LCD_TOP, RGB_BLACK);
    SetMasterBrightnessByColor(PM_LCD_BOTTOM, RGB_BLACK);
    ResetVisibleHardwareWindows(PM_LCD_TOP);
    ResetVisibleHardwareWindows(PM_LCD_BOTTOM);
}

BOOL BerryPotsApp_Initialize(OverlayManager *manager, int *state) {
    BerryPotsAppData *data;

    switch (*state) {
    case 0:
        BerryPotsApp_ResetDisplay();
        Heap_Create(HEAP_ID_3, HEAP_ID_BERRY_POTS_APP, 0x20000);

        data = OverlayManager_CreateAndGetData(manager, sizeof(BerryPotsAppData), HEAP_ID_BERRY_POTS_APP);
        MI_CpuFill8(data, 0, sizeof(BerryPotsAppData));
        data->heapID = HEAP_ID_BERRY_POTS_APP;
        data->args = OverlayManager_GetArgs(manager);
        BerryPotsApp_InitData(data);

        (*state)++;
        break;

    case 1:
        data = OverlayManager_GetData(manager);
        if (BerryPotsApp_Setup(data)) {
            return TRUE;
        }

        break;
    }

    return FALSE;
}

BOOL BerryPotsApp_Run(OverlayManager *manager, int *state) {
    BerryPotsAppData *data = OverlayManager_GetData(manager);

    switch (*state) {
    case 0:
        *state = BerryPotsApp_MainSeq_FadeIn(data);
        break;
    case 1:
        *state = BerryPotsApp_MainSeq_FadeOut(data);
        break;
    case 2:
        *state = BerryPotsApp_MainSeq_SelectPot(data);
        break;
    case 3:
        *state = BerryPotsApp_MainSeq_EmptyPotMenu(data);
        break;
    case 4:
        *state = BerryPotsApp_MainSeq_MulchedPotMenu(data);
        break;
    case 5:
        *state = BerryPotsApp_MainSeq_WaterMenu(data);
        break;
    case 6:
        *state = BerryPotsApp_MainSeq_HarvestMenu(data);
        break;
    case 7:
        *state = BerryPotsApp_MainSeq_ItemUsedMessage(data);
        break;
    case 8:
        *state = BerryPotsApp_MainSeq_WaitMessage(data);
        break;
    case 9:
        return TRUE;
    }

    SpriteSystem_DrawSprites(data->spriteGfxHandler1);
    return FALSE;
}

BOOL BerryPotsApp_Exit(OverlayManager *manager, int *state) {
    BerryPotsAppData *data = OverlayManager_GetData(manager);
    if (!BerryPotsApp_Teardown(data)) {
        return FALSE;
    }

    BerryPotsApp_FreeData(data);

    OverlayManager_FreeData(manager);
    Heap_Destroy(HEAP_ID_BERRY_POTS_APP);

    return TRUE;
}

static void BerryPotsApp_InitData(BerryPotsAppData *data) {
    data->fieldMenuState = MenuInputStateMgr_GetState(data->args->launchArgs->menuInputPtr);
    Options *options = Save_PlayerData_GetOptionsAddr(data->args->launchArgs->savedata);
    data->textFrameDelay = Options_GetTextFrameDelay(options);
    data->frame = Options_GetFrame(options);
    data->bag = Save_Bag_Get(data->args->launchArgs->savedata);
    data->stats = Save_GameStats_Get(data->args->launchArgs->savedata);
    data->berryGrowthProperties = BerryPots_LoadGrowthProperties(data->heapID);
    data->berryPots = Save_BerryPots_Get(data->args->launchArgs->savedata);
    data->berryDatetime = Save_BerryPotRTC_Get(data->args->launchArgs->savedata);
    data->selectedPot = (data->args->itemAction != 0) ? data->args->potIndex : 0;
    BerryPotsApp_ApplyChosenItem(data);
    data->totalBerryQuantity = GetTotalBerryQuantity(data->bag, data->heapID);
    data->totalMulchQuantity = GetTotalMulchQuantity(data->bag, data->heapID);
    BerryPotsApp_RefreshPotData(data);
}

static void BerryPotsApp_FreeData(BerryPotsAppData *data) {
    Heap_Free(data->berryGrowthProperties);
    MenuInputStateMgr_SetState(data->args->launchArgs->menuInputPtr, data->fieldMenuState);
}

static BOOL BerryPotsApp_Setup(BerryPotsAppData *data) {
    switch (data->state70) {
    case 0:
        BerryPotsApp_SetupResources(data);
        data->state70++;
        break;
    case 1:
        BerryPotsApp_InitDisplay(data);
        GF_TouchpadResetSampling();
        GF_TouchpadStartSampling(1);
        Sound_SetSceneAndPlayBGM(0x45, 0, 0);

        data->state70 = 0;
        return TRUE;
    }

    return FALSE;
}

static BOOL BerryPotsApp_Teardown(BerryPotsAppData *data) {
    GF_TouchpadStopSampling();
    BerryPotsApp_FreeResources(data);
    return TRUE;
}

static u32 BerryPotsApp_MainSeq_FadeIn(BerryPotsAppData *data) {
    switch (data->state70) {
    case 0:
        BeginNormalPaletteFade(FADE_BOTH_SCREENS, FADE_TYPE_BRIGHTNESS_IN, FADE_TYPE_BRIGHTNESS_IN, RGB_BLACK, 6, 1, data->heapID);
        data->state70++;
        break;
    case 1:
        if (!IsPaletteFadeFinished()) {
            break;
        }

        data->state70 = 0;
        return BerryPotsApp_GetStateAfterFadeIn(data);
    }

    return 0;
}

static u32 BerryPotsApp_MainSeq_FadeOut(BerryPotsAppData *data) {
    switch (data->state70) {
    case 0:
        BeginNormalPaletteFade(FADE_BOTH_SCREENS, FADE_TYPE_BRIGHTNESS_OUT, FADE_TYPE_BRIGHTNESS_OUT, RGB_BLACK, 6, 1, data->heapID);
        data->state70++;
        break;
    case 1:
        if (!IsPaletteFadeFinished()) {
            break;
        }

        data->state70 = 0;
        return 9;
    }

    return 1;
}

static u32 BerryPotsApp_MainSeq_SelectPot(BerryPotsAppData *data) {
    u32 unk = BerryPotsApp_HandleSelectPotInput(data);
    if (unk == 2) {
        BerryPotsApp_UpdateGrowthFromRTC(data, 1);
    }
    return unk;
}

static u32 BerryPotsApp_MainSeq_EmptyPotMenu(BerryPotsAppData *data) {
    switch (data->state70) {
    case 0:
        BerryPotsApp_CreateMenu(data, 0);
        BerryPotsApp_PrintMessage(data, BERRY_POTS_APP_MSG_SOFT_SOIL, data->selectedPot, TRUE);
        BerryPotsApp_SetAnimsActive(data, FALSE);
        data->state70++;
        break;
    case 1: {
        int unk = BerryPotsApp_HandleMenuInput(data);
        if (unk == -1) {
            break;
        }

        data->state70 = 0;

        if (unk == 2 || unk == -2) {
            BerryPotsApp_PrintMessage(data, BERRY_POTS_APP_MSG_TOUCH_POT, 4, TRUE);
            BerryPotsApp_SetAnimsActive(data, TRUE);
            return 2;
        }

        data->args->potIndex = data->selectedPot;
        data->args->itemId = 0xFFFF;

        if (unk == 0) {
            if (data->totalMulchQuantity == 0) {
                BerryPotsApp_PrintMessage(data, BERRY_POTS_APP_MSG_NO_FERTILIZER, 4, FALSE);
                return 8;
            } else {
                data->args->itemAction = 1;
            }
        } else if (data->totalBerryQuantity == 0) {
            BerryPotsApp_PrintMessage(data, BERRY_POTS_APP_MSG_NO_BERRIES, 4, FALSE);
            return 8;
        } else {
            data->args->itemAction = 2;
        }

        return 1;
    }
    }

    return 3;
}

static u32 BerryPotsApp_MainSeq_MulchedPotMenu(BerryPotsAppData *data) {
    switch (data->state70) {
    case 0:
        BerryPotsApp_CreateMenu(data, 1);
        BerryPotsApp_PrintMessage(data, BERRY_POTS_APP_MSG_MULCH_LAID, data->selectedPot, 1);
        BerryPotsApp_SetAnimsActive(data, 0);
        data->state70++;
        break;
    case 1: {
        int unk = BerryPotsApp_HandleMenuInput(data);
        if (unk == -1) {
            break;
        }

        data->state70 = 0;

        if (unk == 1 || unk == -2) {
            BerryPotsApp_PrintMessage(data, BERRY_POTS_APP_MSG_TOUCH_POT, 4, 1);
            BerryPotsApp_SetAnimsActive(data, 1);
            return 2;
        }

        if (data->totalBerryQuantity == 0) {
            BerryPotsApp_PrintMessage(data, BERRY_POTS_APP_MSG_NO_BERRIES, 4, 0);
            return 8;
        } else {
            data->args->potIndex = data->selectedPot;
            data->args->itemId = 0xFFFF;
            data->args->itemAction = 2;
            return 1;
        }
    }
    }

    return 4;
}

static u32 BerryPotsApp_MainSeq_WaterMenu(BerryPotsAppData *data) {
    switch (data->state70) {
    case 0:
        BerryPotsApp_CreateMenu(data, 2);
        BerryPotsApp_SetAnimsActive(data, FALSE);
        BerryPotsApp_PrintMessage(data, (enum BerryPotsAppMessage)(BERRY_POTS_APP_MSG_MULCH_LAID + data->pots[data->selectedPot].growthStage), data->selectedPot, TRUE);
        data->state70++;
        break;
    case 1: {
        int unk = BerryPotsApp_HandleMenuInput(data);
        if (unk == -1) {
            break;
        }

        if (unk == 1 || unk == -2) {
            data->state70 = 0;
            BerryPotsApp_PrintMessage(data, BERRY_POTS_APP_MSG_TOUCH_POT, 4, TRUE);
            BerryPotsApp_SetAnimsActive(data, TRUE);
            return 2;
        }

        BerryPotsApp_PrintMessage(data, BERRY_POTS_APP_MSG_WATERING_HELP, 4, FALSE);
        data->state70++;
        break;
    }
    case 2:
        if (!BerryPotsApp_WateringAnim(data)) {
            break;
        }

        BerryPotsApp_PrintMessage(data, BERRY_POTS_APP_MSG_WATERED, 4, FALSE);
        data->state70++;

        // fallthrough

    default:
        if (!BerryPotsApp_WaitMessageInput(data)) {
            break;
        }

        BerryPotsApp_UpdateCursor(data);
        BerryPotsApp_SetAnimsActive(data, TRUE);
        BerryPotsApp_PrintMessage(data, BERRY_POTS_APP_MSG_TOUCH_POT, 4, TRUE);

        data->state70 = 0;
        return 2;
    }

    return 5;
}

static u32 BerryPotsApp_MainSeq_HarvestMenu(BerryPotsAppData *data) {
    switch (data->state70) {
    case 0:
        BerryPotsApp_CreateMenu(data, 3);
        BerryPotsApp_PrintMessage(data, BERRY_POTS_APP_MSG_HAS_BERRIES, data->selectedPot, TRUE);
        BerryPotsApp_SetAnimsActive(data, FALSE);
        data->state70++;
        break;

    case 1: {
        int unk = BerryPotsApp_HandleMenuInput(data);
        if (unk == -1) {
            break;
        }

        if (unk == 1 || unk == -2) {
            data->state70 = 0;
            BerryPotsApp_PrintMessage(data, BERRY_POTS_APP_MSG_TOUCH_POT, 4, TRUE);
            BerryPotsApp_SetAnimsActive(data, TRUE);
            return 2;
        }

        BerryPotsAppPot *unkStruct = &data->pots[data->selectedPot];
        if (Bag_HasSpaceForItem(data->bag, BerryIdToItemId(unkStruct->berryId), unkStruct->quantityOrYieldMaybe, data->heapID)) {
            data->state70 = 3;
            BerryPotsApp_SetCursorVisible(data, FALSE);
        } else {
            BerryPotsApp_PrintMessage(data, BERRY_POTS_APP_MSG_BAG_FULL, data->selectedPot, FALSE);
            data->state70++;
        }

        break;
    }
    case 2:
        if (BerryPotsApp_WaitMessageInput(data)) {
            data->state70 = 0;
            BerryPotsApp_PrintMessage(data, BERRY_POTS_APP_MSG_TOUCH_POT, 4, TRUE);
            BerryPotsApp_SetAnimsActive(data, TRUE);
            return 2;
        }
        break;

    case 3:
        if (BerryPotsApp_HarvestAnim(data)) {
            BerryPotsApp_PrintMessage(data, BERRY_POTS_APP_MSG_PICKED, data->selectedPot, FALSE);
            PlayFanfare(SEQ_ME_KINOMI);
            data->state70++;
        }
        break;

    case 4:
        if (IsFanfarePlaying()) {
            break;
        }

        BerryPotsApp_PrintMessage(data, BERRY_POTS_APP_MSG_PUT_AWAY, data->selectedPot, FALSE);
        data->state70++;

        // fallthrough
    case 5:
        if (BerryPotsApp_WaitMessageInput(data)) {
            BerryPotsApp_HarvestBerries(data);
            BerryPotsApp_UpdateCursor(data);
            BerryPotsApp_SetAnimsActive(data, TRUE);
            BerryPotsApp_SetCursorVisible(data, TRUE);
            BerryPotsApp_PrintMessage(data, BERRY_POTS_APP_MSG_TOUCH_POT, 4, TRUE);
            data->state70 = 0;
            return 2;
        }
        break;
    }

    return 6;
}

static u32 BerryPotsApp_MainSeq_ItemUsedMessage(BerryPotsAppData *data) {
    switch (data->state70) {
    case 0:
        PlaySE(SEQ_SE_GS_KP01);
        data->state70++;
        break;
    case 1:
        if (BerryPotsApp_WaitMessageInput(data)) {
            BerryPotsApp_PrintMessage(data, BERRY_POTS_APP_MSG_TOUCH_POT, 4, TRUE);
            BerryPotsApp_SetAnimsActive(data, TRUE);
            data->state70 = 0;
            return 2;
        }
        break;
    }

    return 7;
}

static u32 BerryPotsApp_MainSeq_WaitMessage(BerryPotsAppData *data) {
    if (BerryPotsApp_WaitMessageInput(data)) {
        BerryPotsApp_PrintMessage(data, BERRY_POTS_APP_MSG_TOUCH_POT, 4, TRUE);
        BerryPotsApp_SetAnimsActive(data, TRUE);
        return 2;
    } else {
        return 8;
    }
}

static BOOL ItemIdIsMulch(u16 itemId) {
    // BUG: Root Fossils count as a mulch.
    if (itemId < ITEM_GROWTH_MULCH || itemId > ITEM_ROOT_FOSSIL) {
        return FALSE;
    } else {
        return TRUE;
    }
}

static void BerryPotsApp_ApplyChosenItem(BerryPotsAppData *data) {
    data->appliedItemAction = 0;

    u8 unk = data->args->itemAction;
    if (unk == 0) {
        return;
    }

    u16 itemId = data->args->itemId;
    if (itemId == 0xFFFF) {
        return;
    }

    if (unk == 1) {
        if (!ItemIdIsMulch(itemId)) {
            return;
        }

        BerryPots_SetPotMulch(data->berryPots, data->selectedPot, ItemIdToMulchId(data->args->itemId));
        data->appliedItemAction = 1;
    } else if (unk == 2) {
        if (!ItemIdIsBerry(itemId)) {
            return;
        }

        BerryPots_PlantBerry(data->berryPots, data->selectedPot, data->berryGrowthProperties, ItemIdToBerryId(data->args->itemId));
        GameStats_Inc(data->stats, GAME_STAT_BERRIES_PLANTED);
        data->appliedItemAction = 2;
    }

    Bag_TakeItem(data->bag, data->args->itemId, 1, data->heapID);
}

static u32 BerryPotsApp_GetStateAfterFadeIn(BerryPotsAppData *data) {
    u8 unk14 = data->args->itemAction;
    data->args->itemAction = 0;

    if (unk14 == 0) {
        return 2;
    }

    if (data->appliedItemAction != 0) {
        return 7;
    }

    return 2;
}

static BOOL BerryPotsApp_WaitMessageInput(BerryPotsAppData *data) {
    if (System_GetTouchNew() != 0) {
        PlaySE(SEQ_SE_DP_SELECT);
        data->fieldMenuState = MENU_INPUT_STATE_TOUCH;
        return TRUE;
    }

    if (gSystem.newKeys & (PAD_BUTTON_Y | PAD_BUTTON_X | PAD_KEY_DOWN | PAD_KEY_UP | PAD_KEY_LEFT | PAD_KEY_RIGHT | PAD_BUTTON_B | PAD_BUTTON_A)) {
        data->fieldMenuState = MENU_INPUT_STATE_BUTTONS;
    }

    if (gSystem.newKeys & (PAD_BUTTON_B | PAD_BUTTON_A)) {
        PlaySE(SEQ_SE_DP_SELECT);
        return TRUE;
    }

    u8 unk = data->messageTimer--;
    if (unk == 0) {
        PlaySE(SEQ_SE_DP_SELECT);
        return TRUE;
    }

    return FALSE;
}

static void BerryPotsApp_SetupResources(BerryPotsAppData *data) {
    BerryPotsApp_SetupBgConfig(data);
    BerryPotsApp_SetupNarc(data);
    BerryPotsApp_SetupText(data);
    BerryPotsApp_SetupWindows(data);
    BerryPotsApp_SetupListMenuItems(data);
    BerryPotsApp_SetupSpriteSystem(data);
    Main_SetVBlankIntrCB((GFIntrCB)BerryPotsApp_VBlankCB, data);
}

static void BerryPotsApp_FreeResources(BerryPotsAppData *data) {
    BerryPotsApp_FreeSpriteSystem(data);
    BerryPotsApp_FreeListMenuItems(data);
    BerryPotsApp_FreeWindows(data);
    BerryPotsApp_FreeText(data);
    BerryPotsApp_FreeNarc(data);
    BerryPotsApp_FreeBgConfig(data);
}

static void BerryPotsApp_InitDisplay(BerryPotsAppData *data) {
    BerryPotsApp_DrawCancelButton(data, 0);
    BerryPotsApp_UpdateCursor(data);

    switch (data->appliedItemAction) {
    case 1:
        BerryPotsApp_PrintMessage(data, BERRY_POTS_APP_MSG_MULCH_SCATTERED, data->selectedPot, TRUE);
        BerryPotsApp_SetAnimsActive(data, FALSE);
        break;
    case 2:
        BerryPotsApp_PrintMessage(data, BERRY_POTS_APP_MSG_BERRY_PLANTED, data->selectedPot, TRUE);
        BerryPotsApp_SetAnimsActive(data, FALSE);
        break;
    case 0:
    default:
        BerryPotsApp_PrintMessage(data, BERRY_POTS_APP_MSG_TOUCH_POT, 4, TRUE);
        BerryPotsApp_SetAnimsActive(data, TRUE);
        break;
    }

    BerryPotsApp_UpdateGrowthFromRTC(data, FALSE);

    for (int i = 0; i < MAX_BERRY_POT; i++) {
        BerryPotsApp_LoadPlantGfx(data, i);
        BerryPotsApp_UpdatePlantSprite(data, i);
    }
}

static BOOL BerryPotsApp_WateringAnim(BerryPotsAppData *data) {
    switch (data->state72) {
    case 0:
        data->wateringPot = data->selectedPot;
        data->nextWateringPot = 0xFF;
        Sprite_SetPositionXY(data->sprites[2], (s16)(data->wateringPot * 27 + 31), 56);
        Sprite_SetAnimCtrlSeq(data->sprites[2], 8);
        Sprite_ResetAnimCtrlState(data->sprites[2]);
        Sprite_SetAnimActiveFlag(data->sprites[2], TRUE);
        Sprite_SetDrawFlag(data->sprites[2], TRUE);
        data->waitFrames = 0;
        data->state72++;
        break;
    case 1:
        if (data->waitFrames++ >= 15) {
            PlaySE(SEQ_SE_GS_MIZU);
            Sprite_ResetAnimCtrlState(data->sprites[2]);
            Sprite_SetAnimCtrlSeq(data->sprites[2], 9);
            Sprite_SetAnimActiveFlag(data->sprites[1], TRUE);
            data->waitFrames = 0;
            data->state72++;
        }
        break;
    case 2:
        if (data->waitFrames++ == 0) {
            BerryPotsApp_WaterPot(data, data->wateringPot);
        }

        if (data->waitFrames < 30) {
            BerryPotsApp_HandleWateringInput(data);
            break;
        }

        data->waitFrames = 0;
        if (data->nextWateringPot == 0xFF) {
            StopSE(SEQ_SE_GS_MIZU, 2);
            Sprite_SetAnimCtrlSeq(data->sprites[2], 8);
            data->state72 = 4;
        } else {
            BerryPotsApp_StartWateringCanMove(data);
            data->state72++;
        }

        break;
    case 3:
        if (data->runningTasks == 0) {
            data->waitFrames = 0;
            data->wateringPot = data->nextWateringPot;
            data->nextWateringPot = 0xFF;
            Sprite_SetAnimActiveFlag(data->sprites[1], TRUE);
            data->state72 = 2;
        }
        break;
    case 4:
        if (data->waitFrames++ >= 15) {
            BerryPotsApp_RefreshPotData(data);
            Sprite_SetDrawFlag(data->sprites[2], FALSE);
            Sprite_SetAnimActiveFlag(data->sprites[1], FALSE);
            data->waitFrames = 0;
            data->state72 = 0;
            return TRUE;
        }
        break;
    }

    return FALSE;
}

static BOOL BerryPotsApp_HarvestAnim(BerryPotsAppData *data) {
    switch (data->state72) {
    case 0:
        BerryPotsApp_LoadBerryIconGfx(data, data->selectedPot);
        data->harvestBerriesLeft = data->pots[data->selectedPot].quantityOrYieldMaybe;
        data->waitFrames = 0;
        data->state72++;
        break;
    case 1:
        if (data->waitFrames == 0) {
            BerryPotsApp_StartHarvestBerryAnim(data);
            data->harvestBerriesLeft--;
            data->waitFrames = 8;
        }
        data->waitFrames--;
        if (data->harvestBerriesLeft == 0) {
            data->state72++;
        }
        break;
    case 2:
        if (data->runningTasks != 0) {
            break;
        }

        data->waitFrames = 0;
        data->state72 = 0;
        return TRUE;
    }

    return FALSE;
}

void BerryPotsApp_DrawCancelButton(BerryPotsAppData *data, u32 seqNo) {
    Sprite_SetAnimCtrlSeq(data->sprites[0], seqNo);
    Sprite_ResetAnimCtrlState(data->sprites[0]);
    FillWindowPixelBuffer(&data->windows[0], 0);

    u32 stringWidth = FontID_String_GetWidth(4, data->cancelString, 0);
    AddTextPrinterParameterizedWithColor(&data->windows[0], 4, data->cancelString, (48 - stringWidth) / 2, 0, 0xFF, MAKE_TEXT_COLOR(1, 2, 0), NULL);

    ScheduleWindowCopyToVram(&data->windows[0]);
}

void BerryPotsApp_UpdateCursor(BerryPotsAppData *data) {
    u8 unk7C = data->selectedPot;
    Sprite_SetPositionXY(data->sprites[1], (unk7C + 1) * 27, 96);
    if (data->pots[unk7C].growthStage > BERRY_POT_GROWTH_STAGE_GROWING) {
        Sprite_SetAnimCtrlSeq(data->sprites[1], 3);
    } else {
        Sprite_SetAnimCtrlSeq(data->sprites[1], 2);
    }
}

static void BerryPotsApp_SetCursorVisible(BerryPotsAppData *data, BOOL visible) {
    Sprite_SetDrawFlag(data->sprites[1], visible);
}

static void BerryPotsApp_SetAnimsActive(BerryPotsAppData *data, BOOL active) {
    Sprite_SetAnimActiveFlag(data->sprites[1], active);

    for (int i = 0; i < MAX_BERRY_POT; i++) {
        if (data->pots[i].soilSpriteMaybe != NULL && data->pots[i].growthStage >= BERRY_POT_GROWTH_STAGE_SPROUTED) {
            Sprite_SetAnimActiveFlag(data->pots[i].soilSpriteMaybe, active);
        }
    }
}

static void BerryPotsApp_PrintMessage(BerryPotsAppData *data, enum BerryPotsAppMessage action, u8 a2, BOOL a3) {
    Window *window = (action == BERRY_POTS_APP_MSG_TOUCH_POT) ? &data->windows[1] : &data->windows[2];
    FillWindowPixelBuffer(window, 0xF);
    if (a3) {
        FillBgTilemapRect(data->bgConfig, 0, RGB_BLACK, 0, 18, 32, 6, 0);
        DrawFrameAndWindow2(window, TRUE, 1, 13);
    }
    BerryPotsApp_FormatMessage(data, action, a2);
    AddTextPrinterParameterized(window, 1, data->currentStatusString, 0, 0, 0xFF, NULL);
    ScheduleWindowCopyToVram(window);
    data->messageTimer = 60;
}

static void BerryPotsApp_CreateMenu(BerryPotsAppData *data, u32 a1) {
    TouchscreenListMenuHeader header;
    MI_CpuFill8(&header, 0, sizeof(TouchscreenListMenuHeader));

    header.template = sMenuTemplate;
    header.listMenuItems = data->listMenuItems[a1];
    header.bgConfig = data->bgConfig;
    header.numWindows = sBerryPotsMenus[a1].numButtons;

    data->menu = TouchscreenListMenu_Create(data->menuSpawner, &header, (u8)data->fieldMenuState, 0xf, 16 - (header.numWindows * 3), 0xe, 0);
}

static int BerryPotsApp_HandleMenuInput(BerryPotsAppData *data) {
    int r4 = TouchscreenListMenu_HandleInput(data->menu);
    int r0 = -1;
    if (r4 != -1) {
        data->fieldMenuState = (MenuInputState)TouchscreenListMenu_WasLastInputTouch(data->menu);
        TouchscreenListMenu_Destroy(data->menu);
        r0 = r4;
    }
    return r0;
}

void BerryPotsApp_UpdatePlantSprite(BerryPotsAppData *data, int index) {
    BerryPotsAppPot *sub = &data->pots[index];
    if (sub->prevGrowthStage != 0 && sub->soilSpriteMaybe != NULL) {
        Sprite_Delete(sub->soilSpriteMaybe);
    }

    if (sub->growthStage == BERRY_POT_GROWTH_STAGE_NONE) {
        sub->soilSpriteMaybe = NULL;
        Sprite_SetAnimationFrame(data->sprites[index + 3], 2);
        return;
    }

    UnmanagedSpriteTemplate unkStruct;
    MI_CpuCopy8(&sBerryPlantSpriteTemplate, &unkStruct, sizeof(UnmanagedSpriteTemplate));
    unkStruct.x = (index + 1) * 27;
    switch (sub->growthStage) {
    case BERRY_POT_GROWTH_STAGE_PLANTED:
        unkStruct.animation = 5;
        break;
    case BERRY_POT_GROWTH_STAGE_SPROUTED:
        unkStruct.animation = 6;
        break;
    default:
        unkStruct.resourceSet = index + 1;
        unkStruct.animation = sub->growthStage - 3;
        break;
    }

    sub->soilSpriteMaybe = SpriteSystem_CreateSpriteFromResourceHeader(data->spriteRenderer, data->spriteGfxHandler1, &unkStruct);
    Sprite_SetDrawFlag(sub->soilSpriteMaybe, TRUE);
    if (sub->growthStage == BERRY_POT_GROWTH_STAGE_PLANTED) {
        Sprite_SetAnimActiveFlag(sub->soilSpriteMaybe, FALSE);
        Sprite_SetAnimationFrame(sub->soilSpriteMaybe, data->pots[index].soilState);
    } else {
        Sprite_SetAnimActiveFlag(sub->soilSpriteMaybe, TRUE);
    }

    Sprite_SetAnimationFrame(data->sprites[index + 3], data->pots[index].soilState);
}

void BerryPotsApp_WaterPot(BerryPotsAppData *data, u8 index) {
    BerryPotsAppPot *unk = &data->pots[index];
    BerryPots_ResetPotMoisture(data->berryPots, index);
    BerryPotsApp_RefreshPotData(data);
    Sprite_SetAnimationFrame(data->sprites[index + 3], 2);
    if (unk->growthStage == BERRY_POT_GROWTH_STAGE_PLANTED) {
        Sprite_SetAnimationFrame(unk->soilSpriteMaybe, unk->soilState);
    }
}

static void BerryPotsApp_HarvestBerries(BerryPotsAppData *data) {
    BerryPotsAppPot *unk = &data->pots[data->selectedPot];
    BerryPots_HarvestPot(data->berryPots, data->selectedPot);
    Bag_AddItem(data->bag, BerryIdToItemId(unk->berryId), unk->quantityOrYieldMaybe, data->heapID);
    BerryPotsApp_RefreshPotData(data);
    Sprite_Delete(unk->soilSpriteMaybe);
    unk->soilSpriteMaybe = NULL;
    Sprite_SetAnimationFrame(data->sprites[data->selectedPot + 3], 2);
    GameStats_AddScore(data->stats, SCORE_EVENT_BERRY_PICKED);
}

static void BerryPotsApp_VBlankCB(BerryPotsAppData *data) {
    if (data->spriteRenderer != NULL) {
        SpriteSystem_TransferOam();
    }
    NNS_GfdDoVramTransfer();
    DoScheduledBgGpuUpdates(data->bgConfig);
    OS_SetIrqCheckFlag(OS_IE_VBLANK);
}

static void BerryPotsApp_SetGfxBanks(void) {
    GraphicsBanks banks = sBerryPotsGraphicsBanks;
    GfGfx_SetBanks(&banks);
}

static void BerryPotsApp_SetupBgConfig(BerryPotsAppData *data) {
    BerryPotsApp_SetGfxBanks();
    data->bgConfig = BgConfig_Alloc(data->heapID);

    GraphicsModes modes = sBerryPotsGraphicsModes;
    SetBothScreensModesAndDisable(&modes);

    GX_SetDispSelect(GX_DISP_SELECT_SUB_MAIN);

    BgTemplate bgTemplate4 = sBerryPotsBgTemplate_Sub0;
    InitBgFromTemplate(data->bgConfig, 4, &bgTemplate4, 0);
    BgClearTilemapBufferAndCommit(data->bgConfig, 4);

    BgTemplate bgTemplate7 = sBerryPotsBgTemplate_Sub3;
    InitBgFromTemplate(data->bgConfig, 7, &bgTemplate7, 0);
    BgClearTilemapBufferAndCommit(data->bgConfig, 7);

    BgTemplate bgTemplate0 = sBerryPotsBgTemplate_Main0;
    InitBgFromTemplate(data->bgConfig, 0, &bgTemplate0, 0);
    BgClearTilemapBufferAndCommit(data->bgConfig, 0);

    BgTemplate bgTemplate2 = sBerryPotsBgTemplate_Main2;
    InitBgFromTemplate(data->bgConfig, 2, &bgTemplate2, 0);
    BgClearTilemapBufferAndCommit(data->bgConfig, 2);

    BgTemplate bgTemplate3 = sBerryPotsBgTemplate_Main3;
    InitBgFromTemplate(data->bgConfig, 3, &bgTemplate3, 0);
    BgClearTilemapBufferAndCommit(data->bgConfig, 3);

    BG_ClearCharDataRange(4, 0x20, 0, data->heapID);
    BG_ClearCharDataRange(7, 0x20, 0, data->heapID);
    BG_ClearCharDataRange(0, 0x20, 0, data->heapID);
    BG_ClearCharDataRange(2, 0x20, 0, data->heapID);
    BG_ClearCharDataRange(3, 0x40, 0, data->heapID);
}

static void BerryPotsApp_FreeBgConfig(BerryPotsAppData *data) {
    FreeBgTilemapBuffer(data->bgConfig, 3);
    FreeBgTilemapBuffer(data->bgConfig, 2);
    FreeBgTilemapBuffer(data->bgConfig, 0);
    FreeBgTilemapBuffer(data->bgConfig, 7);
    FreeBgTilemapBuffer(data->bgConfig, 4);
    Heap_Free(data->bgConfig);

    GX_SetDispSelect(GX_DISP_SELECT_MAIN_SUB);
}

static void BerryPotsApp_SetupNarc(BerryPotsAppData *data) {
    NARC *narc = NARC_New(NARC_a_2_1_6, data->heapID);

    GfGfxLoader_GXLoadPalFromOpenNarc(narc, 3, GF_PAL_LOCATION_MAIN_BG, GF_PAL_SLOT_0_OFFSET, 0, data->heapID);
    GfGfxLoader_LoadCharDataFromOpenNarc(narc, 4, data->bgConfig, GF_BG_LYR_MAIN_3, 0, 0, FALSE, data->heapID);
    GfGfxLoader_LoadScrnDataFromOpenNarc(narc, 5, data->bgConfig, GF_BG_LYR_MAIN_3, 0, 0, FALSE, data->heapID);
    GfGfxLoader_GXLoadPalFromOpenNarc(narc, 0, GF_PAL_LOCATION_SUB_BG, GF_PAL_SLOT_0_OFFSET, 0, data->heapID);
    GfGfxLoader_LoadCharDataFromOpenNarc(narc, 1, data->bgConfig, GF_BG_LYR_SUB_3, 0, 0, FALSE, data->heapID);
    GfGfxLoader_LoadScrnDataFromOpenNarc(narc, 2, data->bgConfig, GF_BG_LYR_SUB_3, 0, 0, FALSE, data->heapID);

    data->berryPotsAppNarc = narc;

    LoadUserFrameGfx2(data->bgConfig, GF_BG_LYR_MAIN_0, 1, 13, data->frame, data->heapID);
    LoadFontPal1(GF_PAL_LOCATION_MAIN_BG, GF_PAL_SLOT_12_OFFSET, data->heapID);
}

void BerryPotsApp_FreeNarc(BerryPotsAppData *data) {
    NARC_Delete(data->berryPotsAppNarc);
}

static void BerryPotsApp_SetupText(BerryPotsAppData *data) {
    FontID_Alloc(4, data->heapID);

    data->msgData = NewMsgDataFromNarc(MSGDATA_LOAD_DIRECT, NARC_msgdata_msg, NARC_msg_msg_0248_bin, data->heapID);
    data->msgFmt = MessageFormat_New_Custom(16, 16, data->heapID);
    data->currentStatusString = String_New(128, data->heapID);
    data->cancelString = NewString_ReadMsgData(data->msgData, msg_0248_00000);

    for (int i = 0; i < (int)NELEMS(data->statusStrings); i++) {
        data->statusStrings[i] = NewString_ReadMsgData(data->msgData, msg_0248_00006 + i);
    }
}

static void BerryPotsApp_FreeText(BerryPotsAppData *data) {
    for (int i = 0; i < (int)NELEMS(data->statusStrings); i++) {
        String_Delete(data->statusStrings[i]);
    }
    String_Delete(data->cancelString);
    String_Delete(data->currentStatusString);

    MessageFormat_Delete(data->msgFmt);
    DestroyMsgData(data->msgData);

    FontID_Release(4);
}

static void BerryPotsApp_SetupWindows(BerryPotsAppData *data) {
    for (int i = 0; i < (int)NELEMS(data->windows); i++) {
        AddWindow(data->bgConfig, &data->windows[i], &sBerryPotsWindowTemplates[i]);
        FillWindowPixelBuffer(&data->windows[i], 0x0);
    }

    data->unk12C = YesNoPrompt_Create(data->heapID);
}

static void BerryPotsApp_FreeWindows(BerryPotsAppData *data) {
    for (int i = 0; i < (int)NELEMS(data->windows); i++) {
        ClearWindowTilemapAndCopyToVram(&data->windows[i]);
        RemoveWindow(&data->windows[i]);
    }

    YesNoPrompt_Destroy(data->unk12C);
}

static void BerryPotsApp_SetupListMenuItems(BerryPotsAppData *data) {
    for (int i = 0; i < (int)NELEMS(data->listMenuItems); i++) {
        const struct BerryPotsMenu *unk = &sBerryPotsMenus[i];
        data->listMenuItems[i] = ListMenuItems_New(unk->numButtons, data->heapID);
        for (int j = 0; j < (int)unk->numButtons; j++) {
            ListMenuItems_AppendFromMsgData(data->listMenuItems[i], data->msgData, unk->msgId[j], j);
        }
    }
}

static void BerryPotsApp_FreeListMenuItems(BerryPotsAppData *data) {
    for (int i = 0; i < (int)NELEMS(data->listMenuItems); i++) {
        ListMenuItems_Delete(data->listMenuItems[i]);
        data->listMenuItems[i] = NULL;
    }
}

static void BerryPotsApp_SetupSpriteRendererAndGfxHandler(BerryPotsAppData *data) {
    GF_CreateVramTransferManager(32, data->heapID);

    data->spriteRenderer = SpriteSystem_Alloc(data->heapID);
    SpriteSystem_Init(data->spriteRenderer, &sBerryPotsOamManagerParam, &sBerryPotsOamCharTransferParam, 8);

    thunk_ClearMainOAM(data->heapID);
    thunk_ClearSubOAM(data->heapID);

    data->spriteGfxHandler1 = SpriteManager_New(data->spriteRenderer);
    SpriteSystem_InitSprites(data->spriteRenderer, data->spriteGfxHandler1, 25);
    SpriteSystem_LoadResourceDataFromFileIdListEx(data->spriteRenderer, data->spriteGfxHandler1, &sBerryPotsResdatIdList, 0, 0);

    data->itemIconNarc = NARC_New(NARC_itemtool_itemdata_item_icon, data->heapID);

    SpriteResourceCountsListUnion counts = sBerryPotsSpriteResourceCounts;
    data->spriteGfxHandler2 = SpriteManager_New(data->spriteRenderer);
    SpriteSystem_InitManagerWithCapacities(data->spriteRenderer, data->spriteGfxHandler2, &counts);
    SpriteManager_SetSpriteList(data->spriteGfxHandler2, SpriteManager_GetSpriteList(data->spriteGfxHandler1));

    SpriteSystem_LoadCharResObjFromOpenNarc(data->spriteRenderer, data->spriteGfxHandler2, data->itemIconNarc, GetItemIndexMapping(ITEM_CHERI_BERRY, ITEMNARC_NCGR), FALSE, NNS_G2D_VRAM_TYPE_2DMAIN, 4000);
    SpriteSystem_LoadPlttResObjFromOpenNarc(data->spriteRenderer, data->spriteGfxHandler2, data->itemIconNarc, GetItemIndexMapping(ITEM_CHERI_BERRY, ITEMNARC_NCLR), FALSE, 1, NNS_G2D_VRAM_TYPE_2DMAIN, 4000);
    SpriteSystem_LoadCellResObjFromOpenNarc(data->spriteRenderer, data->spriteGfxHandler2, data->itemIconNarc, GetItemIconCell(), FALSE, 4000);
    SpriteSystem_LoadAnimResObjFromOpenNarc(data->spriteRenderer, data->spriteGfxHandler2, data->itemIconNarc, GetItemIconAnim(), FALSE, 4000);

    data->menuSpawner = TouchscreenListMenuSpawner_Create(data->heapID, 0);
}

static void BerryPotsApp_FreeSpriteRendererAndGfxHandler(BerryPotsAppData *data) {
    TouchscreenListMenuSpawner_Destroy(data->menuSpawner);

    NARC_Delete(data->itemIconNarc);

    SpriteSystem_FreeResourcesAndManager(data->spriteRenderer, data->spriteGfxHandler2);
    SpriteSystem_DestroySpriteManager(data->spriteRenderer, data->spriteGfxHandler1);
    data->spriteGfxHandler2 = NULL;
    data->spriteGfxHandler1 = NULL;
    SpriteSystem_Free(data->spriteRenderer);
    data->spriteRenderer = NULL;

    GF_DestroyVramTransferManager();

    thunk_ClearMainOAM(data->heapID);
}

static void BerryPotsApp_SetupSpriteSystem(BerryPotsAppData *data) {
    BerryPotsApp_SetupSpriteRendererAndGfxHandler(data);
    BerryPotsApp_SetupSprites(data);
    GfGfx_EngineATogglePlanes(GX_PLANEMASK_OBJ, GF_PLANE_TOGGLE_ON);
}

static void BerryPotsApp_FreeSpriteSystem(BerryPotsAppData *data) {
    GfGfx_EngineATogglePlanes(GX_PLANEMASK_OBJ, GF_PLANE_TOGGLE_OFF);
    BerryPotsApp_FreeSprites(data);
    BerryPotsApp_FreeSpriteRendererAndGfxHandler(data);
}

static void BerryPotsApp_SetupSprites(BerryPotsAppData *data) {
    for (int i = 0; i <= 1; i++) {
        data->sprites[i] = SpriteSystem_CreateSpriteFromResourceHeader(data->spriteRenderer, data->spriteGfxHandler1, &sBerryPotsSpriteTemplates[i]);
        Sprite_SetDrawFlag(data->sprites[i], TRUE);
        Sprite_SetAnimActiveFlag(data->sprites[i], TRUE);
    }

    data->sprites[2] = SpriteSystem_CreateSpriteFromResourceHeader(data->spriteRenderer, data->spriteGfxHandler1, &sBerryPotsSpriteTemplates[2]);
    Sprite_SetDrawFlag(data->sprites[2], FALSE);
    Sprite_SetAnimActiveFlag(data->sprites[2], FALSE);

    for (int i = 0; i < (int)NELEMS(data->pots); i++) {
        u8 spriteIndex = i + 3;
        data->sprites[spriteIndex] = SpriteSystem_CreateSpriteFromResourceHeader(data->spriteRenderer, data->spriteGfxHandler1, &sBerryPotsSpriteTemplates[3]);
        Sprite_SetPositionXY(data->sprites[spriteIndex], i * 27 + 27, 99);
        Sprite_SetAnimationFrame(data->sprites[spriteIndex], data->pots[i].soilState);
        Sprite_SetDrawFlag(data->sprites[spriteIndex], TRUE);
        Sprite_SetAnimActiveFlag(data->sprites[spriteIndex], FALSE);

        spriteIndex = i + 7;
        data->sprites[spriteIndex] = SpriteSystem_CreateSpriteFromResourceHeader(data->spriteRenderer, data->spriteGfxHandler1, &sBerryPotsSpriteTemplates[4]);
        Sprite_SetPositionXY(data->sprites[spriteIndex], i * 27 + 27, 88);
        Sprite_SetDrawFlag(data->sprites[spriteIndex], FALSE);
        Sprite_SetAnimActiveFlag(data->sprites[spriteIndex], TRUE);
    }
}

static void BerryPotsApp_FreeSprites(BerryPotsAppData *data) {
    for (int i = 0; i < (int)NELEMS(data->sprites); i++) {
        if (data->sprites[i] != NULL) {
            Sprite_Delete(data->sprites[i]);
        }
    }
}

static void BerryPotsApp_LoadPlantGfx(BerryPotsAppData *data, int index) {
    u8 berryId = data->pots[index].berryId;
    if (berryId != BERRY_NONE) {
        berryId--;
    }

    SpriteSystem_ReplaceCharResObjFromOpenNarc(data->spriteRenderer, data->spriteGfxHandler1, data->berryPotsAppNarc, berryId + 76, FALSE, index + 1);
    SpriteSystem_ReplacePlttResObjFromOpenNarc(data->spriteRenderer, data->spriteGfxHandler1, data->berryPotsAppNarc, berryId + 12, FALSE, index + 1);
}

static void BerryPotsApp_LoadBerryIconGfx(BerryPotsAppData *data, u8 index) {
    u16 itemId = BerryIdToItemId(data->pots[index].berryId);

    SpriteSystem_ReplaceCharResObjFromOpenNarc(data->spriteRenderer, data->spriteGfxHandler2, data->itemIconNarc, GetItemIndexMapping(itemId, ITEMNARC_NCGR), FALSE, 4000);
    SpriteSystem_ReplacePlttResObjFromOpenNarc(data->spriteRenderer, data->spriteGfxHandler2, data->itemIconNarc, GetItemIndexMapping(itemId, ITEMNARC_NCLR), FALSE, 4000);
}

ManagedSprite *BerryPotsApp_CreateBerryIconSprite(BerryPotsAppData *data, s16 x, s16 y, u8 unused) {
    ManagedSpriteTemplate template;
    MI_CpuFill8(&template, 0, sizeof(ManagedSpriteTemplate));

    template.x = x;
    template.y = y;
    template.z = 0;
    template.animation = 0;
    template.bgPriority = 2;
    template.pal = 0;
    template.vramTransfer = 0;
    template.vram = NNS_G2D_VRAM_TYPE_2DMAIN;
    template.resIdList[GF_GFX_RES_TYPE_CHAR] = 4000;
    template.resIdList[GF_GFX_RES_TYPE_PLTT] = 4000;
    template.resIdList[GF_GFX_RES_TYPE_CELL] = 4000;
    template.resIdList[GF_GFX_RES_TYPE_ANIM] = 4000;
    template.resIdList[GF_GFX_RES_TYPE_MCEL] = -1;
    template.resIdList[GF_GFX_RES_TYPE_MANM] = -1;

    return SpriteSystem_NewSprite(data->spriteRenderer, data->spriteGfxHandler2, &template);
}

u8 BerryPots_DeadstrippedFunction(int a1, int a2) {
    return ov17_02203D60[a1][a2];
}
