#include <string.h>

#include "global.h"

#include "assert.h"
#include "heap.h"
#include "save_wifi_history.h"
#include "wifi_plaza_internal.h"

// The players of the lobby client: the data every player sends (playerDataSize bytes per lobby slot), the record of
// each slot (entry time, channel) and the two id lists (the lobby, the current channel).

#pragma force_active on

// Adds the own country and region to the lobby locations (once) and sends the list.
BOOL WiFiPlazaLobbyClient_AddOwnLocation(WiFiPlazaLobbyClient *client) {
    int country = WifiHistory_GetPlayerCountry(client->wifiHistory);
    int region = WiFiHistory_GetPlayerRegion(client->wifiHistory);
    int i;
    if (!country) {
        return FALSE;
    }
    for (i = 0; i < 20; i++) {
        if (!client->locations[i].active) {
            break;
        }
        if (country == client->locations[i].country && region == client->locations[i].region) {
            return FALSE;
        }
    }
    if (i >= 20) {
        GF_AssertFail();
        return FALSE;
    }
    client->locations[i].country = country;
    client->locations[i].region = region;
    client->locations[i].active = 1;
    return WiFiPlazaLobbyClient_CheckResult(PPW_LobbySetChannelData(-1, 0, ov45_02254EF0, client->locations, sizeof(client->locations)));
}

#pragma force_active reset

void WiFiPlazaLobbyClient_AllocPlayerData(WiFiPlazaLobbyClient *client, u32 size, enum HeapID heapID) {
    u32 bytes = size * 20;
    _022577C0->playerData = Heap_Alloc(heapID, bytes);
    memset(_022577C0->playerData, 0, bytes);
    _022577C0->playerDataSize = size;
}

void WiFiPlazaLobbyClient_FreePlayerData(WiFiPlazaLobbyClient *client) {
    Heap_Free(_022577C0->playerData);
}

void WiFiPlazaLobbyClient_SetPlayerData(WiFiPlazaLobbyClient *client, int id, const void *data) {
    WiFiPlazaLobbyClient_CopyPlayerData(client, id, data, client->playerDataSize);
}

void WiFiPlazaLobbyClient_CopyPlayerData(WiFiPlazaLobbyClient *client, int id, const void *data, u32 size) {
    int slot;
    u32 stride;
    u32 offset;
    u8 *base;
    slot = WiFiPlazaLobbyClient_FindInIdList(client, id, 0);
    GF_ASSERT(slot != -1);
    stride = client->playerDataSize;
    base = client->playerData;
    offset = stride * slot;
    if (stride > size) {
        memset(base + offset, 0, stride);
    } else {
        size = stride;
    }
    memcpy(base + offset, data, size);
}

WiFiPlazaPlayer *WiFiPlazaLobbyClient_GetPlayerData(WiFiPlazaLobbyClient *client, int id) {
    int slot = WiFiPlazaLobbyClient_FindInIdList(client, id, 0);
    GF_ASSERT(slot != -1);
    return (WiFiPlazaPlayer *)(client->playerData + client->playerDataSize * slot);
}

void WiFiPlazaLobbyClient_ClearPlayerData(WiFiPlazaLobbyClient *client, int id) {
    int slot = WiFiPlazaLobbyClient_FindInIdList(client, id, 0);
    GF_ASSERT(slot != -1);
    memset(client->playerData + client->playerDataSize * slot, 0, client->playerDataSize);
}

void WiFiPlazaLobbyClient_SetPlayerRecord(WiFiPlazaLobbyClient *client, int id, const WiFiPlazaLobbyPlayerRecord *record) {
    int slot = WiFiPlazaLobbyClient_FindInIdList(client, id, 0);
    WiFiPlazaLobbyPlayerRecord *dest;
    GF_ASSERT(slot != -1);
    dest = &client->records[slot];
    *dest = *record;
}

WiFiPlazaLobbyPlayerRecord *WiFiPlazaLobbyClient_GetPlayerRecord(WiFiPlazaLobbyClient *client, int id) {
    int slot = WiFiPlazaLobbyClient_FindInIdList(client, id, 0);
    GF_ASSERT(slot != -1);
    return &client->records[slot];
}

