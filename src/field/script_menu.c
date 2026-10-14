#include "field/script_menu.h"

#include "global.h"

#include "constants/maps.h"
#include "constants/sndseq.h"

#include "font.h"
#include "gf_gfx_planes.h"
#include "heap.h"
#include "list_menu_2d.h"
#include "overlay_01.h"
#include "money_box.h"
#include "overlay_03.h"
#include "player_data.h"
#include "pm_string.h"
#include "render_window.h"
#include "save_pokegear.h"
#include "screen_fade.h"
#include "sound_playback.h"
#include "string_util.h"
#include "sys_task_api.h"
#include "system.h"
#include "text.h"
#include "coins.h"
#include "frontier_data.h"
#include "pokeathlon/pokeathlon_save.h"

typedef struct ScriptMenuItem {
    String *text;
    u32 value;
} ScriptMenuItem;

// A script multichoice (2D menu of entries) or scrolling list (items with optional help messages printed
// to printWindow); the chosen value or 0xFFFE (cancelled) is written to *ret_p.
struct ScriptMenu {
    /* 0x000 */ FieldSystem *fieldSystem;
    /* 0x004 */ SysTask *task;
    /* 0x008 */ Window window;
    /* 0x018 */ Window *printWindow;
    /* 0x01C */ String *strbufs[28];
    /* 0x08C */ MsgData *msgData;
    /* 0x090 */ MessageFormat *msgFmt;
    /* 0x094 */ u8 delay;
    /* 0x095 */ u8 unk_95;
    /* 0x096 */ u8 initCursorPos;
    /* 0x097 */ u8 cancellable : 1;
    u8 ownMsgData : 1;
    u8 unk_97_2 : 4;
    u8 anchorRight : 1;
    u8 anchorBottom : 1;
    /* 0x098 */ u8 x;
    /* 0x099 */ u8 y;
    /* 0x09A */ u8 unk_9A;
    /* 0x09B */ u8 count;
    /* 0x09C */ u8 unk_9C[4];
    /* 0x0A0 */ u16 *ret_p;
    /* 0x0A4 */ u16 *scroll_p;
    /* 0x0A8 */ u16 *row_p;
    /* 0x0AC */ struct ListMenu2DTemplate menu2d;
    /* 0x0B8 */ struct ListMenu2D *menu2dPtr;
    /* 0x0BC */ ScriptMenuItem entries[28];
    /* 0x19C */ ListMenuTemplate listTemplate;
    /* 0x1BC */ ListMenu *listMenu;
    /* 0x1C0 */ u16 unk_1C0;
    /* 0x1C2 */ u16 cursorPos;
    /* 0x1C4 */ ListMenuItem items[28];
    /* 0x2A4 */ u16 msgIds[28];
    /* 0x2DC */ u16 lastRow;
    /* 0x2DE */ u16 unk_2DE;
};



void ScriptMenu_Init(FieldSystem *fieldSystem, ScriptMenu *menu, u8 x, u8 y, u8 initCursorPos, u8 cancellable, u16 *ret_p, MessageFormat *msgFmt, Window *window, MsgData *msgData) {
    int i;

    if (msgData == NULL) {
        menu->msgData = NewMsgDataFromNarc(MSGDATA_LOAD_LAZY, NARC_msgdata_msg, 191, HEAP_ID_FIELD1);
        menu->ownMsgData = TRUE;
    } else {
        menu->msgData = msgData;
        menu->ownMsgData = FALSE;
    }
    menu->msgFmt = msgFmt;
    menu->fieldSystem = fieldSystem;
    menu->ret_p = ret_p;
    menu->scroll_p = NULL;
    menu->row_p = NULL;
    menu->cancellable = cancellable;
    menu->initCursorPos = initCursorPos;
    menu->anchorRight = 0;
    menu->anchorBottom = 0;
    menu->x = x;
    menu->y = y;
    menu->count = 0;
    menu->printWindow = window;
    menu->delay = 3;
    menu->lastRow = initCursorPos;
    for (i = 0; i < 28; i++) {
        menu->entries[i].text = NULL;
        menu->entries[i].value = 0;
    }
    for (i = 0; i < 28; i++) {
        menu->items[i].text = NULL;
        menu->items[i].value = 0;
        menu->msgIds[i] = 0xFF;
    }
    for (i = 0; i < 28; i++) {
        menu->strbufs[i] = String_New(80, HEAP_ID_FIELD1);
    }
    *menu->ret_p = 0xEEEE;
}

ScriptMenu *ScriptMenu_New(FieldSystem *fieldSystem, u8 x, u8 y, u8 initCursorPos, u8 cancellable, u16 *ret_p, MessageFormat *msgFmt, Window *window, MsgData *msgData) {
    ScriptMenu *menu = Heap_Alloc(HEAP_ID_FIELD1, sizeof(ScriptMenu));
    if (menu == NULL) {
        return NULL;
    }
    memset(menu, 0, sizeof(ScriptMenu));
    ScriptMenu_Init(fieldSystem, menu, x, y, initCursorPos, cancellable, ret_p, msgFmt, window, msgData);
    return menu;
}

