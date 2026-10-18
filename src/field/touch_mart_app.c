#include "field/touch_mart_app.h"

#include <nitro/gx/gx_vramcnt.h>

#include "global.h"

#include "msgdata/msg.naix"
#include "msgdata/msg/msg_0435.h"
#include "pokeathlon/pokeathlon_save.h"

#include "bg_window.h"
#include "filesystem_files_def.h"
#include "font.h"
#include "gf_gfx_loader.h"
#include "heap.h"
#include "item.h"
#include "menu_input_state.h"
#include "message_format.h"
#include "msgdata.h"
#include "options.h"
#include "overlay_03.h"
#include "player_data.h"
#include "render_text.h"
#include "render_window.h"
#include "screen_fade.h"
#include "sound.h"
#include "sound_playback.h"
#include "sys_task.h"
#include "systask_environment.h"
#include "text.h"
#include "touchscreen.h"
#include "yes_no_prompt.h"

// Bottom-screen counterpart of the field mart: the item list, the
// quantity/price panel and the purchase messages, driven by the MartData of the shop task.

enum TouchMartAppWindow {
    TOUCH_MART_WINDOW_ITEM_0,
    TOUCH_MART_WINDOW_ITEM_1,
    TOUCH_MART_WINDOW_ITEM_2,
    TOUCH_MART_WINDOW_ITEM_3,
    TOUCH_MART_WINDOW_ITEM_4,
    TOUCH_MART_WINDOW_ITEM_5,
    TOUCH_MART_WINDOW_SELECTED_ITEM,
    TOUCH_MART_WINDOW_QUANTITY_TENS,
    TOUCH_MART_WINDOW_QUANTITY_ONES,
    TOUCH_MART_WINDOW_BUY,
    TOUCH_MART_WINDOW_IN_BAG,
    TOUCH_MART_WINDOW_TOTAL_PRICE,
    TOUCH_MART_WINDOW_PROMPT,

    TOUCH_MART_WINDOW_COUNT,
};

enum TouchMartAppState {
    TOUCH_MART_STATE_WAIT_FADE,
    TOUCH_MART_STATE_RUN,
};

// MartData.touchMartAction, set by the mart task for the touch screen
enum TouchMartAction {
    TOUCH_MART_ACTION_NONE,
    TOUCH_MART_ACTION_REFRESH_LIST,
    TOUCH_MART_ACTION_SHOW_QUANTITY,
    TOUCH_MART_ACTION_SHOW_CONFIRM,
    TOUCH_MART_ACTION_SHOW_LIST,
    TOUCH_MART_ACTION_REDRAW_LIST,
    TOUCH_MART_ACTION_UPDATE_QUANTITY,
    TOUCH_MART_ACTION_CONFIRM_PURCHASE,
    TOUCH_MART_ACTION_YES_NO,
    TOUCH_MART_ACTION_PURCHASE_DONE,
    TOUCH_MART_ACTION_CANNOT_AFFORD,
    TOUCH_MART_ACTION_NO_ROOM,
    TOUCH_MART_ACTION_NO_ROOM_IN_BAG,
    TOUCH_MART_ACTION_PREMIER_BALL,
};

enum TouchMartScreen {
    TOUCH_MART_SCREEN_LIST,
    TOUCH_MART_SCREEN_QUANTITY,
    TOUCH_MART_SCREEN_CONFIRM,
};

enum TouchMartButtonSet {
    TOUCH_MART_BUTTONS_ITEM_LIST,
    TOUCH_MART_BUTTONS_QUANTITY,
};

#define TOUCH_MART_ITEMS_PER_PAGE 6

typedef struct TouchMartAppData {
    u32 unk_00;
    BgConfig *bgConfig;
    SysTask *task;
    u32 state;
    u32 unk_10;
    MartData *martData;
    void *lastTouchMenuInput;
    FieldSystem *fieldSystem;
    u8 unk_20[0x10];
    u32 unk_30;
    Window unk_34;
    Window messageWindow;
    Window pageWindow;
    Window cancelWindow;
    Window moneyWindow;
    Window windows[TOUCH_MART_WINDOW_COUNT];
    MessageFormat *messageFormat;
    MsgData *msgData;
    MsgData *itemNameMsgData;
    u32 unk_160;
    Options *options;
    PlayerProfile *playerProfile;
    PokeathlonSave *pokeathlonSave;
    YesNoPrompt *yesNoPrompt;
    u8 unk_174[0x10];
    u32 printerId;
    String *string;
    u32 unk_18C;
} TouchMartAppData; // size: 0x190

typedef struct TouchMartWindowPos {
    u32 x;
    u32 y;
    u32 baseTile;
} TouchMartWindowPos;

// One tilemap edit of the item list background: kind 1 copies a rect, kind 2 fills it.
typedef struct TouchMartBgOp {
    u8 kind;
    u8 srcX;
    u8 srcY;
    u8 destX;
    u8 destY;
    u8 width;
    u8 height;
    u8 padding;
} TouchMartBgOp;

static void TouchMartApp_SetupText(TouchMartAppData *data);
static void TouchMartApp_FreeText(TouchMartAppData *data);
static void TouchMartApp_PrintCancel(TouchMartAppData *data, int leftAligned);
static BOOL TouchMartApp_IsSelectionValid(TouchMartAppData *data, u32 selection);
static void TouchMartApp_RunTask(SysTask *task, TouchMartAppData *data);
static void TouchMartApp_HandleAction(TouchMartAppData *data, int action);
static void TouchMartApp_LoadMainTilemap(TouchMartAppData *data, int screen);
static int TouchMartApp_GetTouchedButton(TouchMartAppData *data, int buttonSet);
static void TouchMartApp_SetupBgs(TouchMartAppData *data);
static void TouchMartApp_LoadGraphics(TouchMartAppData *data);
static void TouchMartApp_FreeWindows(TouchMartAppData *data);
static void TouchMartApp_AddWindows(TouchMartAppData *data);
static void TouchMartApp_AddItemWindows(TouchMartAppData *data);
static void TouchMartApp_RemoveItemWindows(TouchMartAppData *data);
static void TouchMartApp_PrintItemList(TouchMartAppData *data);
static void TouchMartApp_PrintItemName(TouchMartAppData *data, Window *window, String *string, int slot);
static void TouchMartApp_PrintPrice(MessageFormat *messageFormat, MsgData *msgData, Window *window, s32 price, int martType);
static void TouchMartApp_PrintMoney(TouchMartAppData *data);
static void TouchMartApp_PrintPageNumber(TouchMartAppData *data);
static void TouchMartApp_EditListTilemap(BgConfig *bgConfig, int bgId, int itemCount);
static void TouchMartApp_LoadListTilemap(TouchMartAppData *data, int itemCount);
static BOOL TouchMartApp_IsItemPurchasable(MartData *martData, int index, int itemId);
static void TouchMartApp_PrintSelectedItem(TouchMartAppData *data);
static void TouchMartApp_PrintInBag(TouchMartAppData *data, int unused);
static void TouchMartApp_PrintQuantity(TouchMartAppData *data, int quantity);
static void TouchMartApp_PrintBuyButton(TouchMartAppData *data);
static void TouchMartApp_BufferItemName(int martType, MessageFormat *messageFormat, int itemId, int fieldno);
static void TouchMartApp_BufferItemNamePlural(int martType, MessageFormat *messageFormat, int itemId, int fieldno);
static void TouchMartApp_BufferItemNameWithIndefArticle(int martType, MessageFormat *messageFormat, int itemId, int fieldno);
static void TouchMartApp_PrintHowMany(TouchMartAppData *data);
static void TouchMartApp_PrintConfirmPurchase(TouchMartAppData *data);
static void TouchMartApp_CreateYesNoPrompt(TouchMartAppData *data);
static BOOL TouchMartApp_HandleYesNoPrompt(TouchMartAppData *data);
static void TouchMartApp_PrintPurchaseDone(TouchMartAppData *data);
static BOOL TouchMartApp_PurchaseDonePrinterCallback(TextPrinterTemplate *template, u16 glyphId);
static String *TouchMartApp_GetCannotAffordMessage(MartData *martData, MsgData *msgData);
static String *TouchMartApp_GetNoRoomMessage(const MartData *martData, MsgData *msgData);
static void TouchMartApp_PrintCannotAfford(TouchMartAppData *data);
static void TouchMartApp_PrintNoRoom(TouchMartAppData *data);
static void TouchMartApp_PrintNoRoomInBag(TouchMartAppData *data);
static void TouchMartApp_PrintPremierBallBonus(TouchMartAppData *data);
static void TouchMartApp_AddDetailWindows(TouchMartAppData *data);
static void TouchMartApp_RemoveDetailWindows(TouchMartAppData *data);

