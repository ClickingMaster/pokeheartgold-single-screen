#include "global.h"

#include "field/fieldmap.h"
#include "msgdata/msg.naix"
#include "msgdata/msg/msg_0014.h"

#include "bag.h"
#include "colosseum_internal.h"
#include "communication_player_state.h"
#include "dialog_box.h"
#include "heap.h"
#include "launch_application.h"
#include "link_synchronization.h"
#include "map_object.h"
#include "player_avatar.h"
#include "pokedex_util.h"
#include "pokemon.h"
#include "render_window.h"
#include "save_link_ruleset.h"
#include "save_special_ribbons.h"
#include "screen_fade.h"
#include "sound_playback.h"
#include "system.h"
#include "text.h"
#include "unk_02035900.h"
#include "unk_02037C94.h"
#include "unk_02062108.h"

// Communication core and communication player manager functions, declared with the types their call sites were
// matched against.
BOOL sub_02037108(int command, void *data, int size);
BOOL sub_02037088(int command, void *data, u32 size);
BOOL sub_02036FD8(int command, void *data, u32 size);
void sub_0203996C(u8 *selectedOrder);
void sub_02056E60(void);
void sub_02057E08(void);
void sub_02057F70(void);
int CommPlayerManager_GetPlayerX(int netId);
int CommPlayerManager_GetPlayerZ(int netId);
int CommPlayerManager_GetPlayerFrontX(void);
int CommPlayerManager_GetPlayerFrontZ(int netId);
u8 *CommClubLink_GetCard(int netId, int a1, int a2);
int LinkSync_GetPlayerState(int netId);

// The cursor position is passed as an int here (u8 x, y in list_menu_cursor.h).
#define ListMenuUpdateCursorObj_Int ((void (*)(struct ListMenuCursor *, Window *, int, int))ListMenuUpdateCursorObj)

// Summary pages shown for a Pokemon of the party being entered.
ALIGN(4)
const u8 _020FC7CC[8] = { 0, 1, 2, 3, 4, 0, 0, 0 };

// Union Room state (union_room.c); defined here for link order.
ALIGN(4)
u8 _021D41CC[0xC] = { 0 };
ALIGN(4)
u8 _021D41D8[0x40] = { 0 };

void Colosseum_OpenSummary(ColosseumEntry *entry, FieldSystem *fieldSystem, Party *party, int partySlot, int mode, enum HeapID heapId) {
    SaveData *saveData = fieldSystem->saveData;
    PokemonSummaryArgs *args = Heap_AllocAtEnd(heapId, sizeof(PokemonSummaryArgs));
    MI_CpuFill8(args, 0, sizeof(PokemonSummaryArgs));
    sub_0208AD34(args, Save_PlayerData_GetProfile(fieldSystem->saveData));
    args->natDexEnabled = SaveArray_IsNatDexEnabled(saveData);
    args->unk2C = sub_02088288(saveData);
    args->options = Save_PlayerData_GetOptionsAddr(saveData);
    args->party = party;
    args->unk11 = 1;
    args->partySlot = partySlot;
    args->partyCount = Party_GetCount(args->party);
    args->moveToLearn = 0;
    args->unk12 = mode;
    args->ribbons = Save_SpecialRibbons_Get(saveData);
    args->menuInputStatePtr = NULL;
    args->isFlag982Set = sub_0208828C(saveData);
    sub_02089D40(args, _020FC7CC);
    FieldSystem_LaunchApplication(fieldSystem, &gOverlayTemplate_PokemonSummary, args);
    entry->summaryArgs = args;
}

void Colosseum_OpenPartyMenu(ColosseumEntry *entry, enum HeapID heapId) {
    PartyMenuArgs *args = Heap_Alloc(heapId, sizeof(PartyMenuArgs));
    int i;
    MI_CpuFill8(args, 0, sizeof(PartyMenuArgs));
    args->options = Save_PlayerData_GetOptionsAddr(entry->fieldSystem->saveData);
    args->linkBattleRuleset = entry->fieldSystem->linkBattleRuleset;
    args->party = SaveArray_Party_Get(entry->fieldSystem->saveData);
    args->bag = Save_Bag_Get(entry->fieldSystem->saveData);
    args->menuInputStatePtr = &entry->fieldSystem->menuInputState;
    args->unk_25 = 0;
    args->context = 2;
    if (entry->fieldSystem->linkBattleRuleset != NULL) {
        args->minMonsToSelect = LinkBattleRuleset_GetRuleValue(entry->fieldSystem->linkBattleRuleset, LINKBATTLERULE_PARTY_COUNT);
        args->maxMonsToSelect = args->minMonsToSelect;
    } else {
        args->minMonsToSelect = 3;
        args->maxMonsToSelect = 3;
    }
    args->maxLevel = 100;
    args->partySlot = entry->partySlot;
    for (i = 0; i < 6; i++) {
        args->selectedOrder[i] = entry->selectedOrder[i];
    }
    FieldSystem_LaunchApplication(entry->fieldSystem, &gOverlayTemplate_PartyMenu, args);
    entry->partyMenuArgs = args;
}

