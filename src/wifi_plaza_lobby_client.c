#include <string.h>

#include "global.h"

#include "assert.h"
#include "friend_roster.h"
#include "heap.h"
#include "save_wifi_history.h"
#include "wifi_connection.h"
#include "wifi_plaza_internal.h"

// The lobby client of the Wi-Fi Plaza (gWiFiPlazaLobbyClient): one instance for the plaza and its apps on top of the lobby
// library (PPW_LobbyInitializeAsync ..). It keeps the lobby state, the location of every lobby slot, the message tables, the
// lobby time and up to three matchmaking sessions.

// Slot getter of news event type 8 (sWiFiPlazaNewsEventSlotGetters).
u16 WiFiPlazaNewsEvent_GetMinigameResultSlot(WiFiPlazaNewsEvent *event, u32 index) {
    if (event->b <= index) {
        return 20;
    }
    return event->slots[index];
}

BOOL WiFiPlaza_IsMinigamePlayerCountValid(u32 activity, u32 count) {
    if (activity <= 1) {
        if (count < 1 || count > 4) {
            return FALSE;
        }
    } else {
        if (count < 2 || count > 4) {
            return FALSE;
        }
    }
    return TRUE;
}

#pragma force_active on

void WiFiPlazaLobbyClient_Init(enum HeapID heapID, SaveData *save, u32 count, WiFiPlazaCallbacks *callbacks, void *callbackArg) {
    GF_ASSERT(gWiFiPlazaLobbyClient == NULL);
    GF_ASSERT(count < 300);
    gWiFiPlazaLobbyClient = Heap_Alloc(heapID, sizeof(WiFiPlazaLobbyClient));
    memset(gWiFiPlazaLobbyClient, 0, sizeof(WiFiPlazaLobbyClient));
    gWiFiPlazaLobbyClient->save = save;
    gWiFiPlazaLobbyClient->friendRoster = Save_FriendRoster_Get(save);
    gWiFiPlazaLobbyClient->wifiHistory = Save_WiFiHistory_Get(save);
    gWiFiPlazaLobbyClient->callbacks = *callbacks;
    gWiFiPlazaLobbyClient->callbackArg = callbackArg;
    WiFiPlazaLobbyClient_AllocPlayerData(gWiFiPlazaLobbyClient, count, heapID);
    WiFiPlazaLobbyClient_AllocIdLists(gWiFiPlazaLobbyClient, 20, 8, heapID);
    WiFiPlazaLobbyClient_InitMatches(gWiFiPlazaLobbyClient);
    WiFiPlazaLobbyIdValueList_Init(&gWiFiPlazaLobbyClient->unk_6a0, 128, heapID);
    gWiFiPlazaLobbyClient->initResult = 0;
}

#pragma force_active reset

void WiFiPlazaLobbyClient_Free(void) {
    GF_ASSERT(gWiFiPlazaLobbyClient != NULL);
    WiFiPlazaLobbyIdValueList_Free(&gWiFiPlazaLobbyClient->unk_6a0);
    WiFiPlazaLobbyClient_FreeIdLists(gWiFiPlazaLobbyClient);
    WiFiPlazaLobbyClient_FreePlayerData(gWiFiPlazaLobbyClient);
    Heap_Free(gWiFiPlazaLobbyClient);
    gWiFiPlazaLobbyClient = NULL;
}

#pragma force_active on

// Once per frame: follows the state of the lobby library and the channel, refreshes the lobby time and the
// matchmaking sessions.
int WiFiPlazaLobbyClient_Update(void) {
    if (gWiFiPlazaLobbyClient == NULL) {
        return 0;
    }
    switch (PPW_LobbyProcess()) {
    case 0:
        gWiFiPlazaLobbyClient->state = 0;
        break;
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
    case 6:
    case 7:
        gWiFiPlazaLobbyClient->state = 1;
        break;
    case 8:
        gWiFiPlazaLobbyClient->state = 2;
        break;
    case 9:
        gWiFiPlazaLobbyClient->state = 3;
        break;
    case 10:
        gWiFiPlazaLobbyClient->state = 4;
        break;
    }
    if (gWiFiPlazaLobbyClient->state == 2) {
        WiFiPlazaLobbyMatches_Update(gWiFiPlazaLobbyClient);
        PPW_LobbyGetTimeInfo(&gWiFiPlazaLobbyClient->time);
        switch (PPW_LobbyGetSubChannelState()) {
        case 0:
            gWiFiPlazaLobbyClient->channelState = 0;
            gWiFiPlazaLobbyClient->channel = 4;
            break;
        case 1:
            gWiFiPlazaLobbyClient->channelState = 1;
            break;
        case 2:
            if (gWiFiPlazaLobbyClient->channelState != 3) {
                gWiFiPlazaLobbyClient->channelState = 2;
            }
            break;
        case 3:
            gWiFiPlazaLobbyClient->channelState = 4;
            break;
        }
    }
    return WiFiPlazaLobbyClient_GetState();
}