static const TouchscreenHitbox sTouchMartTextSkipHitbox[] = {
    { .rect = { 0, 191, 0, 255 } },
};

static const TouchscreenHitbox sTouchMartQuantityButtons[] = {
    { .rect = { 88, 111, 120, 151 } },
    { .rect = { 88, 111, 152, 183 } },
    { .rect = { 136, 159, 120, 151 } },
    { .rect = { 136, 159, 152, 183 } },
    { .rect = { 168, 191, 96, 173 } },
    { .rect = { 168, 191, 178, 255 } },
    { .rect = { TOUCHSCREEN_RECTLIST_END } },
};

static const TouchscreenHitbox sTouchMartItemListButtons[] = {
    { .rect = { 32, 73, 0, 127 } },
    { .rect = { 32, 73, 128, 255 } },
    { .rect = { 74, 117, 0, 127 } },
    { .rect = { 74, 117, 128, 255 } },
    { .rect = { 118, 153, 0, 127 } },
    { .rect = { 118, 153, 128, 255 } },
    { .rect = { 168, 191, 0, 39 } },
    { .rect = { 168, 191, 40, 79 } },
    { .rect = { 168, 191, 192, 255 } },
    { .rect = { TOUCHSCREEN_RECTLIST_END } },
};

static const TouchMartWindowPos sTouchMartItemWindowPositions[TOUCH_MART_ITEMS_PER_PAGE] = {
    { 4,  5,  173 },
    { 20, 5,  217 },
    { 4,  10, 261 },
    { 20, 10, 305 },
    { 4,  15, 349 },
    { 20, 15, 393 },
};

static const BgTemplate sTouchMartBgTemplates[3] = {
    {
     .x = 0,
     .y = 0,
     .bufferSize = GF_BG_BUF_SIZE_256x256_4BPP,
     .baseTile = 0,
     .size = GF_BG_SCR_SIZE_256x256,
     .colorMode = GX_BG_COLORMODE_16,
     .screenBase = GX_BG_SCRBASE_0x6800,
     .charBase = GX_BG_CHARBASE_0x00000,
     .bgExtPltt = GX_BG_EXTPLTT_01,
     .priority = 0,
     .areaOver = GX_BG_AREAOVER_XLU,
     .mosaic = 0,
     },
    {
     .x = 0,
     .y = 0,
     .bufferSize = GF_BG_BUF_SIZE_256x256_4BPP,
     .baseTile = 0,
     .size = GF_BG_SCR_SIZE_256x256,
     .colorMode = GX_BG_COLORMODE_16,
     .screenBase = GX_BG_SCRBASE_0x7000,
     .charBase = GX_BG_CHARBASE_0x04000,
     .bgExtPltt = GX_BG_EXTPLTT_01,
     .priority = 1,
     .areaOver = GX_BG_AREAOVER_XLU,
     .mosaic = 0,
     },
    {
     .x = 0,
     .y = 0,
     .bufferSize = GF_BG_BUF_SIZE_256x256_4BPP,
     .baseTile = 0,
     .size = GF_BG_SCR_SIZE_256x256,
     .colorMode = GX_BG_COLORMODE_16,
     .screenBase = GX_BG_SCRBASE_0x7800,
     .charBase = GX_BG_CHARBASE_0x04000,
     .bgExtPltt = GX_BG_EXTPLTT_01,
     .priority = 2,
     .areaOver = GX_BG_AREAOVER_XLU,
     .mosaic = 0,
     },
};

static const TouchscreenHitbox *sTouchMartButtonSets[] = {
    sTouchMartItemListButtons,
    sTouchMartQuantityButtons,
};

// Indexed by the item count of the page; a full page needs no edits.
static TouchMartBgOp sTouchMartListTilemapEdits[TOUCH_MART_ITEMS_PER_PAGE][4] = {
    {
     { 0 },
     { 2, 0, 0, 0, 4, 32, 16 },
     { 0 },
     { 0, 0, 0, 16, 4, 16, 16 },
     },
    {
     { 1, 0, 19, 0, 9, 16, 1 },
     { 2, 0, 0, 0, 10, 16, 10 },
     { 0 },
     { 2, 0, 0, 16, 4, 16, 16 },
     },
    {
     { 1, 0, 19, 0, 9, 16, 1 },
     { 0 },
     { 1, 16, 19, 16, 9, 16, 1 },
     { 2, 0, 0, 0, 10, 32, 10 },
     },
    {
     { 1, 0, 19, 0, 14, 16, 1 },
     { 2, 0, 0, 0, 15, 16, 5 },
     { 1, 16, 19, 16, 9, 16, 1 },
     { 2, 0, 0, 16, 10, 16, 10 },
     },
    {
     { 1, 0, 19, 0, 14, 16, 1 },
     { 0 },
     { 1, 16, 19, 16, 14, 16, 1 },
     { 2, 0, 0, 0, 15, 32, 5 },
     },
    {
     { 0 },
     { 0 },
     { 1, 16, 19, 16, 14, 16, 1 },
     { 2, 0, 0, 16, 15, 16, 5 },
     },
};

SysTask *TouchMartApp_Init(BgConfig *bgConfig, void *lastTouchMenuInput, FieldSystem *fieldSystem, MartData *martData) {
    SysTask *task;
    TouchMartAppData *data;

    Heap_Create(HEAP_ID_3, HEAP_ID_8, 3 << 15);
    task = CreateSysTaskAndEnvironment((SysTaskFunc)TouchMartApp_RunTask, sizeof(TouchMartAppData), 10, HEAP_ID_8);
    data = SysTask_GetData(task);
    data->task = task;
    data->unk_00 = 0;
    data->bgConfig = bgConfig;
    data->lastTouchMenuInput = lastTouchMenuInput;
    data->fieldSystem = fieldSystem;
    data->unk_30 = 0;
    data->options = Save_PlayerData_GetOptionsAddr(data->fieldSystem->saveData);
    data->playerProfile = Save_PlayerData_GetProfile(data->fieldSystem->saveData);
    data->pokeathlonSave = Save_Pokeathlon_Get(data->fieldSystem->saveData);
    data->state = TOUCH_MART_STATE_WAIT_FADE;
    data->martData = martData;
    TouchMartApp_SetupBgs(data);
    TouchMartApp_LoadGraphics(data);
    TouchMartApp_SetupText(data);
    TouchMartApp_PrintCancel(data, FALSE);
    TouchMartApp_PrintMoney(data);
    TouchMartApp_PrintPageNumber(data);
    TouchMartApp_PrintItemList(data);
    MartData_CreateSprites(data->martData);
    reg_GXS_DB_DISPCNT &= 0xFFFF1FFF;
    GfGfx_EngineBTogglePlanes(GX_PLANEMASK_BG0, GF_PLANE_TOGGLE_ON);
    GfGfx_EngineBTogglePlanes(GX_PLANEMASK_BG1, GF_PLANE_TOGGLE_ON);
    GfGfx_EngineBTogglePlanes(GX_PLANEMASK_BG2, GF_PLANE_TOGGLE_ON);
    GfGfx_EngineBTogglePlanes(GX_PLANEMASK_OBJ, GF_PLANE_TOGGLE_ON);
    TextFlags_SetCanTouchSpeedUpPrint(TRUE);
    TextFlags_SetFastForwardTouchButtonHitbox(sTouchMartTextSkipHitbox);
    return task;
}

