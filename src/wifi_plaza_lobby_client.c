#include <string.h>

#include "global.h"

#include "assert.h"
#include "friend_roster.h"
#include "heap.h"
#include "save_wifi_history.h"
#include "wifi_connection.h"
#include "wifi_plaza_internal.h"

// The lobby client of the Wi-Fi Plaza (_022577C0): one instance for the plaza and its apps on top of the lobby
// library (ov45_022310C0 ..). It keeps the lobby state, the location of every lobby slot, the message tables, the
// lobby time and up to three matchmaking sessions.

// Slot getter of news event type 8 (ov45_02254BE8).
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
    GF_ASSERT(_022577C0 == NULL);
    GF_ASSERT(count < 300);
    _022577C0 = Heap_Alloc(heapID, sizeof(WiFiPlazaLobbyClient));
    memset(_022577C0, 0, sizeof(WiFiPlazaLobbyClient));
    _022577C0->save = save;
    _022577C0->friendRoster = Save_FriendRoster_Get(save);
    _022577C0->wifiHistory = Save_WiFiHistory_Get(save);
    _022577C0->callbacks = *callbacks;
    _022577C0->callbackArg = callbackArg;
    WiFiPlazaLobbyClient_AllocPlayerData(_022577C0, count, heapID);
    WiFiPlazaLobbyClient_AllocIdLists(_022577C0, 20, 8, heapID);
    WiFiPlazaLobbyClient_InitMatches(_022577C0);
    WiFiPlazaLobbyIdValueList_Init(&_022577C0->unk_6a0, 128, heapID);
    _022577C0->initResult = 0;
}

#pragma force_active reset

void WiFiPlazaLobbyClient_Free(void) {
    GF_ASSERT(_022577C0 != NULL);
    WiFiPlazaLobbyIdValueList_Free(&_022577C0->unk_6a0);
    WiFiPlazaLobbyClient_FreeIdLists(_022577C0);
    WiFiPlazaLobbyClient_FreePlayerData(_022577C0);
    Heap_Free(_022577C0);
    _022577C0 = NULL;
}

#pragma force_active on

// Once per frame: follows the state of the lobby library and the channel, refreshes the lobby time and the
// matchmaking sessions.
int WiFiPlazaLobbyClient_Update(void) {
    if (_022577C0 == NULL) {
        return 0;
    }
    switch (ov45_02231514()) {
    case 0:
        _022577C0->state = 0;
        break;
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
    case 6:
    case 7:
        _022577C0->state = 1;
        break;
    case 8:
        _022577C0->state = 2;
        break;
    case 9:
        _022577C0->state = 3;
        break;
    case 10:
        _022577C0->state = 4;
        break;
    }
    if (_022577C0->state == 2) {
        WiFiPlazaLobbyMatches_Update(_022577C0);
        ov45_022320C4(&_022577C0->time);
        switch (ov45_022321E0()) {
        case 0:
            _022577C0->channelState = 0;
            _022577C0->channel = 4;
            break;
        case 1:
            _022577C0->channelState = 1;
            break;
        case 2:
            if (_022577C0->channelState != 3) {
                _022577C0->channelState = 2;
            }
            break;
        case 3:
            _022577C0->channelState = 4;
            break;
        }
    }
    return WiFiPlazaLobbyClient_GetState();
}

#pragma force_active reset

void ov45_0222E7CC(void) {
    GF_ASSERT(_022577C0 != NULL);
    if (_022577C0->initResult != 0) {
        ov45_02233220();
    } else {
        ov45_022331E8();
    }
}

void ov45_0222E7FC(void) {
    ov45_02233204();
}

void ov45_0222E804(int value) {
    ov45_0222E810(value, 0);
}

#pragma force_active on

void ov45_0222E810(int value, int mode) {
    void (*callbacks[17])(void);
    void *userData;
    GF_ASSERT(_022577C0 != NULL);
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
    callbacks[13] = (void (*)(void))ov45_022300B0;
    callbacks[14] = (void (*)(void))ov45_022300DC;
    callbacks[15] = (void (*)(void))ov45_02230108;
    callbacks[16] = (void (*)(void))ov45_02230130;
    userData = FriendRoster_GetUserData(_022577C0->friendRoster);
    _022577C0->initResult = ov45_022310C0(ov45_02254F04, ov45_02254F14, mode, callbacks, userData, value, _022577C0->playerDataSize);
    WiFiPlazaLobbyClient_CheckResult(_022577C0->initResult);
}