BOOL Colosseum_FinishPartyMenu(ColosseumEntry *entry, FieldSystem *fieldSystem) {
    if (FieldSystem_ApplicationIsRunning(fieldSystem)) {
        return FALSE;
    }
    MI_CpuCopy8(entry->partyMenuArgs->selectedOrder, entry->selectedOrder, 6);
    if (entry->partyMenuArgs->partySlot == 6) {
        goto confirm;
    }
    if (entry->partyMenuArgs->partySlot != 7) {
        goto summary;
    }
    entry->partyMenuResult = COLOSSEUM_PARTY_MENU_CANCEL;
    goto done;
confirm:
    entry->partyMenuResult = COLOSSEUM_PARTY_MENU_CONFIRM;
    goto done;
summary:
    entry->partyMenuResult = COLOSSEUM_PARTY_MENU_SUMMARY;
done:
    entry->partySlot = entry->partyMenuArgs->partySlot;
    Heap_Free(entry->partyMenuArgs);
    entry->partyMenuArgs = NULL;
    return TRUE;
}

BOOL Colosseum_FinishSummary(ColosseumEntry *entry, FieldSystem *fieldSystem) {
    if (FieldSystem_ApplicationIsRunning(fieldSystem)) {
        return FALSE;
    }
    entry->partySlot = entry->summaryArgs->partySlot;
    Heap_Free(entry->summaryArgs);
    entry->summaryArgs = NULL;
    return TRUE;
}

BOOL Colosseum_WaitFieldResumed(ColosseumEntry *entry) {
    if (sub_020505C8(entry->fieldSystem) != 0) {
        FieldMap_FadeScreen(FADE_TYPE_BRIGHTNESS_IN);
        sub_02056E60();
        return TRUE;
    }
    return FALSE;
}

BOOL Colosseum_AnyPlayerCancelled(void) {
    int i;
    int count = Comm_CountConnected();
    i = 0;
    for (i = 0; i < count; i++) {
        if (LinkSync_GetPlayerState(i) == 94) {
            return TRUE;
        }
    }
    return FALSE;
}

