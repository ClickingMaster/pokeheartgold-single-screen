#include "frontier/frontier_link_lobby.h"

#include "global.h"

#include "frontier/frontier.h"

#include "bag.h"
#include "error_handling.h"
#include "field_system.h"
#include "heap.h"
#include "mail.h"
#include "party.h"
#include "party_menu.h"
#include "player_data.h"
#include "pokedex_util.h"
#include "save.h"
#include "save_special_ribbons.h"
#include "unk_02088288.h"

// The frontier link lobby (frontier/frontier_link_lobby.h): its link messages and the party menu / summary it opens.

typedef struct FrontierLinkLobbyGlobals {
    FrontierLinkLobby *lobby;
    u8 filler_04[0xC];
} FrontierLinkLobbyGlobals;

// Link messages (battle_frontier_communication.c).
extern BOOL FrontierLinkLobby_SendFacility(FrontierLinkLobby *lobby);
extern BOOL FrontierLinkLobby_SendChoice(FrontierLinkLobby *lobby, u16 choice);
extern BOOL FrontierLinkLobby_SendParty(FrontierLinkLobby *lobby, u16 first, u16 second);
extern BOOL FrontierLinkLobby_SendValue74(FrontierLinkLobby *lobby, u16 value);
extern BOOL FrontierLinkLobby_SendValue71(FrontierLinkLobby *lobby, u16 value);
extern const u8 ov80_0223C040[];

void FrontierLinkLobby_New(SaveData *saveData);
void FrontierLinkLobby_LaunchPartyMenu(void *frontierWork, FrontierLinkLobby *lobby, FieldSystem *fieldSystem, enum HeapID heapId);
void FrontierLinkLobby_ReadPartyMenuResult(void *frontierWork, FrontierLinkLobby *lobby, FieldSystem *fieldSystem, enum HeapID heapId);
void FrontierLinkLobby_LaunchSummary(void *frontierWork, FrontierLinkLobby *lobby, FieldSystem *fieldSystem, enum HeapID heapId);
void FrontierLinkLobby_ReadSummaryResult(void *frontierWork, FrontierLinkLobby *lobby, FieldSystem *fieldSystem, enum HeapID heapId);

FrontierLinkLobbyGlobals gFrontierLinkLobby;

// Declared returning the lobby by its caller (frontier_cmd_link_lobby.c): it is still in r0 after the last store.
void FrontierLinkLobby_New(SaveData *saveData) {
    gFrontierLinkLobby.lobby = Heap_Alloc(HEAP_ID_FIELD2, sizeof(FrontierLinkLobby));
    MI_CpuFill8(gFrontierLinkLobby.lobby, 0, sizeof(FrontierLinkLobby));
    gFrontierLinkLobby.lobby->saveData = saveData;
}

void FrontierLinkLobby_Free(FrontierLinkLobby *lobby) {
    if (lobby != NULL) {
        Heap_Free(lobby);
    }
}

BOOL FrontierLinkLobby_Send(FrontierLinkLobby *lobby, int kind, u16 a, u16 b) {
    switch (kind) {
    case LINK_LOBBY_SEND_FACILITY:
        return FrontierLinkLobby_SendFacility(lobby);
    case LINK_LOBBY_SEND_CHOICE:
        return FrontierLinkLobby_SendChoice(lobby, a);
    case LINK_LOBBY_SEND_PARTY:
        return FrontierLinkLobby_SendParty(lobby, a, b);
    case 3:
        break;
    case LINK_LOBBY_SEND_VALUE74:
        return FrontierLinkLobby_SendValue74(lobby, a);
    case LINK_LOBBY_SEND_VALUE71:
        return FrontierLinkLobby_SendValue71(lobby, a);
    }
}

void FrontierLinkLobby_RunPartyApp(void *frontierWork, FrontierLinkLobby *lobby, u32 kind) {
    FieldSystem *fieldSystem = lobby->fieldSystem;
    switch (kind) {
    case LINK_LOBBY_APP_LAUNCH_PARTY_MENU:
        FrontierLinkLobby_LaunchPartyMenu(frontierWork, lobby, fieldSystem, HEAP_ID_FIELD2);
        break;
    case LINK_LOBBY_APP_READ_PARTY_MENU:
        FrontierLinkLobby_ReadPartyMenuResult(frontierWork, lobby, fieldSystem, HEAP_ID_FIELD2);
        break;
    case LINK_LOBBY_APP_LAUNCH_SUMMARY:
        FrontierLinkLobby_LaunchSummary(frontierWork, lobby, fieldSystem, HEAP_ID_FIELD2);
        break;
    case LINK_LOBBY_APP_READ_SUMMARY:
        FrontierLinkLobby_ReadSummaryResult(frontierWork, lobby, fieldSystem, HEAP_ID_FIELD2);
        break;
    case 4:
        break;
    }
}