void ScriptMenu_AddItem(ScriptMenu *menu, u32 msgId, u32 value) {
    ScriptMenu_AppendItem(menu, msgId, value);
}

void ScriptMenu_ShowMultichoice(ScriptMenu *menu) {
    u32 width;

    width = ScriptMenu_GetItemsWidth(menu);
    if ((width & 7) == 0) {
        width = width >> 3;
    } else {
        width = (width >> 3) + 1;
    }
    if (menu->anchorRight) {
        menu->x -= width;
    }
    if (menu->anchorBottom) {
        menu->y -= menu->count * 2;
    }
    AddWindowParameterized(menu->fieldSystem->bgConfig, &menu->window, GF_BG_LYR_MAIN_3, menu->x, menu->y, (u8)width, (u8)(menu->count * 2), 13, 0x3D);
    LoadUserFrameGfx1(menu->fieldSystem->bgConfig, GF_BG_LYR_MAIN_3, 0x3D9, 11, 0, HEAP_ID_FIELD1);
    DrawFrameAndWindow1(&menu->window, 1, 0x3D9, 11);
    ScriptMenu_SetupMultichoiceTemplate(menu);
    menu->menu2dPtr = Create2dMenu(&menu->menu2d, menu->initCursorPos, HEAP_ID_FIELD1);
    menu->task = SysTask_CreateOnMainQueue((SysTaskFunc)ScriptMenu_MultichoiceTask, menu, 0);
}

void ScriptMenu_AppendItem(ScriptMenu *menu, u32 msgId, u32 value) {
    String *tmp = String_New(80, HEAP_ID_FIELD1);
    ReadMsgDataIntoString(menu->msgData, msgId, tmp);
    StringExpandPlaceholders(menu->msgFmt, menu->strbufs[menu->count], tmp);
    menu->entries[menu->count].text = menu->strbufs[menu->count];
    String_Delete(tmp);
    menu->entries[menu->count].value = value;
    menu->count++;
}

u32 ScriptMenu_GetItemsWidth(ScriptMenu *menu) {
    u32 maxWidth = 0;
    int i;

    for (i = 0; i < menu->count; i++) {
        if (menu->entries[i].text == NULL) {
            break;
        }
        {
            u32 width = FontID_String_GetWidth(0, menu->entries[i].text, 0);
            if (maxWidth < width) {
                maxWidth = width;
            }
        }
    }
    return maxWidth + 12;
}

void ScriptMenu_SetupMultichoiceTemplate(ScriptMenu *menu) {
    menu->menu2d.items = (ListMenuItem *)menu->entries;
    menu->menu2d.window = &menu->window;
    menu->menu2d.fontId = 0;
    menu->menu2d.itemsWide = 1;
    menu->menu2d.itemsHigh = menu->count;
    menu->menu2d.yTop = 0;
    menu->menu2d.cursorType = 0;
    if (menu->count >= 4) {
        menu->menu2d.enableWrap = 1;
    } else {
        menu->menu2d.enableWrap = 0;
    }
}

void ScriptMenu_MultichoiceTask(SysTask *task, ScriptMenu *menu) {
    if (menu->delay != 0) {
        menu->delay--;
        return;
    }
    if (IsPaletteFadeFinished()) {
        int input = Handle2dMenuInput(menu->menu2dPtr);
        switch (input) {
        case -2:
            if (menu->cancellable == 1) {
                *menu->ret_p = 0xfffe;
                ScriptMenu_CloseMultichoice(menu);
                return;
            }
            break;
        case -1:
            break;
        default:
            *menu->ret_p = input;
            ScriptMenu_CloseMultichoice(menu);
            break;
        }
    }
}

void ScriptMenu_Free(ScriptMenu *menu) {
    int i;

    for (i = 0; i < 28; i++) {
        String_Delete(menu->strbufs[i]);
    }
    if (menu->ownMsgData == 1) {
        DestroyMsgData(menu->msgData);
    }
    Heap_Free(menu);
}

void ScriptMenu_CloseMultichoice(ScriptMenu *menu) {
    PlaySE(SEQ_SE_DP_SELECT);
    Delete2dMenu(menu->menu2dPtr, NULL);
    EraseFrameAndWindow1(menu->menu2d.window, 0);
    RemoveWindow(menu->menu2d.window);
    SysTask_Destroy(menu->task);
    ScriptMenu_Free(menu);
}