static void TouchMartApp_SetupText(TouchMartAppData *data) {
    data->messageFormat = MessageFormat_New_Custom(8, 64, HEAP_ID_8);
    data->msgData = NewMsgDataFromNarc(MSGDATA_LOAD_DIRECT, NARC_msgdata_msg, NARC_msg_msg_0435_bin, HEAP_ID_8);
    data->itemNameMsgData = NewMsgDataFromNarc(MSGDATA_LOAD_DIRECT, NARC_msgdata_msg, NARC_msg_msg_0222_bin, HEAP_ID_8);
    data->string = String_New(144, HEAP_ID_8);
}

static void TouchMartApp_FreeText(TouchMartAppData *data) {
    String_Delete(data->string);
    DestroyMsgData(data->itemNameMsgData);
    DestroyMsgData(data->msgData);
    MessageFormat_Delete(data->messageFormat);
}

static void TouchMartApp_PrintCancel(TouchMartAppData *data, int leftAligned) {
    String *string = NewString_ReadMsgData(data->msgData, msg_0435_00017);
    ClearWindowTilemapAndCopyToVram(&data->cancelWindow);
    FillWindowPixelBuffer(&data->cancelWindow, 0);
    if (leftAligned == FALSE) {
        u32 width = FontID_String_GetWidth(0, string, 0);
        AddTextPrinterParameterizedWithColor(&data->cancelWindow, 0, string, ((48 - width) >> 1) + 8, 0, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(15, 14, 0), NULL);
    } else {
        AddTextPrinterParameterizedWithColor(&data->cancelWindow, 0, string, 5, 0, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(15, 14, 0), NULL);
    }
    ScheduleWindowCopyToVram(&data->cancelWindow);
    String_Delete(string);
}

void TouchMartApp_Exit(BgConfig *bgConfig, SysTask *task) {
    TouchMartAppData *data = SysTask_GetData(task);
    TextFlags_SetCanTouchSpeedUpPrint(FALSE);
    TouchMartApp_FreeWindows(data);
    TouchMartApp_FreeText(data);
    DestroySysTaskAndEnvironment(task);
    FreeBgTilemapBuffer(bgConfig, GF_BG_LYR_SUB_2);
    FreeBgTilemapBuffer(bgConfig, GF_BG_LYR_SUB_1);
    FreeBgTilemapBuffer(bgConfig, GF_BG_LYR_SUB_0);
    Heap_Destroy(HEAP_ID_8);
}

BOOL TouchMartApp_IsExitDone(void) {
    return TRUE;
}

static BOOL TouchMartApp_IsSelectionValid(TouchMartAppData *data, u32 selection) {
    switch (selection) {
    case 0:
    case 1:
    case 2:
    case 3:
    case 4:
    case 5: {
        MartData *martData = data->martData;
        if (selection + martData->pageStart < martData->numItems) {
            return TRUE;
        }
        break;
    }
    case 6:
    case 7:
    case 8:
        return TRUE;
    }
    return FALSE;
}

static void TouchMartApp_RunTask(SysTask *task, TouchMartAppData *data) {
    int selection;
    switch (data->state) {
    case TOUCH_MART_STATE_WAIT_FADE:
        if (IsPaletteFadeFinished() == TRUE) {
            data->state = TOUCH_MART_STATE_RUN;
        }
        return;
    case TOUCH_MART_STATE_RUN:
        switch (data->martData->state) {
        case 3:
            selection = TouchMartApp_GetTouchedButton(data, TOUCH_MART_BUTTONS_ITEM_LIST);
            if (selection != -1 && TouchMartApp_IsSelectionValid(data, selection)) {
                data->martData->touchButton = selection;
            }
            break;
        case 7:
            selection = TouchMartApp_GetTouchedButton(data, TOUCH_MART_BUTTONS_QUANTITY);
            if (selection != -1) {
                data->martData->touchButton = selection;
            }
            break;
        case 11:
            TouchMartApp_HandleYesNoPrompt(data);
            break;
        }
        if (data->martData->touchMartAction != TOUCH_MART_ACTION_NONE) {
            TouchMartApp_HandleAction(data, data->martData->touchMartAction);
            data->martData->touchMartAction = TOUCH_MART_ACTION_NONE;
        }
        return;
    }
}

static void TouchMartApp_HandleAction(TouchMartAppData *data, int action) {
    switch (action) {
    case TOUCH_MART_ACTION_NONE:
        break;
    case TOUCH_MART_ACTION_REFRESH_LIST:
        TouchMartApp_PrintItemList(data);
        TouchMartApp_PrintPageNumber(data);
        TouchMartApp_PrintCancel(data, FALSE);
        break;
    case TOUCH_MART_ACTION_SHOW_QUANTITY:
        TouchMartApp_LoadMainTilemap(data, TOUCH_MART_SCREEN_QUANTITY);
        TouchMartApp_RemoveItemWindows(data);
        ClearWindowTilemapAndScheduleTransfer(&data->pageWindow);
        TouchMartApp_AddDetailWindows(data);
        TouchMartApp_PrintSelectedItem(data);
        TouchMartApp_PrintInBag(data, data->martData->item);
        TouchMartApp_PrintQuantity(data, data->martData->quantity);
        TouchMartApp_PrintBuyButton(data);
        TouchMartApp_PrintCancel(data, TRUE);
        TouchMartApp_PrintHowMany(data);
        break;
    case TOUCH_MART_ACTION_SHOW_CONFIRM:
        TouchMartApp_LoadMainTilemap(data, TOUCH_MART_SCREEN_CONFIRM);
        TouchMartApp_RemoveItemWindows(data);
        TouchMartApp_AddDetailWindows(data);
        TouchMartApp_PrintSelectedItem(data);
        TouchMartApp_PrintInBag(data, data->martData->item);
        TouchMartApp_PrintConfirmPurchase(data);
        ClearWindowTilemapAndScheduleTransfer(&data->pageWindow);
        ClearWindowTilemapAndScheduleTransfer(&data->cancelWindow);
        break;
    case TOUCH_MART_ACTION_SHOW_LIST:
        TouchMartApp_LoadMainTilemap(data, TOUCH_MART_SCREEN_LIST);
        TouchMartApp_RemoveDetailWindows(data);
        TouchMartApp_PrintPageNumber(data);
        TouchMartApp_PrintCancel(data, FALSE);
        TouchMartApp_AddItemWindows(data);
        ClearFrameAndWindow2(&data->messageWindow, TRUE);
        TouchMartApp_PrintItemList(data);
        TouchMartApp_PrintMoney(data);
        ScheduleWindowCopyToVram(&data->cancelWindow);
        break;
    case TOUCH_MART_ACTION_REDRAW_LIST:
        TouchMartApp_LoadMainTilemap(data, TOUCH_MART_SCREEN_LIST);
        TouchMartApp_PrintPageNumber(data);
        ClearFrameAndWindow2(&data->messageWindow, FALSE);
        TouchMartApp_PrintItemList(data);
        TouchMartApp_PrintMoney(data);
        break;
    case TOUCH_MART_ACTION_UPDATE_QUANTITY:
        TouchMartApp_PrintQuantity(data, data->martData->quantity);
        break;
    case TOUCH_MART_ACTION_CONFIRM_PURCHASE:
        ClearWindowTilemapAndScheduleTransfer(&data->cancelWindow);
        ClearWindowTilemapAndScheduleTransfer(&data->windows[TOUCH_MART_WINDOW_BUY]);
        TouchMartApp_PrintConfirmPurchase(data);
        break;
    case TOUCH_MART_ACTION_YES_NO:
        TouchMartApp_CreateYesNoPrompt(data);
        break;
    case TOUCH_MART_ACTION_PURCHASE_DONE:
        TouchMartApp_PrintPurchaseDone(data);
        break;
    case TOUCH_MART_ACTION_CANNOT_AFFORD:
        TouchMartApp_PrintCannotAfford(data);
        break;
    case TOUCH_MART_ACTION_NO_ROOM:
        ClearWindowTilemapAndScheduleTransfer(&data->cancelWindow);
        ClearWindowTilemapAndScheduleTransfer(&data->windows[TOUCH_MART_WINDOW_BUY]);
        TouchMartApp_PrintNoRoom(data);
        break;
    case TOUCH_MART_ACTION_NO_ROOM_IN_BAG:
        TouchMartApp_PrintNoRoomInBag(data);
        break;
    case TOUCH_MART_ACTION_PREMIER_BALL:
        TouchMartApp_PrintPremierBallBonus(data);
        break;
    }
}