BOOL Task_ColosseumEntry(TaskManager *taskManager) {
    ColosseumEntry *entry = TaskManager_GetEnvironment(taskManager);
    FieldSystem *fieldSystem = TaskManager_GetFieldSystem(taskManager);

    switch (entry->state) {
    case 0:
        entry->timer--;

        if (entry->timer == 0) {
            entry->state = 1;
            sub_02057E08();
        }
        break;
    case 1:
        ReadMsgDataIntoString(entry->msgData, msg_0014_00001, entry->message);
        entry->printerId = Colosseum_PrintMessage(entry, entry->message);
        entry->state = 2;
        break;
    case 2:
        if (DialogBox_IsPrintFinished(entry->printerId)) {
            LinkSync_StartSync(93);
            entry->state = 3;
        }
        break;
    case 3:
        if (LinkSync_IsSynced(93)) {
            entry->state = 7;
            sub_0205F55C(entry->fieldSystem->mapObjectManager);
            entry->callback(TRUE, entry->party);
        } else if (gSystem.newKeys & PAD_BUTTON_B) {
            entry->state = 4;
            LinkSync_StartSync(92);
            entry->timer = 5;
        }
        break;
    case 4:
        if (LinkSync_IsSynced(93)) {
            entry->state = 7;
            sub_0205F55C(entry->fieldSystem->mapObjectManager);
            entry->callback(TRUE, entry->party);
        }

        entry->timer--;

        if (entry->timer == 0) {
            entry->state = 8;
        }
        break;
    case 7:
        Colosseum_FreeEntry(entry);
        Heap_Free(entry);
        return TRUE;
    case 5:
        Colosseum_FreeEntry(entry);
        Heap_Free(entry);
        sub_02057F70();
        return TRUE;
    case 8:
        if (LinkSync_IsSynced(93)) {
            entry->state = 5;
            entry->callback(TRUE, entry->party);
        } else {
            entry->callback(FALSE, entry->party);
            entry->state = 5;
        }
        break;
    case 9:
        entry->state = 10;
        entry->delay = 5;
        break;
    case 10:
        if (entry->delay != 0) {
            entry->delay--;
        } else {
            if (MapObject_IsMovementPaused(PlayerAvatar_GetMapObject(fieldSystem->playerAvatar))) {
                entry->state = 11;
            }
        }
        break;
    case 11:
        sub_02057E08();
        ReadMsgDataIntoString(entry->msgData, msg_0014_00013, entry->message);

        entry->printerId = Colosseum_PrintMessage(entry, entry->message);
        entry->state = 12;
        break;
    case 12:
        if (DialogBox_IsPrintFinished(entry->printerId)) {
            entry->state = 13;
        }
        break;
    case 13:
        FieldMap_FadeScreen(FADE_TYPE_BRIGHTNESS_OUT);
        entry->state = 14;
        break;
    case 14:
        entry->timer--;

        if (entry->timer == 0) {
            sub_02057E08();
            entry->state = 15;
        }
        break;
    case 15:
        Colosseum_RemoveWindows(entry, FALSE);
        Colosseum_OpenPartyMenu(entry, HEAP_ID_FIELD2);
        entry->state = 16;
        break;
    case 16:
        if (Colosseum_FinishPartyMenu(entry, entry->fieldSystem)) {
            switch (entry->partyMenuResult) {
            case COLOSSEUM_PARTY_MENU_CANCEL:
                entry->state = 20;
                break;
            case COLOSSEUM_PARTY_MENU_CONFIRM:
                entry->state = 19;
                break;
            case COLOSSEUM_PARTY_MENU_SUMMARY:
                entry->state = 17;
                break;
            }
        }
        break;
    case 17:
        Colosseum_OpenSummary(entry, entry->fieldSystem, SaveArray_Party_Get(entry->fieldSystem->saveData), entry->partySlot, 0, HEAP_ID_FIELD2);
        entry->state = 18;
        break;
    case 18:
        if (Colosseum_FinishSummary(entry, entry->fieldSystem)) {
            entry->state = 15;
        }
        break;
    case 19:
        FieldSystem_LoadFieldOverlay(entry->fieldSystem);

        if (entry->linkMode != 3) {
            entry->timer = 5;
            entry->state = 21;
        } else {
            entry->state = 26;
        }
        break;
    case 21:
        sub_0203996C(entry->selectedOrder);

        if (Colosseum_WaitFieldResumed(entry)) {
            entry->timer = 5;
            entry->state = 0;
        }
        break;
    case 20:
        FieldSystem_LoadFieldOverlay(entry->fieldSystem);

        if (entry->linkMode != 3) {
            entry->state = 22;
        } else {
            entry->state = 26;
        }
        break;
    case 22:
        if (Colosseum_WaitFieldResumed(entry)) {
            entry->state = 8;
        }
        break;
    case 23:
        entry->delay--;

        if (entry->delay == 0) {
            entry->state = 24;
        }
        break;
    case 24:
        sub_02057E08();
        ReadMsgDataIntoString(entry->msgData, msg_0014_00019, entry->message);
        entry->printerId = Colosseum_PrintMessage(entry, entry->message);
        entry->state = 25;
        break;
    case 25:
        if (DialogBox_IsPrintFinished(entry->printerId)) {
            entry->state = 13;
        }
        break;
    case 26:
        if (Colosseum_WaitFieldResumed(entry)) {
            if (Colosseum_AnyPlayerCancelled()) {
                entry->state = 5;
            } else {
                entry->ready = (entry->partyMenuResult != COLOSSEUM_PARTY_MENU_CANCEL);
                Colosseum_PackMixBattleMons(entry, entry->ready);
                LinkSync_StartSync(0);
                BufferPlayersName(entry->msgFormat, 0, entry->opponentProfile);
                ReadMsgDataIntoString(entry->msgData, msg_0014_00014, entry->message);
                StringExpandPlaceholders(entry->msgFormat, entry->expandedMessage, entry->message);
                entry->printerId = Colosseum_PrintMessage(entry, entry->expandedMessage);
                entry->state = 27;
            }
        }
        break;
    case 27:
        if (DialogBox_IsPrintFinished(entry->printerId)) {
            if (Colosseum_AnyPlayerCancelled()) {
                entry->state = 5;
            } else if (LinkSync_IsSynced(0)) {
                Colosseum_SendMixBattleMons(entry);
                entry->state = 28;
            }
        }
        break;
    case 28:
        if (Colosseum_MixBattleMonsExchanged(entry)) {
            LinkSync_StartSync(1);
            entry->state = 29;
        }
        break;
    case 29:
        if (LinkSync_IsSynced(1)) {
            entry->opponentReady = Colosseum_OpponentIsReady(entry);

            if (entry->ready && entry->opponentReady) {
                ReadMsgDataIntoString(entry->msgData, msg_0014_00020, entry->message);
                entry->printerId = Colosseum_PrintMessage(entry, entry->message);
                entry->state = 30;
            } else {
                entry->state = 42;
            }
        }
        break;
    case 30:
        if (DialogBox_IsPrintFinished(entry->printerId)) {
            entry->exchangeFlags = 0;
            ReadMsgDataIntoString(entry->msgData, msg_0014_00017, entry->message);
            entry->printerId = Colosseum_PrintMessage(entry, entry->message);
            entry->choice = 0;
            entry->state = 31;
        }
        break;
    case 31:
        if (DialogBox_IsPrintFinished(entry->printerId)) {
            Colosseum_SetOpponentParty(entry);
            Colosseum_ShowOpponentMons(entry, entry->choice);
            entry->state = 32;
        }
        break;
    case 32:
        switch (Colosseum_HandleMenuInput(entry)) {
        case COLOSSEUM_MENU_SELECT:
            Colosseum_CloseMenu(entry);
            entry->choice = entry->menuCursor;
            entry->state = 36;
            break;
        case COLOSSEUM_MENU_CANCEL:
            Colosseum_CloseMenu(entry);
            entry->choice = COLOSSEUM_NO_CHOICE;
            ReadMsgDataIntoString(entry->msgData, msg_0014_00015, entry->message);
            entry->printerId = Colosseum_PrintMessage(entry, entry->message);
            LinkSync_StartSync(2);
            entry->state = 39;
            break;
        }
        break;
    case 36:
        BufferBoxMonSpeciesName(entry->msgFormat, 1, Mon_GetBoxMon(Party_GetMonByIndex(entry->party, entry->choice)));
        ReadMsgDataIntoString(entry->msgData, msg_0014_00018, entry->message);
        StringExpandPlaceholders(entry->msgFormat, entry->expandedMessage, entry->message);
        entry->printerId = Colosseum_PrintMessage(entry, entry->expandedMessage);
        entry->state = 37;
        break;
    case 37:
        if (DialogBox_IsPrintFinished(entry->printerId)) {
            Colosseum_ShowChoiceMenu(entry, 0);
            entry->state = 38;
        }
        break;
    case 38:
        switch (Colosseum_HandleMenuInput(entry)) {
        case COLOSSEUM_MENU_CANCEL:
            Colosseum_CloseMenu(entry);
            ReadMsgDataIntoString(entry->msgData, msg_0014_00017, entry->message);
            entry->printerId = Colosseum_PrintMessage(entry, entry->message);
            entry->state = 31;
            break;
        case COLOSSEUM_MENU_SELECT:
            if (entry->menuCursor == 1) {
                Colosseum_CloseMenu(entry);
                ReadMsgDataIntoString(entry->msgData, msg_0014_00014, entry->message);
                StringExpandPlaceholders(entry->msgFormat, entry->expandedMessage, entry->message);
                entry->printerId = Colosseum_PrintMessage(entry, entry->expandedMessage);
                LinkSync_StartSync(2);
                entry->state = 39;
            } else {
                FieldMap_FadeScreen(FADE_TYPE_BRIGHTNESS_OUT);
                entry->state = 33;
            }
            break;
        }
        break;
    case 33:
        if (IsPaletteFadeFinished()) {
            Colosseum_RemoveWindows(entry, FALSE);
            Colosseum_OpenSummary(entry, entry->fieldSystem, entry->party, entry->choice, 1, HEAP_ID_FIELD2);
            entry->state = 34;
        }
        break;
    case 34:
        if (Colosseum_FinishSummary(entry, entry->fieldSystem)) {
            FieldSystem_LoadFieldOverlay(entry->fieldSystem);
            entry->state = 35;
        }
        break;
    case 35:
        if (Colosseum_WaitFieldResumed(entry)) {
            entry->state = 36;
        }
        break;
    case 39:
        if (DialogBox_IsPrintFinished(entry->printerId)) {
            if (LinkSync_IsSynced(2)) {
                Colosseum_SendMixBattleChoice(entry);
                entry->state = 41;
            }
        }
        break;
    case 41:
        if (Colosseum_MixBattleChoicesExchanged(entry)) {
            if (entry->choice == COLOSSEUM_NO_CHOICE) {
                LinkSync_StartSync(4);
                entry->state = 44;
            } else if (entry->opponentChoice == COLOSSEUM_NO_CHOICE) {
                entry->state = 42;
            } else {
                Colosseum_ExchangeMixBattleMons(entry);
                LinkSync_StartSync(93);
                entry->state = 2;
            }
        }
        break;
    case 42:
        ReadMsgDataIntoString(entry->msgData, msg_0014_00015, entry->message);
        entry->printerId = Colosseum_PrintMessage(entry, entry->message);
        entry->timer = 0;
        entry->state = 43;
        break;
    case 43:
        if (DialogBox_IsPrintFinished(entry->printerId)) {
            if (++entry->timer > 60) {
                LinkSync_StartSync(4);
                entry->state = 44;
            }
        }
        break;
    case 44:
        if (LinkSync_IsSynced(4)) {
            ClearFrameAndWindow2(&entry->messageWindow, FALSE);
            entry->callback(FALSE, NULL);
            entry->state = 5;
        }
        break;
    }

    return FALSE;
}