ScriptMenu *ScriptMenu_Create(FieldSystem *fieldSystem, u8 x, u8 y, u8 initCursorPos, u8 cancellable, u16 *ret_p, MessageFormat *msgFmt, Window *window, MsgData *msgData) {
    return ScriptMenu_New(fieldSystem, x, y, initCursorPos, cancellable, ret_p, msgFmt, window, msgData);
}

void ScriptMenu_AddListItem(ScriptMenu *menu, int msgId, int helpMsgId, int value) {
    ScriptMenu_AppendListItem(menu, msgId, helpMsgId, value);
}

// force_active only sets a section flag; it is kept so the objects stay identical to the matched ones
#pragma force_active on
void ScriptMenu_ShowList(ScriptMenu *menu) {
    u32 width = ScriptMenu_GetListItemsWidth(menu);
    u32 tiles;
    if ((width & 7) == 0) {
        tiles = width >> 3;
    } else {
        tiles = (width >> 3) + 1;
    }
    if (menu->anchorRight) {
        menu->x -= tiles;
    }
    if (menu->anchorBottom) {
        if (menu->count > 8) {
            menu->y -= 16;
        } else {
            menu->y -= menu->count * 2;
        }
    }
    ScriptMenu_OpenListWindow(menu, tiles);
}
#pragma force_active reset

void ScriptMenu_ShowListWithWidth(ScriptMenu *menu, u16 width) {
    ScriptMenu_OpenListWindow(menu, width);
}

void ScriptMenu_OpenListWindow(ScriptMenu *menu, u32 width) {
    if (menu->count > 8) {
        AddWindowParameterized(menu->fieldSystem->bgConfig, &menu->window, GF_BG_LYR_MAIN_3, menu->x, menu->y, (u8)width, 16, 13, 0x3D);
    } else {
        AddWindowParameterized(menu->fieldSystem->bgConfig, &menu->window, GF_BG_LYR_MAIN_3, menu->x, menu->y, (u8)width, (u8)(menu->count * 2), 13, 0x3D);
    }
    LoadUserFrameGfx1(menu->fieldSystem->bgConfig, GF_BG_LYR_MAIN_3, 0x3D9, 11, 0, HEAP_ID_FIELD1);
    DrawFrameAndWindow1(&menu->window, 1, 0x3D9, 11);
    ScriptMenu_SetupListTemplate(menu);
    menu->listMenu = ListMenuInit(&menu->listTemplate, 0, menu->initCursorPos, HEAP_ID_FIELD1);
    ScriptMenu_PrintCursorHelpMessage(menu);
    menu->task = SysTask_CreateOnMainQueue((SysTaskFunc)ScriptMenu_ListTask, menu, 0);
}

#pragma force_active on
void ScriptMenu_ShowListAtPosition(ScriptMenu *menu, u16 *scroll_p, u16 *row_p) {
    u32 width = ScriptMenu_GetListItemsWidth(menu);
    u32 tiles;
    if ((width & 7) == 0) {
        tiles = width >> 3;
    } else {
        tiles = (width >> 3) + 1;
    }
    if (menu->anchorRight) {
        menu->x -= tiles;
    }
    if (menu->anchorBottom) {
        if (menu->count > 8) {
            menu->y -= 16;
        } else {
            menu->y -= menu->count * 2;
        }
    }
    if (menu->count > 8) {
        AddWindowParameterized(menu->fieldSystem->bgConfig, &menu->window, GF_BG_LYR_MAIN_3, menu->x, menu->y, (u8)tiles, 16, 13, 0x3D);
    } else {
        AddWindowParameterized(menu->fieldSystem->bgConfig, &menu->window, GF_BG_LYR_MAIN_3, menu->x, menu->y, (u8)tiles, (u8)(menu->count * 2), 13, 0x3D);
    }
    LoadUserFrameGfx1(menu->fieldSystem->bgConfig, GF_BG_LYR_MAIN_3, 0x3D9, 11, 0, HEAP_ID_FIELD1);
    DrawFrameAndWindow1(&menu->window, TRUE, 0x3D9, 11);
    ScriptMenu_SetupListTemplate(menu);
    menu->scroll_p = scroll_p;
    menu->row_p = row_p;
    menu->lastRow = *menu->scroll_p + *menu->row_p;
    menu->listMenu = ListMenuInit(&menu->listTemplate, *scroll_p, *row_p, HEAP_ID_FIELD1);
    ScriptMenu_PrintCursorHelpMessage(menu);
    menu->task = SysTask_CreateOnMainQueue((SysTaskFunc)ScriptMenu_ListTask, menu, 0);
}
#pragma force_active reset