static void TouchMartApp_LoadMainTilemap(TouchMartAppData *data, int screen) {
    void *raw;
    NNSG2dScreenData *screenData;
    switch (screen) {
    case TOUCH_MART_SCREEN_LIST:
        raw = GfGfxLoader_GetScrnData(NARC_a_0_6_0, 17, FALSE, &screenData, HEAP_ID_8);
        BG_LoadScreenTilemapData(data->bgConfig, GF_BG_LYR_SUB_2, screenData->rawData, screenData->szByte);
        ScheduleBgTilemapBufferTransfer(data->bgConfig, GF_BG_LYR_SUB_2);
        Heap_Free(raw);
        GfGfx_EngineBTogglePlanes(GX_PLANEMASK_BG1, GF_PLANE_TOGGLE_ON);
        break;
    case TOUCH_MART_SCREEN_QUANTITY:
        BgFillTilemapBufferAndSchedule(data->bgConfig, GF_BG_LYR_SUB_1, 0);
        raw = GfGfxLoader_GetScrnData(NARC_a_0_6_0, 19, FALSE, &screenData, HEAP_ID_8);
        BG_LoadScreenTilemapData(data->bgConfig, GF_BG_LYR_SUB_2, screenData->rawData, screenData->szByte);
        ScheduleBgTilemapBufferTransfer(data->bgConfig, GF_BG_LYR_SUB_2);
        Heap_Free(raw);
        break;
    case TOUCH_MART_SCREEN_CONFIRM:
        BgFillTilemapBufferAndSchedule(data->bgConfig, GF_BG_LYR_SUB_1, 0);
        raw = GfGfxLoader_GetScrnData(NARC_a_0_6_0, 20, FALSE, &screenData, HEAP_ID_8);
        BG_LoadScreenTilemapData(data->bgConfig, GF_BG_LYR_SUB_2, screenData->rawData, screenData->szByte);
        ScheduleBgTilemapBufferTransfer(data->bgConfig, GF_BG_LYR_SUB_2);
        Heap_Free(raw);
        break;
    }
}

static int TouchMartApp_GetTouchedButton(TouchMartAppData *data, int buttonSet) {
    GF_ASSERT(buttonSet == TOUCH_MART_BUTTONS_ITEM_LIST || buttonSet == TOUCH_MART_BUTTONS_QUANTITY);
    return TouchscreenHitbox_FindRectAtTouchNew(sTouchMartButtonSets[buttonSet]);
}

static void TouchMartApp_SetupBgs(TouchMartAppData *data) {
    vu32 *dispcnt = &reg_GXS_DB_DISPCNT;
    GXS_SetGraphicsMode(GX_BGMODE_0);
    GX_SetBankForSubBG(GX_VRAM_SUB_BG_32_H);
    GX_SetBankForSubOBJ(GX_VRAM_SUB_OBJ_16_I);
    *dispcnt = (*dispcnt & 0xFFCFFFEF) | 0x10;
    InitBgFromTemplate(data->bgConfig, GF_BG_LYR_SUB_0, &sTouchMartBgTemplates[0], GF_BG_TYPE_TEXT);
    InitBgFromTemplate(data->bgConfig, GF_BG_LYR_SUB_1, &sTouchMartBgTemplates[1], GF_BG_TYPE_TEXT);
    InitBgFromTemplate(data->bgConfig, GF_BG_LYR_SUB_2, &sTouchMartBgTemplates[2], GF_BG_TYPE_TEXT);
    BG_ClearCharDataRange(GF_BG_LYR_SUB_2, 0x20, 0, HEAP_ID_FIELD1);
    BgClearTilemapBufferAndCommit(data->bgConfig, GF_BG_LYR_SUB_2);
}

static void TouchMartApp_LoadGraphics(TouchMartAppData *data) {
    reg_G2S_DB_BLDCNT = 0;
    GfGfxLoader_GXLoadPal(NARC_a_0_6_0, 15, GF_PAL_LOCATION_SUB_BG, GF_PAL_SLOT_0_OFFSET, 0, HEAP_ID_8);
    GfGfxLoader_LoadCharData(NARC_a_0_6_0, 16, data->bgConfig, GF_BG_LYR_SUB_1, 0, 0, FALSE, HEAP_ID_8);
    TouchMartApp_LoadMainTilemap(data, TOUCH_MART_SCREEN_LIST);
    LoadFontPal1(GF_PAL_LOCATION_SUB_BG, GF_PAL_SLOT_11_OFFSET, HEAP_ID_8);
    LoadFontPal0(GF_PAL_LOCATION_SUB_BG, GF_PAL_SLOT_12_OFFSET, HEAP_ID_8);
    TouchMartApp_AddWindows(data);
    TouchMartApp_AddItemWindows(data);
}

static void TouchMartApp_FreeWindows(TouchMartAppData *data) {
    TouchMartApp_RemoveItemWindows(data);
    RemoveWindow(&data->unk_34);
    RemoveWindow(&data->messageWindow);
    RemoveWindow(&data->moneyWindow);
    RemoveWindow(&data->pageWindow);
    RemoveWindow(&data->cancelWindow);
}

static void TouchMartApp_AddWindows(TouchMartAppData *data) {
    int i;
    AddWindowParameterized(data->bgConfig, &data->unk_34, GF_BG_LYR_SUB_0, 2, 1, 27, 2, 11, 1);
    AddWindowParameterized(data->bgConfig, &data->messageWindow, GF_BG_LYR_SUB_0, 2, 1, 27, 4, 11, 29);
    AddWindowParameterized(data->bgConfig, &data->moneyWindow, GF_BG_LYR_SUB_0, 1, 0, 9, 4, 11, 137);
    AddWindowParameterized(data->bgConfig, &data->pageWindow, GF_BG_LYR_SUB_0, 10, 21, 7, 2, 12, 1);
    FillWindowPixelBuffer(&data->pageWindow, 0);
    AddWindowParameterized(data->bgConfig, &data->cancelWindow, GF_BG_LYR_SUB_0, 24, 21, 7, 2, 12, 15);
    FillWindowPixelBuffer(&data->cancelWindow, 0);
    for (i = 0; i < TOUCH_MART_WINDOW_COUNT; i++) {
        data->windows[i].bgConfig = NULL;
    }
}