#pragma force_active reset

void WiFiPlazaLobbyClient_GetLastError(void) {
    GF_ASSERT(gWiFiPlazaLobbyClient != NULL);
    if (gWiFiPlazaLobbyClient->initResult != 0) {
        ov45_02233220();
    } else {
        PPW_LobbyGetLastError();
    }
}

void WiFiPlazaLobbyClient_ToErrorCode(void) {
    PPW_LobbyToErrorCode();
}

void WiFiPlazaLobbyClient_Initialize(int value) {
    WiFiPlazaLobbyClient_InitializeEx(value, 0);
}

#pragma force_active on

void WiFiPlazaLobbyClient_InitializeEx(int value, int mode) {
    void (*callbacks[17])(void);
    void *userData;
    GF_ASSERT(gWiFiPlazaLobbyClient != NULL);
    callbacks[0] = (void (*)(void))WiFiPlazaLobbyClient_OnEnterChannel;
    callbacks[1] = (void (*)(void))WiFiPlazaLobbyClient_OnPlayerEnter;
    callbacks[2] = (void (*)(void))WiFiPlazaLobbyClient_OnPlayerLeave;
    callbacks[3] = (void (*)(void))ov45_0222FDD4;
    callbacks[4] = (void (*)(void))WiFiPlazaLobbyClient_OnMessage;
    callbacks[5] = (void (*)(void))WiFiPlazaLobbyClient_OnLocationsChanged;
    callbacks[6] = (void (*)(void))WiFiPlazaLobbyClient_OnPlayerRecordChanged;
    callbacks[7] = (void (*)(void))WiFiPlazaLobbyClient_OnPlayerUpdate;
    callbacks[8] = (void (*)(void))WiFiPlazaLobbyClient_OnMatchInfo;
    callbacks[9] = (void (*)(void))WiFiPlazaLobbyClient_OnMatchClosed;
    callbacks[10] = (void (*)(void))WiFiPlazaLobbyClient_OnLobbyEvent;
    callbacks[11] = (void (*)(void))WiFiPlazaLobbyClient_OnOwnRecord;
    callbacks[12] = (void (*)(void))ov45_02230090;
    callbacks[13] = (void (*)(void))WiFiPlazaLobbyClient_OnSpecialVisitorsReceived;
    callbacks[14] = (void (*)(void))WiFiPlazaLobbyClient_OnSurveyReceived;
    callbacks[15] = (void (*)(void))WiFiPlazaLobbyClient_OnSurveySubmitted;
    callbacks[16] = (void (*)(void))ov45_02230130;
    userData = FriendRoster_GetUserData(gWiFiPlazaLobbyClient->friendRoster);
    gWiFiPlazaLobbyClient->initResult = PPW_LobbyInitializeAsync(sStr_pokemonplatds, sStr_IIup73, mode, callbacks, userData, value, gWiFiPlazaLobbyClient->playerDataSize);
    WiFiPlazaLobbyClient_CheckResult(gWiFiPlazaLobbyClient->initResult);
}

#pragma force_active reset

BOOL WiFiPlazaLobbyClient_IsInLobby(void) {
    GF_ASSERT(gWiFiPlazaLobbyClient != NULL);
    return gWiFiPlazaLobbyClient->state == 2;
}

void WiFiPlazaLobbyClient_Shutdown(void) {
    GF_ASSERT(gWiFiPlazaLobbyClient != NULL);
    WiFiPlazaLobbyClient_ClearErrors();
    GF_ASSERT(WiFiPlazaLobbyClient_CheckResult(PPW_LobbyShutdownAsync()));
}

BOOL WiFiPlazaLobbyClient_IsDisconnected(void) {
    GF_ASSERT(gWiFiPlazaLobbyClient != NULL);
    return gWiFiPlazaLobbyClient->state == 0;
}

// The lobby state, 4 (error) when the channel or the client failed.
int WiFiPlazaLobbyClient_GetState(void) {
    WiFiPlazaLobbyClient *client;
    GF_ASSERT(gWiFiPlazaLobbyClient != NULL);
    client = gWiFiPlazaLobbyClient;
    if (client->channel != 4 && client->channelState == 4) {
        return 4;
    }
    if (client->unk_133) {
        return 4;
    }
    if (client->unk_132) {
        return 4;
    }
    return client->state;
}