void ScriptMenu_AppendListItem(ScriptMenu *menu, int msgId, int helpMsgId, int value) {
    String *tmp = String_New(80, HEAP_ID_FIELD1);
    ReadMsgDataIntoString(menu->msgData, msgId, tmp);
    StringExpandPlaceholders(menu->msgFmt, menu->strbufs[menu->count], tmp);
    menu->items[menu->count].text = menu->strbufs[menu->count];
    String_Delete(tmp);
    if (value == 0xFA) {
        menu->items[menu->count].value = -3;
    } else {
        menu->items[menu->count].value = value;
    }
    menu->msgIds[menu->count] = helpMsgId;
    menu->count++;
}

u32 ScriptMenu_GetListItemsWidth(ScriptMenu *menu) {
    u32 maxWidth = 0;
    int i;

    for (i = 0; i < menu->count; i++) {
        if (menu->items[i].text == NULL) {
            break;
        }
        {
            u32 width = FontID_String_GetWidth(0, menu->items[i].text, 0);
            if (maxWidth < width) {
                maxWidth = width;
            }
        }
    }
    return maxWidth + 12;
}

void ScriptMenu_SetupListTemplate(ScriptMenu *menu) {
    menu->listTemplate.items = menu->items;
    menu->listTemplate.moveCursorFunc = (LM_MoveCursorFunc_t)ScriptMenu_ListMoveCursorCallback;
    menu->listTemplate.itemPrintFunc = (LM_ItemPrintFunc_t)ScriptMenu_ListItemPrintCallback;
    menu->listTemplate.window = &menu->window;
    menu->listTemplate.totalItems = menu->count;
    menu->listTemplate.maxShowed = 8;
    menu->listTemplate.header_X = 1;
    menu->listTemplate.item_X = 12;
    menu->listTemplate.cursor_X = 2;
    menu->listTemplate.upText_Y = 1;
    menu->listTemplate.cursorPal = 1;
    menu->listTemplate.fillValue = 15;
    menu->listTemplate.cursorShadowPal = 2;
    menu->listTemplate.lettersSpacing = 0;
    menu->listTemplate.itemVerticalPadding = 0;
    menu->listTemplate.scrollMultiple = 1;
    menu->listTemplate.fontId = 0;
    menu->listTemplate.cursorKind = 0;
    menu->listTemplate.data = menu;
}

void ScriptMenu_ListItemPrintCallback(ScriptMenu *menu, s32 itemId, u8 onInit) {
    if (itemId == -3) {
        ListMenuOverrideSetColors((ListMenu *)menu, 3, 15, 4);
    } else {
        ListMenuOverrideSetColors((ListMenu *)menu, 1, 15, 2);
    }
}

void ScriptMenu_ListMoveCursorCallback(ListMenu *listMenu, s32 itemId, u8 onInit) {
    u16 row;
    u16 scroll;
    ScriptMenu *menu;

    row = 0;
    scroll = 0;
    menu = (ScriptMenu *)ListMenuGetTemplateField(listMenu, (enum ListMenuAttr)0x13);
    ListMenuGetScrollAndRow(listMenu, &row, &scroll);
    if (menu->scroll_p != NULL && menu->row_p != NULL) {
        *menu->scroll_p = row;
        *menu->row_p = scroll;
    }
}

void ScriptMenu_ListTask(SysTask *task, ScriptMenu *menu) {
    if (menu->delay != 0) {
        menu->delay--;
        return;
    }
    if (IsPaletteFadeFinished()) {
        u16 oldRow;
        int input = ListMenu_ProcessInput(menu->listMenu);
        oldRow = menu->lastRow;
        ListMenuGetCurrentItemArrayId(menu->listMenu, &menu->lastRow);
        if (oldRow != menu->lastRow) PlaySE(SEQ_SE_DP_SELECT);
        int keys = gSystem.newAndRepeatedKeys;
        if ((keys & 0x40) || (keys & 0x80) || (keys & 0x20) || (keys & 0x10)) ScriptMenu_PrintCursorHelpMessage(menu);
        switch (input) {
        case -2:
            if (menu->cancellable == 1) {
                PlaySE(SEQ_SE_DP_SELECT);
                *menu->ret_p = 0xfffe;
                ScriptMenu_CloseList(menu);
                return;
            }
            break;
        case -1:
            break;
        default:
            PlaySE(SEQ_SE_DP_SELECT);
            *menu->ret_p = input;
            ScriptMenu_CloseList(menu);
            break;
        }
    }
}

void ScriptMenu_CloseList(ScriptMenu *menu) {
    int i;

    PlaySE(SEQ_SE_DP_SELECT);
    DestroyListMenu(menu->listMenu, NULL, NULL);
    EraseFrameAndWindow1(menu->listTemplate.window, 0);
    RemoveWindow(&menu->window);
    for (i = 0; i < 28; i++) {
        String_Delete(menu->strbufs[i]);
    }
    if (menu->ownMsgData == 1) {
        DestroyMsgData(menu->msgData);
    }
    SysTask_Destroy(menu->task);
    Heap_Free(menu);
}