void FrontierLinkLobby_LaunchPartyMenu(void *frontierWork, FrontierLinkLobby *lobby, FieldSystem *fieldSystem, enum HeapID heapId) {
    u8 i;

    lobby->partyMenuArgs = Heap_Alloc(HEAP_ID_FIELD2, sizeof(PartyMenuArgs));
    MI_CpuClearFast(lobby->partyMenuArgs, sizeof(PartyMenuArgs));
    lobby->partyMenuArgs->party = SaveArray_Party_Get(lobby->saveData);
    lobby->partyMenuArgs->bag = Save_Bag_Get(lobby->saveData);
    lobby->partyMenuArgs->mailbox = Save_Mailbox_Get(lobby->saveData);
    lobby->partyMenuArgs->options = Save_PlayerData_GetOptionsAddr(lobby->saveData);
    lobby->partyMenuArgs->panelLayout = 0;
    if (lobby->facility == 1) {
        lobby->partyMenuArgs->context = PARTY_MENU_CONTEXT_17;
    } else if (lobby->facility == 5) {
        lobby->partyMenuArgs->context = PARTY_MENU_CONTEXT_BATTLE_HALL;
    } else if (lobby->facility == 4) {
        lobby->partyMenuArgs->context = PARTY_MENU_CONTEXT_23;
    } else if (lobby->facility == 6) {
        lobby->partyMenuArgs->context = PARTY_MENU_CONTEXT_23;
    } else {
        GF_AssertFail();
    }
    lobby->partyMenuArgs->fieldSystem = fieldSystem;
    lobby->partyMenuArgs->partySlot = lobby->partySlot;
    for (i = 0; i < 2; i++) {
        lobby->partyMenuArgs->selectedOrder[i] = lobby->selectedOrder[i];
    }
    if (lobby->facility == 1) {
        lobby->partyMenuArgs->minMonsToSelect = 2;
        lobby->partyMenuArgs->maxMonsToSelect = 2;
        lobby->partyMenuArgs->maxLevel = 100;
    } else if (lobby->facility == 5) {
        lobby->partyMenuArgs->minMonsToSelect = 1;
        lobby->partyMenuArgs->maxMonsToSelect = 1;
        lobby->partyMenuArgs->maxLevel = 30;
    } else if (lobby->facility == 4) {
        lobby->partyMenuArgs->minMonsToSelect = 2;
        lobby->partyMenuArgs->maxMonsToSelect = 2;
        lobby->partyMenuArgs->maxLevel = 100;
    } else if (lobby->facility == 6) {
        lobby->partyMenuArgs->minMonsToSelect = 2;
        lobby->partyMenuArgs->maxMonsToSelect = 2;
        lobby->partyMenuArgs->maxLevel = 100;
    } else {
        GF_AssertFail();
    }
    lobby->partyMenuArgs->menuInputStatePtr = &fieldSystem->menuInputState;
    Frontier_LaunchApplication(frontierWork, &gOverlayTemplate_PartyMenu, lobby->partyMenuArgs, 0, NULL);
}

void FrontierLinkLobby_ReadPartyMenuResult(void *frontierWork, FrontierLinkLobby *lobby, FieldSystem *fieldSystem, enum HeapID heapId) {
    switch (lobby->partyMenuArgs->partySlot) {
    case 7:
        *lobby->resultVar = 4;
        lobby->appResult = 4;
        break;
    case 6:
        *lobby->resultVar = 4;
        lobby->appResult = 4;
        break;
    default:
        MI_CpuCopy8(lobby->partyMenuArgs->selectedOrder, lobby->selectedOrder, 2);
        lobby->partySlot = lobby->partyMenuArgs->partySlot;
        Heap_Free(lobby->partyMenuArgs);
        lobby->partyMenuArgs = NULL;
        *lobby->resultVar = 2;
        lobby->appResult = 2;
        break;
    }
}

void FrontierLinkLobby_LaunchSummary(void *frontierWork, FrontierLinkLobby *lobby, FieldSystem *fieldSystem, enum HeapID heapId) {
    lobby->summaryArgs = Heap_AllocAtEnd(heapId, sizeof(PokemonSummaryArgs));
    MI_CpuFill8(lobby->summaryArgs, 0, sizeof(PokemonSummaryArgs));
    lobby->summaryArgs->options = Save_PlayerData_GetOptionsAddr(lobby->saveData);
    lobby->summaryArgs->party = SaveArray_Party_Get(lobby->saveData);
    lobby->summaryArgs->natDexEnabled = SaveArray_IsNatDexEnabled(lobby->saveData);
    lobby->summaryArgs->unk2C = sub_02088288(lobby->saveData);
    lobby->summaryArgs->monDataType = 1;
    lobby->summaryArgs->partySlot = lobby->partySlot;
    lobby->summaryArgs->partyCount = Party_GetCount(lobby->summaryArgs->party);
    lobby->summaryArgs->moveToLearn = 0;
    lobby->summaryArgs->mode = 0;
    lobby->summaryArgs->ribbons = Save_SpecialRibbons_Get(lobby->saveData);
    lobby->summaryArgs->isFlag982Set = PokemonSummary_IsPokeathlonUnlocked(lobby->saveData);
    PokemonSummary_SetPages(lobby->summaryArgs, ov80_0223C040);
    PokemonSummaryArgs_SetPlayer(lobby->summaryArgs, Save_PlayerData_GetProfile(lobby->saveData));
    Frontier_LaunchApplication(frontierWork, &gOverlayTemplate_PokemonSummary, lobby->summaryArgs, 0, NULL);
}

void FrontierLinkLobby_ReadSummaryResult(void *frontierWork, FrontierLinkLobby *lobby, FieldSystem *fieldSystem, enum HeapID heapId) {
    lobby->partySlot = lobby->summaryArgs->partySlot;
    Heap_Free(lobby->summaryArgs);
    lobby->summaryArgs = NULL;
    *lobby->resultVar = 0;
    lobby->appResult = 0;
}

const u8 gBattleHallCellTypes[20] __attribute__((aligned(4))) = {
    0x00,
    0x0A,
    0x0B,
    0x0D,
    0x0C,
    0x0F,
    0x01,
    0x03,
    0x04,
    0x02,
    0x0E,
    0x06,
    0x05,
    0x07,
    0x10,
    0x11,
    0x08,
    0xFE,
    0xFE,
    0x09,
};