u8 ov45_0222E9BC(void) {
    u8 value;
    GF_ASSERT(gWiFiPlazaLobbyClient != NULL);
    value = gWiFiPlazaLobbyClient->unk_131;
    gWiFiPlazaLobbyClient->unk_131 = 0;
    return value;
}

int WiFiPlazaLobbyClient_GetOwnId(void) {
    GF_ASSERT(gWiFiPlazaLobbyClient != NULL);
    return PPW_LobbyGetMyUserId();
}

void WiFiPlazaLobbyClient_SetOwnPlayer(WiFiPlazaPlayer *player) {
    int ownId;
    GF_ASSERT(gWiFiPlazaLobbyClient != NULL);
    ownId = PPW_LobbyGetMyUserId();
    WiFiPlazaLobbyClient_SetPlayerData(gWiFiPlazaLobbyClient, ownId, player);
    PPW_LobbyUpdateMyProfile(player, gWiFiPlazaLobbyClient->playerDataSize, 0);
}

WiFiPlazaPlayer *WiFiPlazaLobbyClient_GetPlayer(int id) {
    GF_ASSERT(gWiFiPlazaLobbyClient != NULL);
    return WiFiPlazaLobbyClient_GetPlayerData(gWiFiPlazaLobbyClient, id);
}

#pragma force_active on

// The time the player entered the lobby.
void WiFiPlazaLobbyClient_GetEntryTime(int id, s64 *time) {
    WiFiPlazaLobbyEntryTime *entered;
    GF_ASSERT(gWiFiPlazaLobbyClient != NULL);
    entered = &WiFiPlazaLobbyClient_GetPlayerRecord(gWiFiPlazaLobbyClient, id)->entryTime;
    *(WiFiPlazaLobbyEntryTime *)time = *entered;
}

#pragma force_active reset

// The player who has been in the lobby the longest, -1 if none.
int WiFiPlazaLobbyClient_GetOldestPlayer(void) {
    s64 oldest;
    s64 time;
    int result;
    int slot;

    result = -1;
    oldest = 0x0FFFFFFFFFFFFFFFLL;
    GF_ASSERT(gWiFiPlazaLobbyClient != NULL);
    for (slot = 0; slot < 20; slot++) {
        int id = WiFiPlazaLobbyClient_GetIdListEntry(gWiFiPlazaLobbyClient, slot, 0);
        if (id != -1) {
            WiFiPlazaLobbyClient_GetEntryTime(id, &time);
            if (time < oldest) {
                oldest = time;
                result = id;
            }
        }
    }
    return result;
}

// The same without the given player.
int WiFiPlazaLobbyClient_GetOldestOtherPlayer(int excluded) {
    s64 oldest;
    s64 time;
    int result;
    int slot;

    result = -1;
    oldest = 0x0FFFFFFFFFFFFFFFLL;
    GF_ASSERT(gWiFiPlazaLobbyClient != NULL);
    for (slot = 0; slot < 20; slot++) {
        int id = WiFiPlazaLobbyClient_GetIdListEntry(gWiFiPlazaLobbyClient, slot, 0);
        if (id != -1 && id != excluded) {
            WiFiPlazaLobbyClient_GetEntryTime(id, &time);
            if (time < oldest) {
                oldest = time;
                result = id;
            }
        }
    }
    return result;
}

#pragma force_active on

BOOL WiFiPlazaLobbyClient_JoinChannel(int channel) {
    BOOL result;
    GF_ASSERT(gWiFiPlazaLobbyClient != NULL);
    result = WiFiPlazaLobbyClient_CheckResult(PPW_LobbyJoinSubChannelAsync(WiFiPlazaLobbyClient_ChannelToLib(channel)));
    if (result == TRUE) {
        gWiFiPlazaLobbyClient->channel = channel;
        gWiFiPlazaLobbyClient->unk_12f = 0;
    }
    return result;
}

#pragma force_active reset

u8 WiFiPlazaLobbyClient_GetJoinChannelResult(void) {
    GF_ASSERT(gWiFiPlazaLobbyClient != NULL);
    return gWiFiPlazaLobbyClient->unk_12f;
}

#pragma force_active on

BOOL WiFiPlazaLobbyClient_LeaveChannel(void) {
    GF_ASSERT(gWiFiPlazaLobbyClient != NULL);
    {
        BOOL result = WiFiPlazaLobbyClient_CheckResult(PPW_LobbyLeaveSubChannel());
        if (result == TRUE) {
            gWiFiPlazaLobbyClient->channelState = 3;
        }
        return result;
    }
}

#pragma force_active reset

