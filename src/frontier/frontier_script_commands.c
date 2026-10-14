#include "global.h"

#include "frontier/frontier.h"
#include "frontier/frontier_map.h"
#include "frontier/frontier_script_context.h"
#include "frontier/frontier_script_ui.h"
#include "frontier/frontier_system.h"

#include "communication_player_state.h"
#include "filesystem.h"
#include "heap.h"
#include "list_menu_2d.h"
#include "player_data.h"
#include "render_text.h"
#include "screen_fade.h"
#include "sound_playback.h"
#include "sys_task_api.h"
#include "system.h"
#include "text.h"
#include "communication_core.h"

// Script variables: 0x8000-0x8007 are the task's own, 0x8008-0x800F the frontier system's (shared by all tasks),
// 0x8010-0x8013 the task's data words. Other values are literals.
#define FRONTIER_VAR_TASK_START   0x8000
#define FRONTIER_VAR_SYSTEM_START 0x8008
#define FRONTIER_VAR_DATA_START   0x8010
#define FRONTIER_VAR_END          0x8014

// List terminator and placeholders in the scripts' object / graphics lists
#define FRONTIER_LIST_END     0xFD13
#define FRONTIER_ID_PLAYER    0xEEEE
#define FRONTIER_ID_LINK_MONS 0xEEEF

typedef Field2DObject *(*FrontierMapGetObjectFunc)(Field2DObjectManager *objectManager, u16 id);

// Result of the comparison commands (0: less, 1: equal, 2: greater) x the condition byte of a jump/call.
extern const u8 ov80_0223B9D8[][3];
extern const WindowTemplate ov80_0223B9D0;

void *Frontier_GetMap(void *frontierWork);
void Frontier_RequestExit(void *frontierWork);
void Frontier_ChangeMode(void *frontierWork, u16 mode, u16 taskId);
u16 Frontier_GetPlayerSpriteId(PlayerProfile *profile);

