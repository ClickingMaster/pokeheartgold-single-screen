#include "global.h"

#include "data/resdat.naix"
#include "field/field_sprite_manager.h"
#include "field/fieldmap.h"
#include "field/map_prop_animation.h"
#include "field/map_prop_render.h"
#include "msgdata/msg.naix"

#include "bag.h"
#include "bag_view.h"
#include "gf_gfx_loader.h"
#include "launch_application.h"
#include "mart.h"
#include "overlay_01.h"
#include "overlay_03.h"
#include "render_window.h"
#include "scrcmd.h"
#include "screen_fade.h"
#include "sprite_transfer.h"
#include "sys_flags.h"
#include "sys_vars.h"
#include "task.h"
#include "text.h"
#include "sound_playback.h"
#include "communication_club_link.h"
#include "seal_data.h"
#include "mart_bag_task.h"

static void RocketHQ_UpdateElectrodePropAnimation(FieldSystem *fieldSystem, u8 index);
static void MartTask_InitMartMessageData(MartData *data);
static void MartData_InitMessageData(MartData *data);
static void MartData_InitCamera(FieldSystem *fieldSystem, MartData *data);
static void MartData_AddWindows(MartData *data);
static u8 MartData_WaitForBottomScreenApp(FieldSystem *fieldSystem, MartData *data);
static void MartData_PrintItemDescription(MartData *data, u32 position);
static int MartData_HandleItemListInput(MartData *data);
static u8 MartData_SelectItemListButton(MartData *data, u32 button);
static u8 MartData_PrevPage(MartData *data);
static u8 MartData_NextPage(MartData *data);
static void MartData_DrawPageItemIcons(MartData *data, int pageStart, u8 numItems);
static u8 MartData_CloseMenu(MartData *data);
static u8 MartData_BeginPurchase(MartData *data, u16 itemID);
static u8 MartData_WaitQuantityPrompt(MartData *data);
static u8 MartData_FinishQuantityInput(MartData *data);
static u8 MartData_CancelQuantityInput(MartData *data);
static u8 MartData_HandleQuantityInput(MartData *data);
static u8 MartData_HandleQuantityButton(MartData *data, u32 button);
static u8 MartData_CheckInventorySpace(MartData *data);
static u8 MartData_WaitConfirmPrompt(MartData *data);
static u8 MartData_HandleConfirmInput(MartData *data, u32 button);
static u8 MartData_PerformTransaction(MartData *data);
static u8 MartData_WaitPurchaseMessage(MartData *data);
static u8 MartData_WaitErrorMessage(MartData *data);
static u8 MartData_WaitPremierBallMessage(MartData *data);
static void MartData_RestoreBottomScreenApp(FieldSystem *fieldSystem, MartData *data_unused);
static u8 MartData_FinishClose(FieldSystem *fieldSystem, MartData *data);
static void MartData_DeleteSprites(MartData *data);
static void MartData_SetScrollArrowPositions(MartData *data, BOOL arg1);
static void MartData_SetSprite2PaletteOverride(MartData *data, int flag);
static u8 MartData_StartButtonPressAnim(MartData *data, u8 arg1, u8 arg2);
static u8 MartData_AnimateButtonPress(MartData *data);
static void Mart_FadeOut(FieldSystem *fieldSystem_unused, MartData *data);
static void Task_Mart_LaunchSellBag(TaskManager *taskManager);
static u8 MartData_WaitFade(FieldSystem *fieldSystem_unused, MartData *data_unused);
static void MartData_RestoreBgPriorities(MartData *data);
static BOOL MartData_Destroy(FieldSystem *fieldSystem_unused, MartData *data);
static void MartData_UpdateButtonSprites(MartData *data, int arg1);
static void MartData_SetSelectedItemIcon(MartData *data, u16 itemID);
static void MartData_LoadItemIcon(MartData *data, int charID, int paletteID, u16 item);
static int Mart_StepQuantity(s16 currentQuantity, u16 maxQuantity, s16 modifier);

static const u16 sRocketHQElectrodePropModels[4] = {
    149,
    152,
    153,
    0
};

static const WindowTemplate sMartWindowTemplates[6] = {
    {
     .bgId = GF_BG_LYR_MAIN_2,
     .left = 13,
     .top = 2,
     .width = 18,
     .height = 14,
     .palette = 13,
     .baseTile = 1,
     },
    {
     .bgId = GF_BG_LYR_MAIN_2,
     .left = 5,
     .top = 18,
     .width = 27,
     .height = 6,
     .palette = 13,
     .baseTile = 253,
     },
    {
     .bgId = GF_BG_LYR_MAIN_3,
     .left = 1,
     .top = 1,
     .width = 10,
     .height = 4,
     .palette = 13,
     .baseTile = 40,
     },
    {
     .bgId = GF_BG_LYR_MAIN_3,
     .left = 19,
     .top = 13,
     .width = 12,
     .height = 4,
     .palette = 13,
     .baseTile = 80,
     },
    {
     .bgId = GF_BG_LYR_MAIN_3,
     .left = 1,
     .top = 15,
     .width = 14,
     .height = 2,
     .palette = 13,
     .baseTile = 128,
     },
    {
     .bgId = GF_BG_LYR_MAIN_3,
     .left = 2,
     .top = 19,
     .width = 27,
     .height = 4,
     .palette = 12,
     .baseTile = 156,
     }
};

static const WindowTemplate ov03_02259464 = {
    .bgId = GF_BG_LYR_MAIN_2,
    .left = 1,
    .top = 18,
    .width = 27,
    .height = 6,
    .palette = 13,
    .baseTile = 253,
};

BOOL ScrCmd_UpdateMapPropAnimsFromFlag96B(ScriptContext *ctx) {
    for (int i = 0; i < 3; i++) {
        RocketHQ_UpdateElectrodePropAnimation(ctx->fieldSystem, i);
    }
    return FALSE;
}

static void RocketHQ_UpdateElectrodePropAnimation(FieldSystem *fieldSystem, u8 index) {
    u16 modelID = sRocketHQElectrodePropModels[index];
    Field3dRenderObj *unkC0_SubC = Field3dObjectList_GetRenderObjectByID(fieldSystem->renderObjManager, modelID);
    if (unkC0_SubC == NULL) {
        GF_AssertFail();
    } else {
        MapPropAnimationManager_AddAnimationToRenderObj(modelID, CheckRocketHQElectrodeDefeated(Save_VarsFlags_Get(fieldSystem->saveData), index), 1, &unkC0_SubC->renderObj, fieldSystem->mapPropAnimationManager); 
    }
}

static u32 Mart_CountItems(const u16 *items, u16 *priceOverrides, u32 martType) {
    int i;
    if (martType == MART_TYPE_3 || martType == MART_TYPE_4) {
        for (i = 0; i < 256; i++) {
            if (*priceOverrides != 0xFFFF) {
                priceOverrides += 2;
            } else {
                break;
            }
        }
    } else {
        for (i = 0; i < 256; i++) {
            if (*items != 0xFFFF) {
                items++;
            } else {
                break;
            }
        }
    }
    return i;
}

static void MartData_CopyItems(MartData *data, const u16 *items, BOOL flag09A) {
    int max = data->numItems;
    int i;
    int k = 0;
    for (i = 0; i < max; i++) {
        if (flag09A == FALSE) {
            if (items[i] == 4) {
                data->numItems--;
            } else {
                data->itemIds[k] = items[i];
                k++;
            }
        } else {
            data->itemIds[k] = items[i];
            k++;
        }
    }
}

static void MartData_CopyPriceOverrideItems(MartData *data, u16 *priceOverrides) {
    for (int i = 0, j = 0; i < data->numItems; i++, j += 2) {
        data->itemIds[i] = priceOverrides[j]; // martItem->cost?
    }
}

static void MartData_InitItemList(MartData *data, const u16 *items, BOOL flag09A, const struct MartItem *priceOverrides) {
    data->numItems = Mart_CountItems(items, (u16 *)priceOverrides, data->martType);
    if (data->numItems >= 0xFF) {
        GF_AssertFail();
    }
    data->itemIds = Heap_Alloc(HEAP_ID_FIELD2, data->numItems * 2);
    if (data->martType == MART_TYPE_3 || data->martType == MART_TYPE_4) {
        MartData_CopyPriceOverrideItems(data, (u16 *)priceOverrides);
    } else {
        MartData_CopyItems(data, items, flag09A);
    }
}

