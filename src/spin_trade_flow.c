#include "global.h"

#include "field_system.h"
#include "game_stats.h"
#include "heap.h"
#include "launch_application.h"
#include "options.h"
#include "player_data.h"
#include "spin_trade_link.h"
#include "task.h"
#include "unk_02037C94.h"

typedef struct SpinTradeFlow {
    u32 state;
    u32 selection;
    u8 sendData[4];
    u32 frame;
    SaveData *saveData;
    void *unk14;
    Options *options;
    GameStats *gameStats;
    FieldSystem *fieldSystem2;
    SpinTradeArgs *subwork;
    FieldSystem *fieldSystem;
    PartyMenuArgs *partyMenu;
    PokemonSummaryArgs *summary;
} SpinTradeFlow;

SpinTradeArgs *SpinTradeArgs_New(const SpinTradeCommonArgs *input, enum HeapID heapID);
void SpinTradeArgs_Free(SpinTradeArgs *work);
u32 SpinTradeArgs_GetResult(SpinTradeArgs *work);
void *SpinTradeFlow_New(FieldSystem *fieldSystem);
BOOL SpinTradeFlow_Run(void *data);
typedef BOOL (*SpinTradeFlowStateFunc)(SpinTradeFlow *work);
extern const SpinTradeFlowStateFunc gSpinTradeFlowStates[];
extern const OverlayManagerTemplate gOverlayTemplate_SpinTradeGroup;
extern const OverlayManagerTemplate gOverlayTemplate_SpinTrade;
BOOL SpinTradeFlow_LaunchGroupApp(SpinTradeFlow *work);
BOOL SpinTradeFlow_WaitGroupApp(SpinTradeFlow *work);
BOOL SpinTradeFlow_WaitPartyMenu(SpinTradeFlow *work);
BOOL SpinTradeFlow_WaitSummary(SpinTradeFlow *work);
BOOL SpinTradeFlow_WaitSpinTradeApp(SpinTradeFlow *work);
BOOL SpinTradeFlow_Finish(SpinTradeFlow *work);

SpinTradeArgs *SpinTradeArgs_New(const SpinTradeCommonArgs *input, enum HeapID heapID) {
    SpinTradeArgs *work = Heap_Alloc(heapID, sizeof(SpinTradeArgs));
    memset(work, 0, sizeof(SpinTradeArgs));
    work->input = *input;
    work->menu = SpinTradeLink_New(work, heapID);
    return work;
}

void SpinTradeArgs_Free(SpinTradeArgs *work) {
    SpinTradeLink_Close(work->menu);
    SpinTradeLink_Free(work->menu);
    Heap_Free(work);
}

u32 SpinTradeArgs_GetResult(SpinTradeArgs *work) {
    return work->result;
}

void *SpinTradeFlow_New(FieldSystem *fieldSystem) {
    SpinTradeFlow *work = Heap_Alloc((enum HeapID)11, sizeof(SpinTradeFlow));
    memset(work, 0, sizeof(SpinTradeFlow));
    work->fieldSystem = fieldSystem;
    work->saveData = fieldSystem->saveData;
    work->unk14 = fieldSystem->unk84;
    work->options = Save_PlayerData_GetOptionsAddr(fieldSystem->saveData);
    work->gameStats = Save_GameStats_Get(fieldSystem->saveData);
    work->frame = Options_GetFrame(work->options);
    work->fieldSystem2 = fieldSystem;
    return work;
}

BOOL SpinTradeFlow_Run(void *data) {
    SpinTradeFlow *work = data;
    if (gSpinTradeFlowStates[work->state](work) == TRUE) {
        Heap_Free(work);
        return TRUE;
    }
    return FALSE;
}

BOOL SpinTradeFlow_LaunchGroupApp(SpinTradeFlow *work) {
    work->subwork = SpinTradeArgs_New((const SpinTradeCommonArgs *)work->sendData, (enum HeapID)11);
    work->state = 1;
    FieldSystem_LaunchApplication(work->fieldSystem, &gOverlayTemplate_SpinTradeGroup, work->subwork);
    return FALSE;
}

BOOL SpinTradeFlow_WaitGroupApp(SpinTradeFlow *work) {
    if (!FieldSystem_ApplicationIsRunning(work->fieldSystem)) {
        if (SpinTradeArgs_GetResult(work->subwork) == 0) {
            work->state = 5;
        } else {
            sub_020398D4(1, 1);
            work->partyMenu = PartyMenu_LaunchApp_Unk5(work->fieldSystem, work->selection);
            work->subwork->unk0[0] = 1;
            work->state = 2;
        }
    }
    return FALSE;
}

BOOL SpinTradeFlow_WaitPartyMenu(SpinTradeFlow *work) {
    u8 slot;
    if (!FieldSystem_ApplicationIsRunning(work->fieldSystem)) {
        slot = work->partyMenu->partySlot;
        Heap_Free(work->partyMenu);
        if (work->partyMenu->selectedAction == 1) {
            work->summary = PokemonSummary_CreateArgs(work->fieldSystem, (enum HeapID)3, 0);
            work->selection = slot;
            work->summary->partySlot = slot;
            PokemonSummary_LearnForget_LaunchApp(work->fieldSystem, work->summary);
            work->state = 3;
        } else {
            work->subwork->unk0[1] = slot;
            FieldSystem_LaunchApplication(work->fieldSystem, &gOverlayTemplate_SpinTrade, work->subwork);
            work->subwork->unk0[0] = 3;
            work->state = 4;
        }
    }
    return FALSE;
}

BOOL SpinTradeFlow_WaitSummary(SpinTradeFlow *work) {
    if (!FieldSystem_ApplicationIsRunning(work->fieldSystem)) {
        Heap_Free(work->summary);
        work->partyMenu = PartyMenu_LaunchApp_Unk5(work->fieldSystem, work->selection);
        work->state = 2;
    }
    return FALSE;
}

BOOL SpinTradeFlow_WaitSpinTradeApp(SpinTradeFlow *work) {
    if (!FieldSystem_ApplicationIsRunning(work->fieldSystem)) {
        work->state = 5;
    }
    return FALSE;
}

BOOL SpinTradeFlow_Finish(SpinTradeFlow *work) {
    SpinTradeArgs_Free(work->subwork);
    return TRUE;
}