u8 Colosseum_PrintMessage(ColosseumEntry *entry, String *message) {
    Window *window = &entry->messageWindow;
    Options *options;
    if (!WindowIsInUse(window)) {
        DialogBox_AddWindowToLayer3(entry->fieldSystem->bgConfig, window, GF_BG_LYR_MAIN_3);
        options = Save_PlayerData_GetOptionsAddr(entry->fieldSystem->saveData);
        DialogBox_LoadFrame(window, options);
    } else {
        DialogBox_Clear(window);
    }
    options = Save_PlayerData_GetOptionsAddr(entry->fieldSystem->saveData);
    return DialogBox_PrintMessage(window, message, options, TRUE);
}

void Colosseum_RemoveWindows(ColosseumEntry *entry, BOOL clearFrame) {
    if (WindowIsInUse(&entry->messageWindow)) {
        if (clearFrame) {
            ClearFrameAndWindow2(&entry->messageWindow, FALSE);
            ClearWindowTilemapAndCopyToVram(&entry->messageWindow);
        }
        RemoveWindow(&entry->messageWindow);
        InitWindow(&entry->messageWindow);
    }
    if (WindowIsInUse(&entry->monListWindow)) {
        RemoveWindow(&entry->monListWindow);
        InitWindow(&entry->monListWindow);
    }
    if (WindowIsInUse(&entry->choiceWindow)) {
        RemoveWindow(&entry->choiceWindow);
        InitWindow(&entry->choiceWindow);
    }
}

