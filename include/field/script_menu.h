#ifndef POKEHEARTGOLD_FIELD_SCRIPT_MENU_H
#define POKEHEARTGOLD_FIELD_SCRIPT_MENU_H

#include "bg_window.h"
#include "field_system.h"
#include "list_menu.h"
#include "message_format.h"
#include "msgdata.h"
#include "sys_task.h"

// Script menus (multichoice, list, floor display) and the money, points and savings boxes.
typedef struct ScriptMenu ScriptMenu;

void ScriptMenu_Init(FieldSystem *fieldSystem, ScriptMenu *menu, u8 x, u8 y, u8 initCursorPos, u8 cancellable, u16 *ret_p, MessageFormat *msgFmt, Window *window, MsgData *msgData);
ScriptMenu *ScriptMenu_New(FieldSystem *fieldSystem, u8 x, u8 y, u8 initCursorPos, u8 cancellable, u16 *ret_p, MessageFormat *msgFmt, Window *window, MsgData *msgData);
void ScriptMenu_AddItem(ScriptMenu *menu, u32 msgId, u32 value);
void ScriptMenu_ShowMultichoice(ScriptMenu *menu);
void ScriptMenu_AppendItem(ScriptMenu *menu, u32 msgId, u32 value);
u32 ScriptMenu_GetItemsWidth(ScriptMenu *menu);
void ScriptMenu_SetupMultichoiceTemplate(ScriptMenu *menu);
void ScriptMenu_Free(ScriptMenu *menu);
void ScriptMenu_CloseMultichoice(ScriptMenu *menu);
ScriptMenu *ScriptMenu_Create(FieldSystem *fieldSystem, u8 x, u8 y, u8 initCursorPos, u8 cancellable, u16 *ret_p, MessageFormat *msgFmt, Window *window, MsgData *msgData);
void ScriptMenu_AddListItem(ScriptMenu *menu, int msgId, int a2, int value);
void ScriptMenu_ShowList(ScriptMenu *menu);
void ScriptMenu_ShowListWithWidth(ScriptMenu *menu, u16 width);
void ScriptMenu_OpenListWindow(ScriptMenu *menu, u32 width);
void ScriptMenu_ShowListAtPosition(ScriptMenu *menu, u16 *scroll_p, u16 *row_p);
void ScriptMenu_AppendListItem(ScriptMenu *menu, int msgId, int a2, int value);
u32 ScriptMenu_GetListItemsWidth(ScriptMenu *menu);
void ScriptMenu_SetupListTemplate(ScriptMenu *menu);
void ScriptMenu_CloseList(ScriptMenu *menu);
void ScriptMenu_PrintHelpMessage(ScriptMenu *menu, u32 msgId, u32 a2);
void ScriptMenu_PrintCursorHelpMessage(ScriptMenu *menu);
void PrintCurFloorInNewWindow(FieldSystem *fieldSystem, u8 x, u8 y, u16 *p_ret, MessageFormat *msgFmt, u16 floor);
void ScriptMenu_PrintMessage(ScriptMenu *menu, u16 msgId, u8 x, u8 y);
int GetFloorNameMessage(u32 mapId, int a1, u8 *width_p);
void ScriptMenu_ShowMultichoiceGrid(ScriptMenu *menu, u8 divisor);
void ScriptMenu_SetupGridTemplate(ScriptMenu *menu, u8 wide, u8 high);
Window *FieldSystem_ShowMoneyBox(FieldSystem *fieldSystem, u8 x, u8 y);
void MoneyBoxSys_Delete(Window *moneybox);
void MoneyBoxSys_Update(FieldSystem *fieldSystem, Window *moneybox);
Window *PointsBox_New(FieldSystem *fieldSystem, u8 type, u8 x, u8 y);
void PointsBox_Delete(Window *window);
void PointsBox_Update(FieldSystem *fieldSystem, Window *window, u8 type);
Window *SavingsBox_New(FieldSystem *fieldSystem, u8 x, u8 y);
void SavingsBox_Delete(Window *window);
void SavingsBox_Update(FieldSystem *fieldSystem, Window *window);
ListMenuItem *ScriptMenu_GetListItems(ScriptMenu *menu);
u8 ScriptMenu_GetCount(ScriptMenu *menu);
Window *ov01_021EEF68(FieldSystem *fieldSystem, u16 a1);
void ov01_021EEF88(Window *window);
void ScriptMenu_PrintItemHelpMessage(ScriptMenu *menu, int idx);
u8 ScriptMenu_IsCancellable(ScriptMenu *menu);
void ScriptMenu_SetAnchorRight(ScriptMenu *menu, int set);
void ScriptMenu_SetAnchorBottom(ScriptMenu *menu, int set);
void ScriptMenu_MultichoiceTask(SysTask *task, ScriptMenu *menu);
void ScriptMenu_ListItemPrintCallback(ScriptMenu *menu, s32 itemId, u8 onInit);
void ScriptMenu_ListMoveCursorCallback(ListMenu *listMenu, s32 itemId, u8 onInit);
void ScriptMenu_ListTask(SysTask *task, ScriptMenu *menu);
void ScriptMenu_FloorWindowTask(SysTask *task, ScriptMenu *menu);

#endif // POKEHEARTGOLD_FIELD_SCRIPT_MENU_H