BOOL WiFiPlazaLobbyClient_IsNotInChannel(void) {
    GF_ASSERT(gWiFiPlazaLobbyClient != NULL);
    return gWiFiPlazaLobbyClient->channelState == 0;
}

BOOL WiFiPlazaLobbyClient_CanJoinChannel(int channel) {
    GF_ASSERT(gWiFiPlazaLobbyClient != NULL);
    return WiFiPlazaLobbyClient_HasChannelRoom(gWiFiPlazaLobbyClient, channel);
}

void WiFiPlazaLobbyClient_GetPlayerList(WiFiPlazaPlayerList *list) {
    GF_ASSERT(gWiFiPlazaLobbyClient != NULL);
    list->count = gWiFiPlazaLobbyClient->idLists[0].count;
    list->ids = gWiFiPlazaLobbyClient->idLists[0].ids;
}

void WiFiPlazaLobbyClient_GetChannelPlayerList(u32 *out) {
    GF_ASSERT(gWiFiPlazaLobbyClient != NULL);
    out[0] = gWiFiPlazaLobbyClient->idLists[1].count;
    out[1] = (u32)gWiFiPlazaLobbyClient->idLists[1].ids;
}

// Lobby player id <-> lobby slot.
int WiFiPlazaLobbyClient_IdToSlot(int id) {
    return WiFiPlazaLobbyClient_FindInIdList(gWiFiPlazaLobbyClient, id, 0);
}

int WiFiPlazaLobbyClient_IdToChannelSlot(int id) {
    return WiFiPlazaLobbyClient_FindInIdList(gWiFiPlazaLobbyClient, id, 1);
}

int WiFiPlazaLobbyClient_SlotToId(int slot) {
    return WiFiPlazaLobbyClient_GetIdListEntry(gWiFiPlazaLobbyClient, slot, 0);
}

int WiFiPlazaLobbyClient_ChannelSlotToId(int slot) {
    return WiFiPlazaLobbyClient_GetIdListEntry(gWiFiPlazaLobbyClient, slot, 1);
}

void WiFiPlazaLobbyClient_GetTime(s64 *time) {
    GF_ASSERT(gWiFiPlazaLobbyClient != NULL);
    *time = gWiFiPlazaLobbyClient->time;
}

u32 WiFiPlazaLobbyClient_GetScheduleValue(int field) {
    WiFiPlazaLobbyClientUnk1B4 *unk;
    u32 result;
    GF_ASSERT(gWiFiPlazaLobbyClient != NULL);
    {
        BOOL valid = TRUE;
        if (gWiFiPlazaLobbyClient->state != 2 && gWiFiPlazaLobbyClient->state != 4) {
            valid = FALSE;
        }
        GF_ASSERT(valid);
    }
    GF_ASSERT(field < 8);
    unk = &gWiFiPlazaLobbyClient->unk_1b4;
    switch (field) {
    case 0:
        result = unk->unk_00;
        break;
    case 1:
        result = unk->unk_04;
        break;
    case 2:
        result = unk->unk_0c;
        break;
    case 3:
        result = unk->unk_0d;
        break;
    case 4:
        result = (unk->unk_08 & 1) != 0;
        break;
    case 5:
        result = WiFiPlazaLobbySchedule_FindValue(unk, 0x13);
        break;
    case 6:
        result = WiFiPlazaLobbySchedule_FindValue(unk, 0x10);
        break;
    case 7:
        result = WiFiPlazaLobbySchedule_FindValue(unk, 0x11);
        break;
    }
    return result;
}

#pragma force_active on

int WiFiPlazaLobbyClient_StartAddOwnLocation(void) {
    GF_ASSERT(gWiFiPlazaLobbyClient != NULL);
    {
        int result = WiFiPlazaLobbyClient_AddOwnLocation(gWiFiPlazaLobbyClient);
        if (result) {
            gWiFiPlazaLobbyClient->unk_130 = 1;
        }
        return result;
    }
}

#pragma force_active reset

BOOL WiFiPlazaLobbyClient_IsOwnLocationAdded(void) {
    return gWiFiPlazaLobbyClient->unk_130 == 0;
}

u32 WiFiPlazaLobbyClient_GetLocationCountry(u8 slot) {
    GF_ASSERT(gWiFiPlazaLobbyClient != NULL);
    GF_ASSERT((u32)slot < 20);
    return gWiFiPlazaLobbyClient->locations[slot].country;
}

u8 WiFiPlazaLobbyClient_GetLocationRegion(u8 slot) {
    WiFiPlazaLobbyLocation *location;
    GF_ASSERT(gWiFiPlazaLobbyClient != NULL);
    GF_ASSERT((u32)slot < 20);
    location = &gWiFiPlazaLobbyClient->locations[slot];
    return location->region;
}

