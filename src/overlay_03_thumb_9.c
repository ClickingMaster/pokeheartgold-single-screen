#include "overlay_01.h"
#include "overlay_03.h"

#include "field_system.h"
#include "global.h"
#include "link_ruleset_data.h"
#include "mart.h"
#include "render_text.h"
#include "render_window.h"
#include "task.h"
#include "text.h"
#include "text_0205B4EC.h"
#include "unk_02005D10.h"
#include "unk_02034354.h"
#include "unk_02035900.h"
#include "unk_020379A0.h"
#include "unk_02037C94.h"

#include "msgdata/msg.naix"
#include "msgdata/msg/msg_0182.h"

extern UnkStruct_ov03* ov03_022598A0;

void ov03_02255A00();
void ov03_02255A2C(SysTask *task);
void ov03_02255A70(UnkStruct_ov03 *data);
void ov03_02255AB0(SysTask *task, void *data);
void ov03_02255ADC(void *data);
void ov03_02255B04(SysTask *task, void *data);
void ov03_02255B44();
void ov03_02255BEC();
void ov03_02255C54();

void sub_020356C0(u32 arg0);
int sub_0203775C(u32 arg0);
int sub_0203774C(u32 arg0);
void sub_02037EC0(u32 arg0);
void sub_02058180();

void ov03_02255A00() {
    if (IsPrintFinished(ov03_022598A0->printerID)) {
        ov03_02253E20(0, FALSE);
        ov03_02254B4C(&ov03_02255A2C);
    }
}

void ov03_02255A2C(SysTask *task) {
    if (IsPrintFinished(ov03_022598A0->printerID) && ((PAD_BUTTON_A | PAD_BUTTON_B) & gSystem.newKeys || System_GetTouchNew() == TRUE)) {
        SysTask_Destroy(task);
        ov03_02254B44();
        ov03_022598A0->unk88 = 1;
    }
}

void ov03_02255A70(UnkStruct_ov03 *data) {
    sub_020351DC(data->unk8E, ov03_022598A0->playerProfile[1]);
    BufferPlayersName(data->messageFormat[2], 1, ov03_022598A0->playerProfile[1]);
    ov03_02253E20(5, TRUE);
    sub_02058180();
    SysTask_CreateOnMainQueue(&ov03_02255AB0, data, 0);
}

void ov03_02255AB0(SysTask *task, void *data) {
    if (IsPrintFinished(ov03_022598A0->printerID)) {
        sub_02058180();
        ov03_022542C8(data);
        SysTask_Destroy(task);
    }
}

void ov03_02255ADC(void *data) {
    sub_02058180();
    for (int i = 0; i < 8; i++) {
        sub_020346E8(i);
    }
    SysTask_CreateOnMainQueue(&ov03_02255B04, data, 0);
}

void ov03_02255B04(SysTask *task, void *data) {
    if (IsPrintFinished(ov03_022598A0->printerID) && ((PAD_BUTTON_A | PAD_BUTTON_B) & gSystem.newKeys || System_GetTouchNew() == TRUE)) {
        ov03_022542C8(data);
        SysTask_Destroy(task);
    }
}

void ov03_02255B44() {
    sub_02037EC0(0);
    ov03_022598A0->unk88 = 2;
    sub_020398D4(1, 1);
    sub_02034434();
    sub_020356C0(1);
}

int ov03_02255B70() {
    return sub_0203775C(ov03_022598A0->unk91);
}

int ov03_02255B84() {
    return sub_0203774C(ov03_022598A0->unk91);
}

int ov03_02255B98() {
    return ov03_022598A0->unk92 + (ov03_022598A0->unk93 * 16);
}

void ov03_02255BB0(FieldSystem *fieldSystem, u32 arg1, u16 arg2, u16 arg3) {
    ov03_02253F74(fieldSystem);
    ov03_022598A0->unk91 = arg1;
    if (arg1 - 3 <= 1) {
        fieldSystem->linkBattleRuleset = NULL;
    }
    ov03_022598A0->unk92 = arg2;
    ov03_022598A0->unk93 = arg3;
    ov03_022540E8(ov03_022598A0);
}