void ScriptMenu_PrintHelpMessage(ScriptMenu *menu, u32 msgId, u32 textSpeed) {
    String *tmp = String_New(90, HEAP_ID_FIELD1);
    String *dest = String_New(90, HEAP_ID_FIELD1);
    FillWindowPixelBuffer(menu->printWindow, 15);
    ReadMsgDataIntoString(menu->msgData, msgId, tmp);
    StringExpandPlaceholders(menu->msgFmt, dest, tmp);
    AddTextPrinterParameterized(menu->printWindow, 1, dest, 0, 0, textSpeed, 0);
    String_Delete(tmp);
    String_Delete(dest);
}

void ScriptMenu_PrintCursorHelpMessage(ScriptMenu *menu) {
    ListMenuGetCurrentItemArrayId(menu->listMenu, &menu->cursorPos);
    if (menu->msgIds[menu->cursorPos] != 0xFF) {
        ScriptMenu_PrintHelpMessage(menu, menu->msgIds[menu->cursorPos], 0);
    }
}

// the matched code needs copy propagation off here
#pragma opt_propagation off
#pragma force_active on
void PrintCurFloorInNewWindow(FieldSystem *fieldSystem, u8 x, u8 y, u16 *p_ret, MessageFormat *msgFmt, u16 floor) {
    u32 tiles;
    u8 flag;
    ScriptMenu *menu;
    u32 width;
    u32 msg;

    menu = ScriptMenu_New(fieldSystem, x, y, 0, 0, p_ret, msgFmt, NULL, NULL);
    {
        u8 fontId = 0;
        width = GetFontAttribute(fontId, 0) * 8;
    }
    if (width % 8 == 0) {
        tiles = width / 8;
    } else {
        u32 roundedUp = 1 + width / 8;
        tiles = roundedUp;
    }
    AddWindowParameterized(menu->fieldSystem->bgConfig, &menu->window, GF_BG_LYR_MAIN_3, menu->x, menu->y, tiles, 4, 13, 221);
    LoadUserFrameGfx1(menu->fieldSystem->bgConfig, GF_BG_LYR_MAIN_3, 985, 11, 0, HEAP_ID_FIELD1);
    DrawFrameAndWindow1(&menu->window, TRUE, 985, 11);
    FillWindowPixelRect(&menu->window, 15, 0, 0, tiles * 8, 32);
    ScriptMenu_PrintMessage(menu, 16, 0, 0);
    msg = GetFloorNameMessage(fieldSystem->location->mapId, floor, &flag);
    ScriptMenu_PrintMessage(menu, (u16)msg, flag, 16);
    menu->menu2d.window = &menu->window;
    CopyWindowToVram(&menu->window);
    menu->task = SysTask_CreateOnMainQueue((SysTaskFunc)ScriptMenu_FloorWindowTask, menu, 0);
}
#pragma opt_propagation reset
#pragma force_active reset

#pragma force_active on
void ScriptMenu_PrintMessage(ScriptMenu *menu, u16 msgId, u8 x, u8 y) {
    String *tmp = String_New(90, HEAP_ID_FIELD1);
    String *dest = String_New(90, HEAP_ID_FIELD1);
    ReadMsgDataIntoString(menu->msgData, msgId, tmp);
    StringExpandPlaceholders(menu->msgFmt, dest, tmp);
    AddTextPrinterParameterized(&menu->window, 0, dest, x, y, TEXT_SPEED_NOTRANSFER, 0);
    String_Delete(tmp);
    String_Delete(dest);
}
#pragma force_active reset

void ScriptMenu_FloorWindowTask(SysTask *task, ScriptMenu *menu) {
    int i;

    if (*menu->ret_p != 0xFFFF) {
        return;
    }
    EraseFrameAndWindow1(menu->menu2d.window, 0);
    RemoveWindow(menu->menu2d.window);
    for (i = 0; i < 28; i++) {
        String_Delete(menu->strbufs[i]);
    }
    if (menu->ownMsgData == 1) {
        DestroyMsgData(menu->msgData);
    }
    SysTask_Destroy(menu->task);
    Heap_Free(menu);
}