void Colosseum_StartEntry(FieldSystem *fieldSystem, ColosseumEntryCallback callback) {
    ColosseumEntry *entry;
    u32 size;
    if (fieldSystem->taskman != NULL) {
        return;
    }
    entry = Heap_AllocAtEnd(HEAP_ID_FIELD2, sizeof(ColosseumEntry));
    MI_CpuFill8(entry, 0, sizeof(ColosseumEntry));
    entry->timer = 5;
    entry->fieldSystem = fieldSystem;
    entry->callback = callback;
    entry->msgFormat = MessageFormat_New(HEAP_ID_FIELD2);
    entry->msgData = NewMsgDataFromNarc(MSGDATA_LOAD_DIRECT, NARC_msgdata_msg, NARC_msg_msg_0014_bin, HEAP_ID_FIELD2);
    entry->message = String_New(200, HEAP_ID_FIELD2);
    entry->expandedMessage = String_New(200, HEAP_ID_FIELD2);
    InitWindow(&entry->messageWindow);
    InitWindow(&entry->monListWindow);
    InitWindow(&entry->choiceWindow);
    entry->cursor = ListMenuCursorNew(HEAP_ID_FIELD2);
    entry->linkMode = sub_0203993C();
    entry->sentMons = NULL;
    entry->receivedMons = NULL;
    entry->party = NULL;
    entry->exchangeFlags = 0;
    entry->netId = Comm_GetLocalNetId();
    entry->opponentProfile = CommPlayerInfo_GetProfile(1 ^ entry->netId);
    switch (entry->linkMode) {
    case 3:
        size = Colosseum_GetMixBattleMonsSize();
        entry->sentMons = Heap_AllocAtEnd(HEAP_ID_FIELD2, size);
        entry->receivedMons = Heap_AllocAtEnd(HEAP_ID_FIELD2, size);
        entry->party = SaveArray_Party_Alloc(HEAP_ID_FIELD2);
        Party_InitWithMaxSize(entry->party, COLOSSEUM_MIX_BATTLE_MONS);
        entry->delay = 5;
        entry->state = 23;
        break;
    case 4:
        entry->state = 9;
        break;
    default:
        if (entry->fieldSystem->linkBattleRuleset != NULL) {
            entry->state = 9;
        } else {
            entry->state = 0;
        }
        break;
    }
    FieldSystem_CreateTask(fieldSystem, Task_ColosseumEntry, entry);
}

void Colosseum_FreeEntry(ColosseumEntry *entry) {
    if (entry->party) {
        Heap_Free(entry->party);
    }
    if (entry->sentMons) {
        Heap_Free(entry->sentMons);
    }
    if (entry->receivedMons) {
        Heap_Free(entry->receivedMons);
    }
    DestroyMsgData(entry->msgData);
    MessageFormat_Delete(entry->msgFormat);
    String_Delete(entry->message);
    String_Delete(entry->expandedMessage);
    DestroyListMenuCursorObj(entry->cursor);
    Colosseum_RemoveWindows(entry, TRUE);
}

ColosseumEntry *Colosseum_GetEntry(FieldSystem *fieldSystem) {
    return TaskManager_GetEnvironment(fieldSystem->taskman);
}