void ov03_02255BEC() {
    ov03_022542C8(ov03_022598A0);
}

u32 ov03_02255BFC() {
    u32 unk88 = ov03_022598A0->unk88; // Needs to be reserved as the pointer will be null after the next line.
    if (unk88) {
        Unk03_Delete();
    }
    return unk88;
}

void ov03_02255C18(FieldSystem *fieldSystem, u32 arg1, u16 arg2, u16 arg3) {
    ov03_02253F74(fieldSystem);
    ov03_022598A0->unk91 = arg1;
    if (arg1 - 3 <= 1) {
        fieldSystem->linkBattleRuleset = NULL;
    }
    ov03_022598A0->unk92 = arg2;
    ov03_022598A0->unk93 = arg3;
    ov03_02254B2C(ov03_022598A0);
}

void ov03_02255C54() {
    ov03_02254B58(ov03_022598A0);
}

u32 ov03_02255C64() {
    u32 unk88 = ov03_022598A0->unk88;
    if (unk88) {
        Unk03_Delete();
    }
    return unk88;
}

BOOL ov03_02255C80() {
    return FALSE;
}

void ov03_02255C84(UnkStruct_ov03 *data) {
    ov01_021F6A9C(data->fieldSystem, 3, NULL);
    data->unkA8 = 0;
    data->unkA4 = 0;
}

BOOL ov03_02255CA0(UnkStruct_ov03 *data) {
    int unkInt = ov01_021F6B00(data->fieldSystem);
    BOOL unkBool = ov01_021F6B10(data->fieldSystem);
    if (unkInt == 3 && unkBool == TRUE) {
        ov01_021F6ABC(data->fieldSystem, 3, 3, &data->unkA4);
        return TRUE;
    }
    return FALSE;
}

BOOL ov03_02255CD0(UnkStruct_ov03 *data) {
    return ov01_021F6AEC(data->fieldSystem) >= 4;
}

void ov03_02255CE4(UnkStruct_ov03 *data) {
    ov01_021F6ABC(data->fieldSystem, 3, 11, &data->unkA4);
}

void BattleRegulationMenu_PrintMessage(BattleRegulationMenu *menu, int entryID) {
    if (WindowIsInUse(&menu->windows[REGULATION_MENU_WINDOW_MSGBOX]) == FALSE) {
        InitWindow(&menu->windows[REGULATION_MENU_WINDOW_MSGBOX]);
        sub_0205B514(menu->fieldSystem->bgConfig, &menu->windows[REGULATION_MENU_WINDOW_MSGBOX], 3);
        sub_0205B564(&menu->windows[REGULATION_MENU_WINDOW_MSGBOX], Save_PlayerData_GetOptionsAddr(menu->fieldSystem->saveData));
    } else {
        sub_0205B5A8(&menu->windows[REGULATION_MENU_WINDOW_MSGBOX]);
    }
    ReadMsgDataIntoString(menu->msgData, entryID, menu->strings[REGULATION_MENU_STRING_FMT]);
    StringExpandPlaceholders(menu->messageFormat, menu->strings[REGULATION_MENU_STRING_DESTINATION], menu->strings[REGULATION_MENU_STRING_FMT]);
    menu->printerID = sub_0205B5B4(&menu->windows[REGULATION_MENU_WINDOW_MSGBOX], menu->strings[REGULATION_MENU_STRING_DESTINATION], Save_PlayerData_GetOptionsAddr(menu->fieldSystem->saveData), 1);
}

void BattleRegulationMenu_RemoveMsgBox(BattleRegulationMenu *menu, BOOL clear) {
    if (clear) {
        ClearFrameAndWindow2(&menu->windows[REGULATION_MENU_WINDOW_MSGBOX], FALSE);
        ClearWindowTilemapAndCopyToVram(&menu->windows[REGULATION_MENU_WINDOW_MSGBOX]);
    }
    RemoveWindow(&menu->windows[REGULATION_MENU_WINDOW_MSGBOX]);
}