// Clears the 12 bytes one at a time.
void WiFiPlazaLobbyClient_ClearPlayerRecord(WiFiPlazaLobbyClient *client, int id) {
    int slot = WiFiPlazaLobbyClient_FindInIdList(client, id, 0);
    u8 *bytes;
    GF_ASSERT(slot != -1);
    bytes = (u8 *)&client->records[slot];
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

void WiFiPlazaLobbyClient_AllocIdLists(WiFiPlazaLobbyClient *client, u32 lobbyCapacity, u32 channelCapacity, enum HeapID heapID) {
    int i;
    u32 capacity;
    WiFiPlazaLobbyClient *p;
    for (i = 0, p = client; i < 2; i++) {
        GF_ASSERT(p->idLists[0].ids == NULL);
        if (i == 0) {
            capacity = lobbyCapacity;
        } else {
            capacity = channelCapacity;
        }
        p->idLists[0].count = 0;
        p->idLists[0].ids = Heap_Alloc(heapID, capacity * 4);
        p->idLists[0].capacity = capacity;
        WiFiPlazaLobbyClient_ClearIdList(client, i);
        // step to the next id list
        p = (WiFiPlazaLobbyClient *)((u8 *)p + sizeof(WiFiPlazaLobbyIdList));
    }
}

void WiFiPlazaLobbyClient_FreeIdLists(WiFiPlazaLobbyClient *client) {
    int i;
    WiFiPlazaLobbyClient *p;
    for (i = 0, p = client; i < 2; i++) {
        GF_ASSERT(p->idLists[0].ids != NULL);
        Heap_Free(p->idLists[0].ids);
        p->idLists[0].ids = NULL;
        // step to the next id list
        p = (WiFiPlazaLobbyClient *)((u8 *)p + sizeof(WiFiPlazaLobbyIdList));
    }
}

void WiFiPlazaLobbyClient_ClearIdList(WiFiPlazaLobbyClient *client, int which) {
    int i;
    client->idLists[which].count = 0;
    for (i = 0; i < client->idLists[which].capacity; i++) {
        client->idLists[which].ids[i] = -1;
    }
}

void WiFiPlazaLobbyClient_AddToIdList(WiFiPlazaLobbyClient *client, int id, int which) {
    int i;
    int freeSlot;
    if (WiFiPlazaLobbyClient_FindInIdList(client, id, which) == -1) {
        GF_ASSERT(client->idLists[which].count < client->idLists[which].capacity);
        if (client->idLists[which].count < client->idLists[which].capacity) {
            for (i = 0; i < client->idLists[which].capacity; i++) {
                if (client->idLists[which].ids[i] == -1) {
                    freeSlot = i;
                    break;
                }
            }
            client->idLists[which].ids[freeSlot] = id;
            client->idLists[which].count++;
        }
    }
}

void WiFiPlazaLobbyClient_RemoveFromIdList(WiFiPlazaLobbyClient *client, int id, int which) {
    int i = WiFiPlazaLobbyClient_FindInIdList(client, id, which);
    if (i != -1) {
        client->idLists[which].ids[i] = -1;
        client->idLists[which].count--;
    }
}

// Lobby slot of a player id, -1 if not in the list.
int WiFiPlazaLobbyClient_FindInIdList(WiFiPlazaLobbyClient *client, int id, int which) {
    int i;
    for (i = 0; i < client->idLists[which].capacity; i++) {
        if (client->idLists[which].ids[i] == id) {
            return i;
        }
    }
    return -1;
}

int WiFiPlazaLobbyClient_GetIdListEntry(WiFiPlazaLobbyClient *client, int slot, int which) {
    GF_ASSERT(slot < client->idLists[which].capacity);
    return client->idLists[which].ids[slot];
}

// Rebuilds the channel list from the players' channels.
void WiFiPlazaLobbyClient_RebuildChannelList(WiFiPlazaLobbyClient *client, u32 channel) {
    int i;
    u32 libChannel = WiFiPlazaLobbyClient_ChannelToLib(channel);
    WiFiPlazaLobbyClient_ClearIdList(client, 1);
    for (i = 0; i < client->idLists[0].capacity; i++) {
        int id = WiFiPlazaLobbyClient_GetIdListEntry(client, i, 0);
        if (id != -1 && libChannel == client->records[i].channel) {
            WiFiPlazaLobbyClient_AddToIdList(client, id, 1);
        }
    }
}