int MapNumToFloorNo(int mapId) {
    switch (mapId) {
    case MAP_GOLDENROD_RADIO_TOWER_5F:
        return 0;
    case MAP_GOLDENROD_RADIO_TOWER_OBSERVATION_DECK:
        return 1;
    case MAP_OLIVINE_LIGHTHOUSE_1F:
        return 0;
    case MAP_OLIVINE_LIGHTHOUSE_LIGHT_ROOM:
        return 1;
    case MAP_SAFFRON_SILPH_CO_HQ:
        return 0;
    case MAP_SAFFRON_SILPH_CO_ROTOM_ROOM:
        return 1;
    case MAP_GOLDENROD_DEPARTMENT_STORE_1F:
        return 1;
    case MAP_GOLDENROD_DEPARTMENT_STORE_2F:
        return 2;
    case MAP_GOLDENROD_DEPARTMENT_STORE_3F:
        return 3;
    case MAP_GOLDENROD_DEPARTMENT_STORE_4F:
        return 4;
    case MAP_GOLDENROD_DEPARTMENT_STORE_5F:
        return 5;
    case MAP_GOLDENROD_DEPARTMENT_STORE_6F:
        return 6;
    case MAP_GOLDENROD_DEPARTMENT_STORE_BASEMENT:
        return 0;
    case MAP_CELADON_DEPARTMENT_STORE_1F:
        return 0;
    case MAP_CELADON_DEPARTMENT_STORE_2F:
        return 1;
    case MAP_CELADON_DEPARTMENT_STORE_3F:
        return 2;
    case MAP_CELADON_DEPARTMENT_STORE_4F:
        return 3;
    case MAP_CELADON_DEPARTMENT_STORE_5F:
        return 4;
    case MAP_CELADON_DEPARTMENT_STORE_ROOF:
        return 5;
    case MAP_CELADON_CONDOMINIUMS_1F:
        return 0;
    case MAP_CELADON_CONDOMINIUMS_2F:
        return 1;
    case MAP_CELADON_CONDOMINIUMS_3F:
        return 2;
    case MAP_CELADON_CONDOMINIUMS_ROOF:
        return 3;
    default:
        GF_AssertFail();
        return 0;
    }
}

int GetFloorNameMessage(u32 mapId, int floor, u8 *width_p) {
    switch (mapId) {
    case 0x1B9:
        if (floor == 0) {
            *width_p = 0x20;
            return 0x7A;
        }
        break;
    case 0x1BB:
    case 0x1BC:
        if (floor == 3) {
            *width_p = 0x10;
            return 0x7D;
        }
        break;
    }
    *width_p = 0x20;
    return 0x11;
}

void ScriptMenu_ShowMultichoiceGrid(ScriptMenu *menu, u8 divisor) {
    u32 width;
    int rows;
    u8 columns;

    width = ScriptMenu_GetItemsWidth(menu);
    if ((width & 7) == 0) {
        rows = width >> 3;
    } else {
        rows = (width >> 3) + 1;
    }
    columns = menu->count / divisor;
    if (menu->count % divisor) {
        columns++;
    }
    AddWindowParameterized(menu->fieldSystem->bgConfig, &menu->window, GF_BG_LYR_MAIN_3, menu->x, menu->y, (u8)(rows * divisor), (u8)(columns * 2), 13, 0x3D);
    LoadUserFrameGfx1(menu->fieldSystem->bgConfig, GF_BG_LYR_MAIN_3, 0x3D9, 11, 0, HEAP_ID_FIELD1);
    DrawFrameAndWindow1(&menu->window, TRUE, 0x3D9, 11);
    ScriptMenu_SetupGridTemplate(menu, divisor, columns);
    menu->menu2dPtr = Create2dMenu(&menu->menu2d, menu->initCursorPos, HEAP_ID_FIELD1);
    menu->task = SysTask_CreateOnMainQueue((SysTaskFunc)ScriptMenu_MultichoiceTask, menu, 0);
}

void ScriptMenu_SetupGridTemplate(ScriptMenu *menu, u8 wide, u8 high) {
    menu->menu2d.items = (ListMenuItem *)menu->entries;
    menu->menu2d.window = &menu->window;
    menu->menu2d.fontId = 0;
    menu->menu2d.itemsWide = wide;
    menu->menu2d.itemsHigh = high;
    menu->menu2d.yTop = 0;
    menu->menu2d.cursorType = 0;
}

Window *FieldSystem_ShowMoneyBox(FieldSystem *fieldSystem, u8 x, u8 y) {
    Window *window;
    MsgData *msgData;
    String *string;

    window = AllocWindows(HEAP_ID_FIELD1, 1);
    AddWindowParameterized(fieldSystem->bgConfig, window, GF_BG_LYR_MAIN_3, x, y, 10, 4, 13, 1);
    LoadUserFrameGfx1(fieldSystem->bgConfig, GF_BG_LYR_MAIN_3, 0x3D9, 11, 0, HEAP_ID_FIELD1);
    DrawFrameAndWindow1(window, TRUE, 0x3D9, 11);
    FillWindowPixelBuffer(window, 15);
    msgData = NewMsgDataFromNarc((MsgDataLoadType)0, NARC_msgdata_msg, 0x1B3, HEAP_ID_FIELD1);
    string = NewString_ReadMsgData(msgData, 30);
    AddTextPrinterParameterized(window, 0, string, 0, 0, 0xFF, 0);
    DestroyMsgData(msgData);
    String_Delete(string);
    MoneyBoxSys_Update(fieldSystem, window);
    return window;
}