#pragma force_active reset

BOOL WiFiPlazaLobbyClient_IsInLobby(void) {
    GF_ASSERT(_022577C0 != NULL);
    return _022577C0->state == 2;
}

void ov45_0222E91C(void) {
    GF_ASSERT(_022577C0 != NULL);
    WiFiPlazaLobbyClient_ClearErrors();
    GF_ASSERT(WiFiPlazaLobbyClient_CheckResult(ov45_02231490()));
}

BOOL WiFiPlazaLobbyClient_IsDisconnected(void) {
    GF_ASSERT(_022577C0 != NULL);
    return _022577C0->state == 0;
}

// The lobby state, 4 (error) when the channel or the client failed.
int WiFiPlazaLobbyClient_GetState(void) {
    WiFiPlazaLobbyClient *client;
    GF_ASSERT(_022577C0 != NULL);
    client = _022577C0;
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
    GF_ASSERT(_022577C0 != NULL);
    value = _022577C0->unk_131;
    _022577C0->unk_131 = 0;
    return value;
}

int WiFiPlazaLobbyClient_GetOwnId(void) {
    GF_ASSERT(_022577C0 != NULL);
    return ov45_02232580();
}

void WiFiPlazaLobbyClient_SetOwnPlayer(WiFiPlazaPlayer *player) {
    int ownId;
    GF_ASSERT(_022577C0 != NULL);
    ownId = ov45_02232580();
    WiFiPlazaLobbyClient_SetPlayerData(_022577C0, ownId, player);
    ov45_022325B0(player, _022577C0->playerDataSize, 0);
}

WiFiPlazaPlayer *WiFiPlazaLobbyClient_GetPlayer(int id) {
    GF_ASSERT(_022577C0 != NULL);
    return WiFiPlazaLobbyClient_GetPlayerData(_022577C0, id);
}

#pragma force_active on

