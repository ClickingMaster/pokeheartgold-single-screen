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

void BattleRegulationMenu_RemoveListMenuRegulations(BattleRegulationMenu *menu) {
    if (menu->listMenu[REGULATION_MENU_REGULATIONS]) {
        DestroyListMenu(menu->listMenu[REGULATION_MENU_REGULATIONS], NULL, NULL);
        sub_0200E5D4(&menu->windows[REGULATION_MENU_STRING_DESTINATION], TRUE);
        ScheduleBgTilemapBufferTransfer(menu->windows[REGULATION_MENU_STRING_DESTINATION].bgConfig, menu->windows[REGULATION_MENU_STRING_DESTINATION].bgId);
        RemoveWindow(&menu->windows[REGULATION_MENU_STRING_DESTINATION]);
        ListMenuItems_Delete(menu->items[REGULATION_MENU_REGULATIONS]);
        menu->listMenu[REGULATION_MENU_REGULATIONS] = NULL;
    }
}

int BattleRegulationMenu_ProcessListMenuInputRegulations(BattleRegulationMenu *menu) {
    int input = ListMenu_ProcessInput(menu->listMenu[REGULATION_MENU_REGULATIONS]);
    ListMenuGetScrollAndRow(menu->listMenu[REGULATION_MENU_REGULATIONS], 0, &menu->itemsAbove[REGULATION_MENU_REGULATIONS]);
    
    u16 prev = menu->unk80[REGULATION_MENU_REGULATIONS];
    ListMenuGetCurrentItemArrayId(menu->listMenu[REGULATION_MENU_REGULATIONS], &menu->unk80[REGULATION_MENU_REGULATIONS]);
    
    if (prev != menu->unk80[REGULATION_MENU_REGULATIONS]) {
        PlaySE(SEQ_SE_DP_SELECT);
    }
    
    switch (input) {
    case -1:
        return 0;
    case 12:
        PlaySE(SEQ_SE_DP_SELECT);
        menu->fieldSystem->linkBattleRuleset = NULL;
        BattleRegulationMenu_RemoveListMenuRegulations(menu);
        return 2;
    case -2:
        PlaySE(SEQ_SE_DP_SELECT);
        menu->fieldSystem->linkBattleRuleset = NULL;
        BattleRegulationMenu_RemoveListMenuRegulations(menu);
        return -1;
    default:
        PlaySE(SEQ_SE_DP_SELECT);
        &menu->fieldSystem->linkBattleRuleset->rules[0] = sub_020291E8(menu->fieldSystem->saveData, input);
        return 1;
    }
}

/*void BattleRegulationMenu_ShowListMenuConfirm(BattleRegulationMenu *menu) {
    u32 *menuEntry = ov03_02259838;
    menu->items[REGULATION_MENU_CONFIRM] = ListMenuItems_New(3, HEAP_ID_FIELD1);
    AddWindowParameterized(menu->fieldSystem->bgConfig, &menu->windows[REGULATION_MENU_WINDOW_CONFIRM], 3, 0x16, 10, 9, 6, 13, 0x201);
    DrawFrameAndWindow1(&menu->windows[REGULATION_MENU_WINDOW_CONFIRM], TRUE, 0x3D9, 11);

    for (int i = 0; i < 3; i++) {
        ListMenuItems_AppendFromMsgData(menu->items[REGULATION_MENU_CONFIRM], menu->msgData, menuEntry[0], menuEntry[1]);
        menuEntry += 2;
    };
    
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
    
    ov03_0225943C.totalItems = 3;
    ov03_0225943C.maxShowed = 3;
    ov03_0225943C.items = menu->items[REGULATION_MENU_CONFIRM];
    ov03_0225943C.window = &menu->windows[REGULATION_MENU_WINDOW_CONFIRM];
    ov03_0225943C.data = menu;
    menu->listMenu[REGULATION_MENU_CONFIRM] = ListMenuInit(&ov03_0225943C, 0, menu->itemsAbove[REGULATION_MENU_CONFIRM], HEAP_ID_FIELD1);
    CopyWindowToVram(&menu->windows[REGULATION_MENU_WINDOW_CONFIRM]);
}*/