u16 *FrontierScript_ReadVarPtr(FrontierScriptContext *ctx);
u16 FrontierScript_ReadVar(FrontierScriptContext *ctx);
u16 *FrontierScript_GetVarPtr(FrontierScriptContext *ctx, u32 index);
u16 FrontierScript_GetVar(FrontierScriptContext *ctx, u16 index);
BOOL FrtCmd_Nop(FrontierScriptContext *ctx);
BOOL FrtCmd_End(FrontierScriptContext *ctx);
BOOL FrtCmd_Exit(FrontierScriptContext *ctx);
BOOL FrtCmd_ChangeMode(FrontierScriptContext *ctx);
BOOL FrontierScriptNative_WaitFrame(FrontierScriptContext *ctx);
BOOL FrtCmd_ChangeModeWithTask(FrontierScriptContext *ctx);
BOOL FrtCmd_Wait(FrontierScriptContext *ctx);
BOOL FrontierScriptNative_WaitFrames(FrontierScriptContext *ctx);
BOOL FrtCmd_SetVar(FrontierScriptContext *ctx);
BOOL FrtCmd_CopyVar(FrontierScriptContext *ctx);
BOOL FrtCmd_AddVar(FrontierScriptContext *ctx);
BOOL FrtCmd_SubVar(FrontierScriptContext *ctx);
u8 FrontierScript_Compare(u16 a, u16 b);
BOOL FrtCmd_CompareVarToValue(FrontierScriptContext *ctx);
BOOL FrtCmd_CompareVarToVar(FrontierScriptContext *ctx);
void FrontierScript_Jump(FrontierScriptContext *ctx, const u8 *ptr);
BOOL FrtCmd_GoTo(FrontierScriptContext *ctx);
BOOL FrtCmd_GoToIf(FrontierScriptContext *ctx);
BOOL FrtCmd_Call(FrontierScriptContext *ctx);
BOOL FrtCmd_Return(FrontierScriptContext *ctx);
BOOL FrtCmd_CallIf(FrontierScriptContext *ctx);
BOOL FrtCmd_PrintMessageInstant(FrontierScriptContext *ctx);
BOOL FrtCmd_PrintMessage(FrontierScriptContext *ctx);
BOOL FrtCmd_PrintMessageCanSpeedUp(FrontierScriptContext *ctx);
BOOL FrontierScriptNative_WaitMessage(FrontierScriptContext *ctx);
BOOL FrtCmd_CloseMessage(FrontierScriptContext *ctx);
BOOL FrtCmd_FadeScreen(FrontierScriptContext *ctx);
BOOL FrtCmd_WaitFade(FrontierScriptContext *ctx);
BOOL FrontierScriptNative_WaitPaletteFade(FrontierScriptContext *ctx);
BOOL FrtCmd_MenuInitStdGmm(FrontierScriptContext *ctx);
BOOL FrtCmd_MenuInit(FrontierScriptContext *ctx);
BOOL FrtCmd_MenuItemAdd(FrontierScriptContext *ctx);
BOOL FrtCmd_MenuItemAddWithDescription(FrontierScriptContext *ctx);
BOOL FrtCmd_MenuExec(FrontierScriptContext *ctx);
BOOL FrontierScriptNative_WaitMenu(FrontierScriptContext *ctx);
BOOL FrtCmd_ListMenuInitStdGmm(FrontierScriptContext *ctx);
BOOL FrtCmd_ListMenuInit(FrontierScriptContext *ctx);
BOOL FrtCmd_ListMenuItemAdd(FrontierScriptContext *ctx);
BOOL FrtCmd_ListMenuExec(FrontierScriptContext *ctx);
BOOL FrtCmd_ListMenuCancel(FrontierScriptContext *ctx);
BOOL FrtCmd_YesNo(FrontierScriptContext *ctx);
BOOL FrontierScriptNative_WaitYesNo(FrontierScriptContext *ctx);
BOOL FrtCmd_LoadObjectGfx(FrontierScriptContext *ctx);
BOOL FrtCmd_RemoveObjectGfx(FrontierScriptContext *ctx);
BOOL FrtCmd_CreateObjects(FrontierScriptContext *ctx);
BOOL FrtCmd_DeleteObject(FrontierScriptContext *ctx);
BOOL FrtCmd_SetObjectVisible(FrontierScriptContext *ctx);
BOOL FrtCmd_Dummy039(FrontierScriptContext *ctx);
BOOL FrtCmd_ApplyMovement(FrontierScriptContext *ctx);
void FrontierScript_StartMovement(int id, FrontierMapObject *object, const u8 *script, void *unk3, u8 *runningCount, enum HeapID heapId);
BOOL FrtCmd_WaitMovement(FrontierScriptContext *ctx);
BOOL FrontierScriptNative_WaitMovements(FrontierScriptContext *ctx);
BOOL FrtCmd_LoadSpriteResources(FrontierScriptContext *ctx);
BOOL FrtCmd_UnloadSpriteResources(FrontierScriptContext *ctx);

static inline u8 ReadScriptByte(FrontierScriptContext *ctx) {
    const u8 *ptr = ctx->scriptPtr;
    ctx->scriptPtr = ptr + 1;
    return *ptr;
}

u16 *FrontierScript_ReadVarPtr(FrontierScriptContext *ctx) {
    u16 *ptr = FrontierScript_GetVarPtr(ctx, FrontierScriptContext_ReadHalfWord(ctx));
    if (ptr == NULL) {
        GF_AssertFail();
    }
    return ptr;
}

u16 FrontierScript_ReadVar(FrontierScriptContext *ctx) {
    return FrontierScript_GetVar(ctx, FrontierScriptContext_ReadHalfWord(ctx));
}

u16 *FrontierScript_GetVarPtr(FrontierScriptContext *ctx, u32 index) {
    if (index < FRONTIER_VAR_TASK_START) {
        return NULL;
    }
    if (index < FRONTIER_VAR_SYSTEM_START) {
        return &ctx->taskVars[index - FRONTIER_VAR_TASK_START];
    }
    if (index < FRONTIER_VAR_DATA_START) {
        return FrontierSystem_GetVarPtr(ctx->frontierSystem, index - FRONTIER_VAR_SYSTEM_START);
    }
    if (index < FRONTIER_VAR_END) {
        return &ctx->data[index - FRONTIER_VAR_DATA_START];
    }
    if (index == FRONTIER_LIST_END) {
        return NULL;
    }
    if (index == FRONTIER_ID_PLAYER) {
        return NULL;
    }
    if (index == FRONTIER_ID_LINK_MONS) {
        return NULL;
    }
    GF_AssertFail();
    return NULL;
}