void MoneyBoxSys_Delete(Window *moneybox) {
    EraseFrameAndWindow1(moneybox, 0);
    WindowArray_Delete(moneybox, 1);
}

// Second line of the money box: the player's money, right-aligned.
void MoneyBoxSys_Update(FieldSystem *fieldSystem, Window *moneybox) {
    MsgData *msgData;
    MessageFormat *msgFmt;
    String *string;
    String *template;
    int x;

    FillWindowPixelRect(moneybox, 15, 0, 16, 80, 16);
    msgData = NewMsgDataFromNarc(MSGDATA_LOAD_DIRECT, NARC_msgdata_msg, 0x1B3, HEAP_ID_FIELD1);
    msgFmt = MessageFormat_New(HEAP_ID_FIELD1);
    string = String_New(16, HEAP_ID_FIELD1);
    template = NewString_ReadMsgData(msgData, 31);
    BufferIntegerAsString(msgFmt, 0, PlayerProfile_GetMoney(Save_PlayerData_GetProfile(fieldSystem->saveData)), 6, PRINTING_MODE_RIGHT_ALIGN, TRUE);
    StringExpandPlaceholders(msgFmt, string, template);
    x = 80 - FontID_String_GetWidth(0, string, 0);
    AddTextPrinterParameterized(moneybox, 0, string, x, 16, TEXT_SPEED_NOTRANSFER, NULL);
    String_Delete(template);
    String_Delete(string);
    MessageFormat_Delete(msgFmt);
    DestroyMsgData(msgData);
    ScheduleWindowCopyToVram(moneybox);
}

#pragma force_active on
Window *PointsBox_New(FieldSystem *fieldSystem, u8 type, u8 x, u8 y) {
    Window *window = AllocWindows(HEAP_ID_FIELD1, 1);
    AddWindowParameterized(fieldSystem->bgConfig, window, GF_BG_LYR_MAIN_3, x, y, 10, 2, 13, 41);
    LoadUserFrameGfx1(fieldSystem->bgConfig, GF_BG_LYR_MAIN_3, 0x3D9, 11, 0, HEAP_ID_FIELD1);
    DrawFrameAndWindow1(window, TRUE, 0x3D9, 11);
    PointsBox_Update(fieldSystem, window, type);
    return window;
}
#pragma force_active reset

void PointsBox_Delete(Window *window) {
    EraseFrameAndWindow1(window, 0);
    WindowArray_Delete(window, 1);
}

#pragma force_active on
// type 0: coins, 1: Battle Points, otherwise: athlete points.
void PointsBox_Update(FieldSystem *fieldSystem, Window *window, u8 type) {
    MsgData *msgData;
    MessageFormat *msgFmt;
    String *string;
    String *template;
    int value;
    int x;

    FillWindowPixelBuffer(window, 15);
    msgData = NewMsgDataFromNarc(MSGDATA_LOAD_DIRECT, NARC_msgdata_msg, 191, HEAP_ID_FIELD1);
    msgFmt = MessageFormat_New(HEAP_ID_FIELD1);
    string = String_New(16, HEAP_ID_FIELD1);
    switch (type) {
    case 0:
        template = NewString_ReadMsgData(msgData, 193);
        value = Coins_GetValue(Save_PlayerData_GetCoinsAddr(fieldSystem->saveData));
        break;
    case 1:
        template = NewString_ReadMsgData(msgData, 220);
        value = FrontierData_BattlePointAction(Save_FrontierData_Get(fieldSystem->saveData), 0, 0);
        break;
    case 2:
    default:
        template = NewString_ReadMsgData(msgData, 223);
        value = PokeathlonSave_GetAthletePoints(Save_Pokeathlon_Get(fieldSystem->saveData));
        break;
    }
    BufferIntegerAsString(msgFmt, 0, value, 5, PRINTING_MODE_RIGHT_ALIGN, TRUE);
    StringExpandPlaceholders(msgFmt, string, template);
    x = 80 - FontID_String_GetWidth(0, string, 0);
    AddTextPrinterParameterized(window, 0, string, x, 0, TEXT_SPEED_NOTRANSFER, NULL);
    String_Delete(template);
    String_Delete(string);
    MessageFormat_Delete(msgFmt);
    DestroyMsgData(msgData);
    ScheduleWindowCopyToVram(window);
}