static MartData *Mart_Alloc() {
    MartData *data = Heap_Alloc(HEAP_ID_FIELD2, sizeof(MartData));
    memset(data, 0, sizeof(MartData));
    return data;
}

void Mart_Init(TaskManager *taskManager, FieldSystem *fieldSystem, const u16 *items, int kind, u8 buy_sell, int deco_which, const struct MartItem *priceOverrides) {
    MartData *martData = Mart_Alloc();
    martData->bgConfig = fieldSystem->bgConfig;
    martData->string = String_New(96, HEAP_ID_FIELD2);
    martData->playerProfile = Save_PlayerData_GetProfile(fieldSystem->saveData);
    martData->options = Save_PlayerData_GetOptionsAddr(fieldSystem->saveData);
    martData->gameStats = Save_GameStats_Get(fieldSystem->saveData);
    martData->varsFlags = Save_VarsFlags_Get(fieldSystem->saveData);
    martData->apricornBox = Save_ApricornBox_Get(fieldSystem->saveData);
    martData->unk264 = deco_which;
    martData->martType = kind;
    martData->pageStart = 0;
    martData->cursorPos = 0;
    martData->touchButton = -1;
    martData->touchMartAction = 0;
    martData->pokeathlonSave = Save_Pokeathlon_Get(fieldSystem->saveData);
    martData->priceOverrides = priceOverrides;
    martData->sellOnly = buy_sell;
    if (martData->martType == MART_TYPE_NORMAL || martData->martType == MART_TYPE_3 || martData->martType == MART_TYPE_4) {
        martData->inventory = Save_Bag_Get(fieldSystem->saveData);
    } else {
        martData->inventory = Save_SealCase_Get(fieldSystem->saveData);
    }
    // Mart_SetItemsForSale?
    MartData_InitItemList(martData, items, CheckFlag09A(martData->varsFlags), priceOverrides);
    TaskManager_Call(taskManager, Task_Mart, martData);
}

enum MartTaskStates {
    TASK_MART_START,
    TASK_MART_1,
    TASK_MART_2,
    TASK_MART_3,
    TASK_MART_4,
    TASK_MART_5,
    TASK_MART_6,
    TASK_MART_7,
    TASK_MART_8,
    TASK_MART_9,
    TASK_MART_10,
    TASK_MART_11,
    TASK_MART_12,
    TASK_MART_13,
    TASK_MART_14,
    TASK_MART_15,
    TASK_MART_16,
    TASK_MART_17,
    TASK_MART_18,
    TASK_MART_19,
    TASK_MART_20,
    TASK_MART_21,
    TASK_MART_22,
    TASK_MART_23,
    TASK_MART_24,
    TASK_MART_25,
    TASK_MART_26,
    TASK_MART_27
};

BOOL Task_Mart(TaskManager *taskManager) {
    FieldSystem *fieldSystem = TaskManager_GetFieldSystem(taskManager);
    MartData *data = TaskManager_GetEnvironment(taskManager);

    switch (data->state) {
    case TASK_MART_START:
        MartTask_InitMartMessageData(data);
        if (data->sellOnly == 0) {
            data->state = TASK_MART_1;
        } else {
            data->state = 22;
        }
        break;
    case TASK_MART_1:
        MartData_InitCamera(fieldSystem, data);
        FieldSystem_SwitchBottomScreenApp(fieldSystem, 2, data);
        data->state = TASK_MART_2;
        break;
    case TASK_MART_2:
        data->state = MartData_WaitForBottomScreenApp(fieldSystem, data); // Returns 2 or 3.
        break;
    case TASK_MART_3:
        if (data->touchButton == -1) {
            data->state = MartData_HandleItemListInput(data);
        } else {
            data->state = MartData_SelectItemListButton(data, data->touchButton);
            data->touchButton = -1;
        }
        break;
    case TASK_MART_4:
        data->state = TASK_MART_3;
        break;
    case TASK_MART_6:
        if (!Sprite_IsAnimated(data->sprites[10])) {
            data->state = MartData_BeginPurchase(data, data->itemIds[data->cursorPos + data->pageStart]);
        }
        break;
    case TASK_MART_5:
        data->state = MartData_WaitQuantityPrompt(data); // Returns 5 or 7.
        break;
    case TASK_MART_7:
        if (data->touchButton == -1) {
            data->state = MartData_HandleQuantityInput(data);
        } else {
            data->state = MartData_HandleQuantityButton(data, data->touchButton);
            data->touchButton = -1;
        }
        break;
    case TASK_MART_8:
        data->state = MartData_FinishQuantityInput(data);
        break;
    case TASK_MART_9:
        data->state = MartData_CancelQuantityInput(data); // Always returns 3.
        break;
    case TASK_MART_10:
        data->state = MartData_WaitConfirmPrompt(data);
        break;
    case TASK_MART_11:
        if (data->touchButton != -1) {
            data->state = MartData_HandleConfirmInput(data, data->touchButton);
            data->touchButton = -1;
        }
        break;
    case TASK_MART_12:
        data->state = MartData_PerformTransaction(data);
        break;
    case TASK_MART_13:
        data->state = MartData_WaitPurchaseMessage(data);
        break;
    case TASK_MART_14:
        data->state = MartData_WaitErrorMessage(data);
        break;
    case TASK_MART_15:
        data->state = MartData_WaitPremierBallMessage(data);
        break;
    case TASK_MART_16:
        data->state = MartData_CloseMenu(data); // Always returns 17.
        break;
    case TASK_MART_17:
        MartData_RestoreBgPriorities(data);
        MartData_RestoreBottomScreenApp(fieldSystem, data);
        data->state = TASK_MART_18;
        break;
    case TASK_MART_18:
        data->state = MartData_FinishClose(fieldSystem, data);
        break;
    case TASK_MART_19:
        data->state = MartData_PrevPage(data); // Always returns 3.
        break;
    case TASK_MART_20:
        data->state = MartData_NextPage(data); // Always returns 3.
        break;
    case TASK_MART_21:
        data->state = MartData_AnimateButtonPress(data);
        break;
    case TASK_MART_22:
        Mart_FadeOut(fieldSystem, data); // Always sets state to 23.
        break;
    case TASK_MART_23:
        Task_Mart_LaunchSellBag(taskManager);
        break;
    case TASK_MART_26:
        data->state = MartData_WaitFade(fieldSystem, data);
        break;
    case TASK_MART_27:
        return MartData_Destroy(fieldSystem, data);
    }

    for (int i = 0; i < 19; i++) {
        if (data->sprites[i]) {
            Sprite_UpdateAnim(data->sprites[i], 0x1000);
        }
    }
    if (data->fieldSpriteManager.spriteList) {
        SpriteList_RenderAndAnimateSprites(data->fieldSpriteManager.spriteList);
    }
    return FALSE;
}

static void MartTask_InitMartMessageData(MartData *data) {
    MartData_InitMessageData(data);
}

static void MartData_InitMessageData(MartData *data) {
    data->messageData = NewMsgDataFromNarc(MSGDATA_LOAD_DIRECT, NARC_msgdata_msg, NARC_msg_msg_0435_bin, HEAP_ID_FIELD2);
    data->messageFormat = MessageFormat_New(HEAP_ID_FIELD2);
}

static BOOL MartData_Destroy(FieldSystem *fieldSystem_unused, MartData *data) {
    DestroyMsgData(data->messageData);
    MessageFormat_Delete(data->messageFormat);
    String_Delete(data->string);
    Heap_Free(data->itemIds);
    Heap_Free(data);
    return TRUE;
}