u16 FrontierScript_GetVar(FrontierScriptContext *ctx, u16 index) {
    u16 *ptr = FrontierScript_GetVarPtr(ctx, index);
    if (ptr != NULL) {
        index = *ptr;
    }
    return index;
}

BOOL FrtCmd_Nop(FrontierScriptContext *ctx) {
    return FALSE;
}

BOOL FrtCmd_End(FrontierScriptContext *ctx) {
    FrontierScriptContext_Stop(ctx);
    return FALSE;
}

BOOL FrtCmd_Exit(FrontierScriptContext *ctx) {
    FrontierScriptContext_Stop(ctx);
    Frontier_RequestExit(ctx->frontierSystem->frontierWork);
    return FALSE;
}

BOOL FrtCmd_ChangeMode(FrontierScriptContext *ctx) {
    FrontierSystem *system = ctx->frontierSystem;
    u16 mode = FrontierScript_ReadVar(ctx);
    Frontier_ChangeMode(system->frontierWork, mode, 0xFFFF);
    FrontierScriptContext_Pause(ctx, FrontierScriptNative_WaitFrame);
    return TRUE;
}

BOOL FrontierScriptNative_WaitFrame(FrontierScriptContext *ctx) {
    return TRUE;
}

BOOL FrtCmd_ChangeModeWithTask(FrontierScriptContext *ctx) {
    FrontierSystem *system = ctx->frontierSystem;
    u16 mode = FrontierScript_ReadVar(ctx);
    u16 taskId = FrontierScript_ReadVar(ctx);
    Frontier_ChangeMode(system->frontierWork, mode, taskId);
    FrontierScriptContext_Stop(ctx);
    return FALSE;
}

// Waits the given number of frames, counting down in the given variable.
BOOL FrtCmd_Wait(FrontierScriptContext *ctx) {
    u16 frames = FrontierScriptContext_ReadHalfWord(ctx);
    u16 varId = FrontierScriptContext_ReadHalfWord(ctx);
    *FrontierScript_GetVarPtr(ctx, varId) = frames;
    ctx->data[0] = varId;
    FrontierScriptContext_Pause(ctx, FrontierScriptNative_WaitFrames);
    return TRUE;
}

BOOL FrontierScriptNative_WaitFrames(FrontierScriptContext *ctx) {
    u16 *counter = FrontierScript_GetVarPtr(ctx, ctx->data[0]);
    (*counter)--;
    return *counter == 0;
}

BOOL FrtCmd_SetVar(FrontierScriptContext *ctx) {
    u16 *dest = FrontierScript_ReadVarPtr(ctx);
    *dest = FrontierScriptContext_ReadHalfWord(ctx);
    return FALSE;
}

BOOL FrtCmd_CopyVar(FrontierScriptContext *ctx) {
    u16 *dest = FrontierScript_ReadVarPtr(ctx);
    *dest = *FrontierScript_ReadVarPtr(ctx);
    return FALSE;
}

BOOL FrtCmd_AddVar(FrontierScriptContext *ctx) {
    u16 *dest = FrontierScript_ReadVarPtr(ctx);
    *dest += FrontierScript_ReadVar(ctx);
    return FALSE;
}

BOOL FrtCmd_SubVar(FrontierScriptContext *ctx) {
    u16 *dest = FrontierScript_ReadVarPtr(ctx);
    *dest -= FrontierScript_ReadVar(ctx);
    return FALSE;
}

u8 FrontierScript_Compare(u16 a, u16 b) {
    if (a < b) {
        return 0;
    }
    if (a == b) {
        return 1;
    }
    return 2;
}

BOOL FrtCmd_CompareVarToValue(FrontierScriptContext *ctx) {
    u16 value = FrontierScript_ReadVar(ctx);
    ctx->comparisonResult = FrontierScript_Compare(value, FrontierScriptContext_ReadHalfWord(ctx));
    return FALSE;
}

