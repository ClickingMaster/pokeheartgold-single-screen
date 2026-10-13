#include <string.h>

#include "global.h"

#include "assert.h"
#include "wifi_plaza_internal.h"

// The callbacks the lobby library calls (ov45_0222E810 hands them over): channel joined, players entering, leaving
// and updating, messages, the location list, match updates and the results of the library's requests.

// TRUE while the channel has room (ov45_02254C30 per channel).
BOOL WiFiPlazaLobbyClient_HasChannelRoom(WiFiPlazaLobbyClient *client, int channel) {
    u32 libChannel;
    int count;
    int i;

    GF_ASSERT(channel < 4);
    libChannel = WiFiPlazaLobbyClient_ChannelToLib(channel);
    count = 0;
    for (i = 0; i < client->idLists[0].capacity; i++) {
        int id = WiFiPlazaLobbyClient_GetIdListEntry(client, i, 0);
        if (id != -1 && libChannel == client->records[i].channel) {
            count++;
        }
    }
    return ov45_02254C30[channel] > count;
}

// Entered the lobby (libChannel 0) or a channel.
void WiFiPlazaLobbyClient_OnEnterChannel(int result, u32 libChannel) {
    WiFiPlazaLobbyClient *client;

    if (libChannel == 0) {
        _022577C0->unk_5b4 = sizeof(_022577C0->unk_1b4);
        GF_ASSERT(WiFiPlazaLobbyClient_CheckResult(PPW_LobbyGetSchedule(&_022577C0->unk_1b4, &_022577C0->unk_5b4)));
        PPW_LobbyGetChannelDataAsync(-1, 0, ov45_02254EF0, 0);
        return;
    }
    client = _022577C0;
    if (client->channel == WiFiPlazaLobbyClient_ChannelFromLib(libChannel)) {
        if (result == 1) {
            client->unk_12f = 1;
            WiFiPlazaLobbyClient_RebuildChannelList(_022577C0, _022577C0->channel);
            WiFiPlazaLobbyClient_AddToIdList(_022577C0, PPW_LobbyGetMyUserId(), 1);
        } else {
            client->unk_12f = 2;
        }
    } else {
        GF_AssertFail();
        WiFiPlazaLobbyClient_LeaveChannel();
    }
}

void WiFiPlazaLobbyClient_OnPlayerEnter(int id, const WiFiPlazaLobbyPlayerRecord *record, const void *data, u32 size) {
    BOOL isOwn;
    WiFiPlazaLobbyClient *client;
    WiFiPlazaPlayer *player;

    if (id == PPW_LobbyGetMyUserId()) {
        PPW_LobbyGetTimeInfo(&_022577C0->time);
        isOwn = TRUE;
    } else {
        isOwn = FALSE;
    }
    WiFiPlazaLobbyClient_AddToIdList(_022577C0, id, 0);
    WiFiPlazaLobbyClient_CopyPlayerData(_022577C0, id, data, size);
    WiFiPlazaLobbyClient_SetPlayerRecord(_022577C0, id, record);
    client = _022577C0;
    player = WiFiPlazaLobbyClient_GetPlayerData(client, id);
    _022577C0->callbacks.onPlayerEnter(id, player, client->callbackArg, isOwn);
}

void WiFiPlazaLobbyClient_OnPlayerLeave(int id) {
    int i;

    if (WiFiPlazaLobbyClient_FindInIdList(_022577C0, id, 0) == -1) {
        return;
    }
    for (i = 0; i < 3; i++) {
        if (id == _022577C0->matches[i].hostId) {
            WiFiPlazaLobbyClient_OnMatchClosed(id, &_022577C0->matches[i].info);
        }
    }
    _022577C0->callbacks.onPlayerLeave(id, _022577C0->callbackArg);
    WiFiPlazaLobbyClient_ClearPlayerRecord(_022577C0, id);
    WiFiPlazaLobbyClient_ClearPlayerData(_022577C0, id);
    WiFiPlazaLobbyClient_RemoveFromIdList(_022577C0, id, 0);
    WiFiPlazaLobbyClient_RemoveFromIdList(_022577C0, id, 1);
}

void ov45_0222FDD4(void) {
}

#pragma force_active on

// A message of the given type from a player: calls the receive callback of the lobby (libChannel 0 / 5) or channel
// table.
void WiFiPlazaLobbyClient_OnMessage(int id, u32 libChannel, u32 type, const void *data, u32 size) {
    int table;

    if (WiFiPlazaLobbyClient_FindInIdList(_022577C0, id, 0) == -1) {
        return;
    }
    switch (libChannel) {
    case 0:
        table = 0;
        break;
    case 1:
    case 2:
    case 3:
    case 4:
        if (_022577C0->channel != WiFiPlazaLobbyClient_ChannelFromLib(libChannel)) {
            return;
        }
        table = 1;
        break;
    case 5:
        table = 0;
        break;
    default:
        return;
    }
    if (_022577C0->commandTables[table].count <= type) {
        return;
    }
    GF_ASSERT(_022577C0->commandTables[table].commands[type].size <= size);
    _022577C0->commandTables[table].commands[type].callback(id, data, _022577C0->commandTables[table].commands[type].size, _022577C0->commandTables[table].arg);
}