static void MartData_InitCamera(FieldSystem *fieldSystem, MartData *data) {
    FillBgTilemapRect(data->bgConfig, 3, 0, 0, 18, 32, 6, 0);
    ScheduleBgTilemapBufferTransfer(data->bgConfig, 3);
    MartData_AddWindows(data);
    data->camera = Camera_New(HEAP_ID_FIELD2);
    Camera_Copy(fieldSystem->camera, data->camera);
    Camera_SetStaticPtr(data->camera);
    data->unk281 = 0;
}

static void MartData_AddWindows(MartData *data) {
    for (u32 i = 0; i < 6; i++) {
        if (data->martType != MART_TYPE_NORMAL && data->martType != MART_TYPE_3 && data->martType != MART_TYPE_4 && i == 1) {
            AddWindow(data->bgConfig, &data->windows[i], &ov03_02259464);
        } else {
            AddWindow(data->bgConfig, &data->windows[i], &sMartWindowTemplates[i]);
        }
    }
}

static void MartData_RemoveWindows(MartData *data) {
    EraseFrameAndWindow1(&data->windows[2], TRUE);
    for (u32 i = 0; i < 6; i++) {
        ClearWindowTilemapAndScheduleTransfer(&data->windows[i]);
        RemoveWindow(&data->windows[i]);
    };
}

static void MartData_LoadBgGraphics(MartData *data) { // MartData_LoadGraphics?
    GfGfxLoader_LoadCharData(NARC_a_0_6_0, 0, data->bgConfig, GF_BG_LYR_MAIN_1, 0, 0, FALSE, HEAP_ID_FIELD2);
    if (data->martType == MART_TYPE_NORMAL || data->martType == MART_TYPE_3 || data->martType == MART_TYPE_4) {
        GfGfxLoader_LoadScrnData(NARC_a_0_6_0, 2, data->bgConfig, GF_BG_LYR_MAIN_1, 0, 0, FALSE, HEAP_ID_FIELD2);
    } else {
        GfGfxLoader_LoadScrnData(NARC_a_0_6_0, 3, data->bgConfig, GF_BG_LYR_MAIN_1, 0, 0, FALSE, HEAP_ID_FIELD2);
    }
    GfGfxLoader_GXLoadPal(NARC_a_0_6_0, 1, GF_PAL_LOCATION_MAIN_BG, GF_PAL_SLOT_0_OFFSET, 32, HEAP_ID_FIELD2);
    LoadUserFrameGfx1(data->bgConfig, GF_BG_LYR_MAIN_3, 31, 11, 0, HEAP_ID_FIELD2);
    LoadUserFrameGfx2(data->bgConfig, GF_BG_LYR_MAIN_3, 1, 10, Options_GetFrame(data->options), HEAP_ID_FIELD2);
}

static void MartData_SaveAndSetBgPriorities(MartData *data) { // MartData_ToggleBgOn?
    data->engineAPlanes = GfGfx_EngineAGetPlanes();
    data->bgPriority[0] = GetBgPriority(data->bgConfig, 0);
    data->bgPriority[1] = GetBgPriority(data->bgConfig, 1);
    data->bgPriority[2] = GetBgPriority(data->bgConfig, 2);
    data->bgPriority[3] = GetBgPriority(data->bgConfig, 3);
    SetBgPriority(0, 3);
    SetBgPriority(1, 2);
    SetBgPriority(2, 1);
    SetBgPriority(3, 0);
    GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG0, TRUE);
    GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG1, TRUE);
    GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG2, TRUE);
    GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG3, TRUE);
}

static void MartData_RestoreBgPriorities(MartData *data) {
    SetBgPriority(0, data->bgPriority[0]);
    SetBgPriority(1, data->bgPriority[1]);
    SetBgPriority(2, data->bgPriority[2]);
    SetBgPriority(3, data->bgPriority[3]);
    GfGfx_EngineASetPlanes(data->engineAPlanes);
}

static u8 MartData_WaitForBottomScreenApp(FieldSystem *fieldSystem, MartData *data) {
    if (FieldSystem_GetBottomScreenAppState(fieldSystem) != TRUE) {
        return TASK_MART_2;
    }
    data->unk281 = 0;
    MartData_LoadBgGraphics(data); // MartData_InitFrames?
    MartData_SaveAndSetBgPriorities(data); // MartData_ToggleBgOn?
    MartData_PrintItemDescription(data, data->cursorPos);
    Sprite_SetDrawFlag(data->sprites[3], TRUE);
    return TASK_MART_3;
}

static const u8 sMartCursorSpriteParams[10][4] = {
    { 48,  56,  0, 7 },
    { 176, 56,  0, 7 },
    { 48,  96,  0, 7 },
    { 176, 96,  0, 7 },
    { 48,  136, 0, 7 },
    { 176, 136, 0, 7 },
    { 24,  176, 4, 7 },
    { 64,  176, 4, 7 },
    { 224, 176, 7, 7 },
    { 160, 176, 7, 7 }
};

static void MartData_SetCursorPosition(MartData *data, int index, int arg2) {
    switch (arg2) {
    case 0:
        Sprite_SetAnimCtrlSeq(data->sprites[10], sMartCursorSpriteParams[data->cursorPos][2]);
        thunk_Sprite_SetPaletteOverride(data->sprites[10], sMartCursorSpriteParams[index][3]);
        Sprite_SetPositionXY(data->sprites[10], sMartCursorSpriteParams[index][0], sMartCursorSpriteParams[index][1]);
    case 1:
    case 2:
        break;
    }
}

static void MartData_PrintItemDescription(MartData *data, u32 position) {
    MsgData *msgData;
    String *string;
    u16 itemID = data->itemIds[data->cursorPos + data->pageStart];
    switch (position) {
    case 0:
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
        FillWindowPixelBuffer(&data->windows[1], 0);
        if (data->cursorPos + data->pageStart < data->numItems) {
            if (data->martType == MART_TYPE_NORMAL || data->martType == MART_TYPE_3 || data->martType == MART_TYPE_4) {
                string = String_New(130, HEAP_ID_FIELD2);
                GetItemDescIntoString(string, itemID, HEAP_ID_FIELD2);
            } else if (data->martType == MART_TYPE_1) {
                msgData = NewMsgDataFromNarc(MSGDATA_LOAD_DIRECT, NARC_msgdata_msg, NARC_msg_msg_0737_bin, HEAP_ID_FIELD2);
                string = NewString_ReadMsgData(msgData, itemID + 138);
                DestroyMsgData(msgData);
            } else {
                msgData = NewMsgDataFromNarc(MSGDATA_LOAD_DIRECT, NARC_msgdata_msg, NARC_msg_msg_0434_bin, HEAP_ID_FIELD2);
                string = NewString_ReadMsgData(msgData, Seal_GetDescriptionMsgId((u8)itemID));
                DestroyMsgData(msgData);
            }
            AddTextPrinterParameterizedWithColor(&data->windows[1], 0, string, 0, 0, 0xFF, 0xF0E00, 0);
            String_Delete(string);
            MartData_SetSelectedItemIcon(data, itemID);
        } else {
            Sprite_SetDrawFlag(data->sprites[3], 0);
        }
        ScheduleWindowCopyToVram(&data->windows[1]);
        break;
    case 8:
        FillWindowPixelBuffer(&data->windows[1], 0);
        MartData_SetSelectedItemIcon(data, 0xFFFF);
        ScheduleWindowCopyToVram(&data->windows[1]);
    case 6:
    case 7:
        break;
    }
}

static const u8 sMartCursorNeighbors[9][4] = {
    { 4, 2, 6, 1 },
    { 8, 3, 0, 7 },
    { 0, 4, 6, 3 },
    { 1, 5, 2, 7 },
    { 2, 0, 6, 5 },
    { 3, 8, 4, 7 },
    { 4, 0, 8, 8 },
    { 4, 0, 8, 8 },
    { 5, 1, 8, 8 }
};