// The three Pokemon sent in a Mix Battle, followed by the ready flag (NULL: we cancelled).
void Colosseum_PackMixBattleMons(ColosseumEntry *entry, BOOL ready) {
    Party *party = SaveArray_Party_Get(entry->fieldSystem->saveData);
    u8 *buffer = entry->sentMons;
    u32 size = sub_02070D90();
    int i;
    *(BOOL *)(buffer + size * COLOSSEUM_MIX_BATTLE_MONS) = ready;
    if (*(BOOL *)(buffer + size * COLOSSEUM_MIX_BATTLE_MONS)) {
        for (i = 0; i < COLOSSEUM_MIX_BATTLE_MONS; i++) {
            Pokemon *mon = Party_GetMonByIndex(party, entry->selectedOrder[i] - 1);
            MI_CpuCopy8(mon, buffer, size);
            buffer += size;
        }
    }
}

#pragma force_active on

BOOL Colosseum_SendMixBattleMons(ColosseumEntry *entry) {
    BOOL result;
    if ((entry->exchangeFlags & COLOSSEUM_EXCHANGE_SENT) == 0) {
        u8 *mons = entry->sentMons;
        u32 size = Colosseum_GetMixBattleMonsSize();
        if (entry->netId == 0) {
            result = sub_02037088(106, mons, size);
        } else {
            result = sub_02036FD8(106, mons, size);
        }
        if (result) {
            entry->exchangeFlags |= COLOSSEUM_EXCHANGE_SENT;
        }
    } else {
        result = TRUE;
    }
    return result;
}

#pragma force_active off

BOOL Colosseum_MixBattleMonsExchanged(ColosseumEntry *entry) {
    return entry->exchangeFlags == COLOSSEUM_EXCHANGE_DONE;
}

BOOL Colosseum_OpponentIsReady(ColosseumEntry *entry) {
    u32 size = sub_02070D90();
    return *(BOOL *)(entry->receivedMons + size * COLOSSEUM_MIX_BATTLE_MONS);
}

BOOL Colosseum_SendMixBattleChoice(ColosseumEntry *entry) {
    BOOL result;
    if (entry->netId == 0) {
        result = sub_02037108(107, &entry->choice, 1);
    } else {
        result = Comm_SendCommand(107, &entry->choice, 1);
    }
    if (result) {
        entry->exchangeFlags |= COLOSSEUM_EXCHANGE_SENT;
    }
    return result;
}

BOOL Colosseum_MixBattleChoicesExchanged(ColosseumEntry *entry) {
    return entry->exchangeFlags == COLOSSEUM_EXCHANGE_DONE;
}

void Colosseum_SetOpponentParty(ColosseumEntry *entry) {
    u32 size = sub_02070D90();
    int i;
    Party_InitWithMaxSize(entry->party, COLOSSEUM_MIX_BATTLE_MONS);
    for (i = 0; i < COLOSSEUM_MIX_BATTLE_MONS; i++) {
        Party_AddMon(entry->party, (Pokemon *)(entry->receivedMons + i * size));
    }
}

// Our Pokemon the opponent picked is replaced with the one we picked from theirs.
void Colosseum_ExchangeMixBattleMons(ColosseumEntry *entry) {
    u32 size = sub_02070D90();
    u8 choice = entry->choice;
    u8 opponentChoice = entry->opponentChoice;
    int i;
    MI_CpuCopy8(entry->receivedMons + choice * size, entry->sentMons + opponentChoice * size, size);
    Party_InitWithMaxSize(entry->party, COLOSSEUM_MIX_BATTLE_MONS);
    for (i = 0; i < COLOSSEUM_MIX_BATTLE_MONS; i++) {
        Party_AddMon(entry->party, (Pokemon *)(entry->sentMons + i * size));
    }
}

#pragma force_active on

void Colosseum_ShowOpponentMons(ColosseumEntry *entry, int cursor) {
    Window *window = &entry->monListWindow;
    u32 monSize;
    MsgData *speciesNames;
    int i, offset, y;
    if (!WindowIsInUse(window)) {
        speciesNames = NewMsgDataFromNarc(MSGDATA_LOAD_LAZY, NARC_msgdata_msg, NARC_msg_msg_0237_bin, HEAP_ID_FIELD1);
        monSize = sub_02070D90();
        AddWindowParameterized(entry->fieldSystem->bgConfig, window, GF_BG_LYR_MAIN_3, 21, 9, 10, 8, 13, 10);
        LoadUserFrameGfx1(entry->fieldSystem->bgConfig, GF_BG_LYR_MAIN_3, 1, 11, 0, HEAP_ID_FIELD1);
        FillWindowPixelBuffer(window, 15);
        i = 0;
        offset = i;
        y = i;
        for (; i < COLOSSEUM_MIX_BATTLE_MONS; i++) {
            ReadMsgDataIntoString(speciesNames, GetMonData((Pokemon *)(entry->receivedMons + offset), MON_DATA_SPECIES, NULL), entry->message);
            AddTextPrinterParameterized(window, 0, entry->message, 16, y, TEXT_SPEED_NOTRANSFER, NULL);
            offset += monSize;
            y += 16;
        }
        ReadMsgDataIntoString(entry->msgData, msg_0014_00021, entry->message);
        AddTextPrinterParameterized(window, 0, entry->message, 16, i << 4, TEXT_SPEED_NOTRANSFER, NULL);
        DestroyMsgData(speciesNames);
    }
    FillWindowPixelRect(window, 15, 0, 0, 16, window->height * 8);
    ListMenuUpdateCursorObj_Int(entry->cursor, &entry->monListWindow, 0, cursor << 4);
    DrawFrameAndWindow1(&entry->monListWindow, FALSE, 1, 11);
    entry->menuCursor = cursor;
    entry->menuCount = COLOSSEUM_MIX_BATTLE_MONS + 1;
    entry->menuWindow = window;
}

