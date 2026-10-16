#include "scrcmd_link_sync.h"

#include "global.h"

#include "assert.h"
#include "bag.h"
#include "ball_capsule_editor.h"
#include "heap.h"
#include "launch_application.h"
#include "mail.h"
#include "map_header.h"
#include "party.h"
#include "party_menu.h"
#include "player_avatar.h"
#include "player_data.h"
#include "pokemon.h"
#include "render_text.h"
#include "save_arrays.h"
#include "scrcmd.h"
#include "script.h"
#include "script_manager.h"
#include "seal_case.h"
#include "task.h"
#include "link_synchronization.h"
#include "field_overworld_transition.h"
#include "field_overworld_restore.h"
#include "seal_data.h"

FS_EXTERN_OVERLAY(OVY_57);

typedef struct BallCapsuleEditorTaskData {
    u8 filler00[4];
    BallCapsuleEditorArgs *editorArgs;
    PartyMenuArgs *partyMenu;
    SaveData *saveData;
    u32 state;
} BallCapsuleEditorTaskData;

typedef struct LinkSyncWorkParams {
    u8 unk00;
    u8 unk01;
    u8 unk02;
    u8 filler03[21];
    SaveData *saveData;
} LinkSyncWorkParams;

typedef struct LinkSyncWork {
    u8 filler00[4];
    u8 syncEnabled;
    u8 filler05[15];
} LinkSyncWork;

static const OverlayManagerTemplate sOverlayTemplate_BallCapsuleEditor = {
    .init = BallCapsuleEditor_Init,
    .exec = BallCapsuleEditor_Main,
    .exit = BallCapsuleEditor_Exit,
    .ovy_id = FS_OVERLAY_ID(OVY_57),
};

static BOOL Task_BallCapsuleEditor(TaskManager *taskManager);
static BOOL ScrNative_WaitLinkSyncWork(ScriptContext *ctx);
static void LinkSyncWork_StartSync(LinkSyncWork *work, u8 syncState);
static BOOL LinkSyncWork_IsSynced(LinkSyncWork *work, u8 syncState);
static LinkSyncWork *LinkSyncWork_New(void);
static LinkSyncWork *LinkSyncWork_CreateWithHeap(LinkSyncWorkParams *unused);
static void LinkSyncWork_Delete(LinkSyncWork *work);

void CallTask_BallCapsuleEditor(TaskManager *taskManager, SaveData *saveData) {
    FieldSystem *fieldSystem = TaskManager_GetFieldSystem(taskManager);
    BallCapsuleEditorTaskData *taskData = Heap_AllocAtEnd(HEAP_ID_FIELD2, sizeof(BallCapsuleEditorTaskData));
    memset(taskData, 0, sizeof(BallCapsuleEditorTaskData));
    taskData->saveData = saveData;
    taskData->editorArgs = Heap_Alloc(HEAP_ID_FIELD2, sizeof(BallCapsuleEditorArgs));
    memset(taskData->editorArgs, 0, sizeof(BallCapsuleEditorArgs));
    taskData->editorArgs->options = Save_PlayerData_GetOptionsAddr(saveData);
    taskData->editorArgs->saveData = saveData;
    taskData->editorArgs->menuInputStatePtr = &fieldSystem->menuInputState;
    taskData->partyMenu = Heap_Alloc(HEAP_ID_FIELD2, sizeof(PartyMenuArgs));
    memset(taskData->partyMenu, 0, sizeof(PartyMenuArgs));
    TaskManager_Call(taskManager, Task_BallCapsuleEditor, taskData);
}

static BOOL Task_BallCapsuleEditor(TaskManager *taskManager) {
    BallCapsuleEditorTaskData *taskData = TaskManager_GetEnvironment(taskManager);
    BallCapsuleEditorArgs *editorArgs = taskData->editorArgs;
    FieldSystem *fieldSystem = TaskManager_GetFieldSystem(taskManager);
    switch (taskData->state) {
    case 0:
        CallTask_LeaveOverworld(taskManager);
        SealCase *sealCase = Save_SealCase_Get(taskData->saveData);
        editorArgs->sealCase = sealCase;
        editorArgs->setCapsuleOnMon = 0;
        editorArgs->menuInputStatePtr = &fieldSystem->menuInputState;
        Party *party = SaveArray_Party_Get(taskData->saveData);
        editorArgs->party = party;
        s32 partyCount = Party_GetCount(party);
        editorArgs->partyCount = partyCount;
        s32 i;
        for (i = 0; i < partyCount; i++) {
            editorArgs->mons[i] = Party_GetMonByIndex(editorArgs->party, i);
        }
        for (; i < PARTY_SIZE; i++) {
            editorArgs->mons[i] = NULL;
        }
        taskData->state = 1;
        break;
    case 1:
        CallApplicationAsTask(taskManager, &sOverlayTemplate_BallCapsuleEditor, editorArgs);
        taskData->state = 2;
        break;
    case 2:
        switch (taskData->editorArgs->setCapsuleOnMon) {
        default:
            GF_ASSERT(FALSE);
        case 0:
            taskData->state = 5;
            break;
        case 1:
            taskData->state = 3;
            break;
        }
        break;
    case 3:
        PartyMenuArgs *partyMenu = taskData->partyMenu;
        partyMenu->party = editorArgs->party;
        partyMenu->bag = Save_Bag_Get(taskData->saveData);
        partyMenu->mailbox = Save_Mailbox_Get(taskData->saveData);
        partyMenu->partySlot = 0;
        partyMenu->panelLayout = 0;
        partyMenu->context = PARTY_MENU_CONTEXT_ATTACH_CAPSULE;
        partyMenu->options = editorArgs->options;
        partyMenu->fieldSystem = fieldSystem;
        partyMenu->menuInputStatePtr = &(fieldSystem->menuInputState);
        CallApplicationAsTask(taskManager, &gOverlayTemplate_PartyMenu, partyMenu);
        taskData->state = 4;
        break;
    case 4: {
        PartyMenuArgs *partyMenu = taskData->partyMenu;
        u32 index = taskData->editorArgs->capsuleIndex + 1;
        if (partyMenu->partySlot != 7) {
            Pokemon *mon = taskData->editorArgs->mons[partyMenu->partySlot];
            SetMonData(mon, MON_DATA_BALL_CAPSULE_ID, &index);
            SetMonData(mon, MON_DATA_BALL_CAPSULE, SealCase_GetCapsuleI(editorArgs->sealCase, index - 1));
            Seal_GetNameMsgId(SealOnCapsuleGetID(CapsuleGetSealI(SealCase_GetCapsuleI(editorArgs->sealCase, index - 1), 0)));
            Save_Block23_Get(fieldSystem->saveData);
        }
        taskData->state = 1;
        break;
    }
    case 5:
        CallTask_RestoreOverworld(taskManager);
        taskData->state = 6;
        break;
    case 6:
        Heap_Free(taskData->partyMenu);
        Heap_Free(taskData->editorArgs);
        Heap_Free(taskData);
        return TRUE;
    }
    return FALSE;
}

