#ifndef POKEHEARTGOLD_FRONTIER_SCRIPT_UI_H
#define POKEHEARTGOLD_FRONTIER_SCRIPT_UI_H

#include "frontier/frontier_map.h"
#include "frontier/frontier_script_context.h"
#include "frontier/frontier_system.h"

#include "list_menu.h"
#include "list_menu_2d.h"
#include "pokemon.h"

#define FRONTIER_MENU_ITEMS_MAX      28
#define FRONTIER_MENU_NO_DESCRIPTION 0xFF

// Menu result values besides the selected item's value
#define FRONTIER_MENU_RESULT_WAITING 0xEEEE
#define FRONTIER_MENU_RESULT_CLOSE   0xEEDD
#define FRONTIER_MENU_RESULT_CANCEL  0xFFFE

// Text options of a message (speed, auto scroll, font).
typedef struct FrontierMessageParams {
    u8 speed;
    u8 autoScroll;
    u8 font;
    u8 unk_03;
} FrontierMessageParams;

// Script menus: a 2D menu (one column, FrtCmd_021/022) or a list menu (FrtCmd_026/027), with an optional description
// message per item printed into the frontier system's message window.
typedef struct FrontierMenu {
    FrontierSystem *system;                   // 0x000
    SysTask *task;                            // 0x004
    Window window;                            // 0x008
    Window *messageWindow;                    // 0x018
    String *strings[FRONTIER_MENU_ITEMS_MAX]; // 0x01C
    MsgData *msgData;                         // 0x08C
    MessageFormat *msgFormat;                 // 0x090
    u8 delay;                                 // 0x094, frames before input is read
    u8 unk_095;
    u8 initialSelection; // 0x096
    u8 canCancel : 1;    // 0x097
    u8 ownsMsgData : 1;
    u8 unk_097_2 : 4;
    u8 alignRight : 1;
    u8 alignBottom : 1;
    u8 x; // 0x098
    u8 y; // 0x099
    u8 unk_09A;
    u8 count; // 0x09B
    u8 filler_09C[4];
    u16 *result;                                     // 0x0A0
    struct ListMenu2DTemplate menuTemplate;          // 0x0A4
    struct ListMenu2D *menu;                         // 0x0B0
    ListMenuItem menuItems[FRONTIER_MENU_ITEMS_MAX]; // 0x0B4
    ListMenuTemplate listTemplate;                   // 0x194
    ListMenu *listMenu;                              // 0x1B4
    u16 unk_1B8;
    u16 listCursor;                                  // 0x1BA
    ListMenuItem listItems[FRONTIER_MENU_ITEMS_MAX]; // 0x1BC
    u16 descriptions[FRONTIER_MENU_ITEMS_MAX];       // 0x29C
    u16 listCursorPos;                               // 0x2D4
    u16 unk_2D6;
} FrontierMenu; // size 0x2D8

// Work of a map object's movement task (FrtCmd_040).
typedef struct FrontierMovementTask {
    u8 state;                        // 0x00
    u8 repeat;                       // 0x01
    s16 wait;                        // 0x02
    u16 id;                          // 0x04
    u8 *runningCount;                // 0x08, the frontier system's count of running movements
    const u16 *script;               // 0x0C, (opcode, count) pairs
    Field2DMoveQueue *unk_10; // 0x10, the map's unk30
    FrontierMapObject *object;       // 0x14
} FrontierMovementTask;

// One step passed to Field2DMoveQueue_Push (read there as Field2DMoveQueueEntry)
typedef struct FrontierMovementStep {
    u16 x;
    u16 y;
    u16 sequence;
    u8 direction;
    u8 id;
} FrontierMovementStep;