static void TouchMartApp_AddItemWindows(TouchMartAppData *data) {
    int i;
    for (i = 0; i < TOUCH_MART_ITEMS_PER_PAGE; i++) {
        AddWindowParameterized(data->bgConfig, &data->windows[i], GF_BG_LYR_SUB_0, (u8)sTouchMartItemWindowPositions[i].x, (u8)sTouchMartItemWindowPositions[i].y, 11, 4, 12, (u16)sTouchMartItemWindowPositions[i].baseTile);
    }
}

static void TouchMartApp_RemoveItemWindows(TouchMartAppData *data) {
    int i = 0;
    Window *window = &data->windows[TOUCH_MART_WINDOW_ITEM_0];
    for (; i < TOUCH_MART_ITEMS_PER_PAGE; i++, window++) {
        ClearWindowTilemapAndCopyToVram(window);
        RemoveWindow(window);
    }
}

static void TouchMartApp_PrintItemList(TouchMartAppData *data) {
    Window *window;
    MartData *martData;
    int itemId;
    String *string;
    int index;
    MartData *martData2;
    int count;
    int i;

    i = 0;
    window = &data->windows[TOUCH_MART_WINDOW_ITEM_0];
    for (; i < TOUCH_MART_ITEMS_PER_PAGE; i++) {
        FillWindowPixelBuffer(window, 0);
        window++;
    }

    count = data->martData->numItems - data->martData->pageStart;
    if (count > TOUCH_MART_ITEMS_PER_PAGE) {
        count = TOUCH_MART_ITEMS_PER_PAGE;
    } else if (count < 0) {
        count = 0;
    }

    i = 0;
    if (count > 0) {
        window = &data->windows[TOUCH_MART_WINDOW_ITEM_0];
        do {
            martData = data->martData;
            index = i + martData->pageStart;
            itemId = martData->itemIds[index];
            string = NewString_ReadMsgData(data->itemNameMsgData, itemId);
            TouchMartApp_PrintItemName(data, window, string, i);
            String_Delete(string);
            if (TouchMartApp_IsItemPurchasable(data->martData, index, itemId)) {
                int price;
                martData2 = data->martData;
                price = MartData_GetItemPrice(martData2, (u16)itemId);
                TouchMartApp_PrintPrice(data->messageFormat, data->msgData, window, price, martData2->martType);
            }
            i++;
            window++;
        } while (i < count);
    }

    i = 0;
    window = &data->windows[TOUCH_MART_WINDOW_ITEM_0];
    for (; i < TOUCH_MART_ITEMS_PER_PAGE; i++) {
        ScheduleWindowCopyToVram(window);
        window++;
    }
    TouchMartApp_LoadListTilemap(data, count);
}

static void TouchMartApp_PrintItemName(TouchMartAppData *data, Window *window, String *string, int slot) {
    AddTextPrinterParameterizedWithColor(window, 0, string, 0, 0, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(1, 2, 0), NULL);
}

static void TouchMartApp_PrintPrice(MessageFormat *messageFormat, MsgData *msgData, Window *window, s32 price, int martType) {
    int msgId = msg_0435_00018;
    String *string;
    if ((u32)(martType - 3) <= 1) {
        msgId = msg_0435_00019;
    }
    BufferIntegerAsString(messageFormat, 0, price, 4, PRINTING_MODE_LEFT_ALIGN, TRUE);
    string = ReadMsgData_ExpandPlaceholders(messageFormat, msgData, msgId, HEAP_ID_8);
    AddTextPrinterParameterizedWithColor(window, 0, string, 36, 16, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(1, 2, 0), NULL);
    String_Delete(string);
}

static void TouchMartApp_PrintMoney(TouchMartAppData *data) {
    String *label;
    String *template;
    String *string;
    u32 amount;
    int width;

    FillWindowPixelBuffer(&data->moneyWindow, 0);
    {
        u8 martType = data->martData->martType;
        martType += 0xFD;
        if (martType <= 1) {
            label = NewString_ReadMsgData(data->msgData, msg_0435_00033);
        } else {
            label = NewString_ReadMsgData(data->msgData, msg_0435_00030);
        }
    }
    AddTextPrinterParameterizedWithColor(&data->moneyWindow, 0, label, 0, 0, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(15, 14, 0), NULL);
    String_Delete(label);
    string = String_New(16, HEAP_ID_FIELD2);
    {
        u8 martType = data->martData->martType;
        martType += 0xFD;
        if (martType <= 1) {
            template = NewString_ReadMsgData(data->msgData, msg_0435_00032);
            amount = PokeathlonSave_GetAthletePoints(data->pokeathlonSave);
        } else {
            template = NewString_ReadMsgData(data->msgData, msg_0435_00031);
            amount = PlayerProfile_GetMoney(data->playerProfile);
        }
    }
    BufferIntegerAsString(data->messageFormat, 0, amount, 6, PRINTING_MODE_RIGHT_ALIGN, TRUE);
    StringExpandPlaceholders(data->messageFormat, string, template);
    width = FontID_String_GetWidth(0, string, 0);
    AddTextPrinterParameterizedWithColor(&data->moneyWindow, 0, string, 72 - width, 16, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(15, 14, 0), NULL);
    String_Delete(template);
    String_Delete(string);
    ScheduleWindowCopyToVram(&data->moneyWindow);
}

static void TouchMartApp_PrintPageNumber(TouchMartAppData *data) {
    MartData *martData;
    String *string;
    String *template;
    int page;
    int pageCount;
    int itemCount;

    FillWindowPixelBuffer(&data->pageWindow, 0);
    string = String_New(6, HEAP_ID_FIELD2);
    template = NewString_ReadMsgData(data->msgData, msg_0435_00043);
    martData = data->martData;
    page = (martData->pageStart + TOUCH_MART_ITEMS_PER_PAGE) / TOUCH_MART_ITEMS_PER_PAGE;
    itemCount = martData->numItems;
    pageCount = itemCount / TOUCH_MART_ITEMS_PER_PAGE;
    if (itemCount % TOUCH_MART_ITEMS_PER_PAGE != 0) {
        pageCount++;
    }
    BufferIntegerAsString(data->messageFormat, 0, page, 2, PRINTING_MODE_RIGHT_ALIGN, TRUE);
    BufferIntegerAsString(data->messageFormat, 1, pageCount, 2, PRINTING_MODE_RIGHT_ALIGN, TRUE);
    StringExpandPlaceholders(data->messageFormat, string, template);
    AddTextPrinterParameterizedWithColor(&data->pageWindow, 0, string, 0, 0, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(15, 14, 0), NULL);
    String_Delete(template);
    String_Delete(string);
    ScheduleWindowCopyToVram(&data->pageWindow);
}

static void TouchMartApp_EditListTilemap(BgConfig *bgConfig, int bgId, int itemCount) {
    void *tilemap = GetBgTilemapBuffer(bgConfig, (u8)bgId);
    int i;
    const TouchMartBgOp *op;
    if (itemCount == TOUCH_MART_ITEMS_PER_PAGE) {
        return;
    }
    op = sTouchMartListTilemapEdits[itemCount];
    for (i = 0; i < 4; i++, op++) {
        if (op->kind == 1) {
            LoadRectToBgTilemapRect(bgConfig, (u8)bgId, (u8 *)tilemap + (op->srcX + op->srcY * 32) * 2, op->destX, op->destY, op->width, op->height);
        } else if (op->kind == 2) {
            FillBgTilemapRect(bgConfig, (u8)bgId, 0, op->destX, op->destY, op->width, op->height, 0);
        }
    }
}