// The message types sent with WiFiPlazaLobbyClient_SendToLobby / SendToPlayer (each with the size of its data).
void WiFiPlazaLobbyClient_SetLobbyCommands(const void *commands, u32 count, void *arg) {
    GF_ASSERT(gWiFiPlazaLobbyClient != NULL);
    gWiFiPlazaLobbyClient->commandTables[0].commands = commands;
    gWiFiPlazaLobbyClient->commandTables[0].count = count;
    gWiFiPlazaLobbyClient->commandTables[0].arg = arg;
}

// The message types sent with WiFiPlazaLobbyClient_SendToChannel.
void WiFiPlazaLobbyClient_SetChannelCommands(const void *commands, u32 count, void *arg) {
    GF_ASSERT(gWiFiPlazaLobbyClient != NULL);
    gWiFiPlazaLobbyClient->commandTables[1].commands = commands;
    gWiFiPlazaLobbyClient->commandTables[1].count = count;
    gWiFiPlazaLobbyClient->commandTables[1].arg = arg;
}

// Clears the 12 bytes one at a time.
void WiFiPlazaLobbyClient_ClearLobbyCommands(void) {
    u8 *bytes;
    GF_ASSERT(gWiFiPlazaLobbyClient != NULL);
    bytes = (u8 *)&gWiFiPlazaLobbyClient->commandTables[0];
    bytes[0] = 0;
    bytes[1] = 0;
    bytes[2] = 0;
    bytes[3] = 0;
    bytes[4] = 0;
    bytes[5] = 0;
    bytes[6] = 0;
    bytes[7] = 0;
    bytes[8] = 0;
    bytes[9] = 0;
    bytes[10] = 0;
    bytes[11] = 0;
}

void WiFiPlazaLobbyClient_ClearChannelCommands(void) {
    u8 *bytes;
    GF_ASSERT(gWiFiPlazaLobbyClient != NULL);
    bytes = (u8 *)&gWiFiPlazaLobbyClient->commandTables[1];
    bytes[0] = 0;
    bytes[1] = 0;
    bytes[2] = 0;
    bytes[3] = 0;
    bytes[4] = 0;
    bytes[5] = 0;
    bytes[6] = 0;
    bytes[7] = 0;
    bytes[8] = 0;
    bytes[9] = 0;
    bytes[10] = 0;
    bytes[11] = 0;
}

void WiFiPlazaLobbyClient_SendToLobby(int type, const void *data, int size) {
    const WiFiPlazaLobbyCommand *command;
    GF_ASSERT(gWiFiPlazaLobbyClient != NULL);
    GF_ASSERT(gWiFiPlazaLobbyClient->commandTables[0].count > (u32)type);
    command = &gWiFiPlazaLobbyClient->commandTables[0].commands[type];
    GF_ASSERT(size == command->size);
    GF_ASSERT(WiFiPlazaLobbyClient_CheckResult(PPW_LobbySendChannelBinaryMessage(0, type, data, size)) == TRUE);
}

void WiFiPlazaLobbyClient_SendToPlayer(int type, int id, const void *data, int size) {
    const WiFiPlazaLobbyCommand *command;
    GF_ASSERT(gWiFiPlazaLobbyClient != NULL);
    GF_ASSERT(gWiFiPlazaLobbyClient->commandTables[0].count > (u32)type);
    command = &gWiFiPlazaLobbyClient->commandTables[0].commands[type];
    GF_ASSERT(size == command->size);
    GF_ASSERT(WiFiPlazaLobbyClient_CheckResult(PPW_LobbySendPlayerBinaryMessage(id, type, data, size)) == TRUE);
}

void WiFiPlazaLobbyClient_SendToChannel(int type, const void *data, int size) {
    const WiFiPlazaLobbyCommand *command;
    GF_ASSERT(gWiFiPlazaLobbyClient != NULL);
    GF_ASSERT(gWiFiPlazaLobbyClient->commandTables[1].count > (u32)type);
    command = &gWiFiPlazaLobbyClient->commandTables[1].commands[type];
    GF_ASSERT(size == command->size);
    GF_ASSERT(gWiFiPlazaLobbyClient->channel != 4);
    GF_ASSERT(WiFiPlazaLobbyClient_CheckResult(PPW_LobbySendChannelBinaryMessage(WiFiPlazaLobbyClient_ChannelToLib(gWiFiPlazaLobbyClient->channel), type, data, size)) == TRUE);
}

#pragma force_active on
#pragma opt_propagation off