static int MartData_HandleItemListInput(MartData *data) {
    u8 unkExternVal;
    BOOL unkBool = FALSE;
    if (gSystem.newKeys & PAD_KEY_UP) {
        data->cursorPos = sMartCursorNeighbors[data->cursorPos][0];
        PlaySE(SEQ_SE_DP_SELECT);
        unkBool++;
    } else if (gSystem.newKeys & PAD_KEY_DOWN) {
        data->cursorPos = sMartCursorNeighbors[data->cursorPos][1];
        PlaySE(SEQ_SE_DP_SELECT);
        unkBool++;
    } else if (gSystem.newKeys & PAD_KEY_LEFT) {
        unkExternVal = sMartCursorNeighbors[data->cursorPos][2];
        if (unkExternVal == 6) {
            return MartData_SelectItemListButton(data, 6);
        }
        if (data->cursorPos != 8) {
            data->cursorPos = unkExternVal;
            PlaySE(SEQ_SE_DP_SELECT);
            unkBool++;
        }
    } else if (gSystem.newKeys & PAD_KEY_RIGHT) {
        unkExternVal = sMartCursorNeighbors[data->cursorPos][3];
        if (unkExternVal == 7) {
            return MartData_SelectItemListButton(data, 7);
        }
        if (data->cursorPos != 8) {
            data->cursorPos = unkExternVal;
            PlaySE(SEQ_SE_DP_SELECT);
            unkBool++;
        }
    }

    if (unkBool) {
        MartData_SetCursorPosition(data, data->cursorPos, 0);
        MartData_PrintItemDescription(data, data->cursorPos);
    }
    if (gSystem.newKeys & PAD_BUTTON_B) {
        PlaySE(SEQ_SE_GS_GEARCANCEL);
        return MartData_StartButtonPressAnim(data, 13, 16);
    }
    if (gSystem.newKeys & PAD_BUTTON_A) {
        return MartData_SelectItemListButton(data, data->cursorPos);
    }
    return TASK_MART_3;
}

static u8 MartData_SelectItemListButton(MartData *data, u32 button) {
    switch (button) {
    case 0:
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
        if (button + data->pageStart < data->numItems) {
            data->cursorPos = button;
            MartData_SetCursorPosition(data, data->cursorPos, 0);
            Sprite_SetAnimCtrlSeq(data->sprites[10], 19);
            PlaySE(SEQ_SE_DP_SELECT);
            MartData_PrintItemDescription(data, data->cursorPos);
            return TASK_MART_6;
        }
        break;
    case 6:
        if (data->pageStart) {
            PlaySE(SEQ_SE_DP_SELECT);
            return MartData_StartButtonPressAnim(data, 11, 19);
        }
        break;
    case 7:
        if (data->pageStart + 6 < data->numItems) {
            PlaySE(SEQ_SE_DP_SELECT);
            return MartData_StartButtonPressAnim(data, 12, 20);
        }
        break;
    case 8:
        MartData_SetCursorPosition(data, button, 0);
        Sprite_SetAnimCtrlSeq(data->sprites[10], 7);
        PlaySE(SEQ_SE_GS_GEARCANCEL);
        MartData_PrintItemDescription(data, 8);
        return MartData_StartButtonPressAnim(data, 13, 16);
    }
    return TASK_MART_3;
}

static u8 MartData_PrevPage(MartData *data) {
    data->pageStart -= 6;
    MartData_DrawPageItemIcons(data, data->pageStart, data->numItems);
    data->touchMartAction = 1;
    MartData_PrintItemDescription(data, data->cursorPos);
    return TASK_MART_3;
}

static u8 MartData_NextPage(MartData *data) {
    data->pageStart += 6;
    MartData_DrawPageItemIcons(data, data->pageStart, data->numItems);
    data->touchMartAction = 1;
    MartData_PrintItemDescription(data, data->cursorPos);
    return TASK_MART_3;
}

static void MartData_DrawPageItemIcons(MartData *data, int pageStart, u8 numItems) {
    int temp;
    for (int i = 0; i < 6; i++) {
        if (numItems - pageStart > 6) {
            temp = 6;
        } else if (numItems - pageStart < 0) {
            temp = 0;
        } else {
            temp = numItems - pageStart;
        }

        if (i < temp) {
            MartData_LoadItemIcon(data, i + 3, i + 2, data->itemIds[i + pageStart]);
            Sprite_SetDrawFlag(data->sprites[i + 4], TRUE);
        } else {
            Sprite_SetDrawFlag(data->sprites[i + 4], FALSE);
        }
    };
    MartData_UpdateButtonSprites(data, 0);
}

static u8 MartData_CloseMenu(MartData *data) {
    MartData_RemoveWindows(data);
    BgFillTilemapBufferAndSchedule(data->bgConfig, 1, 0);
    Sprite_SetDrawFlag(data->sprites[3], FALSE);
    return TASK_MART_17;
}

static u32 MartData_GetCurrency(MartData *data, u32 martType) {
    if (martType == MART_TYPE_3 || martType == MART_TYPE_4) {
        return PokeathlonSave_GetAthletePoints(data->pokeathlonSave);
    } else {
        return PlayerProfile_GetMoney(data->playerProfile);
    }
}

int MartData_CheckCannotBuy(MartData *data, u32 unkAmount) {
    if (data->martType == MART_TYPE_3) {
        if (PokeathlonSave_IsDailyItemBought(data->pokeathlonSave, data->cursorPos + data->pageStart)) {
            return 2;
        }
    } else if (data->martType == MART_TYPE_4 && PokeathlonSave_IsDataCardBought(data->pokeathlonSave, data->item - 505)) {
        return 3;
    }

    return unkAmount < data->cost;
}

static u8 MartData_BeginPurchase(MartData *data, u16 itemID) {
    data->spriteDrawn[0] = Sprite_GetDrawFlag(data->sprites[0]);
    data->spriteDrawn[1] = Sprite_GetDrawFlag(data->sprites[1]);
    Sprite_SetDrawFlag(data->sprites[0], FALSE);
    Sprite_SetDrawFlag(data->sprites[1], FALSE);
    MartData_SetSprite2PaletteOverride(data, 1);
    data->item = itemID;
    data->quantity = 1;
    data->cost = MartData_GetItemPrice(data, data->item);
    u32 unkAmount = MartData_GetCurrency(data, data->martType);
    if (MartData_CheckCannotBuy(data, unkAmount)) {
        data->touchMartAction = 10;
        return TASK_MART_14;
    }
    data->maxQuantity = unkAmount / data->cost;
    if (data->maxQuantity > 99) {
        data->maxQuantity = 99;
    }
    if (data->martType == MART_TYPE_1 || data->martType == MART_TYPE_3 || data->martType == MART_TYPE_4) {
        return MartData_CheckInventorySpace(data);
    }
    MartData_UpdateButtonSprites(data, 1);
    data->touchMartAction = 2;
    return TASK_MART_5;
}

static u8 MartData_WaitQuantityPrompt(MartData *data) {
    if (DialogBox_IsPrintFinished(data->printerID) == FALSE) {
        return TASK_MART_5;
    }
    Sprite_SetDrawFlag(data->sprites[0], FALSE);
    Sprite_SetDrawFlag(data->sprites[1], FALSE);
    return TASK_MART_7;
}

int MartData_GetOwnedQuantity(MartData *data, int itemID) {
    if (data->martType == MART_TYPE_NORMAL || data->martType == MART_TYPE_1 || data->martType == MART_TYPE_3 || data->martType == MART_TYPE_4) {
        if (itemID >= ITEM_RED_APRICORN && itemID <= ITEM_BLK_APRICORN) {
            return ApricornBox_CountApricorn(data->apricornBox, itemID - ITEM_RED_APRICORN);
        }
        return Bag_GetQuantity(data->inventory, data->item, HEAP_ID_FIELD2);
    }
    if (data->martType == MART_TYPE_SEAL) {
        return SealCase_CountSealOccurrenceAnywhere(data->inventory, data->item);
    }
    return 0;
}

static u8 MartData_FinishQuantityInput(MartData *data) {
    Sprite_SetDrawFlag(data->sprites[0], FALSE);
    Sprite_SetDrawFlag(data->sprites[1], FALSE);
    EraseFrameAndWindow1(&data->windows[4], TRUE);
    EraseFrameAndWindow1(&data->windows[3], TRUE);
    FillWindowPixelBuffer(&data->windows[5], 15);
    data->touchMartAction = 7;
    Sprite_SetDrawFlag(data->sprites[18], FALSE);
    Sprite_SetDrawFlag(data->sprites[13], FALSE);
    Sprite_SetDrawFlag(data->sprites[14], FALSE);
    Sprite_SetDrawFlag(data->sprites[15], FALSE);
    Sprite_SetDrawFlag(data->sprites[16], FALSE);
    Sprite_SetDrawFlag(data->sprites[17], FALSE);
    return MartData_CheckInventorySpace(data);
}