static void TouchMartApp_LoadListTilemap(TouchMartAppData *data, int itemCount) {
    NNSG2dScreenData *screenData;
    void *raw = GfGfxLoader_GetScrnData(NARC_a_0_6_0, 18, FALSE, &screenData, HEAP_ID_8);
    BG_LoadScreenTilemapData(data->bgConfig, GF_BG_LYR_SUB_1, screenData->rawData, screenData->szByte);
    TouchMartApp_EditListTilemap(data->bgConfig, GF_BG_LYR_SUB_1, itemCount);
    ScheduleBgTilemapBufferTransfer(data->bgConfig, GF_BG_LYR_SUB_1);
    Heap_Free(raw);
}

static BOOL TouchMartApp_IsItemPurchasable(MartData *martData, int index, int itemId) {
    if (martData->martType == 3) {
        return PokeathlonSave_IsDailyItemBought(martData->pokeathlonSave, index) == 0;
    }
    if (martData->martType == 4) {
        return PokeathlonSave_IsDataCardBought(martData->pokeathlonSave, index + ((itemId - 505) / 6) * 6) == 0;
    }
    return TRUE;
}

static void TouchMartApp_PrintSelectedItem(TouchMartAppData *data) {
    MartData *martData;
    int index;
    int itemId;
    String *string;

    martData = data->martData;
    index = martData->cursorPos + martData->pageStart;
    string = NewString_ReadMsgData(data->itemNameMsgData, martData->itemIds[index]);
    TouchMartApp_PrintItemName(data, &data->windows[TOUCH_MART_WINDOW_SELECTED_ITEM], string, index);
    String_Delete(string);
    martData = data->martData;
    itemId = martData->itemIds[index];
    if (TouchMartApp_IsItemPurchasable(martData, index, itemId)) {
        int price;
        martData = data->martData;
        itemId = martData->itemIds[index];
        price = MartData_GetItemPrice(martData, itemId);
        TouchMartApp_PrintPrice(data->messageFormat, data->msgData, &data->windows[TOUCH_MART_WINDOW_SELECTED_ITEM], price, martData->martType);
    }
    ScheduleWindowCopyToVram(&data->windows[TOUCH_MART_WINDOW_SELECTED_ITEM]);
}

static void TouchMartApp_PrintInBag(TouchMartAppData *data, int unused) {
    String *string;
    String *template;
    int count;
    int width;

    template = NewString_ReadMsgData(data->msgData, msg_0435_00035);
    AddTextPrinterParameterizedWithColor(&data->windows[TOUCH_MART_WINDOW_IN_BAG], 0, template, 0, 4, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(1, 2, 0), NULL);
    String_Delete(template);
    string = String_New(5, HEAP_ID_FIELD2);
    template = NewString_ReadMsgData(data->msgData, msg_0435_00036);
    count = MartData_GetOwnedQuantity(data->martData, data->martData->item);
    BufferIntegerAsString(data->messageFormat, 0, count, 3, PRINTING_MODE_RIGHT_ALIGN, TRUE);
    StringExpandPlaceholders(data->messageFormat, string, template);
    width = FontID_String_GetWidth(0, string, 0);
    AddTextPrinterParameterizedWithColor(&data->windows[TOUCH_MART_WINDOW_IN_BAG], 0, string, 64 - width, 20, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(1, 2, 0), NULL);
    String_Delete(string);
    String_Delete(template);
    ScheduleWindowCopyToVram(&data->windows[TOUCH_MART_WINDOW_IN_BAG]);
}

static void TouchMartApp_PrintQuantity(TouchMartAppData *data, int quantity) {
    String *tensString;
    String *onesString;
    String *tensTemplate;
    String *onesTemplate;
    int width;

    tensString = String_New(2, HEAP_ID_FIELD2);
    onesString = String_New(2, HEAP_ID_FIELD2);
    tensTemplate = NewString_ReadMsgData(data->msgData, msg_0435_00044);
    onesTemplate = NewString_ReadMsgData(data->msgData, msg_0435_00045);
    BufferIntegerAsString(data->messageFormat, 0, quantity / 10, 1, PRINTING_MODE_RIGHT_ALIGN, TRUE);
    BufferIntegerAsString(data->messageFormat, 1, quantity % 10, 1, PRINTING_MODE_RIGHT_ALIGN, TRUE);
    StringExpandPlaceholders(data->messageFormat, tensString, tensTemplate);
    StringExpandPlaceholders(data->messageFormat, onesString, onesTemplate);
    FillWindowPixelBuffer(&data->windows[TOUCH_MART_WINDOW_QUANTITY_TENS], 0);
    FillWindowPixelBuffer(&data->windows[TOUCH_MART_WINDOW_QUANTITY_ONES], 0);
    AddTextPrinterParameterizedWithColor(&data->windows[TOUCH_MART_WINDOW_QUANTITY_TENS], 0, tensString, 0, 4, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(1, 2, 0), NULL);
    AddTextPrinterParameterizedWithColor(&data->windows[TOUCH_MART_WINDOW_QUANTITY_ONES], 0, onesString, 0, 4, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(1, 2, 0), NULL);
    String_Delete(tensString);
    String_Delete(onesString);
    String_Delete(tensTemplate);
    String_Delete(onesTemplate);
    ScheduleWindowCopyToVram(&data->windows[TOUCH_MART_WINDOW_QUANTITY_TENS]);
    ScheduleWindowCopyToVram(&data->windows[TOUCH_MART_WINDOW_QUANTITY_ONES]);
    FillWindowPixelBuffer(&data->windows[TOUCH_MART_WINDOW_TOTAL_PRICE], 0);
    tensString = String_New(9, HEAP_ID_FIELD2);
    tensTemplate = NewString_ReadMsgData(data->msgData, msg_0435_00038);
    BufferIntegerAsString(data->messageFormat, 0, data->martData->cost * quantity, 6, PRINTING_MODE_RIGHT_ALIGN, TRUE);
    StringExpandPlaceholders(data->messageFormat, tensString, tensTemplate);
    width = FontID_String_GetWidth(0, tensString, 0);
    AddTextPrinterParameterizedWithColor(&data->windows[TOUCH_MART_WINDOW_TOTAL_PRICE], 0, tensString, 64 - width, 4, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(1, 2, 0), NULL);
    String_Delete(tensString);
    String_Delete(tensTemplate);
    ScheduleWindowCopyToVram(&data->windows[TOUCH_MART_WINDOW_TOTAL_PRICE]);
}

static void TouchMartApp_PrintBuyButton(TouchMartAppData *data) {
    String *string = NewString_ReadMsgData(data->msgData, msg_0435_00042);
    AddTextPrinterParameterizedWithColor(&data->windows[TOUCH_MART_WINDOW_BUY], 0, string, 4, 0, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(15, 14, 0), NULL);
    ScheduleWindowCopyToVram(&data->windows[TOUCH_MART_WINDOW_BUY]);
    String_Delete(string);
}

static void TouchMartApp_BufferItemName(int martType, MessageFormat *messageFormat, int itemId, int fieldno) {
    switch (martType) {
    case MART_TYPE_NORMAL:
    case MART_TYPE_3:
    case MART_TYPE_4:
        BufferItemName(messageFormat, fieldno, itemId);
        break;
    case MART_TYPE_1:
        BufferDecorationName(messageFormat, fieldno, itemId);
        break;
    }
}

