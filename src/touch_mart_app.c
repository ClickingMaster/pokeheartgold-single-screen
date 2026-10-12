#include "touch_mart_app.h"

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
#include "sys_task.h"
#include "systask_environment.h"
#include "text.h"
#include "touchscreen.h"
#include "sound_playback.h"
#include "yes_no_prompt.h"

// Bottom-screen counterpart of the field mart (overlay_03 shop_menu.c): the item list, the
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

#define TOUCH_MART_ITEMS_PER_PAGE 6

typedef struct TouchMartAppData {
    u32 unk_00;
    BgConfig *bgConfig;
    SysTask *task;
    u32 state;
    u32 unk_10;
    MartData *martData;
    void *unk_18;
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
    u8 x;
    u8 y;
    u8 destX;
    u8 width;
    u8 height;
    u8 value;
    u8 padding;
} TouchMartBgOp;

extern const TouchscreenHitbox sTouchMartTextSkipHitbox[];
extern const TouchMartWindowPos sTouchMartItemWindowPositions[TOUCH_MART_ITEMS_PER_PAGE];
extern const BgTemplate sTouchMartBgTemplate_Sub0;
extern const BgTemplate sTouchMartBgTemplate_Sub1;
extern const BgTemplate sTouchMartBgTemplate_Sub2;
extern const TouchscreenHitbox *sTouchMartButtonSets[];
extern const TouchMartBgOp sTouchMartListTilemapEdits[][4];

void TouchMartApp_RunTask(SysTask *task, TouchMartAppData *data);
void TouchMartApp_SetupText(TouchMartAppData *data);
void TouchMartApp_FreeText(TouchMartAppData *data);
void TouchMartApp_PrintCancel(TouchMartAppData *data, int leftAligned);
BOOL TouchMartApp_IsSelectionValid(TouchMartAppData *data, u32 selection);
void TouchMartApp_HandleAction(TouchMartAppData *data, int action);
void TouchMartApp_LoadMainTilemap(TouchMartAppData *data, int screen);
int TouchMartApp_GetTouchedButton(TouchMartAppData *data, int buttonSet);
void TouchMartApp_SetupBgs(TouchMartAppData *data);
void TouchMartApp_LoadGraphics(TouchMartAppData *data);
void TouchMartApp_FreeWindows(TouchMartAppData *data);
void TouchMartApp_AddWindows(TouchMartAppData *data);
void TouchMartApp_AddItemWindows(TouchMartAppData *data);
void TouchMartApp_RemoveItemWindows(TouchMartAppData *data);
void TouchMartApp_PrintItemList(TouchMartAppData *data);
void TouchMartApp_PrintItemName(TouchMartAppData *data, Window *window, String *string, int slot);
void TouchMartApp_PrintPrice(MessageFormat *messageFormat, MsgData *msgData, Window *window, s32 price, int martType);
void TouchMartApp_PrintMoney(TouchMartAppData *data);
void TouchMartApp_PrintPageNumber(TouchMartAppData *data);
void TouchMartApp_EditListTilemap(BgConfig *bgConfig, int bgId, int itemCount);
void TouchMartApp_LoadListTilemap(TouchMartAppData *data, int itemCount);
BOOL ov31_0225E12C(MartData *martData, int index, int itemId);
void TouchMartApp_PrintSelectedItem(TouchMartAppData *data);
void TouchMartApp_PrintInBag(TouchMartAppData *data, int unused);
void TouchMartApp_PrintQuantity(TouchMartAppData *data, int quantity);
void TouchMartApp_PrintBuyButton(TouchMartAppData *data);
void TouchMartApp_BufferItemName(int martType, MessageFormat *messageFormat, int itemId, int fieldno);
void TouchMartApp_BufferItemNamePlural(int martType, MessageFormat *messageFormat, int itemId, int fieldno);
void TouchMartApp_BufferItemNameWithIndefArticle(int martType, MessageFormat *messageFormat, int itemId, int fieldno);
void TouchMartApp_PrintHowMany(TouchMartAppData *data);
void TouchMartApp_PrintConfirmPurchase(TouchMartAppData *data);
void TouchMartApp_CreateYesNoPrompt(TouchMartAppData *data);
BOOL TouchMartApp_HandleYesNoPrompt(TouchMartAppData *data);
void TouchMartApp_PrintPurchaseDone(TouchMartAppData *data);
BOOL TouchMartApp_PurchaseDonePrinterCallback(TextPrinterTemplate *template, u16 glyphId);
String *TouchMartApp_GetCannotAffordMessage(MartData *martData, MsgData *msgData);
String *TouchMartApp_GetNoRoomMessage(const MartData *martData, MsgData *msgData);
void TouchMartApp_PrintCannotAfford(TouchMartAppData *data);
void TouchMartApp_PrintNoRoom(TouchMartAppData *data);
void TouchMartApp_PrintNoRoomInBag(TouchMartAppData *data);
void TouchMartApp_PrintPremierBallBonus(TouchMartAppData *data);
void TouchMartApp_AddDetailWindows(TouchMartAppData *data);
void TouchMartApp_RemoveDetailWindows(TouchMartAppData *data);