// The savings window: Mom's savings and the player's money.
Window *SavingsBox_New(FieldSystem *fieldSystem, u8 x, u8 y) {
    Window *window = AllocWindows(HEAP_ID_FIELD1, 1);
    MsgData *msgData;
    String *string;
    AddWindowParameterized(fieldSystem->bgConfig, window, GF_BG_LYR_MAIN_3, x, y, 16, 8, 13, 1);
    LoadUserFrameGfx1(fieldSystem->bgConfig, GF_BG_LYR_MAIN_3, 0x3D9, 11, 0, HEAP_ID_FIELD1);
    DrawFrameAndWindow1(window, TRUE, 0x3D9, 11);
    FillWindowPixelBuffer(window, 15);
    msgData = NewMsgDataFromNarc(MSGDATA_LOAD_DIRECT, NARC_msgdata_msg, 191, HEAP_ID_FIELD1);
    string = NewString_ReadMsgData(msgData, 330);
    AddTextPrinterParameterized(window, 0, string, 0, 0, TEXT_SPEED_NOTRANSFER, NULL);
    String_Delete(string);
    string = NewString_ReadMsgData(msgData, 331);
    AddTextPrinterParameterized(window, 0, string, 0, 32, TEXT_SPEED_NOTRANSFER, NULL);
    String_Delete(string);
    DestroyMsgData(msgData);
    SavingsBox_Update(fieldSystem, window);
    return window;
}
#pragma force_active reset

void SavingsBox_Delete(Window *window) {
    EraseFrameAndWindow1(window, 0);
    WindowArray_Delete(window, 1);
}

#pragma force_active on
void SavingsBox_Update(FieldSystem *fieldSystem, Window *window) {
    FillWindowPixelRect(window, 15, 0, 128, 128, 16);
    FillWindowPixelRect(window, 15, 0, 384, 128, 16);
    MsgData *msgData = NewMsgDataFromNarc(MSGDATA_LOAD_DIRECT, NARC_msgdata_msg, 191, HEAP_ID_FIELD1);
    MessageFormat *msgFmt = MessageFormat_New(HEAP_ID_FIELD1);
    String *string = String_New(16, HEAP_ID_FIELD1);
    String *template = NewString_ReadMsgData(msgData, 332);
    u32 money = PhoneCallPersistentState_MomSavings_BalanceAction(SaveData_GetPhoneCallPersistentState(fieldSystem->saveData), 0, 0);
    BufferIntegerAsString(msgFmt, 0, money, 6, PRINTING_MODE_LEFT_ALIGN, TRUE);
    StringExpandPlaceholders(msgFmt, string, template);
    u32 x = 128 - FontID_String_GetWidth(0, string, 0);
    AddTextPrinterParameterized(window, 0, string, x, 16, TEXT_SPEED_NOTRANSFER, NULL);
    money = PlayerProfile_GetMoney(Save_PlayerData_GetProfile(fieldSystem->saveData));
    BufferIntegerAsString(msgFmt, 0, money, 6, PRINTING_MODE_LEFT_ALIGN, TRUE);
    StringExpandPlaceholders(msgFmt, string, template);
    x = 128 - FontID_String_GetWidth(0, string, 0);
    AddTextPrinterParameterized(window, 0, string, x, 48, TEXT_SPEED_NOTRANSFER, NULL);
    String_Delete(template);
    String_Delete(string);
    MessageFormat_Delete(msgFmt);
    DestroyMsgData(msgData);
    ScheduleWindowCopyToVram(window);
}
#pragma force_active reset

ListMenuItem *ScriptMenu_GetListItems(ScriptMenu *menu) {
    return menu->items;
}

u8 ScriptMenu_GetCount(ScriptMenu *menu) {
    return menu->count;
}

Window *BattleRegulationWindow_New(FieldSystem *fieldSystem, u16 ruleset) {
    Window *windows = AllocWindows(HEAP_ID_FIELD1, 1);
    BattleRegulation_PrintRulesWindow(fieldSystem, windows, ruleset);
    return windows;
}

void BattleRegulationWindow_Delete(Window *window) {
    EraseFrameAndWindow1(window, 0);
    WindowArray_Delete(window, 1);
}

void ScriptMenu_PrintItemHelpMessage(ScriptMenu *menu, int idx) {
    String *tmp;
    String *dest;

    if (menu->msgIds[idx] == 0xFF) {
        return;
    }
    tmp = String_New(90, HEAP_ID_FIELD1);
    dest = String_New(90, HEAP_ID_FIELD1);
    FillWindowPixelBuffer(menu->printWindow, 15);
    ReadMsgDataIntoString(menu->msgData, menu->msgIds[idx], tmp);
    StringExpandPlaceholders(menu->msgFmt, dest, tmp);
    AddTextPrinterParameterized(menu->printWindow, 1, dest, 0, 0, 0, 0);
    String_Delete(tmp);
    String_Delete(dest);
}

u8 ScriptMenu_IsCancellable(ScriptMenu *menu) {
    return menu->cancellable;
}

void ScriptMenu_SetAnchorRight(ScriptMenu *menu, int set) {
    menu->anchorRight = (u8)set;
}

void ScriptMenu_SetAnchorBottom(ScriptMenu *menu, int set) {
    menu->anchorBottom = (u8)set;
}