BOOL FrtCmd_CompareVarToVar(FrontierScriptContext *ctx) {
    u16 *a = FrontierScript_ReadVarPtr(ctx);
    u16 *b = FrontierScript_ReadVarPtr(ctx);
    ctx->comparisonResult = FrontierScript_Compare(*a, *b);
    return FALSE;
}

void FrontierScript_Jump(FrontierScriptContext *ctx, const u8 *ptr) {
    ctx->scriptPtr = ptr;
}

BOOL FrtCmd_GoTo(FrontierScriptContext *ctx) {
    u32 offset = FrontierScriptContext_ReadWord(ctx);
    FrontierScript_Jump(ctx, ctx->scriptPtr + offset);
    return FALSE;
}

BOOL FrtCmd_GoToIf(FrontierScriptContext *ctx) {
    u8 condition = FrontierScriptContext_ReadByte(ctx);
    u32 offset = FrontierScriptContext_ReadWord(ctx);
    if (ov80_0223B9D8[condition][ctx->comparisonResult] == TRUE) {
        FrontierScript_Jump(ctx, ctx->scriptPtr + offset);
    }
    return FALSE;
}

BOOL FrtCmd_Call(FrontierScriptContext *ctx) {
    u32 offset = FrontierScriptContext_ReadWord(ctx);
    FrontierScriptContext_Call(ctx, ctx->scriptPtr + offset);
    return FALSE;
}

BOOL FrtCmd_Return(FrontierScriptContext *ctx) {
    FrontierScriptContext_Return(ctx);
    return FALSE;
}

#pragma force_active on
BOOL FrtCmd_CallIf(FrontierScriptContext *ctx) {
    int condition = FrontierScriptContext_ReadByte(ctx);
    int offset = FrontierScriptContext_ReadWord(ctx);
    if (ov80_0223B9D8[condition][ctx->comparisonResult] == TRUE) {
        FrontierScriptContext_Call(ctx, ctx->scriptPtr + offset);
    }
    return FALSE;
}

BOOL FrtCmd_PrintMessageInstant(FrontierScriptContext *ctx) {
    u16 msgId = FrontierScriptContext_ReadHalfWord(ctx);
    FrontierMessageParams params;
    params.speed = 0;
    params.autoScroll = 0;
    params.font = 1;
    params.unk_03 = 0;
    FrontierSystem_PrintMessage(ctx->frontierSystem, ctx->msgLoader, msgId, FALSE, &params);
    return FALSE;
}
#pragma force_active reset

BOOL FrtCmd_PrintMessage(FrontierScriptContext *ctx) {
    u16 msgId = FrontierScriptContext_ReadHalfWord(ctx);
    FrontierSystem_PrintMessage(ctx->frontierSystem, ctx->msgLoader, msgId, FALSE, NULL);
    FrontierScriptContext_Pause(ctx, FrontierScriptNative_WaitMessage);
    return TRUE;
}

BOOL FrtCmd_PrintMessageCanSpeedUp(FrontierScriptContext *ctx) {
    u16 msgId = FrontierScriptContext_ReadHalfWord(ctx);
    FrontierSystem_PrintMessage(ctx->frontierSystem, ctx->msgLoader, msgId, TRUE, NULL);
    FrontierScriptContext_Pause(ctx, FrontierScriptNative_WaitMessage);
    return TRUE;
}

BOOL FrontierScriptNative_WaitMessage(FrontierScriptContext *ctx) {
    if (!TextPrinterCheckActive(ctx->frontierSystem->textPrinterId)) {
        TextFlags_SetCanTouchSpeedUpPrint(FALSE);
        return TRUE;
    }
    return FALSE;
}

BOOL FrtCmd_CloseMessage(FrontierScriptContext *ctx) {
    FrontierSystem_CloseMessageWindow(ctx->frontierSystem);
    return FALSE;
}