void FrontierSystem_PrintMessage(FrontierSystem *system, MsgData *msgData, u16 msgId, u16 canABSpeedUp, const FrontierMessageParams *params);
void FrontierSystem_ShowMessageWindow(FrontierSystem *system);
void FrontierSystem_FormatMessage(FrontierSystem *system, MsgData *msgData, u16 msgId);
void FrontierSystem_StartTextPrinter(FrontierSystem *system, u8 font, u32 speed, BOOL canABSpeedUp, BOOL autoScroll);
void FrontierSystem_CloseMessageWindow(FrontierSystem *system);
void FrontierSystem_PrintMailMessage(FrontierSystem *system, int speed, u16 bank, u16 number, u16 field0, u16 field1, u8 canABSpeedUp);
void FrontierScript_GetMailMessageString(String *dest, u16 bank, u16 number, u16 field0, u16 field1);
void FrontierMenu_Init(FrontierSystem *system, FrontierMenu *menu, u8 x, u8 y, u8 initialSelection, u8 canCancel, u16 *result, MessageFormat *msgFormat, MsgData *msgData);
FrontierMenu *FrontierMenu_New(FrontierSystem *system, u8 x, u8 y, u8 initialSelection, u8 canCancel, u16 *result, MessageFormat *msgFormat, MsgData *msgData);
void FrontierMenu_AddItem(FrontierMenu *menu, u16 msgId, u16 description, u16 value);
void FrontierMenu_Show(FrontierMenu *menu);
void FrontierMenu_AddItemInternal(FrontierMenu *menu, u32 msgId, u16 description, void *value);
u32 FrontierMenu_GetWidth(FrontierMenu *menu);
void FrontierMenu_InitTemplate(FrontierMenu *menu);
void FrontierMenu_Task(SysTask *task, FrontierMenu *menu);
void FrontierMenu_Close(FrontierMenu *menu);
void FrontierMenu_PrintDescription(FrontierMenu *menu);
FrontierMenu *FrontierListMenu_New(FrontierSystem *system, u8 x, u8 y, u8 initialSelection, u8 canCancel, u16 *result, MessageFormat *msgFormat, MsgData *msgData);
void FrontierListMenu_AddItem(FrontierMenu *menu, u16 msgId, u16 description, u16 value);
void FrontierListMenu_Show(FrontierMenu *menu);
void FrontierListMenu_AddItemInternal(FrontierMenu *menu, u32 msgId, u16 description, void *value);
u32 FrontierListMenu_GetWidth(FrontierMenu *menu);
void FrontierListMenu_InitTemplate(FrontierMenu *menu);
void FrontierListMenu_PrintItem(ListMenu *list, s32 value, u8 onInit);
u32 ov80_0222EC84(ListMenu *list);
void FrontierListMenu_Task(SysTask *task, FrontierMenu *menu);
void FrontierListMenu_Close(FrontierMenu *menu, int playSound);
void FrontierMenu_PrintMessage(FrontierMenu *menu, u16 msgId, u32 speed);
void FrontierListMenu_PrintDescription(FrontierMenu *menu);
void FrontierListMenu_Cancel(FrontierMenu *menu);
void FrontierScript_MovementTask(SysTask *task, FrontierMovementTask *data);
void FrontierScript_GetMovementStep(FrontierMovementStep *step, Field2DObject *object, u16 id, s32 opcode);
void FrontierMap_CreateMonSprite(FrontierMap *map, Pokemon *mon, enum HeapID heapId, int id, int x, int y, int drawPriority, int bgPriority, int blend, u16 blendColor);
void FrontierMap_DeleteMonSprite(FrontierMap *map, u32 id);
void FrontierMap_LoadMonIconResources(FrontierMap *map);
void FrontierMap_UnloadMonIconResources(FrontierMap *map);
ManagedSprite *FrontierMap_CreateMonIcon(FrontierMap *map, Pokemon *mon, s32 index, s32 x, s32 y);
void FrontierMap_DeleteMonIcon(FrontierMap *map, ManagedSprite *sprite, u32 index);
void FrontierMap_LoadItemIconResources(FrontierMap *map);
void FrontierMap_UnloadItemIconResources(FrontierMap *map);
ManagedSprite *FrontierMap_CreateItemIcon(FrontierMap *map, s16 x, s16 y);
void FrontierMap_DeleteItemIcon(FrontierMap *map, ManagedSprite *sprite);
void FrontierScript_PrintMessageEntry(FrontierScriptContext *ctx, const u16 *entry);
void FrontierScript_PrintMessageEntryFromBank(FrontierScriptContext *ctx, const u16 *entry, u32 bank);
BOOL FrontierScriptNative_WaitMessageEntry(FrontierScriptContext *ctx);
void FrontierMap_ShakeTask(SysTask *task, FrontierMapShake *shake);
void FrontierMap_WindowTask(SysTask *task, FrontierMapWindowTask *data);
void FrontierMenu_SetAlignRight(FrontierMenu *menu, int value);
void FrontierMenu_SetAlignBottom(FrontierMenu *menu, int value);

#endif // POKEHEARTGOLD_FRONTIER_SCRIPT_UI_H
