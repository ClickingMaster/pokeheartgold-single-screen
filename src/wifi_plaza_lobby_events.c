#include <string.h>

#include "global.h"

#include "assert.h"
#include "wifi_plaza_internal.h"

// The callbacks the lobby library calls (WiFiPlazaLobbyClient_InitializeEx hands them over): channel joined, players entering, leaving
// and updating, messages, the location list, match updates and the results of the library's requests.

// TRUE while the channel has room (sWiFiPlazaChannelCapacities per channel).
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
    return sWiFiPlazaChannelCapacities[channel] > count;
}

// Entered the lobby (libChannel 0) or a channel.
void WiFiPlazaLobbyClient_OnEnterChannel(int result, u32 libChannel) {
    WiFiPlazaLobbyClient *client;

    if (libChannel == 0) {
        gWiFiPlazaLobbyClient->unk_5b4 = sizeof(gWiFiPlazaLobbyClient->unk_1b4);
        GF_ASSERT(WiFiPlazaLobbyClient_CheckResult(PPW_LobbyGetSchedule(&gWiFiPlazaLobbyClient->unk_1b4, &gWiFiPlazaLobbyClient->unk_5b4)));
        PPW_LobbyGetChannelDataAsync(-1, 0, sWiFiPlazaLocationListKey, 0);
        return;
    }
    client = gWiFiPlazaLobbyClient;
    if (client->channel == WiFiPlazaLobbyClient_ChannelFromLib(libChannel)) {
        if (result == 1) {
            client->unk_12f = 1;
            WiFiPlazaLobbyClient_RebuildChannelList(gWiFiPlazaLobbyClient, gWiFiPlazaLobbyClient->channel);
            WiFiPlazaLobbyClient_AddToIdList(gWiFiPlazaLobbyClient, PPW_LobbyGetMyUserId(), 1);
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
        PPW_LobbyGetTimeInfo(&gWiFiPlazaLobbyClient->time);
        isOwn = TRUE;
    } else {
        isOwn = FALSE;
    }
    WiFiPlazaLobbyClient_AddToIdList(gWiFiPlazaLobbyClient, id, 0);
    WiFiPlazaLobbyClient_CopyPlayerData(gWiFiPlazaLobbyClient, id, data, size);
    WiFiPlazaLobbyClient_SetPlayerRecord(gWiFiPlazaLobbyClient, id, record);
    client = gWiFiPlazaLobbyClient;
    player = WiFiPlazaLobbyClient_GetPlayerData(client, id);
    gWiFiPlazaLobbyClient->callbacks.onPlayerEnter(id, player, client->callbackArg, isOwn);
}

void WiFiPlazaLobbyClient_OnPlayerLeave(int id) {
    int i;

    if (WiFiPlazaLobbyClient_FindInIdList(gWiFiPlazaLobbyClient, id, 0) == -1) {
        return;
    }
    for (i = 0; i < 3; i++) {
        if (id == gWiFiPlazaLobbyClient->matches[i].hostId) {
            WiFiPlazaLobbyClient_OnMatchClosed(id, &gWiFiPlazaLobbyClient->matches[i].info);
        }
    }
    gWiFiPlazaLobbyClient->callbacks.onPlayerLeave(id, gWiFiPlazaLobbyClient->callbackArg);
    WiFiPlazaLobbyClient_ClearPlayerRecord(gWiFiPlazaLobbyClient, id);
    WiFiPlazaLobbyClient_ClearPlayerData(gWiFiPlazaLobbyClient, id);
    WiFiPlazaLobbyClient_RemoveFromIdList(gWiFiPlazaLobbyClient, id, 0);
    WiFiPlazaLobbyClient_RemoveFromIdList(gWiFiPlazaLobbyClient, id, 1);
}

void WiFiPlazaLobbyClient_OnEvent3Dummy(void) {
}

#pragma force_active on

// A message of the given type from a player: calls the receive callback of the lobby (libChannel 0 / 5) or channel
// table.
void WiFiPlazaLobbyClient_OnMessage(int id, u32 libChannel, u32 type, const void *data, u32 size) {
    int table;

    if (WiFiPlazaLobbyClient_FindInIdList(gWiFiPlazaLobbyClient, id, 0) == -1) {
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
        if (gWiFiPlazaLobbyClient->channel != WiFiPlazaLobbyClient_ChannelFromLib(libChannel)) {
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
    if (gWiFiPlazaLobbyClient->commandTables[table].count <= type) {
        return;
    }
    GF_ASSERT(gWiFiPlazaLobbyClient->commandTables[table].commands[type].size <= size);
    gWiFiPlazaLobbyClient->commandTables[table].commands[type].callback(id, data, gWiFiPlazaLobbyClient->commandTables[table].commands[type].size, gWiFiPlazaLobbyClient->commandTables[table].arg);
}

#pragma force_active reset

// The lobby's location list (key sWiFiPlazaLocationListKey) changed.
void WiFiPlazaLobbyClient_OnLocationsChanged(int a0, int a1, int a2, int a3, const char *key, const void *data) {
    if (strcmp(key, sWiFiPlazaLocationListKey) == 0) {
        memcpy(gWiFiPlazaLobbyClient->locations, data, sizeof(gWiFiPlazaLobbyClient->locations));
        if (gWiFiPlazaLobbyClient->unk_130) {
            gWiFiPlazaLobbyClient->unk_130 = 0;
        }
    }
}

#pragma force_active on

// A player's record changed: follows the player into or out of the own channel.
void WiFiPlazaLobbyClient_OnPlayerRecordChanged(int id, const WiFiPlazaLobbyPlayerRecord *record) {
    if (WiFiPlazaLobbyClient_FindInIdList(gWiFiPlazaLobbyClient, id, 0) != -1) {
        if (gWiFiPlazaLobbyClient->channel != 4) {
            int current, old = WiFiPlazaLobbyClient_GetPlayerRecord(gWiFiPlazaLobbyClient, id)->channel;
            current = record->channel;
            {
                int own = WiFiPlazaLobbyClient_ChannelToLib(gWiFiPlazaLobbyClient->channel);
                if (old == 5 && current == own) {
                    WiFiPlazaLobbyClient_AddToIdList(gWiFiPlazaLobbyClient, id, 1);
                } else if (old == own && current != own) {
                    WiFiPlazaLobbyClient_RemoveFromIdList(gWiFiPlazaLobbyClient, id, 1);
                }
            }
        }
        WiFiPlazaLobbyClient_SetPlayerRecord(gWiFiPlazaLobbyClient, id, record);
    }
}

#pragma force_active reset

void WiFiPlazaLobbyClient_OnPlayerUpdate(int id, const void *data) {
    if (WiFiPlazaLobbyClient_FindInIdList(gWiFiPlazaLobbyClient, id, 0) != -1) {
        WiFiPlazaLobbyClient_SetPlayerData(gWiFiPlazaLobbyClient, id, data);
        gWiFiPlazaLobbyClient->callbacks.onPlayerUpdate(id, (WiFiPlazaPlayer *)data, gWiFiPlazaLobbyClient->callbackArg);
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
    int slot = WiFiPlazaLobbyClient_FindInIdList(gWiFiPlazaLobbyClient, id, 0);
    if (slot != -1) {
        if (IsSameId(WiFiPlazaLobbyClient_GetOwnId(), id)) {
        } else if (gWiFiPlazaLobbyClient->unk_69e && gWiFiPlazaLobbyClient->currentMatch == info->index) {
            WiFiPlazaLobbyMatchInfo_GetStartTime(&gWiFiPlazaLobbyClient->matches[gWiFiPlazaLobbyClient->currentMatch].info, &current);
            WiFiPlazaLobbyMatchInfo_GetStartTime(info, &received);
            if (current < received) {
                return;
            }
            gWiFiPlazaLobbyClient->unk_69f = 1;
            WiFiPlazaLobbyClient_StopMatch();
        }
        WiFiPlazaLobbyMatches_Set(gWiFiPlazaLobbyClient, info->index, id, info);
    }
}

#pragma force_active reset

void WiFiPlazaLobbyClient_OnMatchClosed(int id, const WiFiPlazaLobbyMatchInfo *info) {
    int result;
    if (WiFiPlazaLobbyClient_FindInIdList(gWiFiPlazaLobbyClient, id, 0) == -1) {
        return;
    }
    result = WiFiPlazaLobbyMatches_Remove(gWiFiPlazaLobbyClient, info->index, id);
    if (gWiFiPlazaLobbyClient->currentMatch == info->index && result == 1) {
        gWiFiPlazaLobbyClient->unk_69f = 1;
    }
}

void WiFiPlazaLobbyClient_OnLobbyEvent(u32 event) {
    gWiFiPlazaLobbyClient->callbacks.onLobbyEvent(event, gWiFiPlazaLobbyClient->callbackArg);
}

void WiFiPlazaLobbyClient_OnOwnRecord(int result, const WiFiPlazaPlayer *data, u32 size) {
    if (result != 0) {
        gWiFiPlazaLobbyClient->unk_133 = 1;
    } else {
        gWiFiPlazaLobbyClient->callbacks.onOwnRecord(data, size, gWiFiPlazaLobbyClient->callbackArg);
    }
}

BOOL WiFiPlazaLobbyClient_OnEvent12Result(int result) {
    if (result == 0) {
        return TRUE;
    }
    gWiFiPlazaLobbyClient->unk_132 = 1;
    return FALSE;
}

#pragma force_active on

void WiFiPlazaLobbyClient_OnSpecialVisitorsReceived(int result, const void *data, u32 size) {
    if (result == 0) {
        WiFiPlazaLobbyIdValueList_Set(&gWiFiPlazaLobbyClient->unk_6a0, data, size);
    } else {
        gWiFiPlazaLobbyClient->unk_132 = 1;
    }
}

void WiFiPlazaLobbyClient_OnSurveyReceived(int result, const void *data) {
    if (result == 0) {
        WiFiPlazaLobbyClient_CopySurveyData(&gWiFiPlazaLobbyClient->unk_6a0.unk_04[0].unk_04, data);
    } else {
        gWiFiPlazaLobbyClient->unk_132 = 1;
    }
}

#pragma force_active reset

void WiFiPlazaLobbyClient_OnSurveySubmitted(void) {
    GF_ASSERT(gWiFiPlazaLobbyClient->unk_980 == 1);
    gWiFiPlazaLobbyClient->unk_980 = 2;
}

void WiFiPlazaLobbyClient_SetFlag131(void) {
    gWiFiPlazaLobbyClient->unk_131 = 1;
}

void WiFiPlazaLobbyClient_InitMatches(WiFiPlazaLobbyClient *client) {
    memset(client->matches, 0, sizeof(client->matches));
    client->currentMatch = 3;
}