BOOL FrtCmd_FadeScreen(FrontierScriptContext *ctx) {
    u16 steps = FrontierScriptContext_ReadHalfWord(ctx);
    u16 framesPerStep = FrontierScriptContext_ReadHalfWord(ctx);
    u16 type = FrontierScriptContext_ReadHalfWord(ctx);
    u16 color = FrontierScriptContext_ReadHalfWord(ctx);
    BeginNormalPaletteFade(FADE_BOTH_SCREENS, (enum FadeType)type, (enum FadeType)type, color, steps, framesPerStep, HEAP_ID_FIELD2);
    ResetVisibleHardwareWindows(PM_LCD_TOP);
    ResetVisibleHardwareWindows(PM_LCD_BOTTOM);
    return FALSE;
}

BOOL FrtCmd_WaitFade(FrontierScriptContext *ctx) {
    FrontierScriptContext_Pause(ctx, FrontierScriptNative_WaitPaletteFade);
    return TRUE;
}

BOOL FrontierScriptNative_WaitPaletteFade(FrontierScriptContext *ctx) {
    return IsPaletteFadeFinished() == TRUE;
}

#pragma force_active on
BOOL FrtCmd_MenuInitStdGmm(FrontierScriptContext *ctx) {
    FrontierSystem *system = ctx->frontierSystem;
    int x = ReadScriptByte(ctx);
    int y = ReadScriptByte(ctx);
    int initialSelection = ReadScriptByte(ctx);
    int canCancel = ReadScriptByte(ctx);
    u16 varId = FrontierScriptContext_ReadHalfWord(ctx);
    u16 *result = FrontierScript_GetVarPtr(ctx, varId);
    system->menu = FrontierMenu_New(system, x, y, initialSelection, canCancel, result, system->msgFormat, NULL);
    ctx->data[0] = varId;
    return TRUE;
}

BOOL FrtCmd_MenuInit(FrontierScriptContext *ctx) {
    int canCancel, initialSelection, y, x;
    u16 varId;
    u16 *result;
    FrontierSystem *system;
    system = ctx->frontierSystem;
    x = ReadScriptByte(ctx);
    y = ReadScriptByte(ctx);
    initialSelection = ReadScriptByte(ctx);
    canCancel = ReadScriptByte(ctx);
    varId = FrontierScriptContext_ReadHalfWord(ctx);
    result = FrontierScript_GetVarPtr(ctx, varId);
    system->menu = FrontierMenu_New(system, x, y, initialSelection, canCancel, result, system->msgFormat, ctx->msgLoader);
    ctx->data[0] = varId;
    return TRUE;
}
#pragma force_active reset

BOOL FrtCmd_MenuItemAdd(FrontierScriptContext *ctx) {
    FrontierSystem *system = ctx->frontierSystem;
    u16 msgId = FrontierScriptContext_ReadHalfWord(ctx);
    u16 value = FrontierScriptContext_ReadHalfWord(ctx);
    FrontierMenu_AddItem(system->menu, msgId, 0xFF, value);
    return FALSE;
}

BOOL FrtCmd_MenuItemAddWithDescription(FrontierScriptContext *ctx) {
    FrontierSystem *system = ctx->frontierSystem;
    u16 msgId = FrontierScriptContext_ReadHalfWord(ctx);
    u16 unk2 = FrontierScriptContext_ReadHalfWord(ctx);
    u16 value = FrontierScriptContext_ReadHalfWord(ctx);
    FrontierMenu_AddItem(system->menu, msgId, unk2, value);
    return FALSE;
}

BOOL FrtCmd_MenuExec(FrontierScriptContext *ctx) {
    FrontierMenu_Show(ctx->frontierSystem->menu);
    FrontierScriptContext_Pause(ctx, FrontierScriptNative_WaitMenu);
    return TRUE;
}

BOOL FrontierScriptNative_WaitMenu(FrontierScriptContext *ctx) {
    FrontierSystem *system = ctx->frontierSystem;
    u16 *result = FrontierScript_GetVarPtr(ctx, ctx->data[0]);

    if (system->menu != NULL) {
        return FALSE;
    }
    if (*result != FRONTIER_ID_PLAYER) {
        return TRUE;
    }
    return FALSE;
}