// The time the player entered the lobby.
void WiFiPlazaLobbyClient_GetEntryTime(int id, s64 *time) {
    WiFiPlazaLobbyEntryTime *entered;
    GF_ASSERT(_022577C0 != NULL);
    entered = &WiFiPlazaLobbyClient_GetPlayerRecord(_022577C0, id)->entryTime;
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
    GF_ASSERT(_022577C0 != NULL);
    for (slot = 0; slot < 20; slot++) {
        int id = WiFiPlazaLobbyClient_GetIdListEntry(_022577C0, slot, 0);
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
    GF_ASSERT(_022577C0 != NULL);
    for (slot = 0; slot < 20; slot++) {
        int id = WiFiPlazaLobbyClient_GetIdListEntry(_022577C0, slot, 0);
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
    GF_ASSERT(_022577C0 != NULL);
    result = WiFiPlazaLobbyClient_CheckResult(ov45_02231C8C(WiFiPlazaLobbyClient_ChannelToLib(channel)));
    if (result == TRUE) {
        _022577C0->channel = channel;
        _022577C0->unk_12f = 0;
    }
    return result;
}

#pragma force_active reset

u8 ov45_0222EB74(void) {
    GF_ASSERT(_022577C0 != NULL);
    return _022577C0->unk_12f;
}

#pragma force_active on

BOOL WiFiPlazaLobbyClient_LeaveChannel(void) {
    GF_ASSERT(_022577C0 != NULL);
    {
        BOOL result = WiFiPlazaLobbyClient_CheckResult(ov45_02231EC0());
        if (result == TRUE) {
            _022577C0->channelState = 3;
        }
        return result;
    }
}

#pragma force_active reset

BOOL WiFiPlazaLobbyClient_IsNotInChannel(void) {
    GF_ASSERT(_022577C0 != NULL);
    return _022577C0->channelState == 0;
}

BOOL ov45_0222EBF0(int channel) {
    GF_ASSERT(_022577C0 != NULL);
    return WiFiPlazaLobbyClient_HasChannelRoom(_022577C0, channel);
}

void WiFiPlazaLobbyClient_GetPlayerList(WiFiPlazaPlayerList *list) {
    GF_ASSERT(_022577C0 != NULL);
    list->unk_00 = _022577C0->idLists[0].count;
    list->ids = _022577C0->idLists[0].ids;
}

void WiFiPlazaLobbyClient_GetChannelPlayerList(u32 *out) {
    GF_ASSERT(_022577C0 != NULL);
    out[0] = _022577C0->idLists[1].count;
    out[1] = (u32)_022577C0->idLists[1].ids;
}

// Lobby player id <-> lobby slot.
int WiFiPlazaLobbyClient_IdToSlot(int id) {
    return WiFiPlazaLobbyClient_FindInIdList(_022577C0, id, 0);
}

int WiFiPlazaLobbyClient_IdToChannelSlot(int id) {
    return WiFiPlazaLobbyClient_FindInIdList(_022577C0, id, 1);
}

int WiFiPlazaLobbyClient_SlotToId(int slot) {
    return WiFiPlazaLobbyClient_GetIdListEntry(_022577C0, slot, 0);
}

int WiFiPlazaLobbyClient_ChannelSlotToId(int slot) {
    return WiFiPlazaLobbyClient_GetIdListEntry(_022577C0, slot, 1);
}

void WiFiPlazaLobbyClient_GetTime(s64 *time) {
    GF_ASSERT(_022577C0 != NULL);
    *time = _022577C0->time;
}

u32 ov45_0222ECDC(int field) {
    WiFiPlazaLobbyClientUnk1B4 *unk;
    u32 result;
    GF_ASSERT(_022577C0 != NULL);
    {
        BOOL valid = TRUE;
        if (_022577C0->state != 2 && _022577C0->state != 4) {
            valid = FALSE;
        }
        GF_ASSERT(valid);
    }
    GF_ASSERT(field < 8);
    unk = &_022577C0->unk_1b4;
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
        result = ov45_022303BC(unk, 0x13);
        break;
    case 6:
        result = ov45_022303BC(unk, 0x10);
        break;
    case 7:
        result = ov45_022303BC(unk, 0x11);
        break;
    }
    return result;
}

#pragma force_active on

int ov45_0222ED7C(void) {
    GF_ASSERT(_022577C0 != NULL);
    {
        int result = WiFiPlazaLobbyClient_AddOwnLocation(_022577C0);
        if (result) {
            _022577C0->unk_130 = 1;
        }
        return result;
    }
}

#pragma force_active reset

BOOL ov45_0222EDA8(void) {
    return _022577C0->unk_130 == 0;
}

u32 WiFiPlazaLobbyClient_GetLocationCountry(u8 slot) {
    GF_ASSERT(_022577C0 != NULL);
    GF_ASSERT((u32)slot < 20);
    return _022577C0->locations[slot].country;
}

u8 WiFiPlazaLobbyClient_GetLocationRegion(u8 slot) {
    WiFiPlazaLobbyLocation *location;
    GF_ASSERT(_022577C0 != NULL);
    GF_ASSERT((u32)slot < 20);
    location = &_022577C0->locations[slot];
    return location->region;
}

// The message types sent with WiFiPlazaLobbyClient_SendToLobby / SendToPlayer (each with the size of its data).
void WiFiPlazaLobbyClient_SetLobbyCommands(const void *commands, u32 count, void *arg) {
    GF_ASSERT(_022577C0 != NULL);
    _022577C0->commandTables[0].commands = commands;
    _022577C0->commandTables[0].count = count;
    _022577C0->commandTables[0].arg = arg;
}

// The message types sent with WiFiPlazaLobbyClient_SendToChannel.
void WiFiPlazaLobbyClient_SetChannelCommands(const void *commands, u32 count, void *arg) {
    GF_ASSERT(_022577C0 != NULL);
    _022577C0->commandTables[1].commands = commands;
    _022577C0->commandTables[1].count = count;
    _022577C0->commandTables[1].arg = arg;
}

// Clears the 12 bytes one at a time.
void WiFiPlazaLobbyClient_ClearLobbyCommands(void) {
    u8 *bytes;
    GF_ASSERT(_022577C0 != NULL);
    bytes = (u8 *)&_022577C0->commandTables[0];
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
    GF_ASSERT(_022577C0 != NULL);
    bytes = (u8 *)&_022577C0->commandTables[1];
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
    GF_ASSERT(_022577C0 != NULL);
    GF_ASSERT(_022577C0->commandTables[0].count > (u32)type);
    command = &_022577C0->commandTables[0].commands[type];
    GF_ASSERT(size == command->size);
    GF_ASSERT(WiFiPlazaLobbyClient_CheckResult(ov45_0223247C(0, type, data, size)) == TRUE);
}

void WiFiPlazaLobbyClient_SendToPlayer(int type, int id, const void *data, int size) {
    const WiFiPlazaLobbyCommand *command;
    GF_ASSERT(_022577C0 != NULL);
    GF_ASSERT(_022577C0->commandTables[0].count > (u32)type);
    command = &_022577C0->commandTables[0].commands[type];
    GF_ASSERT(size == command->size);
    GF_ASSERT(WiFiPlazaLobbyClient_CheckResult(ov45_02232500(id, type, data, size)) == TRUE);
}

void WiFiPlazaLobbyClient_SendToChannel(int type, const void *data, int size) {
    const WiFiPlazaLobbyCommand *command;
    GF_ASSERT(_022577C0 != NULL);
    GF_ASSERT(_022577C0->commandTables[1].count > (u32)type);
    command = &_022577C0->commandTables[1].commands[type];
    GF_ASSERT(size == command->size);
    GF_ASSERT(_022577C0->channel != 4);
    GF_ASSERT(WiFiPlazaLobbyClient_CheckResult(ov45_0223247C(WiFiPlazaLobbyClient_ChannelToLib(_022577C0->channel), type, data, size)) == TRUE);
}

#pragma force_active on
#pragma opt_propagation off

// Starts matchmaking session `index` as its host.
BOOL WiFiPlazaLobbyClient_HostMatch(int index, int maxPlayers) {
    GF_ASSERT(_022577C0 != NULL);
    if (WiFiPlazaLobbyMatches_IsActive(_022577C0, index) == TRUE) {
        return FALSE;
    }
    if (_022577C0->currentMatch != 3) {
        return FALSE;
    }
    _022577C0->matches[index].info.index = index;
    _022577C0->matches[index].info.maxPlayers = maxPlayers;
    _022577C0->matches[index].info.playerCount = 1;
    _022577C0->matches[index].info.state = 0;
    WiFiPlazaLobbyMatchInfo_SetStartTime(&_022577C0->matches[index].info, &_022577C0->time);
    GF_ASSERT(WiFiPlazaLobbyClient_CheckResult(ov45_0223285C(&_022577C0->matches[index])));
    _022577C0->matches[index].active = 1;
    _022577C0->matches[index].hostId = ov45_02232580();
    _022577C0->matches[index].timer = 1800;
    _022577C0->currentMatch = index;
    _022577C0->unk_69e = 1;
    _022577C0->unk_69f = 0;
    ov00_021E5CEC(_022577C0->matches[index].info.name, maxPlayers, 1, _022577C0->matches[index].timer / 30);
    ov00_021E5CD0(ov45_02230390, _022577C0);
    WifiConnection_SetDisconnectCallback(ov45_02230394, _022577C0);
    return TRUE;
}

#pragma opt_propagation reset
#pragma force_active reset

void ov45_0222F154(void) {
    WiFiPlazaLobbyClient *client;
    int ownId;
    GF_ASSERT(_022577C0 != NULL);
    if (_022577C0->currentMatch != 3 && _022577C0->unk_69e) {
        GF_ASSERT(WiFiPlazaLobbyClient_CheckResult(ov45_02232CA4()));
        client = _022577C0;
        ownId = ov45_02232580();
        GF_ASSERT(WiFiPlazaLobbyMatches_Remove(client, client->currentMatch, ownId));
        _022577C0->unk_69e = 0;
    }
}

#pragma force_active on

void WiFiPlazaLobbyClient_CloseMatch(void) {
    GF_ASSERT(_022577C0 != NULL);
    if (_022577C0->unk_69e) {
        _022577C0->matches[_022577C0->currentMatch].info.state = 1;
        GF_ASSERT(WiFiPlazaLobbyClient_CheckResult(ov45_02232BB0(&_022577C0->matches[_022577C0->currentMatch])));
    }
}

#pragma force_active reset

BOOL WiFiPlazaLobbyClient_IsCurrentMatchClosed(void) {
    u16 index;
    GF_ASSERT(_022577C0 != NULL);
    index = _022577C0->currentMatch;
    if (index == 3) {
        return FALSE;
    }
    return _022577C0->matches[index].info.state == 1;
}

u8 ov45_0222F254(void) {
    GF_ASSERT(_022577C0 != NULL);
    return _022577C0->unk_69f;
}

BOOL WiFiPlazaLobbyClient_IsMatchActive(int index) {
    GF_ASSERT(_022577C0 != NULL);
    return WiFiPlazaLobbyMatches_IsActive(_022577C0, index);
}

BOOL WiFiPlazaLobbyClient_IsMatchOpen(int index) {
    GF_ASSERT(_022577C0 != NULL);
    if (!WiFiPlazaLobbyClient_IsMatchActive(index)) {
        return FALSE;
    }
    return _022577C0->matches[index].info.state == 0;
}

// Free places of matchmaking session `index`.
int WiFiPlazaLobbyClient_GetMatchFreePlaces(int index) {
    WiFiPlazaLobbyMatch *match;
    GF_ASSERT(_022577C0 != NULL);
    GF_ASSERT(index < 3);
    if (!WiFiPlazaLobbyMatches_IsActive(_022577C0, index)) {
        return 0;
    }
    match = WiFiPlazaLobbyMatches_Get(_022577C0, index);
    return match->info.maxPlayers - match->info.playerCount;
}

u32 WiFiPlazaLobbyClient_GetMatchPlayerCount(int index) {
    WiFiPlazaLobbyMatch *match;
    GF_ASSERT(_022577C0 != NULL);
    GF_ASSERT(index < 3);
    if (!WiFiPlazaLobbyMatches_IsActive(_022577C0, index)) {
        return 0;
    }
    match = WiFiPlazaLobbyMatches_Get(_022577C0, index);
    return match->info.playerCount;
}

#pragma force_active on

// Joins matchmaking session `index` of another host.
BOOL WiFiPlazaLobbyClient_JoinMatch(int index) {
    WiFiPlazaLobbyClient *client;
    WiFiPlazaLobbyMatch *match;
    GF_ASSERT(_022577C0 != NULL);
    if (!WiFiPlazaLobbyMatches_IsActive(_022577C0, index)) {
        return FALSE;
    }
    if (_022577C0->currentMatch != 3) {
        return FALSE;
    }
    match = &_022577C0->matches[index];
    if (match->info.state == 1) {
        return FALSE;
    }
    _022577C0->currentMatch = index;
    client = _022577C0;
    ov00_021E5CEC(client->matches[index].info.name, client->matches[index].info.maxPlayers, 0, client->matches[index].timer / 30);
    ov00_021E5CD0(ov45_02230390, _022577C0);
    WifiConnection_SetDisconnectCallback(ov45_02230394, _022577C0);
    return TRUE;
}

#pragma force_active reset

u32 WiFiPlazaLobbyClient_GetMatchTimer(int index) {
    GF_ASSERT(_022577C0 != NULL);
    GF_ASSERT(index < 3);
    if (!WiFiPlazaLobbyMatches_IsActive(_022577C0, index)) {
        return 1800;
    }
    return _022577C0->matches[index].timer;
}

int WiFiPlazaLobbyClient_GetMatchHostId(void) {
    u16 index;
    GF_ASSERT(_022577C0 != NULL);
    index = _022577C0->currentMatch;
    if (index == 3) {
        return -1;
    }
    return _022577C0->matches[index].hostId;
}

u8 WiFiPlazaLobbyClient_IsMatchHost(void) {
    GF_ASSERT(_022577C0 != NULL);
    return _022577C0->unk_69e;
}

int WiFiPlazaLobbyClient_IsSpecialVisitor(int id) {
    GF_ASSERT(_022577C0 != NULL);
    return WiFiPlazaLobbyIdValueList_Contains(&_022577C0->unk_6a0, id);
}

int WiFiPlazaLobbyClient_GetSpecialVisitorCode(int id) {
    GF_ASSERT(_022577C0 != NULL);
    return WiFiPlazaLobbyIdValueList_GetValue(&_022577C0->unk_6a0, id);
}

void ov45_0222F4D4(int index) {
    GF_ASSERT(_022577C0 != NULL);
    GF_ASSERT(index < 3);
    GF_ASSERT(_022577C0->unk_980 != 1);
    GF_ASSERT(WiFiPlazaLobbyClient_CheckResult(ov45_02232E60(index)) == TRUE);
    _022577C0->unk_980 = 1;
}

u32 ov45_0222F524(void) {
    GF_ASSERT(_022577C0 != NULL);
    return _022577C0->unk_980;
}

u32 ov45_0222F544(int field) {
    GF_ASSERT(_022577C0 != NULL);
    GF_ASSERT(field < 11);
    switch (field) {
    case 0:
        return _022577C0->unk_6a0.unk_04[0].unk_04;
    case 1:
        return _022577C0->unk_6a0.unk_04[0].unk_08;
    case 2:
        return _022577C0->unk_6a0.unk_04[1].unk_00;
    case 3:
        return _022577C0->unk_6a0.unk_04[0].unk_08 >= 1000;
    case 4:
        return _022577C0->unk_6a0.unk_04[1].unk_04;
    case 5:
        return _022577C0->unk_6a0.unk_04[1].unk_08;
    case 6:
        return _022577C0->unk_6a0.unk_2c4;
    case 7:
        return _022577C0->unk_6a0.unk_04[1].unk_08 >= 1000;
    case 8:
    case 9:
    case 10:
        return _022577C0->unk_6a0.unk_2d4[field - 8];
    default:
        GF_AssertFail();
        return 0;
    }
}

void *ov45_0222F628(int field) {
    GF_ASSERT(_022577C0 != NULL);
    GF_ASSERT(field < 8);
    switch (field) {
    case 0:
        return _022577C0->unk_6a0.unk_04[0].unk_0c;
    case 1:
    case 2:
    case 3:
        return _022577C0->unk_6a0.unk_04[0].unk_e8[field - 1];
    case 4:
        return _022577C0->unk_6a0.unk_04[1].unk_0c;
    case 5:
    case 6:
    case 7:
        return _022577C0->unk_6a0.unk_04[1].unk_e8[field - 5];
    default:
        GF_AssertFail();
        return NULL;
    }
}

u8 ov45_0222F6B0(int which, int index) {
    GF_ASSERT(_022577C0 != NULL);
    GF_ASSERT(which < 2);
    GF_ASSERT((u32)index < 12);
    if (which >= 2) {
        return 0;
    }
    if ((u32)index >= 12) {
        return 0;
    }
    if (which == 0) {
        return _022577C0->unk_6a0.unk_04[0].unk_154[index];
    }
    return _022577C0->unk_6a0.unk_04[1].unk_154[index];
}

// Ends the matchmaking session.
void WiFiPlazaLobbyClient_EndMatch(void) {
    GF_ASSERT(_022577C0 != NULL);
    ov00_021E5CD0(NULL, NULL);
    WifiConnection_SetDisconnectCallback(NULL, NULL);
    _022577C0->currentMatch = 3;
    _022577C0->unk_69e = 0;
    _022577C0->unk_69f = 0;
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
    _022577C0->unk_133 = 0;
    _022577C0->unk_132 = 0;
}

// Channel <-> lobby library channel (4 = none).
int WiFiPlazaLobbyClient_ChannelFromLib(int libChannel) {
    libChannel--;
    return libChannel < 0 ? 4 : libChannel;
}

int WiFiPlazaLobbyClient_ChannelToLib(int channel) {
    return channel + 1;
}