void BattleRegulationMenu_GetRegulationName(BattleRegulationMenu *menu, int index) {
    sub_0202921C(menu->fieldSystem->saveData, index, menu->strings[REGULATION_MENU_STRING_REGULATION_NAME], HEAP_ID_FIELD1);
    BufferString(menu->messageFormat, 0, menu->strings[REGULATION_MENU_STRING_REGULATION_NAME], 0, 1, LANGUAGE_ENGLISH);
}

/*void BattleRegulationMenu_ShowListMenuRegulations(BattleRegulationMenu *menu) {
    Window *window = &menu->windows[REGULATION_MENU_WINDOW_REGULATIONS];
    LinkBattleRuleset *ruleset = Save_LinkBattleRuleset_GetByIndex(menu->fieldSystem->saveData, 0);
    int items = 5;
    if (ruleset) {
        items++;
    }
    if (!WindowIsInUse(window)) {
        menu->items[REGULATION_MENU_REGULATIONS] = ListMenuItems_New(items + 2, HEAP_ID_FIELD1);
        AddWindowParameterized(menu->fieldSystem->bgConfig, window, 3, 1, 1, 16, (items + 2) * 2, 13, 1);
        DrawFrameAndWindow1(&menu->windows[REGULATION_MENU_WINDOW_REGULATIONS], TRUE, 0x3D9, 11);
        ListMenuItems_AppendFromMsgData(menu->items[REGULATION_MENU_REGULATIONS], menu->msgData, 0x8A, 12); // PokemonCenter2FCommon_Text_NoRestrictions?
        for (int i = 0; i < items; i++) {
            ov03_02255D8C(menu, i);
            ReadMsgDataIntoString(menu->msgData, 0x80, menu->strings[REGULATION_MENU_STRING_REGULATION_NAME]); // PokemonCenter2FCommon_Text_Cup?
            StringExpandPlaceholders(menu->messageFormat, menu->strings[REGULATION_MENU_STRING_CUP_NAME], menu->strings[REGULATION_MENU_STRING_REGULATION_NAME]);
            ListMenuItems_AddItem(menu->items[REGULATION_MENU_REGULATIONS], menu->strings[REGULATION_MENU_STRING_CUP_NAME], i);
        }
        ListMenuItems_AppendFromMsgData(menu->items[REGULATION_MENU_REGULATIONS], menu->msgData, 0x81, -2); // PokemonCenter2FCommon_Text_Cancel?
    }

    ListMenuTemplate ov03_0225943C = {
        NULL,     // LISTMENUITEM *items;
        0,        // LM_MoveCursorFunc_t moveCursorFunc;
        0,        // LM_ItemPrintFunc_t itemPrintFunc;
        NULL,     // Window *window;
        0,        // u16 totalItems;
        0,        // u16 maxShowed;
        0,        // u8 header_X;
        8,        // u8 item_X;
        0,        // u8 cursor_X;
        0,        // u8 upText_Y : 4;
        1,        // u8 cursorPal : 4;
        15,       // u8 fillValue : 4;
        3,        // u8 cursorShadowPal : 4;
        0,        // u16 lettersSpacing : 3;
        0,        // u16 itemVerticalPadding : 4;
        1,        // u16 scrollMultiple : 2;
        0,        // u16 fontId : 6;
        0,        // u16 cursorKind : 1;
        0,        // u32 unk_1C;
    };

    ov03_0225943C.totalItems = items + 2;
    ov03_0225943C.maxShowed = items + 2;
    ov03_0225943C.items = menu->items[REGULATION_MENU_REGULATIONS];
    ov03_0225943C.window = &menu->windows[REGULATION_MENU_WINDOW_REGULATIONS];
    ov03_0225943C.data = menu;
    
    menu->listMenu[REGULATION_MENU_REGULATIONS] = ListMenuInit(&ov03_0225943C, 0, menu->itemsAbove[REGULATION_MENU_REGULATIONS], HEAP_ID_FIELD1);
    CopyWindowToVram(&menu->windows[REGULATION_MENU_WINDOW_REGULATIONS]);
}*/