static void TouchMartApp_BufferItemNamePlural(int martType, MessageFormat *messageFormat, int itemId, int fieldno) {
    switch (martType) {
    case MART_TYPE_NORMAL:
    case MART_TYPE_3:
    case MART_TYPE_4:
        BufferItemNamePlural(messageFormat, fieldno, itemId);
        break;
    case MART_TYPE_1:
        BufferDecorationName(messageFormat, fieldno, itemId);
        break;
    }
}

static void TouchMartApp_BufferItemNameWithIndefArticle(int martType, MessageFormat *messageFormat, int itemId, int fieldno) {
    switch (martType) {
    case MART_TYPE_NORMAL:
    case MART_TYPE_3:
    case MART_TYPE_4:
        BufferItemNameWithIndefArticle(messageFormat, fieldno, itemId);
        break;
    case MART_TYPE_1:
        BufferDecorationName(messageFormat, fieldno, itemId);
        break;
    }
}

static void TouchMartApp_PrintHowMany(TouchMartAppData *data) {
    String *template;
    FillWindowPixelBuffer(&data->windows[TOUCH_MART_WINDOW_PROMPT], 15);
    TouchMartApp_BufferItemName(data->martData->martType, data->messageFormat, data->martData->item, 0);
    template = NewString_ReadMsgData(data->msgData, msg_0435_00012);
    StringExpandPlaceholders(data->messageFormat, data->string, template);
    String_Delete(template);
    LoadUserFrameGfx2(data->bgConfig, GF_BG_LYR_SUB_0, 437, 5, (u8)Options_GetFrame(data->options), HEAP_ID_FIELD1);
    DrawFrameAndWindow2(&data->windows[TOUCH_MART_WINDOW_PROMPT], TRUE, 437, 5);
    data->martData->printerID = AddTextPrinterParameterized(&data->windows[TOUCH_MART_WINDOW_PROMPT], 1, data->string, 0, 0, Options_GetTextFrameDelay(data->options), NULL);
}

static void TouchMartApp_PrintConfirmPurchase(TouchMartAppData *data) {
    String *template;
    FillWindowPixelBuffer(&data->windows[TOUCH_MART_WINDOW_PROMPT], 15);
    if ((u8)(data->martData->martType + 0xFD) <= 1) {
        TouchMartApp_BufferItemNameWithIndefArticle(data->martData->martType, data->messageFormat, data->martData->item, 0);
        template = NewString_ReadMsgData(data->msgData, msg_0435_00046);
    } else {
        BufferIntegerAsString(data->messageFormat, 0, data->martData->quantity, 2, PRINTING_MODE_LEFT_ALIGN, TRUE);
        BufferIntegerAsString(data->messageFormat, 1, data->martData->cost * data->martData->quantity, 6, PRINTING_MODE_LEFT_ALIGN, TRUE);
        template = NewString_ReadMsgData(data->msgData, msg_0435_00014);
    }
    StringExpandPlaceholders(data->messageFormat, data->string, template);
    String_Delete(template);
    LoadUserFrameGfx2(data->bgConfig, GF_BG_LYR_SUB_0, 437, 5, (u8)Options_GetFrame(data->options), HEAP_ID_FIELD1);
    DrawFrameAndWindow2(&data->windows[TOUCH_MART_WINDOW_PROMPT], TRUE, 437, 5);
    data->printerId = AddTextPrinterParameterized(&data->windows[TOUCH_MART_WINDOW_PROMPT], 1, data->string, 0, 0, Options_GetTextFrameDelay(data->options), NULL);
}

static void TouchMartApp_CreateYesNoPrompt(TouchMartAppData *data) {
    YesNoPromptTemplate template;
    MI_CpuFill8(&template, 0, sizeof(template));
    template.bgConfig = data->bgConfig;
    template.bgId = GF_BG_LYR_SUB_1;
    template.tileStart = 0x60;
    template.plttSlot = 8;
    template.x = 26;
    template.y = 6;
    template.ignoreTouchFlag = MenuInputStateMgr_GetState(&data->fieldSystem->menuInputState);
    template.initialCursorPos = 0;
    template.shapeParam = 0;
    BgClearTilemapBufferAndCommit(data->bgConfig, GF_BG_LYR_SUB_1);
    data->yesNoPrompt = YesNoPrompt_Create(HEAP_ID_8);
    YesNoPrompt_InitFromTemplate(data->yesNoPrompt, &template);
}

static BOOL TouchMartApp_HandleYesNoPrompt(TouchMartAppData *data) {
    int result;
    if (data->yesNoPrompt == NULL) {
        return FALSE;
    }
    result = YesNoPrompt_HandleInputForSave(data->yesNoPrompt);
    switch (result) {
    case YESNORESPONSE_YES:
        YesNoPrompt_Destroy(data->yesNoPrompt);
        data->yesNoPrompt = NULL;
        data->martData->touchButton = 0;
        return TRUE;
    case YESNORESPONSE_NO:
        YesNoPrompt_Destroy(data->yesNoPrompt);
        data->yesNoPrompt = NULL;
        data->martData->touchButton = 1;
        return TRUE;
    }
    return FALSE;
}

static void TouchMartApp_PrintPurchaseDone(TouchMartAppData *data) {
    String *template;
    MartData *martData = data->martData;
    if (martData->quantity > 1) {
        TouchMartApp_BufferItemNamePlural(martData->martType, data->messageFormat, martData->item, 0);
    } else {
        TouchMartApp_BufferItemName(martData->martType, data->messageFormat, martData->item, 0);
    }
    switch (data->martData->martType) {
    case MART_TYPE_NORMAL:
        template = NewString_ReadMsgData(data->msgData, msg_0435_00015);
        BufferPocketName(data->messageFormat, 1, GetItemAttr(data->martData->item, ITEMATTR_FIELD_POCKET, HEAP_ID_FIELD2));
        break;
    case MART_TYPE_3:
        if (data->martData->item >= 485 && data->martData->item <= 491) {
            template = NewString_ReadMsgData(data->msgData, msg_0435_00049);
        } else {
            template = NewString_ReadMsgData(data->msgData, msg_0435_00015);
            BufferPocketName(data->messageFormat, 1, GetItemAttr(data->martData->item, ITEMATTR_FIELD_POCKET, HEAP_ID_FIELD2));
        }
        break;
    case MART_TYPE_4:
        template = NewString_ReadMsgData(data->msgData, msg_0435_00049);
        break;
    case MART_TYPE_1:
        template = NewString_ReadMsgData(data->msgData, msg_0435_00021);
        break;
    default:
        template = NewString_ReadMsgData(data->msgData, msg_0435_00023);
        break;
    }
    StringExpandPlaceholders(data->messageFormat, data->string, template);
    String_Delete(template);
    FillWindowPixelBuffer(&data->messageWindow, 15);
    DrawFrameAndWindow2(&data->messageWindow, TRUE, 437, 5);
    data->martData->printerID = AddTextPrinterParameterized(&data->messageWindow, 1, data->string, 0, 0, Options_GetTextFrameDelay(data->options), TouchMartApp_PurchaseDonePrinterCallback);
}

static BOOL TouchMartApp_PurchaseDonePrinterCallback(TextPrinterTemplate *template, u16 glyphId) {
    if (glyphId == 1) {
        PlaySE(SEQ_SE_DP_REGI);
    }
    return FALSE;
}