// Starts matchmaking session `index` as its host.
BOOL WiFiPlazaLobbyClient_HostMatch(int index, int maxPlayers) {
    GF_ASSERT(gWiFiPlazaLobbyClient != NULL);
    if (WiFiPlazaLobbyMatches_IsActive(gWiFiPlazaLobbyClient, index) == TRUE) {
        return FALSE;
    }
    if (gWiFiPlazaLobbyClient->currentMatch != 3) {
        return FALSE;
    }
    gWiFiPlazaLobbyClient->matches[index].info.index = index;
    gWiFiPlazaLobbyClient->matches[index].info.maxPlayers = maxPlayers;
    gWiFiPlazaLobbyClient->matches[index].info.playerCount = 1;
    gWiFiPlazaLobbyClient->matches[index].info.state = 0;
    WiFiPlazaLobbyMatchInfo_SetStartTime(&gWiFiPlazaLobbyClient->matches[index].info, &gWiFiPlazaLobbyClient->time);
    GF_ASSERT(WiFiPlazaLobbyClient_CheckResult(PPW_LobbyStartRecruit(&gWiFiPlazaLobbyClient->matches[index])));
    gWiFiPlazaLobbyClient->matches[index].active = 1;
    gWiFiPlazaLobbyClient->matches[index].hostId = PPW_LobbyGetMyUserId();
    gWiFiPlazaLobbyClient->matches[index].timer = 1800;
    gWiFiPlazaLobbyClient->currentMatch = index;
    gWiFiPlazaLobbyClient->unk_69e = 1;
    gWiFiPlazaLobbyClient->unk_69f = 0;
    WifiConnection_StartAnybodyMatchmaking(gWiFiPlazaLobbyClient->matches[index].info.name, maxPlayers, 1, gWiFiPlazaLobbyClient->matches[index].timer / 30);
    WifiConnection_SetClientCallback(WiFiPlazaLobbyClient_OnMatchClientDummy, gWiFiPlazaLobbyClient);
    WifiConnection_SetDisconnectCallback(WiFiPlazaLobbyClient_OnMatchDisconnect, gWiFiPlazaLobbyClient);
    return TRUE;
}

#pragma opt_propagation reset
#pragma force_active reset

void WiFiPlazaLobbyClient_StopMatch(void) {
    WiFiPlazaLobbyClient *client;
    int ownId;
    GF_ASSERT(gWiFiPlazaLobbyClient != NULL);
    if (gWiFiPlazaLobbyClient->currentMatch != 3 && gWiFiPlazaLobbyClient->unk_69e) {
        GF_ASSERT(WiFiPlazaLobbyClient_CheckResult(PPW_LobbyStopRecruit()));
        client = gWiFiPlazaLobbyClient;
        ownId = PPW_LobbyGetMyUserId();
        GF_ASSERT(WiFiPlazaLobbyMatches_Remove(client, client->currentMatch, ownId));
        gWiFiPlazaLobbyClient->unk_69e = 0;
    }
}

#pragma force_active on

void WiFiPlazaLobbyClient_CloseMatch(void) {
    GF_ASSERT(gWiFiPlazaLobbyClient != NULL);
    if (gWiFiPlazaLobbyClient->unk_69e) {
        gWiFiPlazaLobbyClient->matches[gWiFiPlazaLobbyClient->currentMatch].info.state = 1;
        GF_ASSERT(WiFiPlazaLobbyClient_CheckResult(PPW_LobbyUpdateRecruitInfo(&gWiFiPlazaLobbyClient->matches[gWiFiPlazaLobbyClient->currentMatch])));
    }
}

#pragma force_active reset

BOOL WiFiPlazaLobbyClient_IsCurrentMatchClosed(void) {
    u16 index;
    GF_ASSERT(gWiFiPlazaLobbyClient != NULL);
    index = gWiFiPlazaLobbyClient->currentMatch;
    if (index == 3) {
        return FALSE;
    }
    return gWiFiPlazaLobbyClient->matches[index].info.state == 1;
}

u8 WiFiPlazaLobbyClient_WasMatchCancelled(void) {
    GF_ASSERT(gWiFiPlazaLobbyClient != NULL);
    return gWiFiPlazaLobbyClient->unk_69f;
}

BOOL WiFiPlazaLobbyClient_IsMatchActive(int index) {
    GF_ASSERT(gWiFiPlazaLobbyClient != NULL);
    return WiFiPlazaLobbyMatches_IsActive(gWiFiPlazaLobbyClient, index);
}

BOOL WiFiPlazaLobbyClient_IsMatchOpen(int index) {
    GF_ASSERT(gWiFiPlazaLobbyClient != NULL);
    if (!WiFiPlazaLobbyClient_IsMatchActive(index)) {
        return FALSE;
    }
    return gWiFiPlazaLobbyClient->matches[index].info.state == 0;
}