#pragma force_active off

// SUMMARY / OK / CANCEL for the picked Pokemon.
void Colosseum_ShowChoiceMenu(ColosseumEntry *entry, int cursor) {
    Window *window = &entry->choiceWindow;
    int i;
    int y;

    if (!WindowIsInUse(window)) {
        AddWindowParameterized(entry->fieldSystem->bgConfig, window, GF_BG_LYR_MAIN_3, 20, 11, 11, 6, 13, 90);
        LoadUserFrameGfx1(entry->fieldSystem->bgConfig, GF_BG_LYR_MAIN_3, 1, 11, 0, HEAP_ID_FIELD1);
        FillWindowPixelBuffer(window, 15);
        for (i = 0, y = 0; i < 3; i++, y += 16) {
            ReadMsgDataIntoString(entry->msgData, i + msg_0014_00022, entry->message);
            AddTextPrinterParameterized(window, 0, entry->message, 16, y, TEXT_SPEED_NOTRANSFER, NULL);
        }
    }
    entry->menuCount = 3;
    entry->menuWindow = window;
    entry->menuCursor = cursor;
    FillWindowPixelRect(window, 15, 0, 0, 16, window->height * 8);
    ListMenuUpdateCursorObj_Int(entry->cursor, entry->menuWindow, 0, cursor * 16);
    DrawFrameAndWindow1(entry->menuWindow, FALSE, 1, 11);
}

int Colosseum_HandleMenuInput(ColosseumEntry *entry) {
    int cursor;

    if (gSystem.newKeys & PAD_KEY_UP) {
        entry->menuCursor = entry->menuCursor == 0 ? entry->menuCount - 1 : entry->menuCursor - 1;
    } else if (gSystem.newKeys & PAD_KEY_DOWN) {
        cursor = entry->menuCursor;
        if (cursor == entry->menuCount - 1) {
            cursor = 0;
        } else {
            cursor++;
        }
        entry->menuCursor = cursor;
    } else if (gSystem.newKeys & PAD_BUTTON_A) {
        PlaySE(SEQ_SE_DP_SELECT);
        if (entry->menuCursor < entry->menuCount - 1) {
            return COLOSSEUM_MENU_SELECT;
        }
        return COLOSSEUM_MENU_CANCEL;
    } else if (gSystem.newKeys & PAD_BUTTON_B) {
        PlaySE(SEQ_SE_DP_SELECT);
        return COLOSSEUM_MENU_CANCEL;
    } else {
        return COLOSSEUM_MENU_NONE;
    }

    PlaySE(SEQ_SE_DP_SELECT);
    FillWindowPixelRect(entry->menuWindow, 15, 0, 0, 16, entry->menuWindow->height * 8);
    ListMenuUpdateCursorObj_Int(entry->cursor, entry->menuWindow, 0, entry->menuCursor << 4);
    CopyWindowPixelsToVram_TextMode(entry->menuWindow);
    return COLOSSEUM_MENU_NONE;
}

void Colosseum_CloseMenu(ColosseumEntry *entry) {
    EraseFrameAndWindow1(entry->menuWindow, TRUE);
}

void Colosseum_RecvMixBattleMons(int netId, int size, void *data, FieldSystem *fieldSystem) {
    ColosseumEntry *entry = Colosseum_GetEntry(fieldSystem);
    if (entry->netId != netId) {
        entry->exchangeFlags |= COLOSSEUM_EXCHANGE_RECEIVED;
    }
}

u32 Colosseum_GetMixBattleMonsSize(void) {
    u32 size = sub_02070D90();
    return size * COLOSSEUM_MIX_BATTLE_MONS + 4;
}

u8 *Colosseum_GetMixBattleMonsBuffer(int netId, FieldSystem *fieldSystem) {
    ColosseumEntry *entry = Colosseum_GetEntry(fieldSystem);
    if (entry->netId != netId) {
        return entry->receivedMons;
    }
    return NULL;
}