BOOL FrtCmd_ListMenuInitStdGmm(FrontierScriptContext *ctx) {
    FrontierSystem *system = ctx->frontierSystem;
    u8 x = FrontierScriptContext_ReadByte(ctx);
    u8 y = FrontierScriptContext_ReadByte(ctx);
    u8 initialSelection = FrontierScriptContext_ReadByte(ctx);
    u8 canCancel = FrontierScriptContext_ReadByte(ctx);
    u16 varId = FrontierScriptContext_ReadHalfWord(ctx);
    u16 *result = FrontierScript_GetVarPtr(ctx, varId);

    system->menu = FrontierListMenu_New(system, x, y, initialSelection, canCancel, result, system->msgFormat, NULL);
    ctx->data[0] = varId;
    return TRUE;
}

BOOL FrtCmd_ListMenuInit(FrontierScriptContext *ctx) {
    FrontierSystem *system = ctx->frontierSystem;
    u8 x = FrontierScriptContext_ReadByte(ctx);
    u8 y = FrontierScriptContext_ReadByte(ctx);
    u8 initialSelection = FrontierScriptContext_ReadByte(ctx);
    u8 canCancel = FrontierScriptContext_ReadByte(ctx);
    u16 varId = FrontierScriptContext_ReadHalfWord(ctx);
    u16 *result = FrontierScript_GetVarPtr(ctx, varId);

    system->menu = FrontierListMenu_New(system, x, y, initialSelection, canCancel, result, system->msgFormat, ctx->msgLoader);
    ctx->data[0] = varId;
    return TRUE;
}

BOOL FrtCmd_ListMenuItemAdd(FrontierScriptContext *ctx) {
    FrontierSystem *system = ctx->frontierSystem;
    u16 msgId = FrontierScriptContext_ReadHalfWord(ctx);
    u16 unk2 = FrontierScriptContext_ReadHalfWord(ctx);
    u16 value = FrontierScriptContext_ReadHalfWord(ctx);
    FrontierListMenu_AddItem(system->menu, msgId, unk2, value);
    return FALSE;
}

BOOL FrtCmd_ListMenuExec(FrontierScriptContext *ctx) {
    FrontierListMenu_Show(ctx->frontierSystem->menu);
    FrontierScriptContext_Pause(ctx, FrontierScriptNative_WaitMenu);
    return TRUE;
}

BOOL FrtCmd_ListMenuCancel(FrontierScriptContext *ctx) {
    FrontierListMenu_Cancel(ctx->frontierSystem->menu);
    return TRUE;
}

#pragma force_active on
BOOL FrtCmd_YesNo(FrontierScriptContext *ctx) {
    FrontierSystem *system = ctx->frontierSystem;
    FrontierMap *map = FrontierSystem_GetFrontierMap(system);
    u16 varId = FrontierScriptContext_ReadHalfWord(ctx);
    u8 initialSelection = FrontierScriptContext_ReadByte(ctx);
    system->yesNoMenu = CreateYesNoMenu(map->bgConfig, &ov80_0223B9D0, 985, 12, initialSelection, system->heapId);
    ctx->data[0] = varId;
    FrontierScriptContext_Pause(ctx, FrontierScriptNative_WaitYesNo);
    return TRUE;
}

BOOL FrontierScriptNative_WaitYesNo(FrontierScriptContext *ctx) {
    FrontierSystem *system = ctx->frontierSystem;
    u16 *result = FrontierScript_GetVarPtr(ctx, ctx->data[0]);
    int selection = Handle2dMenuInput_DeleteOnFinish(system->yesNoMenu, system->heapId);
    if (selection == -1) {
        if (System_GetTouchNew() == TRUE) {
            PlaySE(SEQ_SE_DP_SELECT);
            selection = Get2dMenuSelection(system->yesNoMenu);
            Clear2dMenuWindowAndDelete(system->yesNoMenu, system->heapId);
        } else {
            return FALSE;
        }
    }
    if (selection == 0) {
        *result = 0;
    } else {
        *result = 1;
    }
    return TRUE;
}