static u8 MartData_CancelQuantityInput(MartData *data) {
    EraseFrameAndWindow1(&data->windows[4], TRUE);
    EraseFrameAndWindow1(&data->windows[3], TRUE);
    ClearFrameAndWindow2(&data->windows[5], FALSE);
    MartData_SetScrollArrowPositions(data, FALSE);
    Sprite_SetDrawFlag(data->sprites[0], data->spriteDrawn[0]);
    Sprite_SetDrawFlag(data->sprites[1], data->spriteDrawn[1]);
    MartData_SetSprite2PaletteOverride(data, 0);
    MartData_UpdateButtonSprites(data, 0);
    data->touchMartAction = 4;
    Sprite_SetDrawFlag(data->sprites[13], TRUE);
    return TASK_MART_3;
}

static u8 MartData_HandleQuantityInput(MartData *data) {
    if (HandleNumberSelectInput(&data->quantity, data->maxQuantity)) {
        PlaySE(SEQ_SE_DP_BAG_004);
        data->touchMartAction = 6;
        return TASK_MART_7;
    }
    if (PAD_BUTTON_A & gSystem.newKeys) {
        PlaySE(SEQ_SE_DP_SELECT);
        return MartData_StartButtonPressAnim(data, 18, 8);
    }
    if (PAD_BUTTON_B & gSystem.newKeys) {
        PlaySE(SEQ_SE_GS_GEARCANCEL);
        return MartData_StartButtonPressAnim(data, 13, 9);
    }
    return TASK_MART_7;
}

static u8 MartData_HandleQuantityButton(MartData *data, u32 button) {
    switch (button) {
    case 0:
        if (data->maxQuantity >= 10) {
            data->quantity = Mart_StepQuantity(data->quantity, data->maxQuantity, 10);
            Sprite_SetAnimationFrame(data->sprites[14], 0);
            Sprite_SetAnimCtrlSeq(data->sprites[14], 13);
            PlaySE(SEQ_SE_DP_BAG_004);
            data->touchMartAction = 6;
        }
        break;
    case 1:
        if (data->maxQuantity != 1) {
            data->quantity = Mart_StepQuantity(data->quantity, data->maxQuantity, 1);
            Sprite_SetAnimationFrame(data->sprites[15], 0);
            Sprite_SetAnimCtrlSeq(data->sprites[15], 13);
            PlaySE(SEQ_SE_DP_BAG_004);
            data->touchMartAction = 6;
        }
        break;
    case 2:
        if (data->maxQuantity >= 10) {
            data->quantity = Mart_StepQuantity(data->quantity, data->maxQuantity, -10);
            Sprite_SetAnimationFrame(data->sprites[16], 0);
            Sprite_SetAnimCtrlSeq(data->sprites[16], 15);
            PlaySE(SEQ_SE_DP_BAG_004);
            data->touchMartAction = 6;
        }
        break;
    case 3:
        if (data->maxQuantity != 1) {
            data->quantity = Mart_StepQuantity(data->quantity, data->maxQuantity, -1);
            Sprite_SetAnimationFrame(data->sprites[17], 0);
            Sprite_SetAnimCtrlSeq(data->sprites[17], 15);
            PlaySE(SEQ_SE_DP_BAG_004);
            data->touchMartAction = 6;
        }
        break;
    case 4:
        PlaySE(SEQ_SE_DP_SELECT);
        return MartData_StartButtonPressAnim(data, 18, 8);
    case 5:
        PlaySE(SEQ_SE_GS_GEARCANCEL);
        return MartData_StartButtonPressAnim(data, 13, 9);
    }
    return TASK_MART_7;
}

static u8 MartData_CheckInventorySpace(MartData *data) {
    int quantity;
    if (data->martType == MART_TYPE_NORMAL) {
        quantity = Bag_HasSpaceForItem(data->inventory, data->item, data->quantity, HEAP_ID_FIELD2);
    } else if (data->martType == MART_TYPE_1) {
        quantity = 0;
    } else if (data->martType == MART_TYPE_3 || data->martType == MART_TYPE_4) {
        if (data->item >= ITEM_RED_APRICORN && data->item <= ITEM_BLK_APRICORN) {
            if (ApricornBox_CountApricorn(data->apricornBox, data->item - ITEM_RED_APRICORN) == 99) {
                data->touchMartAction = 12;
                return TASK_MART_14;
            }
        } else if (Bag_HasSpaceForItem(data->inventory, data->item, data->quantity, HEAP_ID_FIELD2) == FALSE) {
            data->touchMartAction = 12;
            return TASK_MART_14;
        }
        MartData_UpdateButtonSprites(data, 2);
        data->touchMartAction = 3;
        return TASK_MART_10;
    } else {
        quantity = SealCase_CheckSealQuantity(data->inventory, data->item, data->quantity);
    }

    if (quantity == 0) {
        data->quantity = 0;
        data->touchMartAction = 11;
        return TASK_MART_13;
    }
    return TASK_MART_10;
}

static u8 MartData_WaitConfirmPrompt(MartData *data) {
    if (DialogBox_IsPrintFinished(data->printerID) == FALSE) {
        return TASK_MART_10;
    }
    data->touchMartAction = 8;
    return TASK_MART_11;
}

static u8 MartData_HandleConfirmInput(MartData *data, u32 button) {
    switch (button) {
    case 0:
        data->touchMartAction = 9;
        return TASK_MART_12;
    case 1:
        ClearFrameAndWindow2(&data->windows[5], FALSE);
        MartData_SetScrollArrowPositions(data, FALSE);
        Sprite_SetDrawFlag(data->sprites[0], data->spriteDrawn[0]);
        Sprite_SetDrawFlag(data->sprites[1], data->spriteDrawn[1]);
        MartData_SetSprite2PaletteOverride(data, 0);
        MartData_UpdateButtonSprites(data, 0);
        data->touchMartAction = 4;
        return TASK_MART_3;
    default:
        return TASK_MART_11;
    }
}

static void MartData_SubCurrency(MartData *data, int currency) {
    if (data->martType == MART_TYPE_3 || data->martType == MART_TYPE_4) {
        PokeathlonSave_SubAthletePoints(data->pokeathlonSave, currency);
    } else {
        PlayerProfile_SubMoney(data->playerProfile, data->cost * data->quantity);
    }
}

static void MartData_Inventory_AddApricornOrItem(MartData *data, int item) {
    if (item >= ITEM_RED_APRICORN && item <= ITEM_BLK_APRICORN) {
        ApricornBox_GiveApricorn(data->apricornBox, item - ITEM_RED_APRICORN, 1);
    } else {
        Bag_AddItem(data->inventory, data->item, 1, HEAP_ID_FIELD2);
    }
}

static u8 MartData_PerformTransaction(MartData *data) {
    if (DialogBox_IsPrintFinished(data->printerID) == FALSE) {
        return TASK_MART_12;
    }
    if (data->martType == MART_TYPE_NORMAL) {
        Bag_AddItem(data->inventory, data->item, data->quantity, HEAP_ID_FIELD2);
    } else if (data->martType == MART_TYPE_3) {
        MartData_Inventory_AddApricornOrItem(data, data->item);
        PokeathlonSave_SetDailyItemBought(data->pokeathlonSave, data->cursorPos + data->pageStart);
    } else if (data->martType == MART_TYPE_4) {
        PokeathlonSave_SetDataCardBought(data->pokeathlonSave, data->item - 505);
    } else {
        GiveOrTakeSeal(data->inventory, data->item, data->quantity);
    }
    MartData_SubCurrency(data, data->cost * data->quantity);
    GameStats_Add(data->gameStats, GAME_STAT_CURRENCY_SPENT, data->cost * data->quantity);
    return TASK_MART_13;
}

