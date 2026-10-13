#ifndef POKEHEARTGOLD_FRONTIER_SYSTEM_H
#define POKEHEARTGOLD_FRONTIER_SYSTEM_H

#include "frontier/frontier_map.h"

#include "list_menu_2d.h"
#include "message_format.h"
#include "msgdata.h"
#include "pm_string.h"

struct FrontierScriptContext;

// Runs the frontier map's scripts: up to 8 script contexts (tasks) at a time, which share the map's script data
// (narc 182 member) and message bank unless a task is started for another map.
typedef struct FrontierSystem {
    void *frontierWork;                     // 0x00, the frontier app (Frontier_GetLaunchArgs / Frontier_GetData)
    struct FrontierScriptContext *tasks[8]; // 0x04
    u16 vars[8];                            // 0x24, script variables 0x8008-0x800F
    enum HeapID heapId;                     // 0x34
    u8 unk_38;                              // 0x38, tasks only run while 0
    u8 syncAborted;                         // 0x39, set while FrtCmd_203 waits when the partner stops the link
    u8 filler_3A[2];
    MsgData *msgData;                       // 0x3C
    u8 *scripts;                            // 0x40
    MessageFormat *msgFormat;               // 0x44
    String *msgBuffer;                      // 0x48
    String *msgTemplate;                    // 0x4C
    u8 textPrinterId;                       // 0x50
    s8 taskCount;                           // 0x51
    u16 mapId;                              // 0x52
    u8 filler_54[5];
    u8 movementCount;                       // 0x59, map object movements still running
    u8 messageWindowAdded;                  // 0x5A
    u8 filler_5B[5];
    void *menu;                             // 0x60
    Window messageWindow;                   // 0x64
    struct ListMenu2D *yesNoMenu;           // 0x74
    u8 filler_78[4];
    struct WaitingIcon *waitingIcon;        // 0x7C
    u8 filler_80[0x28];
    Window *castlePointsWindows[2];         // 0xA8, Battle Castle points (BattleCastle_ShowPointsWindows)
    u16 recordSaveState;                    // 0xB0, battle record save (FrtCmd_109)
    u16 recordWriteState;                   // 0xB2
    u16 recordType;                         // 0xB4
    u16 recordAmount;                       // 0xB6
    u16 *recordResult;                      // 0xB8
} FrontierSystem; // size 0xBC

FrontierSystem *FrontierSystem_Create(void *frontierWork, enum HeapID heapId, u32 mapId);
BOOL FrontierSystem_Main(FrontierSystem *system);
void FrontierSystem_Delete(FrontierSystem *system);
void FrontierSystem_AddTask(FrontierSystem *system, u16 mapId, u32 scriptIndex);
BOOL FrontierSystem_InsertTask(FrontierSystem *system, struct FrontierScriptContext *task);
void FrontierSystem_FreeTask(FrontierSystem *system, struct FrontierScriptContext *task);
void FrontierSystem_InitDummy(FrontierSystem *system, u16 mapId, u32 a2, u32 a3);
void FrontierSystem_LoadMapScripts(u8 **scripts, MsgData **msgData, u32 mapId, enum HeapID heapId);
void FrontierSystem_ChangeMsgBank(FrontierSystem *system, u16 mapId, enum HeapID heapId);
u16 *FrontierSystem_SaveVars(FrontierSystem *system, enum HeapID heapId);
void FrontierSystem_RestoreVars(FrontierSystem *system, u16 *saved);
void FrontierScriptContext_GoToScript(struct FrontierScriptContext *ctx, u32 scriptIndex);
u16 *FrontierSystem_GetVarPtr(FrontierSystem *system, u32 index);
FrontierMap *FrontierSystem_GetFrontierMap(FrontierSystem *system);

#endif