// Free places of matchmaking session `index`.
int WiFiPlazaLobbyClient_GetMatchFreePlaces(int index) {
    WiFiPlazaLobbyMatch *match;
    GF_ASSERT(gWiFiPlazaLobbyClient != NULL);
    GF_ASSERT(index < 3);
    if (!WiFiPlazaLobbyMatches_IsActive(gWiFiPlazaLobbyClient, index)) {
        return 0;
    }
    match = WiFiPlazaLobbyMatches_Get(gWiFiPlazaLobbyClient, index);
    return match->info.maxPlayers - match->info.playerCount;
}

u32 WiFiPlazaLobbyClient_GetMatchPlayerCount(int index) {
    WiFiPlazaLobbyMatch *match;
    GF_ASSERT(gWiFiPlazaLobbyClient != NULL);
    GF_ASSERT(index < 3);
    if (!WiFiPlazaLobbyMatches_IsActive(gWiFiPlazaLobbyClient, index)) {
        return 0;
    }
    match = WiFiPlazaLobbyMatches_Get(gWiFiPlazaLobbyClient, index);
    return match->info.playerCount;
}

#pragma force_active on

// Joins matchmaking session `index` of another host.
BOOL WiFiPlazaLobbyClient_JoinMatch(int index) {
    WiFiPlazaLobbyClient *client;
    WiFiPlazaLobbyMatch *match;
    GF_ASSERT(gWiFiPlazaLobbyClient != NULL);
    if (!WiFiPlazaLobbyMatches_IsActive(gWiFiPlazaLobbyClient, index)) {
        return FALSE;
    }
    if (gWiFiPlazaLobbyClient->currentMatch != 3) {
        return FALSE;
    }
    match = &gWiFiPlazaLobbyClient->matches[index];
    if (match->info.state == 1) {
        return FALSE;
    }
    gWiFiPlazaLobbyClient->currentMatch = index;
    client = gWiFiPlazaLobbyClient;
    WifiConnection_StartAnybodyMatchmaking(client->matches[index].info.name, client->matches[index].info.maxPlayers, 0, client->matches[index].timer / 30);
    WifiConnection_SetClientCallback(WiFiPlazaLobbyClient_OnMatchClientDummy, gWiFiPlazaLobbyClient);
    WifiConnection_SetDisconnectCallback(WiFiPlazaLobbyClient_OnMatchDisconnect, gWiFiPlazaLobbyClient);
    return TRUE;
}

#pragma force_active reset

u32 WiFiPlazaLobbyClient_GetMatchTimer(int index) {
    GF_ASSERT(gWiFiPlazaLobbyClient != NULL);
    GF_ASSERT(index < 3);
    if (!WiFiPlazaLobbyMatches_IsActive(gWiFiPlazaLobbyClient, index)) {
        return 1800;
    }
    return gWiFiPlazaLobbyClient->matches[index].timer;
}

int WiFiPlazaLobbyClient_GetMatchHostId(void) {
    u16 index;
    GF_ASSERT(gWiFiPlazaLobbyClient != NULL);
    index = gWiFiPlazaLobbyClient->currentMatch;
    if (index == 3) {
        return -1;
    }
    return gWiFiPlazaLobbyClient->matches[index].hostId;
}

u8 WiFiPlazaLobbyClient_IsMatchHost(void) {
    GF_ASSERT(gWiFiPlazaLobbyClient != NULL);
    return gWiFiPlazaLobbyClient->unk_69e;
}

int WiFiPlazaLobbyClient_IsSpecialVisitor(int id) {
    GF_ASSERT(gWiFiPlazaLobbyClient != NULL);
    return WiFiPlazaLobbyIdValueList_Contains(&gWiFiPlazaLobbyClient->unk_6a0, id);
}

int WiFiPlazaLobbyClient_GetSpecialVisitorCode(int id) {
    GF_ASSERT(gWiFiPlazaLobbyClient != NULL);
    return WiFiPlazaLobbyIdValueList_GetValue(&gWiFiPlazaLobbyClient->unk_6a0, id);
}

void WiFiPlazaLobbyClient_SubmitSurveyAnswer(int index) {
    GF_ASSERT(gWiFiPlazaLobbyClient != NULL);
    GF_ASSERT(index < 3);
    GF_ASSERT(gWiFiPlazaLobbyClient->unk_980 != 1);
    GF_ASSERT(WiFiPlazaLobbyClient_CheckResult(PPW_LobbySubmitQuestionnaire(index)) == TRUE);
    gWiFiPlazaLobbyClient->unk_980 = 1;
}