void Colosseum_RecvMixBattleChoice(int netId, int size, const u8 *data, FieldSystem *fieldSystem) {
    ColosseumEntry *entry = Colosseum_GetEntry(fieldSystem);
    if (entry->netId != netId) {
        entry->opponentChoice = *data;
        entry->exchangeFlags |= COLOSSEUM_EXCHANGE_RECEIVED;
    }
}

#pragma force_active on

BOOL Task_ColosseumTrainerCard(TaskManager *taskManager) {
    FieldSystem *fieldSystem = TaskManager_GetFieldSystem(taskManager);
    ColosseumTrainerCard *env = TaskManager_GetEnvironment(taskManager);
    u8 *card = CommClubLink_GetCard(env->netId, 0, 0);
    switch (env->state) {
    case 0: {
        int message;
        env->msgFormat = MessageFormat_New(HEAP_ID_FIELD1);
        env->msgData = NewMsgDataFromNarc(MSGDATA_LOAD_DIRECT, NARC_msgdata_msg, NARC_msg_msg_0014_bin, HEAP_ID_FIELD1);
        env->strings[0] = String_New(200, HEAP_ID_FIELD1);
        env->strings[1] = String_New(200, HEAP_ID_FIELD1);
        // card[0]: game version; card[3]: stars
        switch (card[0]) {
        case VERSION_HEARTGOLD:
        case VERSION_SOULSILVER:
            message = card[3] + msg_0014_00002;
            break;
        case VERSION_DIAMOND:
            message = msg_0014_00025;
            break;
        case VERSION_PEARL:
            message = msg_0014_00026;
            break;
        case VERSION_PLATINUM:
            message = msg_0014_00027;
            break;
        default:
            message = msg_0014_00002;
            break;
        }
        ReadMsgDataIntoString(env->msgData, message, env->strings[0]);
        BufferPlayersName(env->msgFormat, 0, CommPlayerInfo_GetProfile(env->netId));
        StringExpandPlaceholders(env->msgFormat, env->strings[1], env->strings[0]);
        DialogBox_AddWindowToLayer3(fieldSystem->bgConfig, &env->window, GF_BG_LYR_MAIN_3);
        DialogBox_LoadFrame(&env->window, Save_PlayerData_GetOptionsAddr(fieldSystem->saveData));
        env->printerId = DialogBox_PrintMessage(&env->window, env->strings[1], Save_PlayerData_GetOptionsAddr(fieldSystem->saveData), TRUE);
        env->state++;
    } break;
    case 1:
        if (DialogBox_IsPrintFinished((u8)env->printerId) && gSystem.newKeys & PAD_BUTTON_A) {
            DestroyMsgData(env->msgData);
            MessageFormat_Delete(env->msgFormat);
            String_Delete(env->strings[0]);
            String_Delete(env->strings[1]);
            ClearFrameAndWindow2(&env->window, FALSE);
            RemoveWindow(&env->window);
            FieldMap_FadeScreen(FADE_TYPE_BRIGHTNESS_OUT);
            env->state++;
        }
        break;
    case 2:
        if (IsPaletteFadeFinished()) {
            env->state++;
        }
        break;
    case 3:
        MI_CpuCopy8(card, env->cardArgs.unk0, sizeof(env->cardArgs.unk0));
        TrainerCard_LaunchApp(fieldSystem, &env->cardArgs);
        env->state++;
        break;
    case 4:
        if (!FieldSystem_ApplicationIsRunning(fieldSystem)) {
            env->state++;
        }
        break;
    case 5:
        FieldSystem_LoadFieldOverlay(fieldSystem);
        env->state++;
        break;
    case 6:
        if (!sub_020505C8(fieldSystem)) {
            FieldMap_FadeScreen(FADE_TYPE_BRIGHTNESS_IN);
            sub_02056E60();
            env->state++;
        }
        break;
    case 7:
        sub_02057F70();
        Heap_Free(env);
        return TRUE;
    default:
        return TRUE;
    }
    return FALSE;
}

#pragma force_active off

// The player pressed A: show the Trainer Card of the player sitting opposite (on the square the player faces).
void Colosseum_ShowOpponentTrainerCard(FieldSystem *fieldSystem) {
    int i;
    u32 localNetId = Comm_GetLocalNetId();
    int frontX = CommPlayerManager_GetPlayerFrontX();
    int frontZ = CommPlayerManager_GetPlayerFrontZ(localNetId);

    for (i = 0; i < Comm_CountConnected(); i++) {
        if (i != localNetId && frontX == CommPlayerManager_GetPlayerX(i) && frontZ == CommPlayerManager_GetPlayerZ(i)) {
            ColosseumTrainerCard *env = Heap_AllocAtEnd(HEAP_ID_FIELD2, sizeof(ColosseumTrainerCard));
            env->netId = i;
            env->state = 0;
            FieldSystem_CreateTask(fieldSystem, Task_ColosseumTrainerCard, env);
            sub_0203E2F4();
            return;
        }
    }
}