static String *TouchMartApp_GetCannotAffordMessage(MartData *martData, MsgData *msgData) {
    int amount;
    int result;
    u8 martType = martData->martType;
    martType += 0xFD;
    if (martType <= 1) {
        amount = PokeathlonSave_GetAthletePoints(martData->pokeathlonSave);
    } else {
        amount = PlayerProfile_GetMoney(martData->playerProfile);
    }
    result = MartData_CheckCannotBuy(martData, amount);
    if (result == 2) {
        return NewString_ReadMsgData(msgData, msg_0435_00025);
    }
    if (result == 3) {
        return NewString_ReadMsgData(msgData, msg_0435_00026);
    }
    martType = martData->martType;
    martType += 0xFD;
    if (martType <= 1) {
        return NewString_ReadMsgData(msgData, msg_0435_00048);
    }
    return NewString_ReadMsgData(msgData, msg_0435_00011);
}

static String *TouchMartApp_GetNoRoomMessage(const MartData *martData, MsgData *msgData) {
    int msgId;
    if (martData->martType == MART_TYPE_NORMAL) {
        msgId = msg_0435_00016;
    } else if (martData->martType == MART_TYPE_1) {
        msgId = msg_0435_00022;
    } else if (martData->martType == MART_TYPE_SEAL) {
        msgId = msg_0435_00024;
    } else if (martData->martType == MART_TYPE_3) {
        msgId = msg_0435_00025;
    } else if (martData->martType == MART_TYPE_4) {
        msgId = msg_0435_00026;
    }
    return NewString_ReadMsgData(msgData, msgId);
}

// Expands `template` into the message window and starts printing it.
#define TOUCH_MART_PRINT_MESSAGE(data, template)                                                                                  \
    {                                                                                                                             \
        String *string_ = (template);                                                                                             \
        int frame_;                                                                                                               \
        int delay_;                                                                                                               \
        StringExpandPlaceholders((data)->messageFormat, (data)->string, string_);                                                 \
        String_Delete(string_);                                                                                                   \
        FillWindowPixelBuffer(&(data)->messageWindow, 15);                                                                        \
        frame_ = Options_GetFrame((data)->options);                                                                               \
        LoadUserFrameGfx2((data)->bgConfig, GF_BG_LYR_SUB_0, 437, 5, (u8)frame_, HEAP_ID_FIELD1);                                 \
        DrawFrameAndWindow2(&(data)->messageWindow, TRUE, 437, 5);                                                                \
        delay_ = Options_GetTextFrameDelay((data)->options);                                                                      \
        (data)->martData->printerID = AddTextPrinterParameterized(&(data)->messageWindow, 1, (data)->string, 0, 0, delay_, NULL); \
    }

static void TouchMartApp_PrintCannotAfford(TouchMartAppData *data) {
    TOUCH_MART_PRINT_MESSAGE(data, TouchMartApp_GetCannotAffordMessage(data->martData, data->msgData));
}

static void TouchMartApp_PrintNoRoom(TouchMartAppData *data) {
    TOUCH_MART_PRINT_MESSAGE(data, TouchMartApp_GetNoRoomMessage(data->martData, data->msgData));
}

static void TouchMartApp_PrintNoRoomInBag(TouchMartAppData *data) {
    TOUCH_MART_PRINT_MESSAGE(data, NewString_ReadMsgData(data->msgData, msg_0435_00016));
}

static void TouchMartApp_PrintPremierBallBonus(TouchMartAppData *data) {
    TOUCH_MART_PRINT_MESSAGE(data, NewString_ReadMsgData(data->msgData, msg_0435_00020));
}

static void TouchMartApp_AddDetailWindows(TouchMartAppData *data) {
    AddWindowParameterized(data->bgConfig, &data->windows[TOUCH_MART_WINDOW_SELECTED_ITEM], GF_BG_LYR_SUB_0, 12, 7, 11, 4, 12, 173);
    AddWindowParameterized(data->bgConfig, &data->windows[TOUCH_MART_WINDOW_QUANTITY_TENS], GF_BG_LYR_SUB_0, 16, 14, 2, 3, 12, 217);
    AddWindowParameterized(data->bgConfig, &data->windows[TOUCH_MART_WINDOW_QUANTITY_ONES], GF_BG_LYR_SUB_0, 20, 14, 2, 3, 12, 223);
    AddWindowParameterized(data->bgConfig, &data->windows[TOUCH_MART_WINDOW_BUY], GF_BG_LYR_SUB_0, 14, 21, 7, 2, 12, 229);
    AddWindowParameterized(data->bgConfig, &data->windows[TOUCH_MART_WINDOW_IN_BAG], GF_BG_LYR_SUB_0, 1, 13, 8, 5, 11, 243);
    AddWindowParameterized(data->bgConfig, &data->windows[TOUCH_MART_WINDOW_TOTAL_PRICE], GF_BG_LYR_SUB_0, 23, 14, 8, 3, 12, 283);
    AddWindowParameterized(data->bgConfig, &data->windows[TOUCH_MART_WINDOW_PROMPT], GF_BG_LYR_SUB_0, 12, 1, 17, 4, 12, 307);
    FillWindowPixelBuffer(&data->windows[TOUCH_MART_WINDOW_SELECTED_ITEM], 0);
    FillWindowPixelBuffer(&data->windows[TOUCH_MART_WINDOW_QUANTITY_TENS], 0);
    FillWindowPixelBuffer(&data->windows[TOUCH_MART_WINDOW_QUANTITY_ONES], 0);
    FillWindowPixelBuffer(&data->windows[TOUCH_MART_WINDOW_BUY], 0);
    FillWindowPixelBuffer(&data->windows[TOUCH_MART_WINDOW_IN_BAG], 0);
    FillWindowPixelBuffer(&data->windows[TOUCH_MART_WINDOW_TOTAL_PRICE], 0);
    FillWindowPixelBuffer(&data->windows[TOUCH_MART_WINDOW_PROMPT], 15);
}

static void TouchMartApp_RemoveDetailWindows(TouchMartAppData *data) {
    ClearWindowTilemapAndScheduleTransfer(&data->windows[TOUCH_MART_WINDOW_SELECTED_ITEM]);
    ClearWindowTilemapAndScheduleTransfer(&data->windows[TOUCH_MART_WINDOW_QUANTITY_TENS]);
    ClearWindowTilemapAndScheduleTransfer(&data->windows[TOUCH_MART_WINDOW_QUANTITY_ONES]);
    ClearWindowTilemapAndScheduleTransfer(&data->windows[TOUCH_MART_WINDOW_BUY]);
    ClearWindowTilemapAndScheduleTransfer(&data->windows[TOUCH_MART_WINDOW_IN_BAG]);
    ClearWindowTilemapAndScheduleTransfer(&data->windows[TOUCH_MART_WINDOW_TOTAL_PRICE]);
    ClearWindowTilemapAndScheduleTransfer(&data->windows[TOUCH_MART_WINDOW_PROMPT]);
    ClearFrameAndWindow2(&data->windows[TOUCH_MART_WINDOW_PROMPT], TRUE);
    ClearFrameAndWindow2(&data->messageWindow, FALSE);
    RemoveWindow(&data->windows[TOUCH_MART_WINDOW_SELECTED_ITEM]);
    RemoveWindow(&data->windows[TOUCH_MART_WINDOW_QUANTITY_TENS]);
    RemoveWindow(&data->windows[TOUCH_MART_WINDOW_QUANTITY_ONES]);
    RemoveWindow(&data->windows[TOUCH_MART_WINDOW_BUY]);
    RemoveWindow(&data->windows[TOUCH_MART_WINDOW_IN_BAG]);
    RemoveWindow(&data->windows[TOUCH_MART_WINDOW_TOTAL_PRICE]);
    RemoveWindow(&data->windows[TOUCH_MART_WINDOW_PROMPT]);
}