SysTask *TouchMartApp_Init(BgConfig *bgConfig, void *a1, FieldSystem *fieldSystem, MartData *martData) {
    SysTask *task;
    TouchMartAppData *data;

    Heap_Create(HEAP_ID_3, HEAP_ID_8, 3 << 15);
    task = CreateSysTaskAndEnvironment((SysTaskFunc)TouchMartApp_RunTask, sizeof(TouchMartAppData), 10, HEAP_ID_8);
    data = SysTask_GetData(task);
    data->task = task;
    data->unk_00 = 0;
    data->bgConfig = bgConfig;
    data->unk_18 = a1;
    data->fieldSystem = fieldSystem;
    data->unk_30 = 0;
    data->options = Save_PlayerData_GetOptionsAddr(data->fieldSystem->saveData);
    data->playerProfile = Save_PlayerData_GetProfile(data->fieldSystem->saveData);
    data->pokeathlonSave = Save_Pokeathlon_Get(data->fieldSystem->saveData);
    data->state = 0;
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

void TouchMartApp_SetupText(TouchMartAppData *data) {
    data->messageFormat = MessageFormat_New_Custom(8, 64, HEAP_ID_8);
    data->msgData = NewMsgDataFromNarc(MSGDATA_LOAD_DIRECT, NARC_msgdata_msg, NARC_msg_msg_0435_bin, HEAP_ID_8);
    data->itemNameMsgData = NewMsgDataFromNarc(MSGDATA_LOAD_DIRECT, NARC_msgdata_msg, NARC_msg_msg_0222_bin, HEAP_ID_8);
    data->string = String_New(144, HEAP_ID_8);
}

void TouchMartApp_FreeText(TouchMartAppData *data) {
    String_Delete(data->string);
    DestroyMsgData(data->itemNameMsgData);
    DestroyMsgData(data->msgData);
    MessageFormat_Delete(data->messageFormat);
}

// force_active only sets a section flag here; kept so the object stays identical.
#pragma force_active on

void TouchMartApp_PrintCancel(TouchMartAppData *data, int leftAligned) {
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

#pragma force_active reset

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

BOOL TouchMartApp_IsSelectionValid(TouchMartAppData *data, u32 selection) {
    switch (selection) {
    case 0:
    case 1:
    case 2:
    case 3:
    case 4:
    case 5: {
        MartData *martData = data->martData;
        if (selection + martData->unk271 < martData->unk270) {
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

void TouchMartApp_RunTask(SysTask *task, TouchMartAppData *data) {
    int selection;
    switch (data->state) {
    case 0:
        if (IsPaletteFadeFinished() == TRUE) {
            data->state = 1;
        }
        return;
    case 1:
        switch (data->martData->state) {
        case 3:
            selection = TouchMartApp_GetTouchedButton(data, 0);
            if (selection != -1 && TouchMartApp_IsSelectionValid(data, selection)) {
                data->martData->unk294 = selection;
            }
            break;
        case 7:
            selection = TouchMartApp_GetTouchedButton(data, 1);
            if (selection != -1) {
                data->martData->unk294 = selection;
            }
            break;
        case 11:
            TouchMartApp_HandleYesNoPrompt(data);
            break;
        }
        if (data->martData->unk298 != 0) {
            TouchMartApp_HandleAction(data, data->martData->unk298);
            data->martData->unk298 = 0;
        }
        return;
    }
}

void TouchMartApp_HandleAction(TouchMartAppData *data, int action) {
    switch (action) {
    case 0:
        break;
    case 1:
        TouchMartApp_PrintItemList(data);
        TouchMartApp_PrintPageNumber(data);
        TouchMartApp_PrintCancel(data, FALSE);
        break;
    case 2:
        TouchMartApp_LoadMainTilemap(data, 1);
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
    case 3:
        TouchMartApp_LoadMainTilemap(data, 2);
        TouchMartApp_RemoveItemWindows(data);
        TouchMartApp_AddDetailWindows(data);
        TouchMartApp_PrintSelectedItem(data);
        TouchMartApp_PrintInBag(data, data->martData->item);
        TouchMartApp_PrintConfirmPurchase(data);
        ClearWindowTilemapAndScheduleTransfer(&data->pageWindow);
        ClearWindowTilemapAndScheduleTransfer(&data->cancelWindow);
        break;
    case 4:
        TouchMartApp_LoadMainTilemap(data, 0);
        TouchMartApp_RemoveDetailWindows(data);
        TouchMartApp_PrintPageNumber(data);
        TouchMartApp_PrintCancel(data, FALSE);
        TouchMartApp_AddItemWindows(data);
        ClearFrameAndWindow2(&data->messageWindow, TRUE);
        TouchMartApp_PrintItemList(data);
        TouchMartApp_PrintMoney(data);
        ScheduleWindowCopyToVram(&data->cancelWindow);
        break;
    case 5:
        TouchMartApp_LoadMainTilemap(data, 0);
        TouchMartApp_PrintPageNumber(data);
        ClearFrameAndWindow2(&data->messageWindow, FALSE);
        TouchMartApp_PrintItemList(data);
        TouchMartApp_PrintMoney(data);
        break;
    case 6:
        TouchMartApp_PrintQuantity(data, data->martData->quantity);
        break;
    case 7:
        ClearWindowTilemapAndScheduleTransfer(&data->cancelWindow);
        ClearWindowTilemapAndScheduleTransfer(&data->windows[TOUCH_MART_WINDOW_BUY]);
        TouchMartApp_PrintConfirmPurchase(data);
        break;
    case 8:
        TouchMartApp_CreateYesNoPrompt(data);
        break;
    case 9:
        TouchMartApp_PrintPurchaseDone(data);
        break;
    case 10:
        TouchMartApp_PrintCannotAfford(data);
        break;
    case 11:
        ClearWindowTilemapAndScheduleTransfer(&data->cancelWindow);
        ClearWindowTilemapAndScheduleTransfer(&data->windows[TOUCH_MART_WINDOW_BUY]);
        TouchMartApp_PrintNoRoom(data);
        break;
    case 12:
        TouchMartApp_PrintNoRoomInBag(data);
        break;
    case 13:
        TouchMartApp_PrintPremierBallBonus(data);
        break;
    }
}

void TouchMartApp_LoadMainTilemap(TouchMartAppData *data, int screen) {
    void *raw;
    NNSG2dScreenData *screenData;
    switch (screen) {
    case 0:
        raw = GfGfxLoader_GetScrnData(NARC_a_0_6_0, 17, FALSE, &screenData, HEAP_ID_8);
        BG_LoadScreenTilemapData(data->bgConfig, GF_BG_LYR_SUB_2, screenData->rawData, screenData->szByte);
        ScheduleBgTilemapBufferTransfer(data->bgConfig, GF_BG_LYR_SUB_2);
        Heap_Free(raw);
        GfGfx_EngineBTogglePlanes(GX_PLANEMASK_BG1, GF_PLANE_TOGGLE_ON);
        break;
    case 1:
        BgFillTilemapBufferAndSchedule(data->bgConfig, GF_BG_LYR_SUB_1, 0);
        raw = GfGfxLoader_GetScrnData(NARC_a_0_6_0, 19, FALSE, &screenData, HEAP_ID_8);
        BG_LoadScreenTilemapData(data->bgConfig, GF_BG_LYR_SUB_2, screenData->rawData, screenData->szByte);
        ScheduleBgTilemapBufferTransfer(data->bgConfig, GF_BG_LYR_SUB_2);
        Heap_Free(raw);
        break;
    case 2:
        BgFillTilemapBufferAndSchedule(data->bgConfig, GF_BG_LYR_SUB_1, 0);
        raw = GfGfxLoader_GetScrnData(NARC_a_0_6_0, 20, FALSE, &screenData, HEAP_ID_8);
        BG_LoadScreenTilemapData(data->bgConfig, GF_BG_LYR_SUB_2, screenData->rawData, screenData->szByte);
        ScheduleBgTilemapBufferTransfer(data->bgConfig, GF_BG_LYR_SUB_2);
        Heap_Free(raw);
        break;
    }
}

int TouchMartApp_GetTouchedButton(TouchMartAppData *data, int buttonSet) {
    GF_ASSERT(buttonSet == 0 || buttonSet == 1);
    return TouchscreenHitbox_FindRectAtTouchNew(sTouchMartButtonSets[buttonSet]);
}

void TouchMartApp_SetupBgs(TouchMartAppData *data) {
    vu32 *dispcnt = &reg_GXS_DB_DISPCNT;
    GXS_SetGraphicsMode(GX_BGMODE_0);
    GX_SetBankForSubBG(GX_VRAM_SUB_BG_32_H);
    GX_SetBankForSubOBJ(GX_VRAM_SUB_OBJ_16_I);
    *dispcnt = (*dispcnt & 0xFFCFFFEF) | 0x10;
    InitBgFromTemplate(data->bgConfig, GF_BG_LYR_SUB_0, &sTouchMartBgTemplate_Sub0, GF_BG_TYPE_TEXT);
    InitBgFromTemplate(data->bgConfig, GF_BG_LYR_SUB_1, &sTouchMartBgTemplate_Sub1, GF_BG_TYPE_TEXT);
    InitBgFromTemplate(data->bgConfig, GF_BG_LYR_SUB_2, &sTouchMartBgTemplate_Sub2, GF_BG_TYPE_TEXT);
    BG_ClearCharDataRange(GF_BG_LYR_SUB_2, 0x20, 0, HEAP_ID_FIELD1);
    BgClearTilemapBufferAndCommit(data->bgConfig, GF_BG_LYR_SUB_2);
}

void TouchMartApp_LoadGraphics(TouchMartAppData *data) {
    reg_G2S_DB_BLDCNT = 0;
    GfGfxLoader_GXLoadPal(NARC_a_0_6_0, 15, GF_PAL_LOCATION_SUB_BG, GF_PAL_SLOT_0_OFFSET, 0, HEAP_ID_8);
    GfGfxLoader_LoadCharData(NARC_a_0_6_0, 16, data->bgConfig, GF_BG_LYR_SUB_1, 0, 0, FALSE, HEAP_ID_8);
    TouchMartApp_LoadMainTilemap(data, 0);
    LoadFontPal1(GF_PAL_LOCATION_SUB_BG, (enum GFPalSlotOffset)0x160, HEAP_ID_8);
    LoadFontPal0(GF_PAL_LOCATION_SUB_BG, (enum GFPalSlotOffset)0x180, HEAP_ID_8);
    TouchMartApp_AddWindows(data);
    TouchMartApp_AddItemWindows(data);
}

void TouchMartApp_FreeWindows(TouchMartAppData *data) {
    TouchMartApp_RemoveItemWindows(data);
    RemoveWindow(&data->unk_34);
    RemoveWindow(&data->messageWindow);
    RemoveWindow(&data->moneyWindow);
    RemoveWindow(&data->pageWindow);
    RemoveWindow(&data->cancelWindow);
}

void TouchMartApp_AddWindows(TouchMartAppData *data) {
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

void TouchMartApp_AddItemWindows(TouchMartAppData *data) {
    int i;
    for (i = 0; i < TOUCH_MART_ITEMS_PER_PAGE; i++) {
        AddWindowParameterized(data->bgConfig, &data->windows[i], GF_BG_LYR_SUB_0, (u8)sTouchMartItemWindowPositions[i].x, (u8)sTouchMartItemWindowPositions[i].y, 11, 4, 12, (u16)sTouchMartItemWindowPositions[i].baseTile);
    }
}

void TouchMartApp_RemoveItemWindows(TouchMartAppData *data) {
    int i = 0;
    Window *window = &data->windows[TOUCH_MART_WINDOW_ITEM_0];
    for (; i < TOUCH_MART_ITEMS_PER_PAGE; i++, window++) {
        ClearWindowTilemapAndCopyToVram(window);
        RemoveWindow(window);
    }
}

void TouchMartApp_PrintItemList(TouchMartAppData *data) {
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

    count = data->martData->unk270 - data->martData->unk271;
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
            index = i + martData->unk271;
            itemId = martData->unk268[index];
            string = NewString_ReadMsgData(data->itemNameMsgData, itemId);
            TouchMartApp_PrintItemName(data, window, string, i);
            String_Delete(string);
            if (ov31_0225E12C(data->martData, index, itemId)) {
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

void TouchMartApp_PrintItemName(TouchMartAppData *data, Window *window, String *string, int slot) {
    AddTextPrinterParameterizedWithColor(window, 0, string, 0, 0, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(1, 2, 0), NULL);
}

void TouchMartApp_PrintPrice(MessageFormat *messageFormat, MsgData *msgData, Window *window, s32 price, int martType) {
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

void TouchMartApp_PrintMoney(TouchMartAppData *data) {
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

void TouchMartApp_PrintPageNumber(TouchMartAppData *data) {
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
    page = (martData->unk271 + TOUCH_MART_ITEMS_PER_PAGE) / TOUCH_MART_ITEMS_PER_PAGE;
    itemCount = martData->unk270;
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

void TouchMartApp_EditListTilemap(BgConfig *bgConfig, int bgId, int itemCount) {
    void *tilemap = GetBgTilemapBuffer(bgConfig, (u8)bgId);
    int i;
    const TouchMartBgOp *op;
    if (itemCount == TOUCH_MART_ITEMS_PER_PAGE) {
        return;
    }
    op = sTouchMartListTilemapEdits[itemCount];
    for (i = 0; i < 4; i++, op++) {
        if (op->kind == 1) {
            LoadRectToBgTilemapRect(bgConfig, (u8)bgId, (u8 *)tilemap + (op->x + op->y * 32) * 2, op->destX, op->width, op->height, op->value);
        } else if (op->kind == 2) {
            FillBgTilemapRect(bgConfig, (u8)bgId, 0, op->destX, op->width, op->height, op->value, 0);
        }
    }
}

void TouchMartApp_LoadListTilemap(TouchMartAppData *data, int itemCount) {
    NNSG2dScreenData *screenData;
    void *raw = GfGfxLoader_GetScrnData(NARC_a_0_6_0, 18, FALSE, &screenData, HEAP_ID_8);
    BG_LoadScreenTilemapData(data->bgConfig, GF_BG_LYR_SUB_1, screenData->rawData, screenData->szByte);
    TouchMartApp_EditListTilemap(data->bgConfig, GF_BG_LYR_SUB_1, itemCount);
    ScheduleBgTilemapBufferTransfer(data->bgConfig, GF_BG_LYR_SUB_1);
    Heap_Free(raw);
}

BOOL ov31_0225E12C(MartData *martData, int index, int itemId) {
    if (martData->martType == 3) {
        return PokeathlonSave_GetUnkB7C_AtIndex(martData->pokeathlonSave, index) == 0;
    }
    if (martData->martType == 4) {
        return PokeathlonSave_GetUnkB78_AtIndex(martData->pokeathlonSave, index + ((itemId - 505) / 6) * 6) == 0;
    }
    return TRUE;
}

void TouchMartApp_PrintSelectedItem(TouchMartAppData *data) {
    MartData *martData;
    int index;
    int itemId;
    String *string;

    martData = data->martData;
    index = martData->unk290 + martData->unk271;
    string = NewString_ReadMsgData(data->itemNameMsgData, martData->unk268[index]);
    TouchMartApp_PrintItemName(data, &data->windows[TOUCH_MART_WINDOW_SELECTED_ITEM], string, index);
    String_Delete(string);
    martData = data->martData;
    itemId = martData->unk268[index];
    if (ov31_0225E12C(martData, index, itemId)) {
        int price;
        martData = data->martData;
        itemId = martData->unk268[index];
        price = MartData_GetItemPrice(martData, itemId);
        TouchMartApp_PrintPrice(data->messageFormat, data->msgData, &data->windows[TOUCH_MART_WINDOW_SELECTED_ITEM], price, martData->martType);
    }
    ScheduleWindowCopyToVram(&data->windows[TOUCH_MART_WINDOW_SELECTED_ITEM]);
}

void TouchMartApp_PrintInBag(TouchMartAppData *data, int unused) {
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

void TouchMartApp_PrintQuantity(TouchMartAppData *data, int quantity) {
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

void TouchMartApp_PrintBuyButton(TouchMartAppData *data) {
    String *string = NewString_ReadMsgData(data->msgData, msg_0435_00042);
    AddTextPrinterParameterizedWithColor(&data->windows[TOUCH_MART_WINDOW_BUY], 0, string, 4, 0, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(15, 14, 0), NULL);
    ScheduleWindowCopyToVram(&data->windows[TOUCH_MART_WINDOW_BUY]);
    String_Delete(string);
}

void TouchMartApp_BufferItemName(int martType, MessageFormat *messageFormat, int itemId, int fieldno) {
    switch (martType) {
    case 0:
    case 3:
    case 4:
        BufferItemName(messageFormat, fieldno, itemId);
        break;
    case 1:
        BufferDecorationName(messageFormat, fieldno, itemId);
        break;
    }
}

void TouchMartApp_BufferItemNamePlural(int martType, MessageFormat *messageFormat, int itemId, int fieldno) {
    switch (martType) {
    case 0:
    case 3:
    case 4:
        BufferItemNamePlural(messageFormat, fieldno, itemId);
        break;
    case 1:
        BufferDecorationName(messageFormat, fieldno, itemId);
        break;
    }
}

void TouchMartApp_BufferItemNameWithIndefArticle(int martType, MessageFormat *messageFormat, int itemId, int fieldno) {
    switch (martType) {
    case 0:
    case 3:
    case 4:
        BufferItemNameWithIndefArticle(messageFormat, fieldno, itemId);
        break;
    case 1:
        BufferDecorationName(messageFormat, fieldno, itemId);
        break;
    }
}

// force_active only sets a section flag here; kept so the object stays identical.
#pragma force_active on

void TouchMartApp_PrintHowMany(TouchMartAppData *data) {
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

void TouchMartApp_PrintConfirmPurchase(TouchMartAppData *data) {
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

#pragma force_active reset

void TouchMartApp_CreateYesNoPrompt(TouchMartAppData *data) {
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

BOOL TouchMartApp_HandleYesNoPrompt(TouchMartAppData *data) {
    int result;
    if (data->yesNoPrompt == NULL) {
        return FALSE;
    }
    result = YesNoPrompt_HandleInputForSave(data->yesNoPrompt);
    switch (result) {
    case YESNORESPONSE_YES:
        YesNoPrompt_Destroy(data->yesNoPrompt);
        data->yesNoPrompt = NULL;
        data->martData->unk294 = 0;
        return TRUE;
    case YESNORESPONSE_NO:
        YesNoPrompt_Destroy(data->yesNoPrompt);
        data->yesNoPrompt = NULL;
        data->martData->unk294 = 1;
        return TRUE;
    }
    return FALSE;
}

// force_active only sets a section flag here; kept so the object stays identical.
#pragma force_active on

void TouchMartApp_PrintPurchaseDone(TouchMartAppData *data) {
    String *template;
    MartData *martData = data->martData;
    if (martData->quantity > 1) {
        TouchMartApp_BufferItemNamePlural(martData->martType, data->messageFormat, martData->item, 0);
    } else {
        TouchMartApp_BufferItemName(martData->martType, data->messageFormat, martData->item, 0);
    }
    switch (data->martData->martType) {
    case 0:
        template = NewString_ReadMsgData(data->msgData, msg_0435_00015);
        BufferPocketName(data->messageFormat, 1, GetItemAttr(data->martData->item, ITEMATTR_FIELD_POCKET, HEAP_ID_FIELD2));
        break;
    case 3:
        if (data->martData->item >= 485 && data->martData->item <= 491) {
            template = NewString_ReadMsgData(data->msgData, msg_0435_00049);
        } else {
            template = NewString_ReadMsgData(data->msgData, msg_0435_00015);
            BufferPocketName(data->messageFormat, 1, GetItemAttr(data->martData->item, ITEMATTR_FIELD_POCKET, HEAP_ID_FIELD2));
        }
        break;
    case 4:
        template = NewString_ReadMsgData(data->msgData, msg_0435_00049);
        break;
    case 1:
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

#pragma force_active reset

BOOL TouchMartApp_PurchaseDonePrinterCallback(TextPrinterTemplate *template, u16 glyphId) {
    if (glyphId == 1) {
        PlaySE(SEQ_SE_DP_REGI);
    }
    return FALSE;
}

String *TouchMartApp_GetCannotAffordMessage(MartData *martData, MsgData *msgData) {
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

String *TouchMartApp_GetNoRoomMessage(const MartData *martData, MsgData *msgData) {
    int msgId;
    if (martData->martType == 0) {
        msgId = msg_0435_00016;
    } else if (martData->martType == 1) {
        msgId = msg_0435_00022;
    } else if (martData->martType == 2) {
        msgId = msg_0435_00024;
    } else if (martData->martType == 3) {
        msgId = msg_0435_00025;
    } else if (martData->martType == 4) {
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

void TouchMartApp_PrintCannotAfford(TouchMartAppData *data) {
    TOUCH_MART_PRINT_MESSAGE(data, TouchMartApp_GetCannotAffordMessage(data->martData, data->msgData));
}

void TouchMartApp_PrintNoRoom(TouchMartAppData *data) {
    TOUCH_MART_PRINT_MESSAGE(data, TouchMartApp_GetNoRoomMessage(data->martData, data->msgData));
}

void TouchMartApp_PrintNoRoomInBag(TouchMartAppData *data) {
    TOUCH_MART_PRINT_MESSAGE(data, NewString_ReadMsgData(data->msgData, msg_0435_00016));
}

void TouchMartApp_PrintPremierBallBonus(TouchMartAppData *data) {
    TOUCH_MART_PRINT_MESSAGE(data, NewString_ReadMsgData(data->msgData, msg_0435_00020));
}

void TouchMartApp_AddDetailWindows(TouchMartAppData *data) {
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

void TouchMartApp_RemoveDetailWindows(TouchMartAppData *data) {
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