BOOL ScrCmd_ConditionalLinkSyncStart(ScriptContext *ctx) {
    LinkSyncWork **unkStruct = FieldSysGetAttrAddr(ctx->fieldSystem, SCRIPTENV_MISC_DATA_PTR);
    LinkSyncWork_StartSync(*unkStruct, ScriptGetVar(ctx));
    return FALSE;
}

BOOL ScrCmd_ConditionalLinkSyncWait(ScriptContext *ctx) {
    void *unused = FieldSysGetAttrAddr(ctx->fieldSystem, SCRIPTENV_MISC_DATA_PTR);
    ctx->data[0] = ScriptGetVar(ctx);
    SetupNativeScript(ctx, ScrNative_WaitLinkSyncWork);
    return TRUE;
}

static BOOL ScrNative_WaitLinkSyncWork(ScriptContext *ctx) {
    LinkSyncWork **unkStruct = FieldSysGetAttrAddr(ctx->fieldSystem, SCRIPTENV_MISC_DATA_PTR);
    return LinkSyncWork_IsSynced(*unkStruct, ctx->data[0]);
}

BOOL ScrCmd_ConditionalLinkSyncCreate(ScriptContext *ctx) {
    LinkSyncWork **unkStruct = FieldSysGetAttrAddr(ctx->fieldSystem, SCRIPTENV_MISC_DATA_PTR);
    u16 arg1 = ScriptGetVar(ctx);
    u16 arg2 = ScriptGetVar(ctx);
    u16 arg3 = ScriptGetVar(ctx);
    u16 unused = ScriptGetVar(ctx);
    LinkSyncWorkParams unusedStruct;
    unusedStruct.unk00 = arg2;
    unusedStruct.unk01 = arg1;
    unusedStruct.unk02 = arg3;
    unusedStruct.saveData = ctx->fieldSystem->saveData;
    *unkStruct = LinkSyncWork_CreateWithHeap(&unusedStruct);
    return FALSE;
}

BOOL ScrCmd_ConditionalLinkSyncDestroy(ScriptContext *ctx) {
    LinkSyncWork **unkStruct = FieldSysGetAttrAddr(ctx->fieldSystem, SCRIPTENV_MISC_DATA_PTR);
    u16 unused = ScriptGetVar(ctx);
    u32 unused2 = MapHeader_GetMapSec(ctx->fieldSystem->location->mapId);
    LinkSyncWork_Delete(*unkStruct);
    return FALSE;
}

BOOL ScrCmd_SetVarZero(ScriptContext *ctx) {
    void **unused = FieldSysGetAttrAddr(ctx->fieldSystem, SCRIPTENV_MISC_DATA_PTR);
    u16 *result = ScriptGetVarPointer(ctx);
    *result = 0;
    return FALSE;
}

BOOL ScrCmd_BeginTextAutoScroll(ScriptContext *ctx) {
    void **unused = FieldSysGetAttrAddr(ctx->fieldSystem, SCRIPTENV_MISC_DATA_PTR);
    TextFlags_BeginAutoScroll(TRUE);
    return FALSE;
}

static void LinkSyncWork_StartSync(LinkSyncWork *work, u8 syncState) {
    if (work->syncEnabled != 0) {
        LinkSync_StartSync(syncState);
    }
}

static BOOL LinkSyncWork_IsSynced(LinkSyncWork *work, u8 syncState) {
    if (work->syncEnabled == 0) {
        return TRUE;
    }
    return LinkSync_IsSynced(syncState) == TRUE;
}

static LinkSyncWork *LinkSyncWork_New(void) {
    LinkSyncWork *unkStruct = Heap_Alloc(HEAP_ID_20, sizeof(LinkSyncWork));
    MI_CpuFill8(unkStruct, 0, sizeof(LinkSyncWork));
    return unkStruct;
}

static LinkSyncWork *LinkSyncWork_CreateWithHeap(LinkSyncWorkParams *unused) {
    Heap_Create(HEAP_ID_FIELD2, HEAP_ID_20, 0x4000);
    return LinkSyncWork_New();
}

static void LinkSyncWork_Delete(LinkSyncWork *work) {
    Heap_Free(work);
    Heap_Destroy(HEAP_ID_20);
}