// Loads the object graphics of a list; the player placeholder loads the player's, the link placeholder those of
// every connected player.
BOOL FrtCmd_LoadObjectGfx(FrontierScriptContext *ctx) {
    FrontierSystem *system = ctx->frontierSystem;
    FrontierMap *map = Frontier_GetMap(system->frontierWork);
    u32 offset = FrontierScriptContext_ReadWord(ctx);
    const u8 *saved = ctx->scriptPtr;
    FrontierMapObjectGfx gfx;
    ctx->scriptPtr += offset;
    for (;;) {
        gfx.id = FrontierScript_ReadVar(ctx);
        if (gfx.id == FRONTIER_LIST_END) {
            break;
        }
        gfx.unk_02 = FrontierScriptContext_ReadByte(ctx);
        if (gfx.id == FRONTIER_ID_PLAYER) {
            FrontierLaunchArgs *args = Frontier_GetLaunchArgs(system->frontierWork);
            gfx.id = Frontier_GetPlayerSpriteId(Save_PlayerData_GetProfile(args->saveData));
            FrontierMap_AddObjectGfx(map, &gfx);
        } else if (gfx.id == FRONTIER_ID_LINK_MONS) {
            if (Comm_IsActive() == TRUE) {
                int count = Comm_CountConnected();
                int i;
                for (i = 0; i < count; i++) {
                    gfx.id = Frontier_GetPlayerSpriteId(CommPlayerInfo_GetProfile(i));
                    FrontierMap_AddObjectGfx(map, &gfx);
                }
            }
        } else {
            FrontierMap_AddObjectGfx(map, &gfx);
        }
    }
    ctx->scriptPtr = saved;
    return FALSE;
}
#pragma force_active reset

BOOL FrtCmd_RemoveObjectGfx(FrontierScriptContext *ctx) {
    FrontierMap *map = Frontier_GetMap(ctx->frontierSystem->frontierWork);
    FrontierMap_RemoveObjectGfx(map, FrontierScript_ReadVar(ctx));
    return FALSE;
}

#pragma force_active on
// Creates the objects of a list; the player placeholder creates the player, the link placeholder the connected
// player whose net id follows.
BOOL FrtCmd_CreateObjects(FrontierScriptContext *ctx) {
    FrontierSystem *system = ctx->frontierSystem;
    FrontierMap *map = Frontier_GetMap(system->frontierWork);
    u32 offset = FrontierScriptContext_ReadWord(ctx);
    const u8 *saved = ctx->scriptPtr;
    FrontierMapObjectTemplate template;
    ctx->scriptPtr = saved + offset;
    while (TRUE) {
        int index = -1;
        template.id = FrontierScript_ReadVar(ctx);
        if (template.id == FRONTIER_LIST_END) {
            break;
        }
        template.spriteId = FrontierScript_ReadVar(ctx);
        template.unk_0A = FrontierScriptContext_ReadByte(ctx);
        template.x = FrontierScriptContext_ReadHalfWord(ctx) - 8;
        template.y = FrontierScriptContext_ReadHalfWord(ctx) - 16;
        template.unk_0B = FrontierScriptContext_ReadByte(ctx);
        template.unk_0C = FrontierScriptContext_ReadByte(ctx);
        template.unk_02 = 0;
        if (template.spriteId == FRONTIER_ID_PLAYER) {
            FrontierLaunchArgs *args = Frontier_GetLaunchArgs(system->frontierWork);
            PlayerProfile *profile = Save_PlayerData_GetProfile(args->saveData);
            template.spriteId = Frontier_GetPlayerSpriteId(profile);
            index = FRONTIER_MAP_OBJECT_MAX - 1;
        } else if (template.spriteId == FRONTIER_ID_LINK_MONS) {
            template.spriteId = Frontier_GetPlayerSpriteId(CommPlayerInfo_GetProfile(FrontierScriptContext_ReadByte(ctx)));
        }
        FrontierMap_CreateObject(map, &template, index);
    }
    ctx->scriptPtr = saved;
    return FALSE;
}
#pragma force_active reset

BOOL FrtCmd_DeleteObject(FrontierScriptContext *ctx) {
    FrontierMap *map = Frontier_GetMap(ctx->frontierSystem->frontierWork);
    // the id goes to overlay 42 as the full halfword (the original call does not narrow it to the u8 parameter)
    Field2DObject *object = ((FrontierMapGetObjectFunc)Field2DObjectManager_FindById)(map->objectManager, FrontierScript_ReadVar(ctx));
    FrontierMap_DeleteObject(map, object);
    return FALSE;
}