#pragma force_active reset

// The lobby's location list (key ov45_02254EF0) changed.
void WiFiPlazaLobbyClient_OnLocationsChanged(int a0, int a1, int a2, int a3, const char *key, const void *data) {
    if (strcmp(key, ov45_02254EF0) == 0) {
        memcpy(_022577C0->locations, data, sizeof(_022577C0->locations));
        if (_022577C0->unk_130) {
            _022577C0->unk_130 = 0;
        }
    }
}

#pragma force_active on

// A player's record changed: follows the player into or out of the own channel.
void WiFiPlazaLobbyClient_OnPlayerRecordChanged(int id, const WiFiPlazaLobbyPlayerRecord *record) {
    if (WiFiPlazaLobbyClient_FindInIdList(_022577C0, id, 0) != -1) {
        if (_022577C0->channel != 4) {
            int current, old = WiFiPlazaLobbyClient_GetPlayerRecord(_022577C0, id)->channel;
            current = record->channel;
            {
                int own = WiFiPlazaLobbyClient_ChannelToLib(_022577C0->channel);
                if (old == 5 && current == own) {
                    WiFiPlazaLobbyClient_AddToIdList(_022577C0, id, 1);
                } else if (old == own && current != own) {
                    WiFiPlazaLobbyClient_RemoveFromIdList(_022577C0, id, 1);
                }
            }
        }
        WiFiPlazaLobbyClient_SetPlayerRecord(_022577C0, id, record);
    }
}

#pragma force_active reset

void WiFiPlazaLobbyClient_OnPlayerUpdate(int id, const void *data) {
    if (WiFiPlazaLobbyClient_FindInIdList(_022577C0, id, 0) != -1) {
        WiFiPlazaLobbyClient_SetPlayerData(_022577C0, id, data);
        _022577C0->callbacks.onPlayerUpdate(id, (WiFiPlazaPlayer *)data, _022577C0->callbackArg);
    }
}

#pragma force_active on

static inline BOOL IsSameId(int id, int other) {
    return id == other;
}

// Match info from a player.
void WiFiPlazaLobbyClient_OnMatchInfo(int id, WiFiPlazaLobbyMatchInfo *info) {
    s64 current;
    s64 received;
    int slot = WiFiPlazaLobbyClient_FindInIdList(_022577C0, id, 0);
    if (slot != -1) {
        if (IsSameId(WiFiPlazaLobbyClient_GetOwnId(), id)) {
        } else if (_022577C0->unk_69e && _022577C0->currentMatch == info->index) {
            WiFiPlazaLobbyMatchInfo_GetStartTime(&_022577C0->matches[_022577C0->currentMatch].info, &current);
            WiFiPlazaLobbyMatchInfo_GetStartTime(info, &received);
            if (current < received) {
                return;
            }
            _022577C0->unk_69f = 1;
            ov45_0222F154();
        }
        WiFiPlazaLobbyMatches_Set(_022577C0, info->index, id, info);
    }
}

#pragma force_active reset

void WiFiPlazaLobbyClient_OnMatchClosed(int id, const WiFiPlazaLobbyMatchInfo *info) {
    int result;
    if (WiFiPlazaLobbyClient_FindInIdList(_022577C0, id, 0) == -1) {
        return;
    }
    result = WiFiPlazaLobbyMatches_Remove(_022577C0, info->index, id);
    if (_022577C0->currentMatch == info->index && result == 1) {
        _022577C0->unk_69f = 1;
    }
}

void WiFiPlazaLobbyClient_OnLobbyEvent(u32 event) {
    _022577C0->callbacks.onLobbyEvent(event, _022577C0->callbackArg);
}

void WiFiPlazaLobbyClient_OnOwnRecord(int result, const WiFiPlazaPlayer *data, u32 size) {
    if (result != 0) {
        _022577C0->unk_133 = 1;
    } else {
        _022577C0->callbacks.onOwnRecord(data, size, _022577C0->callbackArg);
    }
}

BOOL ov45_02230090(int result) {
    if (result == 0) {
        return TRUE;
    }
    _022577C0->unk_132 = 1;
    return FALSE;
}

#pragma force_active on

void ov45_022300B0(int result, const void *data, u32 size) {
    if (result == 0) {
        WiFiPlazaLobbyIdValueList_Set(&_022577C0->unk_6a0, data, size);
    } else {
        _022577C0->unk_132 = 1;
    }
}

void ov45_022300DC(int result, const void *data) {
    if (result == 0) {
        ov45_0223048C(&_022577C0->unk_6a0.unk_04[0].unk_04, data);
    } else {
        _022577C0->unk_132 = 1;
    }
}

#pragma force_active reset

void ov45_02230108(void) {
    GF_ASSERT(_022577C0->unk_980 == 1);
    _022577C0->unk_980 = 2;
}

void ov45_02230130(void) {
    _022577C0->unk_131 = 1;
}

void WiFiPlazaLobbyClient_InitMatches(WiFiPlazaLobbyClient *client) {
    memset(client->matches, 0, sizeof(client->matches));
    client->currentMatch = 3;
}