u32 WiFiPlazaLobbyClient_GetSurveySubmitState(void) {
    GF_ASSERT(gWiFiPlazaLobbyClient != NULL);
    return gWiFiPlazaLobbyClient->unk_980;
}

u32 WiFiPlazaLobbyClient_GetSurveyValue(int field) {
    GF_ASSERT(gWiFiPlazaLobbyClient != NULL);
    GF_ASSERT(field < 11);
    switch (field) {
    case 0:
        return gWiFiPlazaLobbyClient->unk_6a0.unk_04[0].unk_04;
    case 1:
        return gWiFiPlazaLobbyClient->unk_6a0.unk_04[0].unk_08;
    case 2:
        return gWiFiPlazaLobbyClient->unk_6a0.unk_04[1].unk_00;
    case 3:
        return gWiFiPlazaLobbyClient->unk_6a0.unk_04[0].unk_08 >= 1000;
    case 4:
        return gWiFiPlazaLobbyClient->unk_6a0.unk_04[1].unk_04;
    case 5:
        return gWiFiPlazaLobbyClient->unk_6a0.unk_04[1].unk_08;
    case 6:
        return gWiFiPlazaLobbyClient->unk_6a0.unk_2c4;
    case 7:
        return gWiFiPlazaLobbyClient->unk_6a0.unk_04[1].unk_08 >= 1000;
    case 8:
    case 9:
    case 10:
        return gWiFiPlazaLobbyClient->unk_6a0.unk_2d4[field - 8];
    default:
        GF_AssertFail();
        return 0;
    }
}

void *WiFiPlazaLobbyClient_GetSurveyText(int field) {
    GF_ASSERT(gWiFiPlazaLobbyClient != NULL);
    GF_ASSERT(field < 8);
    switch (field) {
    case 0:
        return gWiFiPlazaLobbyClient->unk_6a0.unk_04[0].unk_0c;
    case 1:
    case 2:
    case 3:
        return gWiFiPlazaLobbyClient->unk_6a0.unk_04[0].unk_e8[field - 1];
    case 4:
        return gWiFiPlazaLobbyClient->unk_6a0.unk_04[1].unk_0c;
    case 5:
    case 6:
    case 7:
        return gWiFiPlazaLobbyClient->unk_6a0.unk_04[1].unk_e8[field - 5];
    default:
        GF_AssertFail();
        return NULL;
    }
}

u8 ov45_0222F6B0(int which, int index) {
    GF_ASSERT(gWiFiPlazaLobbyClient != NULL);
    GF_ASSERT(which < 2);
    GF_ASSERT((u32)index < 12);
    if (which >= 2) {
        return 0;
    }
    if ((u32)index >= 12) {
        return 0;
    }
    if (which == 0) {
        return gWiFiPlazaLobbyClient->unk_6a0.unk_04[0].unk_154[index];
    }
    return gWiFiPlazaLobbyClient->unk_6a0.unk_04[1].unk_154[index];
}

// Ends the matchmaking session.
void WiFiPlazaLobbyClient_EndMatch(void) {
    GF_ASSERT(gWiFiPlazaLobbyClient != NULL);
    WifiConnection_SetClientCallback(NULL, NULL);
    WifiConnection_SetDisconnectCallback(NULL, NULL);
    gWiFiPlazaLobbyClient->currentMatch = 3;
    gWiFiPlazaLobbyClient->unk_69e = 0;
    gWiFiPlazaLobbyClient->unk_69f = 0;
}

// TRUE when a lobby library call succeeded (result 0).
BOOL WiFiPlazaLobbyClient_CheckResult(int result) {
    BOOL ret = TRUE;
    switch (result) {
    case 0:
        break;
    case 1:
    case 2:
    case 4:
    case 5:
        ret = FALSE;
        break;
    case 3:
    case 6:
        GF_AssertFail();
        ret = FALSE;
        break;
    case 7:
        ret = FALSE;
        break;
    default:
        GF_AssertFail();
        ret = FALSE;
        break;
    }
    return ret;
}

void WiFiPlazaLobbyClient_ClearErrors(void) {
    gWiFiPlazaLobbyClient->unk_133 = 0;
    gWiFiPlazaLobbyClient->unk_132 = 0;
}

// Channel <-> lobby library channel (4 = none).
int WiFiPlazaLobbyClient_ChannelFromLib(int libChannel) {
    libChannel--;
    return libChannel < 0 ? 4 : libChannel;
}

int WiFiPlazaLobbyClient_ChannelToLib(int channel) {
    return channel + 1;
}