static u8 MartData_WaitPurchaseMessage(MartData *data) {
    if (DialogBox_IsPrintFinished(data->printerID) == FALSE) {
        return TASK_MART_13;
    }
    if ((PAD_BUTTON_A | PAD_BUTTON_B) & gSystem.newKeys || gSystem.touchNew) {
        if (data->unk264 == 1) {
            Save_VarsFlags_IncrementVar4042(data->varsFlags);
        }
        if (data->martType == MART_TYPE_NORMAL && data->item == ITEM_POKE_BALL && data->quantity >= 10 && Bag_AddItem(data->inventory, ITEM_PREMIER_BALL, 1, HEAP_ID_FIELD2) == TRUE) {
            data->touchMartAction = 13;
            GameStats_Inc(data->gameStats, GAME_STAT_PREMIER_BALLS_EARNED);
            return TASK_MART_15;
        }
        ClearFrameAndWindow2(&data->windows[5], 0);
        MartData_SetScrollArrowPositions(data, FALSE);
        Sprite_SetDrawFlag(data->sprites[0], data->spriteDrawn[0]);
        Sprite_SetDrawFlag(data->sprites[1], data->spriteDrawn[1]);
        MartData_SetSprite2PaletteOverride(data, 0);
        MartData_UpdateButtonSprites(data, 0);
        data->touchMartAction = 4;
        return TASK_MART_4;
    }
    return TASK_MART_13;
}

static u8 MartData_WaitErrorMessage(MartData *data) {
    if (DialogBox_IsPrintFinished(data->printerID) == FALSE) {
        return TASK_MART_14;
    }
    if ((PAD_BUTTON_A | PAD_BUTTON_B) & gSystem.newKeys || gSystem.touchNew) {
        ClearFrameAndWindow2(&data->windows[5], FALSE);
        MartData_SetScrollArrowPositions(data, FALSE);
        Sprite_SetDrawFlag(data->sprites[0], data->spriteDrawn[0]);
        Sprite_SetDrawFlag(data->sprites[1], data->spriteDrawn[1]);
        MartData_SetSprite2PaletteOverride(data, 0);
        MartData_UpdateButtonSprites(data, 0);
        data->touchMartAction = 5;
        return TASK_MART_4;
    }
    return TASK_MART_14;
}

static u8 MartData_WaitPremierBallMessage(MartData *data) {
    if (DialogBox_IsPrintFinished(data->printerID) == FALSE) {
        return TASK_MART_15;
    }
    if ((PAD_BUTTON_A | PAD_BUTTON_B) & gSystem.newKeys || gSystem.touchNew) {
        ClearFrameAndWindow2(&data->windows[5], FALSE);
        MartData_SetScrollArrowPositions(data, FALSE);
        Sprite_SetDrawFlag(data->sprites[0], data->spriteDrawn[0]);
        Sprite_SetDrawFlag(data->sprites[1], data->spriteDrawn[1]);
        MartData_SetSprite2PaletteOverride(data, 0);
        MartData_UpdateButtonSprites(data, 0);
        data->touchMartAction = 4;
        return TASK_MART_3;
    }
    return TASK_MART_15;
}

static u16 Mart_GetPriceOverride(u16 itemID, const struct MartItem *priceOverrides, u8 unk270) {
    for (int i = 0; i < unk270; i++) {
        if (itemID == priceOverrides[i].item_id) {
            return priceOverrides[i].cost;
        }
    }
    GF_AssertFail();
    return 0;
}

u32 MartData_GetItemPrice(MartData *data, u16 itemID) {
    if (data->martType == MART_TYPE_NORMAL) {
        return GetItemAttr(itemID, 0, HEAP_ID_FIELD2);
    } else if (data->martType == MART_TYPE_1) {
        return 100;
    } else if (data->martType == MART_TYPE_3 || data->martType == MART_TYPE_4) {
        return Mart_GetPriceOverride(itemID, data->priceOverrides, data->numItems);
    }
    return 100;
}

static void MartData_RestoreBottomScreenApp(FieldSystem *fieldSystem, MartData *data_unused) {
    FieldSystem_SwitchBottomScreenApp(fieldSystem, fieldSystem->savedBottomScreenAppId, NULL);
}

static u8 MartData_FinishClose(FieldSystem *fieldSystem, MartData *data) {
    if (FieldSystem_GetBottomScreenAppState(fieldSystem) == FALSE && FieldSystem_GetBottomScreenAppId(fieldSystem) != 1) {
        return TASK_MART_18;
    }
    data->unk281 = 0;
    Camera_Copy(data->camera, fieldSystem->camera);
    Camera_Delete(data->camera);
    Camera_SetStaticPtr(fieldSystem->camera);
    MartData_DeleteSprites(data);
    return TASK_MART_27;
}

static const ResdatIdList sMartSpriteResdat = {
    .charRes = NARC_resdat_resdat_00000066_bin,
    .plttRes = NARC_resdat_resdat_00000067_bin,
    .cellRes = NARC_resdat_resdat_00000065_bin,
    .animRes = NARC_resdat_resdat_00000064_bin,
    .mcelRes = 0xFFFF,
    .manmRes = 0xFFFF,
    .headerId = NARC_resdat_resdat_00000088_bin
};

static const UnmanagedSpriteTemplate sMartSpriteTemplates[19] = {
    {
     .resourceSet = 0,
     .x = 177,
     .y = 8,
     .z = 0,
     .animation = 0,
     .drawPriority = 0,
     .pal = 0,
     .vram = NNS_G2D_VRAM_TYPE_2DMAIN,
     .paletteMode = 0,
     },
    {
     .resourceSet = 0,
     .x = 177,
     .y = 132,
     .z = 0,
     .animation = 1,
     .drawPriority = 0,
     .pal = 0,
     .vram = NNS_G2D_VRAM_TYPE_2DMAIN,
     .paletteMode = 0,
     },

    {
     .resourceSet = 1,
     .x = 176,
     .y = 24,
     .z = 0,
     .animation = 0,
     .drawPriority = 0,
     .pal = 0,
     .vram = NNS_G2D_VRAM_TYPE_2DMAIN,
     .paletteMode = 0,
     },
    {
     .resourceSet = 2,
     .x = 22,
     .y = 172,
     .z = 0,
     .animation = 0,
     .drawPriority = 0,
     .pal = 0,
     .vram = NNS_G2D_VRAM_TYPE_2DMAIN,
     .paletteMode = 0,
     },
    {
     .resourceSet = 3,
     .x = 22,
     .y = 59,
     .z = 0,
     .animation = 0,
     .drawPriority = 0,
     .pal = 0,
     .vram = NNS_G2D_VRAM_TYPE_2DSUB,
     .paletteMode = 0,
     },
    {
     .resourceSet = 4,
     .x = 152,
     .y = 59,
     .z = 0,
     .animation = 0,
     .drawPriority = 0,
     .pal = 0,
     .vram = NNS_G2D_VRAM_TYPE_2DSUB,
     .paletteMode = 0,
     },
    {
     .resourceSet = 5,
     .x = 22,
     .y = 100,
     .z = 0,
     .animation = 0,
     .drawPriority = 0,
     .pal = 0,
     .vram = NNS_G2D_VRAM_TYPE_2DSUB,
     .paletteMode = 0,
     },
    {
     .resourceSet = 6,
     .x = 152,
     .y = 100,
     .z = 0,
     .animation = 0,
     .drawPriority = 0,
     .pal = 0,
     .vram = NNS_G2D_VRAM_TYPE_2DSUB,
     .paletteMode = 0,
     },
    {
     .resourceSet = 7,
     .x = 22,
     .y = 139,
     .z = 0,
     .animation = 0,
     .drawPriority = 0,
     .pal = 0,
     .vram = NNS_G2D_VRAM_TYPE_2DSUB,
     .paletteMode = 0,
     },
    {
     .resourceSet = 8,
     .x = 152,
     .y = 139,
     .z = 0,
     .animation = 0,
     .drawPriority = 0,
     .pal = 0,
     .vram = NNS_G2D_VRAM_TYPE_2DSUB,
     .paletteMode = 0,
     },
    {
     .resourceSet = 9,
     .x = 0,
     .y = 0,
     .z = 0,
     .animation = 0,
     .drawPriority = 0,
     .pal = 0,
     .vram = NNS_G2D_VRAM_TYPE_2DSUB,
     .paletteMode = 0,
     },
    {
     .resourceSet = 9,
     .x = 24,
     .y = 176,
     .z = 0,
     .animation = 2,
     .drawPriority = 0,
     .pal = 0,
     .vram = NNS_G2D_VRAM_TYPE_2DSUB,
     .paletteMode = 0,
     },
    {
     .resourceSet = 9,
     .x = 64,
     .y = 176,
     .z = 0,
     .animation = 3,
     .drawPriority = 0,
     .pal = 0,
     .vram = NNS_G2D_VRAM_TYPE_2DSUB,
     .paletteMode = 0,
     },
    {
     .resourceSet = 9,
     .x = 224,
     .y = 176,
     .z = 0,
     .animation = 6,
     .drawPriority = 0,
     .pal = 0,
     .vram = NNS_G2D_VRAM_TYPE_2DSUB,
     .paletteMode = 0,
     },
    {
     .resourceSet = 9,
     .x = 136,
     .y = 104,
     .z = 0,
     .animation = 12,
     .drawPriority = 0,
     .pal = 0,
     .vram = NNS_G2D_VRAM_TYPE_2DSUB,
     .paletteMode = 0,
     },
    {
     .resourceSet = 9,
     .x = 168,
     .y = 104,
     .z = 0,
     .animation = 12,
     .drawPriority = 0,
     .pal = 0,
     .vram = NNS_G2D_VRAM_TYPE_2DSUB,
     .paletteMode = 0,
     },
    {
     .resourceSet = 9,
     .x = 136,
     .y = 152,
     .z = 0,
     .animation = 14,
     .drawPriority = 0,
     .pal = 0,
     .vram = NNS_G2D_VRAM_TYPE_2DSUB,
     .paletteMode = 0,
     },
    {
     .resourceSet = 9,
     .x = 168,
     .y = 152,
     .z = 0,
     .animation = 14,
     .drawPriority = 0,
     .pal = 0,
     .vram = NNS_G2D_VRAM_TYPE_2DSUB,
     .paletteMode = 0,
     },
    {
     .resourceSet = 9,
     .x = 136,
     .y = 176,
     .z = 0,
     .animation = 24,
     .drawPriority = 0,
     .pal = 0,
     .vram = NNS_G2D_VRAM_TYPE_2DSUB,
     .paletteMode = 0,
     }
};