BOOL FrtCmd_SetObjectVisible(FrontierScriptContext *ctx) {
    FrontierMap *map = Frontier_GetMap(ctx->frontierSystem->frontierWork);
    u16 id = FrontierScript_ReadVar(ctx);
    u8 state = FrontierScriptContext_ReadByte(ctx);
    Field2DObjectSprite *objectSprite = NULL;
    FrontierMap_GetObjectById(map, id, NULL, &objectSprite);
    if (objectSprite == NULL) {
        GF_AssertFail();
    }
    Field2DObjectSprite_SetVisible(objectSprite, state);
    return FALSE;
}

BOOL FrtCmd_Dummy039(FrontierScriptContext *ctx) {
    return FALSE;
}

// Starts the movement script at the given offset on a map object.
BOOL FrtCmd_ApplyMovement(FrontierScriptContext *ctx) {
    FrontierSystem *system = ctx->frontierSystem;
    FrontierMap *map = Frontier_GetMap(system->frontierWork);
    int id = FrontierScript_ReadVar(ctx);
    int offset = FrontierScriptContext_ReadWord(ctx);
    FrontierMapObject *object = FrontierMap_FindObject(system->frontierWork, id);
    if (object == NULL) {
        GF_AssertFail();
    }
    FrontierScript_StartMovement(id, object, ctx->scriptPtr + offset, map->unk30, &system->movementCount, system->heapId);
    system->movementCount++;
    return FALSE;
}

void FrontierScript_StartMovement(int id, FrontierMapObject *object, const u8 *script, void *unk3, u8 *runningCount, enum HeapID heapId) {
    FrontierMovementTask *data = Heap_Alloc(heapId, sizeof(FrontierMovementTask));
    MI_CpuFill8(data, 0, sizeof(FrontierMovementTask));
    data->object = object;
    data->script = (const u16 *)script; // (opcode, count) pairs
    data->unk_10 = unk3;
    data->id = id;
    data->runningCount = runningCount;
    if (object->movementTask != NULL) {
        GF_AssertFail();
    }
    object->movementTask = SysTask_CreateOnMainQueue((SysTaskFunc)FrontierScript_MovementTask, data, 60100);
}

BOOL FrtCmd_WaitMovement(FrontierScriptContext *ctx) {
    FrontierScriptContext_Pause(ctx, FrontierScriptNative_WaitMovements);
    return TRUE;
}

BOOL FrontierScriptNative_WaitMovements(FrontierScriptContext *ctx) {
    return ctx->frontierSystem->movementCount == 0;
}

// Loads the sprite resources of a list.
BOOL FrtCmd_LoadSpriteResources(FrontierScriptContext *ctx) {
    FrontierSystem *system = ctx->frontierSystem;
    FrontierMap *map = Frontier_GetMap(system->frontierWork);
    int offset = FrontierScriptContext_ReadWord(ctx);
    const u8 *saved = ctx->scriptPtr;
    NARC *narc;
    ctx->scriptPtr = saved + offset;
    narc = NARC_New(NARC_a_1_8_4, system->heapId);
    for (;;) {
        u16 kind = FrontierScript_ReadVar(ctx);
        if (kind == FRONTIER_LIST_END) {
            break;
        }
        FrontierMapSprite_LoadResources(map->spriteSystem, map->spriteManager, narc, map->paletteData, kind);
        FrontierMap_AddLoadedResource(map, kind);
    }
    NARC_Delete(narc);
    ctx->scriptPtr = saved;
    return FALSE;
}

BOOL FrtCmd_UnloadSpriteResources(FrontierScriptContext *ctx) {
    FrontierMap *map = Frontier_GetMap(ctx->frontierSystem->frontierWork);
    u16 kind = FrontierScript_ReadVar(ctx);
    FrontierMapSprite_UnloadResources(map->spriteManager, kind);
    FrontierMap_RemoveLoadedResource(map, kind);
    return FALSE;
}