void MartData_CreateSprites(MartData *data) {
    FieldSpriteManager_InitWithResDat(&data->fieldSpriteManager, &sMartSpriteResdat, 19, HEAP_ID_FIELD2);
    for (u32 i = 0; i < 19; i++) {
        data->sprites[i] = FieldSpriteManager_CreateSprite(&data->fieldSpriteManager, &sMartSpriteTemplates[i]);
    }
    Sprite_SetPriority(data->sprites[13], 1);
    Sprite_SetPriority(data->sprites[18], 1);
    Sprite_SetAnimActiveFlag(data->sprites[10], 1);
    Sprite_SetDrawFlag(data->sprites[2], 0);
    Sprite_SetDrawFlag(data->sprites[0], 0);
    Sprite_SetDrawFlag(data->sprites[1], 0);
    Sprite_SetDrawFlag(data->sprites[3], 0);
    GfGfx_EngineATogglePlanes(16, 1);
    GfGfx_EngineBTogglePlanes(16, 1);
    MartData_UpdateButtonSprites(data, 0);
    MartData_SetCursorPosition(data, data->cursorPos, 0);
    MartData_DrawPageItemIcons(data, data->pageStart, data->numItems);
}

static void MartData_DeleteSprites(MartData *data) {
    for (u32 i = 0; i < 19; i++) {
        Sprite_Delete(data->sprites[i]);
        data->sprites[i] = NULL;
    }
    FieldSpriteManager_ReleaseWithResDat(&data->fieldSpriteManager);
    data->fieldSpriteManager.spriteList = NULL;
}

enum OV03_02259850_Data {
    OV03_02259850_SPRITE_INDEX,
    OV03_02259850_DATA_1,
    OV03_02259850_DATA_2,
    OV03_02259850_MAX
};

static u8 ov03_02259850[15][OV03_02259850_MAX] = {
    { 4,  5, 2 },
    { 5,  5, 2 },
    { 6,  5, 2 },
    { 7,  5, 2 },
    { 8,  5, 2 },
    { 9,  5, 2 },
    { 10, 1, 0 },
    { 11, 3, 0 },
    { 12, 4, 0 },
    { 13, 1, 1 },
    { 14, 0, 1 },
    { 15, 0, 1 },
    { 16, 0, 1 },
    { 17, 0, 1 },
    { 18, 0, 1 }
};

static void MartData_UpdateButtonSprites(MartData *data, int arg1) {
    int i;
    int j;
    switch (arg1) {
    case 0:
        for (j = 0; j < 6; j++) {
            VecFx32 vec;
            vec.x = sMartSpriteTemplates[j + 4].x * FX32_ONE;
            vec.y = sMartSpriteTemplates[j + 4].y * FX32_ONE + 0xC0000;
            vec.z = sMartSpriteTemplates[j + 4].z * FX32_ONE;
            Sprite_SetMatrix(data->sprites[j + 4], &vec);
        }
        for (i = 0; i < NELEMS(ov03_02259850); i++) {
            if (ov03_02259850[i][OV03_02259850_DATA_1] == 4) {
                if (data->pageStart + 6 < data->numItems) {
                    Sprite_SetDrawFlag(data->sprites[ov03_02259850[i][OV03_02259850_SPRITE_INDEX]], TRUE);
                } else {
                    Sprite_SetDrawFlag(data->sprites[ov03_02259850[i][OV03_02259850_SPRITE_INDEX]], FALSE);
                }
            } else if (ov03_02259850[i][OV03_02259850_DATA_1] == 3) {
                if (data->pageStart == 0) {
                    Sprite_SetDrawFlag(data->sprites[ov03_02259850[i][OV03_02259850_SPRITE_INDEX]], FALSE);
                } else {
                    Sprite_SetDrawFlag(data->sprites[ov03_02259850[i][OV03_02259850_SPRITE_INDEX]], TRUE);
                }
            } else if (ov03_02259850[i][OV03_02259850_DATA_1] == 5) {
                if (data->pageStart + i < data->numItems) {
                    Sprite_SetDrawFlag(data->sprites[ov03_02259850[i][OV03_02259850_SPRITE_INDEX]], TRUE);
                } else {
                    Sprite_SetDrawFlag(data->sprites[ov03_02259850[i][OV03_02259850_SPRITE_INDEX]], FALSE);
                }
            } else {
                Sprite_SetDrawFlag(data->sprites[ov03_02259850[i][OV03_02259850_SPRITE_INDEX]], ov03_02259850[i][OV03_02259850_DATA_1]);
            }
        }
        Sprite_SetAnimationFrame(data->sprites[13], 0);
        Sprite_SetAnimCtrlSeq(data->sprites[13], 6);
        break;
    case 1:
        for (i = 0; i < NELEMS(ov03_02259850); i++) {
            if (ov03_02259850[i][OV03_02259850_DATA_2] == 2) {
                if (data->cursorPos == i) {
                    VecFx32 vec2;
                    vec2.x = 0x56000;
                    vec2.y = 0x10C000;
                    vec2.z = 0;
                    Sprite_SetMatrix(data->sprites[ov03_02259850[i][OV03_02259850_SPRITE_INDEX]], &vec2);
                    Sprite_SetDrawFlag(data->sprites[ov03_02259850[i][OV03_02259850_SPRITE_INDEX]], TRUE);
                } else {
                    Sprite_SetDrawFlag(data->sprites[ov03_02259850[i][OV03_02259850_SPRITE_INDEX]], FALSE);
                }
            } else {
                if (ov03_02259850[i][OV03_02259850_SPRITE_INDEX] == 14 || ov03_02259850[i][OV03_02259850_SPRITE_INDEX] == 16) {
                    if (data->maxQuantity < 10) {
                        Sprite_SetDrawFlag(data->sprites[ov03_02259850[i][OV03_02259850_SPRITE_INDEX]], FALSE);
                    } else {
                        Sprite_SetDrawFlag(data->sprites[ov03_02259850[i][OV03_02259850_SPRITE_INDEX]], TRUE);
                    }
                } else if (ov03_02259850[i][OV03_02259850_SPRITE_INDEX] == 15 || ov03_02259850[i][OV03_02259850_SPRITE_INDEX] == 17) {
                    if (data->maxQuantity == 1) {
                        Sprite_SetDrawFlag(data->sprites[ov03_02259850[i][OV03_02259850_SPRITE_INDEX]], FALSE);
                    } else {
                        Sprite_SetDrawFlag(data->sprites[ov03_02259850[i][OV03_02259850_SPRITE_INDEX]], TRUE);
                    }
                } else {
                    Sprite_SetDrawFlag(data->sprites[ov03_02259850[i][OV03_02259850_SPRITE_INDEX]], ov03_02259850[i][OV03_02259850_DATA_2]);
                }
            }
        }
        Sprite_SetAnimationFrame(data->sprites[13], 0);
        Sprite_SetAnimCtrlSeq(data->sprites[13], 26);
        break;
    case 2:
        for (i = 0; i < NELEMS(ov03_02259850); i++) {
            if (data->cursorPos == i) {
                VecFx32 vec3;
                vec3.x = 0x56000;
                vec3.y = 0x10C000;
                vec3.z = 0;
                Sprite_SetMatrix(data->sprites[ov03_02259850[i][OV03_02259850_SPRITE_INDEX]], &vec3);
                Sprite_SetDrawFlag(data->sprites[ov03_02259850[i][OV03_02259850_SPRITE_INDEX]], TRUE);
            } else {
                Sprite_SetDrawFlag(data->sprites[ov03_02259850[i][OV03_02259850_SPRITE_INDEX]], FALSE);
            }
        }
        break;
    }
}

static void MartData_SetScrollArrowPositions(MartData *data, BOOL arg1) {
    if (arg1 == FALSE) {
        Sprite_SetPositionXY(data->sprites[0], 177, 8);
        Sprite_SetPositionXY(data->sprites[1], 177, 132);
    } else { // This is never used.
        Sprite_SetPositionXY(data->sprites[0], 162, 108);
        Sprite_SetPositionXY(data->sprites[1], 162, 132);
    }
}

static void MartData_SetSelectedItemIcon(MartData *data, u16 itemID) {
    SpriteResource *spriteResource;
    if (data->martType != 0 && data->martType != 3 && data->martType != 4) {
        Sprite_SetDrawFlag(data->sprites[3], FALSE);
    } else {
        spriteResource = SpriteResourceCollection_Find(data->fieldSpriteManager.spriteResManagers[0], 2);
        ReplaceCharResObjFromNarc(data->fieldSpriteManager.spriteResManagers[0], spriteResource, NARC_itemtool_itemdata_item_icon, GetItemIndexMapping(itemID, 1), FALSE, HEAP_ID_FIELD2);
        SpriteTransfer_ReplaceCharData(spriteResource);
        spriteResource = SpriteResourceCollection_Find(data->fieldSpriteManager.spriteResManagers[1], 1);
        ReplacePlttResObjFromNarc(data->fieldSpriteManager.spriteResManagers[1], spriteResource, NARC_itemtool_itemdata_item_icon, GetItemIndexMapping(itemID, 2), FALSE, HEAP_ID_FIELD2);
        SpriteTransfer_ReplacePlttData(spriteResource);
        Sprite_SetDrawFlag(data->sprites[3], TRUE);
    }
}

static void MartData_LoadItemIcon(MartData *data, int charID, int paletteID, u16 item) {
    SpriteResource *charResObj = SpriteResourceCollection_Find(data->fieldSpriteManager.spriteResManagers[0], charID);
    ReplaceCharResObjFromNarc(data->fieldSpriteManager.spriteResManagers[0], charResObj, NARC_itemtool_itemdata_item_icon, GetItemIndexMapping(item, 1), FALSE, HEAP_ID_FIELD2);
    SpriteTransfer_ReplaceCharData(charResObj);
    SpriteResource *plttResObj = SpriteResourceCollection_Find(data->fieldSpriteManager.spriteResManagers[1], paletteID);
    ReplacePlttResObjFromNarc(data->fieldSpriteManager.spriteResManagers[1], plttResObj, NARC_itemtool_itemdata_item_icon, GetItemIndexMapping(item, 2), FALSE, HEAP_ID_FIELD2);
    SpriteTransfer_ReplacePlttData(plttResObj);
}

static void MartData_SetSprite2PaletteOverride(MartData *data, int flag) {
    thunk_Sprite_SetPaletteOverride(data->sprites[2], flag);
}

static u8 MartData_StartButtonPressAnim(MartData *data, u8 arg1, u8 arg2) {
    MartButtonPressAnim *mini = &data->buttonPressAnim;
    mini->unk0[0] = arg1;
    mini->unk0[1] = 0;
    mini->unk0[2] = 0;
    mini->unk0[3] = arg2;
    return TASK_MART_21;
}

static u8 MartData_AnimateButtonPress(MartData *data) {
    MartButtonPressAnim *mini = &data->buttonPressAnim;
    switch (mini->unk0[1]) {
    case 0:
        thunk_Sprite_SetPaletteOverride(data->sprites[data->buttonPressAnim.unk0[0]], 7);
        mini->unk0[1]++;
        break;
    case 1:
        mini->unk0[2]++;
        if (mini->unk0[2] == 4) {
            thunk_Sprite_SetPaletteOverride(data->sprites[data->buttonPressAnim.unk0[0]], 6);
            mini->unk0[2] = 0;
            mini->unk0[1]++;
        }
        break;
    case 2:
        mini->unk0[2]++;
        if (mini->unk0[2] == 2) {
            return mini->unk0[3];
        }
        break;
    }
    return TASK_MART_21;
}

static void Mart_FadeOut(FieldSystem *fieldSystem_unused, MartData *data) {
    FieldMap_FadeScreen(FADE_TYPE_BRIGHTNESS_OUT);
    data->state = TASK_MART_23;
}

static const u8 sMartSellBagPockets[9] = {
    0,
    1,
    2,
    3,
    4,
    5,
    6,
    7,
    0xFF
};

static void Task_Mart_LaunchSellBag(TaskManager *taskManager) {
    if (IsPaletteFadeFinished()) {
        FieldSystem *fieldSystem = TaskManager_GetFieldSystem(taskManager);
        MartData *data = TaskManager_GetEnvironment(taskManager);
        data->bagView = Bag_CreateView(Save_Bag_Get(fieldSystem->saveData), sMartSellBagPockets, HEAP_ID_FIELD2);
        BagView_Init(data->bagView, fieldSystem->saveData, 2, fieldSystem->bagCursor, &fieldSystem->menuInputState);
        Bag_LaunchApp(fieldSystem, data->bagView);
        TaskManager_Jump(taskManager, Task_MartBag, data);
        data->state = TASK_MART_24;
    }
}

static u8 MartData_WaitFade(FieldSystem *fieldSystem_unused, MartData *data_unused) {
    return IsPaletteFadeFinished() == FALSE ? (u8)TASK_MART_26 : (u8)TASK_MART_27;
}

static int Mart_StepQuantity(s16 currentQuantity, u16 maxQuantity, s16 modifier) {
    int ret;
    if (modifier > 0) {
        if (currentQuantity == maxQuantity) {
            return 1;
        }
        ret = currentQuantity + modifier;
        if (ret > maxQuantity) {
            return maxQuantity;
        }
    } else if (currentQuantity == 1) {
        return maxQuantity;
    } else {
        ret = currentQuantity + modifier;
        if (ret <= 0) {
            ret = 1;
        }
    }
    return ret;
}
