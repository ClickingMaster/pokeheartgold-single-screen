#include <string.h>

#include "global.h"

#include "wifi_plaza/wifi_plaza_internal.h"

#include "assert.h"
#include "wifi_connection.h"

static inline BOOL IsSameId(int id, int other);
static void WiFiPlazaLobbyMatches_Set(WiFiPlazaLobbyClient *client, int index, int hostId, const WiFiPlazaLobbyMatchInfo *info);
static u16 WiFiPlazaLobbyMatches_CalcTimer(WiFiPlazaLobbyClient *client, int index);
static void WiFiPlazaLobbyMatchInfo_GetStartTime(const WiFiPlazaLobbyMatchInfo *info, s64 *time);
static void WiFiPlazaLobbyIdValueList_Set(WiFiPlazaLobbyIdValueList *list, const void *data, u32 count);
static void WiFiPlazaLobbyClient_CopySurveyData(u32 *dest, const void *data);
static const WiFiPlazaSpriteConfig *WiFiPlaza_GetSpriteConfig(int spriteId);
static u32 WiFiPlaza_GetSpriteSequence(int standing, u32 sequence);
static u32 WiFiPlaza_CalcSequenceFrame(u32 divisor, u16 value, u32 factor);
static void WiFiPlazaObjectModel_Load(WiFiPlazaObjectModel *model, NARC *narc, int fileId, enum HeapID heapID);
static void WiFiPlazaObjectModel_Free(WiFiPlazaObjectModel *model);
static void WiFiPlazaObjectModel_SetAlpha(WiFiPlazaObjectModel *model, int alpha);
static int WiFiPlazaObjectModel_GetAlpha(WiFiPlazaObjectModel *model);
static void WiFiPlaza_InitModelInstance(ModelResource *resource, ModelInstance *instance);
static WiFiPlazaPlayerObject *WiFiPlazaPlayerObjects_GetFreeObject(WiFiPlazaPlayerObjects *objects);
static BOOL WiFiPlazaPlayerObject_IsUsed(WiFiPlazaPlayerObject *object);
static void WiFiPlazaPlayerObject_Clear(WiFiPlazaPlayerObject *object);
static void WiFiPlazaPlayerObject_DrawModel(WiFiPlazaPlayerObject *object);
static void WiFiPlazaPlayerObject_EffectTurnAround(WiFiPlazaPlayerObject *object);
static void WiFiPlazaPlayerObject_EffectSetWalkFrameOnce(WiFiPlazaPlayerObject *object);
static void WiFiPlazaPlayerObject_EffectWalkInPlace(WiFiPlazaPlayerObject *object);
static void WiFiPlazaPlayerObject_ApplyVisibility(WiFiPlazaPlayerObject *object);
static void WiFiPlazaPlayerObject_ApplyLightFlags(void *sprite, WiFiPlazaPlayerObject *object);

// The callbacks the lobby library calls: channel joined, players entering, leaving and updating, messages, the
// location list, match updates and the results of the library's requests.

static const u16 sWiFiPlazaSpriteSequenceFiles[2] = { 0x118, 0x119 };

static const u8 sWiFiPlazaChannelCapacities[4] = { 8, 8, 4, 4 };

static const u8 sWiFiPlazaTurnAroundDirections[4] = { 1, 2, 0, 3 };

static const VecFx32 sWiFiPlazaPlayerSpriteScale = { FX32_ONE, FX32_ONE, FX32_ONE };

static const BillboardSequenceRange sWiFiPlazaBillboardRanges[9] = {
    { 0,   15,  0 },
    { 16,  31,  0 },
    { 32,  47,  0 },
    { 48,  63,  0 },
    { 64,  79,  0 },
    { 80,  95,  0 },
    { 96,  111, 0 },
    { 112, 127, 0 },
    { 0,   0,   2 },
};

static const WiFiPlazaSpriteConfig sWiFiPlazaSpriteConfigs[20] = {
    { 0,   69,  TRUE  },
    { 97,  70,  TRUE  },
    { 3,   2,   FALSE },
    { 5,   4,   FALSE },
    { 11,  10,  FALSE },
    { 31,  36,  FALSE },
    { 50,  47,  FALSE },
    { 51,  48,  FALSE },
    { 62,  27,  FALSE },
    { 70,  33,  FALSE },
    { 6,   5,   FALSE },
    { 7,   6,   FALSE },
    { 13,  13,  FALSE },
    { 14,  14,  FALSE },
    { 35,  38,  FALSE },
    { 37,  40,  FALSE },
    { 42,  43,  FALSE },
    { 63,  28,  FALSE },
    { 286, 102, FALSE },
    { 285, 101, FALSE },
};

static void (*sWiFiPlazaPlayerObjectEffects[3])(WiFiPlazaPlayerObject *) = {
    WiFiPlazaPlayerObject_EffectTurnAround,
    WiFiPlazaPlayerObject_EffectSetWalkFrameOnce,
    WiFiPlazaPlayerObject_EffectWalkInPlace,
};

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
        gWiFiPlazaLobbyClient->scheduleSize = sizeof(gWiFiPlazaLobbyClient->schedule);
        GF_ASSERT(WiFiPlazaLobbyClient_CheckResult(PPW_LobbyGetSchedule(&gWiFiPlazaLobbyClient->schedule, &gWiFiPlazaLobbyClient->scheduleSize)));
        PPW_LobbyGetChannelDataAsync(-1, 0, gWiFiPlazaLocationListKey, 0);
        return;
    }
    client = gWiFiPlazaLobbyClient;
    if (client->channel == WiFiPlazaLobbyClient_ChannelFromLib(libChannel)) {
        if (result == 1) {
            client->joinChannelResult = WIFI_PLAZA_JOIN_CHANNEL_DONE;
            WiFiPlazaLobbyClient_RebuildChannelList(gWiFiPlazaLobbyClient, gWiFiPlazaLobbyClient->channel);
            WiFiPlazaLobbyClient_AddToIdList(gWiFiPlazaLobbyClient, PPW_LobbyGetMyUserId(), 1);
        } else {
            client->joinChannelResult = WIFI_PLAZA_JOIN_CHANNEL_FAILED;
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

// The lobby's location list (key gWiFiPlazaLocationListKey) changed.
void WiFiPlazaLobbyClient_OnLocationsChanged(int a0, int a1, int a2, int a3, const char *key, const void *data) {
    if (strcmp(key, gWiFiPlazaLocationListKey) == 0) {
        memcpy(gWiFiPlazaLobbyClient->locations, data, sizeof(gWiFiPlazaLobbyClient->locations));
        if (gWiFiPlazaLobbyClient->addingLocation) {
            gWiFiPlazaLobbyClient->addingLocation = 0;
        }
    }
}

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

void WiFiPlazaLobbyClient_OnPlayerUpdate(int id, const void *data) {
    if (WiFiPlazaLobbyClient_FindInIdList(gWiFiPlazaLobbyClient, id, 0) != -1) {
        WiFiPlazaLobbyClient_SetPlayerData(gWiFiPlazaLobbyClient, id, data);
        gWiFiPlazaLobbyClient->callbacks.onPlayerUpdate(id, (WiFiPlazaPlayer *)data, gWiFiPlazaLobbyClient->callbackArg);
    }
}

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
        } else if (gWiFiPlazaLobbyClient->isMatchHost && gWiFiPlazaLobbyClient->currentMatch == info->index) {
            WiFiPlazaLobbyMatchInfo_GetStartTime(&gWiFiPlazaLobbyClient->matches[gWiFiPlazaLobbyClient->currentMatch].info, &current);
            WiFiPlazaLobbyMatchInfo_GetStartTime(info, &received);
            if (current < received) {
                return;
            }
            gWiFiPlazaLobbyClient->matchCancelled = 1;
            WiFiPlazaLobbyClient_StopMatch();
        }
        WiFiPlazaLobbyMatches_Set(gWiFiPlazaLobbyClient, info->index, id, info);
    }
}

void WiFiPlazaLobbyClient_OnMatchClosed(int id, const WiFiPlazaLobbyMatchInfo *info) {
    int result;
    if (WiFiPlazaLobbyClient_FindInIdList(gWiFiPlazaLobbyClient, id, 0) == -1) {
        return;
    }
    result = WiFiPlazaLobbyMatches_Remove(gWiFiPlazaLobbyClient, info->index, id);
    if (gWiFiPlazaLobbyClient->currentMatch == info->index && result == 1) {
        gWiFiPlazaLobbyClient->matchCancelled = 1;
    }
}

void WiFiPlazaLobbyClient_OnLobbyEvent(u32 event) {
    gWiFiPlazaLobbyClient->callbacks.onLobbyEvent(event, gWiFiPlazaLobbyClient->callbackArg);
}

void WiFiPlazaLobbyClient_OnOwnRecord(int result, const WiFiPlazaPlayer *data, u32 size) {
    if (result != 0) {
        gWiFiPlazaLobbyClient->ownRecordError = 1;
    } else {
        gWiFiPlazaLobbyClient->callbacks.onOwnRecord(data, size, gWiFiPlazaLobbyClient->callbackArg);
    }
}

BOOL WiFiPlazaLobbyClient_OnScheduleReceived(int result) {
    if (result == 0) {
        return TRUE;
    }
    gWiFiPlazaLobbyClient->requestError = 1;
    return FALSE;
}

void WiFiPlazaLobbyClient_OnSpecialVisitorsReceived(int result, const void *data, u32 size) {
    if (result == 0) {
        WiFiPlazaLobbyIdValueList_Set(&gWiFiPlazaLobbyClient->specialVisitors, data, size);
    } else {
        gWiFiPlazaLobbyClient->requestError = 1;
    }
}

void WiFiPlazaLobbyClient_OnSurveyReceived(int result, const void *data) {
    if (result == 0) {
        WiFiPlazaLobbyClient_CopySurveyData(&gWiFiPlazaLobbyClient->surveyData.surveys[0].surveyId, data);
    } else {
        gWiFiPlazaLobbyClient->requestError = 1;
    }
}

void WiFiPlazaLobbyClient_OnSurveySubmitted(void) {
    GF_ASSERT(gWiFiPlazaLobbyClient->surveySubmitState == 1);
    gWiFiPlazaLobbyClient->surveySubmitState = 2;
}

void WiFiPlazaLobbyClient_OnExcessFlood(void) {
    gWiFiPlazaLobbyClient->excessFlood = 1;
}

void WiFiPlazaLobbyClient_InitMatches(WiFiPlazaLobbyClient *client) {
    memset(client->matches, 0, sizeof(client->matches));
    client->currentMatch = 3;
}

// The three matchmaking sessions of the lobby client: the info their hosts send, the start time, the countdown.

static void WiFiPlazaLobbyMatches_Set(WiFiPlazaLobbyClient *client, int index, int hostId, const WiFiPlazaLobbyMatchInfo *info) {
    GF_ASSERT(index < 3);
    if (WiFiPlazaLobbyMatches_IsActive(client, index) == TRUE && hostId != client->matches[index].hostId) {
        return;
    }
    client->matches[index].info = *info;
    if (client->matches[index].active == 0) {
        client->matches[index].active = 1;
        client->matches[index].hostId = hostId;
        client->matches[index].timer = WiFiPlazaLobbyMatches_CalcTimer(client, index);
    }
}

// The host closed the session.
BOOL WiFiPlazaLobbyMatches_Remove(WiFiPlazaLobbyClient *client, int index, int hostId) {
    GF_ASSERT(index < 3);
    if (WiFiPlazaLobbyMatches_IsActive(client, index) == TRUE) {
        if (hostId == client->matches[index].hostId) {
            client->matches[index].active = 0;
            return TRUE;
        }
    }
    return FALSE;
}

BOOL WiFiPlazaLobbyMatches_IsActive(WiFiPlazaLobbyClient *client, int index) {
    GF_ASSERT(index < 3);
    return client->matches[index].active;
}

// Frames left: 60 s after the start, at most 30 s of it gone.
static u16 WiFiPlazaLobbyMatches_CalcTimer(WiFiPlazaLobbyClient *client, int index) {
    s64 start;
    s64 elapsed;
    if (WiFiPlazaLobbyMatches_IsActive(client, index) == FALSE) {
        return 0;
    }
    WiFiPlazaLobbyMatchInfo_GetStartTime(&WiFiPlazaLobbyMatches_Get(client, index)->info, &start);
    elapsed = client->time - start;
    if (elapsed > 30) {
        elapsed = 30;
    } else if (elapsed < 0) {
        elapsed = 0;
    }
    return (60 - elapsed) * 30;
}

WiFiPlazaLobbyMatch *WiFiPlazaLobbyMatches_Get(WiFiPlazaLobbyClient *client, int index) {
    GF_ASSERT(index < 3);
    GF_ASSERT(client->matches[index].active == 1);
    return &client->matches[index];
}

// Once per frame: the player count of the own session, the countdowns.
void WiFiPlazaLobbyMatches_Update(WiFiPlazaLobbyClient *client) {
    int i;
    if (gWiFiPlazaLobbyClient->isMatchHost == 1) {
        u32 count = WifiConnection_GetNumConnectionHost();
        WiFiPlazaLobbyMatch *match;
        GF_ASSERT(count <= 4);
        match = &gWiFiPlazaLobbyClient->matches[gWiFiPlazaLobbyClient->currentMatch];
        if (match->info.playerCount != count) {
            match->info.playerCount = count;
            PPW_LobbyUpdateRecruitInfo(match);
        }
    }
    for (i = 0; i < 3; i++) {
        if (gWiFiPlazaLobbyClient->matches[i].active == 1) {
            u32 frames;
            if (gWiFiPlazaLobbyClient->matches[i].timer) {
                gWiFiPlazaLobbyClient->matches[i].timer--;
            }
            frames = WiFiPlazaLobbyMatches_CalcTimer(gWiFiPlazaLobbyClient, i);
            if (frames < gWiFiPlazaLobbyClient->matches[i].timer) {
                gWiFiPlazaLobbyClient->matches[i].timer = frames;
            }
        }
    }
}

void WiFiPlazaLobbyMatchInfo_SetStartTime(WiFiPlazaLobbyMatchInfo *info, const s64 *time) {
    info->startTime = *(const WiFiPlazaLobbyEntryTime *)time;
}

static void WiFiPlazaLobbyMatchInfo_GetStartTime(const WiFiPlazaLobbyMatchInfo *info, s64 *time) {
    *(WiFiPlazaLobbyEntryTime *)time = info->startTime;
}

void WiFiPlazaLobbyClient_OnMatchClientDummy(int result, void *arg) {
}

// Wi-Fi Plaza player objects: one billboard sprite (field character model) plus a model under it for every player
// on the plaza map.

void WiFiPlazaLobbyClient_OnMatchDisconnect(int result, void *arg) {
    WiFiPlazaLobbyClient *client = arg;
    if (client->isMatchHost == 1) {
        WiFiPlazaLobbyClient_StopMatch();
    } else if (result == 0) {
        gWiFiPlazaLobbyClient->matchCancelled = 1;
    }
}

u32 WiFiPlazaLobbySchedule_FindValue(WiFiPlazaLobbySchedule *schedule, int key) {
    u32 result = 0;
    int i;
    for (i = 0; i < schedule->count; i++) {
        if (key == schedule->values[i].key) {
            result = schedule->values[i].value;
        }
    }
    return result;
}

void WiFiPlazaLobbyIdValueList_Init(WiFiPlazaLobbyIdValueList *list, int count, enum HeapID heapID) {
    list->items = Heap_Alloc(heapID, count * sizeof(WiFiPlazaLobbyIdValue));
    list->capacity = count;
    list->count = 0;
}

void WiFiPlazaLobbyIdValueList_Free(WiFiPlazaLobbyIdValueList *list) {
    Heap_Free(list->items);
    list->items = NULL;
}

static void WiFiPlazaLobbyIdValueList_Set(WiFiPlazaLobbyIdValueList *list, const void *data, u32 count) {
    GF_ASSERT(count < list->capacity);
    if (count >= list->capacity) {
        count = list->capacity;
    }
    MIi_CpuCopy32(data, (u32 *)list->items, count * sizeof(WiFiPlazaLobbyIdValue));
    list->count = count;
}

BOOL WiFiPlazaLobbyIdValueList_Contains(WiFiPlazaLobbyIdValueList *list, int id) {
    int i;
    for (i = 0; i < list->count; i++) {
        if (id == list->items[i].id) {
            return TRUE;
        }
    }
    return FALSE;
}

u32 WiFiPlazaLobbyIdValueList_GetValue(WiFiPlazaLobbyIdValueList *list, int id) {
    int i;
    for (i = 0; i < list->count; i++) {
        if (id == list->items[i].id) {
            return list->items[i].value;
        }
    }
    return 0;
}

static void WiFiPlazaLobbyClient_CopySurveyData(u32 *dest, const void *data) {
    memcpy(dest, data, 0x2D8);
}

WiFiPlazaPlayerObjects *WiFiPlazaPlayerObjects_New(u32 count, BOOL mode, enum HeapID heapID, enum HeapID loadHeapID) {
    WiFiPlazaPlayerObjects *objects;
    int i;
    NARC *modelNarc;
    NARC *narc;
    void *file;
    GF_3DGfxRawResObj *texture;
    BillboardCreateParams params;
    int skipped;

    objects = Heap_Alloc(heapID, sizeof(WiFiPlazaPlayerObjects));
    memset(objects, 0, sizeof(WiFiPlazaPlayerObjects));
    objects->objects = Heap_Alloc(heapID, count * sizeof(WiFiPlazaPlayerObject));
    objects->count = count;
    for (i = 0; i < objects->count; i++) {
        WiFiPlazaPlayerObject_Clear(&objects->objects[i]);
    }

    objects->models = GF2dGfxRawResMan_Create(1, heapID);
    objects->sequences = GF2dGfxRawResMan_Create(2, heapID);
    objects->textures = GF3dGfxRawResMan_Create(20, heapID);
    BillboardLists_Create(1, heapID);
    params.count = count;
    params.heapId = heapID;
    objects->billboards = Billboard_Create(&params);

    modelNarc = NARC_New(NARC_data_mmodel_mmodel, heapID);
    narc = NARC_New(NARC_a_2_0_7, heapID);
    file = GfGfxLoader_LoadFromOpenNarc(narc, 0x7F, FALSE, loadHeapID, FALSE);
    GF2dGfxRawResMan_AllocObj(objects->models, file, 0x7F);
    WiFiPlaza_DisableModelLighting(file);
    objects->lightFlags = NNS_G3dMdlGetMdlLightEnableFlag(NNS_G3dGetMdlByIdx(NNS_G3dGetMdlSet(file), 0), 0);

    for (i = 0; i < 2; i++) {
        file = GfGfxLoader_LoadFromOpenNarc(modelNarc, sWiFiPlazaSpriteSequenceFiles[i], FALSE, loadHeapID, FALSE);
        GF2dGfxRawResMan_AllocObj(objects->sequences, file, sWiFiPlazaSpriteSequenceFiles[i]);
    }

    if (mode == FALSE) {
        skipped = 1;
    } else {
        skipped = 0;
    }
    {
        const WiFiPlazaSpriteConfig *config = sWiFiPlazaSpriteConfigs;
        int index = 0;
        do {
            if (skipped != index) {
                BOOL loadTexture = config->ownTexture == 0;
                file = GfGfxLoader_LoadFromOpenNarc(modelNarc, config->fileId, FALSE, loadHeapID, FALSE);
                texture = GF3dGfxRawResMan_AllocObj(objects->textures, file, config->fileId, loadTexture, loadHeapID);
                if (loadTexture == TRUE) {
                    GF3dGfxRawResObj_AllocVramAndGetKeys(texture);
                    GF3dGfxRawResObj_LoadTex(texture);
                    GF3dGfxRawResObj_FreeVramAndSecondaryHeader(texture);
                }
            }
            config++;
            index++;
        } while (index < 20);
    }

    WiFiPlazaObjectModel_Load(&objects->model, narc, 0x80, loadHeapID);
    NARC_Delete(modelNarc);
    NARC_Delete(narc);
    return objects;
}

void WiFiPlazaPlayerObjects_Delete(WiFiPlazaPlayerObjects *objects) {
    WiFiPlazaObjectModel_Free(&objects->model);
    GF3dGfxRawResMan_FreeAllObjs(objects->textures);
    GF2dGfxRawResMan_FreeAllObjs(objects->models);
    GF2dGfxRawResMan_FreeAllObjs(objects->sequences);
    Billboard_Delete(objects->billboards);
    BillboardLists_Delete();
    GF3dGfxRawResMan_Destroy(objects->textures);
    GF2dGfxRawResObj_Destroy(objects->models);
    GF2dGfxRawResObj_Destroy(objects->sequences);
    Heap_Free(objects->objects);
    Heap_Free(objects);
}

void WiFiPlazaPlayerObjects_Update(WiFiPlazaPlayerObjects *objects) {
    u32 i;
    for (i = 0; i < objects->count; i++) {
        WiFiPlazaPlayerObject_FollowPlayer(&objects->objects[i]);
        WiFiPlazaPlayerObject_UpdateEffect(&objects->objects[i]);
        WiFiPlazaPlayerObject_UpdateCulling(&objects->objects[i]);
    }
}

void WiFiPlazaPlayerObjects_Draw(WiFiPlazaPlayerObjects *objects) {
    u32 i;
    BillboardLists_Draw();
    if (WiFiPlazaObjectModel_GetAlpha(&objects->model)) {
        for (i = 0; i < objects->count; i++) {
            if (WiFiPlazaPlayerObject_IsUsed(&objects->objects[i]) == TRUE) {
                WiFiPlazaPlayerObject_DrawModel(&objects->objects[i]);
            }
        }
    }
}

void WiFiPlazaPlayerObjects_ClearPendingFree(WiFiPlazaPlayerObjects *objects) {
    Billboard_ClearPendingFree(objects->billboards);
}

void WiFiPlazaPlayerObjects_SetModelAlpha(WiFiPlazaPlayerObjects *objects, int alpha) {
    WiFiPlazaObjectModel_SetAlpha(&objects->model, alpha);
}

WiFiPlazaPlayerObject *WiFiPlazaPlayerObjects_Add(WiFiPlazaPlayerObjects *objects, Field2DObject *player) {
    BillboardResource resource;
    BillboardObjectCreateParams create;
    BillboardGfxSequence sequence;
    Field2DPoint gridPos;
    const WiFiPlazaSpriteConfig *config;
    GF_3DGfxRawResObj *texture;
    void *modelFile;
    NNSG3dResTex *tex;
    NNSG3dTexKey tex4x4Key;
    NNSG3dTexKey texKey;

    WiFiPlazaPlayerObject *object = WiFiPlazaPlayerObjects_GetFreeObject(objects);
    object->player = player;
    VecFx32 position = { 0, 0, 0 };
    VecFx32 scale = sWiFiPlazaPlayerSpriteScale;
    config = WiFiPlaza_GetSpriteConfig(Field2DObject_GetParam(player, 7));
    modelFile = GF2dGfxRawResObj_GetData(GF2dGfxRawResMan_GetObjById(objects->models, 0x7F));
    texture = GF3dGfxRawResMan_GetObjById(objects->textures, config->fileId);
    tex = GF3dGfxRawResObj_GetTex(texture);
    if (config->ownTexture == 1) {
        texKey = 0x119;
    } else {
        texKey = 0x118;
    }
    BillboardGfxSequence_Init(GF2dGfxRawResObj_GetData(GF2dGfxRawResMan_GetObjById(objects->sequences, texKey)), &sequence);
    if (config->ownTexture == 1) {
        BillboardResource_Init(&resource, modelFile, tex, sWiFiPlazaBillboardRanges, &sequence);
    } else {
        texKey = GF3dGfxRawResObj_GetTexKey(texture);
        tex4x4Key = GF3dGfxRawResObj_GetTex4x4Key(texture);
        BillboardResource_InitWithKeys(&resource, modelFile, tex, sWiFiPlazaBillboardRanges, &sequence, texKey, tex4x4Key, GF3dGfxRawResObj_GetPlttKey(texture));
    }
    gridPos = Field2DObject_GetPos(player);
    WiFiPlazaMap_PosToVecFx32(&gridPos.x, &position.x);
    position.z += FX32_ONE * 16;
    create.billboard = objects->billboards;
    create.resource = &resource;
    create.position = position;
    create.scale = scale;
    object->sprite = BillboardObject_Create(&create);
    BillboardObject_SetCallback(object->sprite, WiFiPlazaPlayerObject_ApplyLightFlags, object);
    WiFiPlaza_InitModelInstance(&objects->model.resource, &object->model);
    ModelInstance_SetPosition(&object->model, position.x, FX32_ONE * 2, position.z - FX32_ONE * 8);
    object->mode = 1;
    object->visible = 1;
    object->culled = 0;
    object->lightFlags = objects->lightFlags;
    return object;
}

void WiFiPlazaPlayerObject_Delete(WiFiPlazaPlayerObject *object) {
    BillboardObject_Delete(object->sprite);
    memset(object, 0, sizeof(WiFiPlazaPlayerObject));
}

void WiFiPlazaPlayerObject_SetMode(WiFiPlazaPlayerObject *object, u32 mode) {
    object->mode = mode;
}

void WiFiPlazaPlayerObject_SetLightFlags(WiFiPlazaPlayerObject *object, u32 lightFlags) {
    object->lightFlags = lightFlags;
}

u32 WiFiPlazaPlayerObject_IsCulled(WiFiPlazaPlayerObject *object) {
    return object->culled;
}

void WiFiPlazaPlayerObject_SetGridPosition(WiFiPlazaPlayerObject *object, const Field2DPoint *gridPos) {
    VecFx32 position;
    WiFiPlazaMap_PosToVecFx32(&gridPos->x, &position.x);
    position.z += FX32_ONE * 16;
    WiFiPlazaPlayerObject_SetPosition(object, &position);
}

void WiFiPlazaPlayerObject_SetPosition(WiFiPlazaPlayerObject *object, const VecFx32 *position) {
    BillboardObject_SetPosition(object->sprite, position);
    ModelInstance_SetPosition(&object->model, position->x, FX32_ONE * 2, position->z - FX32_ONE * 8);
}

void WiFiPlazaPlayerObject_GetPosition(WiFiPlazaPlayerObject *object, VecFx32 *position) {
    *position = *BillboardObject_GetPosition(object->sprite);
}

void WiFiPlazaPlayerObject_SetStandingSequence(WiFiPlazaPlayerObject *object, u32 sequence) {
    BillboardObject_SetSequence(object->sprite, WiFiPlaza_GetSpriteSequence(1, sequence));
    BillboardObject_SetSequenceFrame(object->sprite, 0);
}

void WiFiPlazaPlayerObject_StartEffect(WiFiPlazaPlayerObject *object, u32 effect) {
    object->effectActive = 1;
    object->effect = effect;
    object->timer = 0;
    object->step = 1;
}

void WiFiPlazaPlayerObject_StopEffect(WiFiPlazaPlayerObject *object) {
    object->effectActive = 0;
}

void WiFiPlazaPlayerObject_SetEffectStep(WiFiPlazaPlayerObject *object, u8 step) {
    object->step = step;
}

void WiFiPlazaPlayerObject_SetVisible(WiFiPlazaPlayerObject *object, int visible) {
    object->visible = visible;
    WiFiPlazaPlayerObject_ApplyVisibility(object);
}

u32 WiFiPlazaPlayerObject_IsVisible(WiFiPlazaPlayerObject *object) {
    return object->visible;
}

static const WiFiPlazaSpriteConfig *WiFiPlaza_GetSpriteConfig(int spriteId) {
    int i;
    for (i = 0; i < 20; i++) {
        if (spriteId == sWiFiPlazaSpriteConfigs[i].spriteId) {
            return &sWiFiPlazaSpriteConfigs[i];
        }
    }
    GF_ASSERT(FALSE);
    return NULL;
}

// Sequence of a sprite: the walking one (+4) or the standing one.
static u32 WiFiPlaza_GetSpriteSequence(int standing, u32 sequence) {
    return standing == 0 ? sequence + 4 : sequence;
}

static u32 WiFiPlaza_CalcSequenceFrame(u32 divisor, u16 value, u32 factor) {
    return ((u16)(value + 1) * factor / divisor) << FX32_SHIFT;
}

static void WiFiPlazaObjectModel_Load(WiFiPlazaObjectModel *model, NARC *narc, int fileId, enum HeapID heapID) {
    NNSG3dResFileHeader *file;

    WiFiPlazaMap_LoadModelFile(&file, narc, fileId, heapID);
    model->resource.file = file;
    model->resource.set = NNS_G3dGetMdlSet(model->resource.file);
    model->resource.model = NNS_G3dGetMdlByIdx(model->resource.set, 0);
    model->resource.texture = NNS_G3dGetTex(model->resource.file);
    GF3dRender_BindModelSet(model->resource.file, model->resource.texture);
    NNS_G3dMdlSetMdlPolygonIDAll(model->resource.model, 20);
}

static void WiFiPlazaObjectModel_Free(WiFiPlazaObjectModel *model) {
    ModelResource_Free(&model->resource);
}

static void WiFiPlazaObjectModel_SetAlpha(WiFiPlazaObjectModel *model, int alpha) {
    model->alpha = alpha;
    NNS_G3dMdlSetMdlAlphaAll(model->resource.model, alpha);
}

static int WiFiPlazaObjectModel_GetAlpha(WiFiPlazaObjectModel *model) {
    return model->alpha;
}

static void WiFiPlaza_InitModelInstance(ModelResource *resource, ModelInstance *instance) {
    ModelInstance_Init(instance, resource);
}

static WiFiPlazaPlayerObject *WiFiPlazaPlayerObjects_GetFreeObject(WiFiPlazaPlayerObjects *objects) {
    u32 i;

    for (i = 0; i < objects->count; i++) {
        if (!WiFiPlazaPlayerObject_IsUsed(&objects->objects[i])) {
            return &objects->objects[i];
        }
    }
    GF_ASSERT(FALSE);
    return NULL;
}

static BOOL WiFiPlazaPlayerObject_IsUsed(WiFiPlazaPlayerObject *object) {
    if (object->player != NULL) {
        return TRUE;
    }
    return FALSE;
}

static void WiFiPlazaPlayerObject_Clear(WiFiPlazaPlayerObject *object) {
    memset(object, 0, sizeof(*object));
}

static void WiFiPlazaPlayerObject_DrawModel(WiFiPlazaPlayerObject *object) {
    ModelInstance_Draw(&object->model);
}

// Follows the player's action (field 5) and its frame (field 8) and moves the sprite to the player's position.
void WiFiPlazaPlayerObject_FollowPlayer(WiFiPlazaPlayerObject *object) {
    int action;
    u32 actionFrame;
    Field2DPoint gridPos;
    if (object->mode) {
        action = Field2DObject_GetParam(object->player, 5);
        actionFrame = Field2DObject_GetParam(object->player, 8);
        if (object->actionFrame > actionFrame || object->action != action) {
            if (WiFiPlazaPlayerObject_ActionKeepsSequence(object->action) == TRUE) {
                object->savedSequence = BillboardObject_GetSequence(object->sprite);
                object->savedFrame = BillboardObject_GetSequenceFrame(object->sprite);
            }
            object->action = action;
        }
        object->actionFrame = actionFrame;
        sWiFiPlazaPlayerObjectActions[action](object);
        gridPos = Field2DObject_GetDrawPos(object->player);
        WiFiPlazaPlayerObject_SetGridPosition(object, &gridPos);
    }
}

void WiFiPlazaPlayerObject_ShowStandingSprite(WiFiPlazaPlayerObject *object) {
    u32 index = Field2DObject_GetParam(object->player, 6);
    u32 sequence = WiFiPlaza_GetSpriteSequence(1, index);
    BillboardObject_SetSequence(object->sprite, sequence);
    BillboardObject_SetSequenceFrame(object->sprite, 0);
}

void WiFiPlazaPlayerObject_PlayStandingSequence(WiFiPlazaPlayerObject *object) {
    u32 index = Field2DObject_GetParam(object->player, 6);
    u32 sequence = WiFiPlaza_GetSpriteSequence(1, index);
    u16 duration = Field2DObject_GetParam(object->player, 9);
    u16 frame = Field2DObject_GetParam(object->player, 8);
    s32 offset = WiFiPlaza_CalcSequenceFrame(duration, frame, 8);
    BillboardObject_SetSequence(object->sprite, sequence);
    if (object->savedSequence == sequence) {
        BillboardObject_SetSequenceFrame(object->sprite, 0);
        BillboardObject_AdvanceFrame(object->sprite, offset + object->savedFrame);
    } else {
        BillboardObject_SetSequenceFrame(object->sprite, 0);
        BillboardObject_AdvanceFrame(object->sprite, offset);
    }
}

void WiFiPlazaPlayerObject_HoldWalkFrameThenRestart(WiFiPlazaPlayerObject *object) {
    u16 frame = Field2DObject_GetParam(object->player, 8);
    if (frame < 4) {
        BillboardObject_SetSequenceFrame(object->sprite, FX32_ONE * 4);
        return;
    }
    BillboardObject_SetSequence(object->sprite, WiFiPlaza_GetSpriteSequence(1, Field2DObject_GetParam(object->player, 6)));
    BillboardObject_SetSequenceFrame(object->sprite, 0);
}

void WiFiPlazaPlayerObject_PlayWalkingSequence(WiFiPlazaPlayerObject *object) {
    u32 index = Field2DObject_GetParam(object->player, 6);
    u32 sequence = WiFiPlaza_GetSpriteSequence(0, index);
    u16 duration = Field2DObject_GetParam(object->player, 9);
    u16 frame = Field2DObject_GetParam(object->player, 8);
    s32 offset = WiFiPlaza_CalcSequenceFrame(duration, frame, 4);
    BillboardObject_SetSequence(object->sprite, sequence);
    if (object->savedSequence == sequence) {
        BillboardObject_SetSequenceFrame(object->sprite, 0);
        BillboardObject_AdvanceFrame(object->sprite, offset + object->savedFrame);
    } else {
        BillboardObject_SetSequenceFrame(object->sprite, 0);
        BillboardObject_AdvanceFrame(object->sprite, offset);
    }
}

void WiFiPlazaPlayerObject_UpdateEffect(WiFiPlazaPlayerObject *object) {
    if (object->mode == 0 && object->effectActive == 1) {
        sWiFiPlazaPlayerObjectEffects[object->effect](object);
    }
}

// Effect 1 (sWiFiPlazaPlayerObjectEffects): turns around, one direction every 4 ticks.
static void WiFiPlazaPlayerObject_EffectTurnAround(WiFiPlazaPlayerObject *object) {
    if (((int)object->timer) % 4 == 0) {
        BillboardObject_SetSequence(object->sprite, WiFiPlaza_GetSpriteSequence(1, sWiFiPlazaTurnAroundDirections[(u32)object->timer >> 2]));
        BillboardObject_SetSequenceFrame(object->sprite, 0);
    }
    if (object->timer + object->step < 16) {
        object->timer += object->step;
    } else {
        object->timer = 0;
    }
}

static void WiFiPlazaPlayerObject_EffectSetWalkFrameOnce(WiFiPlazaPlayerObject *object) {
    if (object->timer == 0) {
        Field2DObject_GetParam(object->player, 6);
        BillboardObject_SetSequence(object->sprite, WiFiPlaza_GetSpriteSequence(1, Field2DObject_GetParam(object->player, 6)));
        BillboardObject_SetSequenceFrame(object->sprite, FX32_ONE * 4);
        object->timer++;
    }
}

static void WiFiPlazaPlayerObject_EffectWalkInPlace(WiFiPlazaPlayerObject *object) {
    if (object->timer == 0) {
        BillboardObject_SetSequence(object->sprite, WiFiPlaza_GetSpriteSequence(1, Field2DObject_GetParam(object->player, 6)));
        BillboardObject_SetSequenceFrame(object->sprite, FX32_ONE * 4);
    } else if (object->timer == 4) {
        BillboardObject_SetSequence(object->sprite, WiFiPlaza_GetSpriteSequence(1, Field2DObject_GetParam(object->player, 6)));
        BillboardObject_SetSequenceFrame(object->sprite, FX32_ONE * 12);
    }
    object->timer = (object->timer + 1) % 8;
}

BOOL WiFiPlazaPlayerObject_ActionKeepsSequence(u32 action) {
    switch (action) {
    case 2:
    case 3:
    case 5:
    case 6:
    case 10:
    case 11:
        return TRUE;
    default:
        return FALSE;
    }
}

void WiFiPlazaPlayerObject_UpdateCulling(WiFiPlazaPlayerObject *object) {
    if (WiFiPlazaPlayerObject_IsUsed(object)) {
        if (WiFiPlaza_IsSpriteInView(object->sprite) == FALSE) {
            object->culled = 1;
        } else {
            object->culled = 0;
        }
        WiFiPlazaPlayerObject_ApplyVisibility(object);
    }
}

static void WiFiPlazaPlayerObject_ApplyVisibility(WiFiPlazaPlayerObject *object) {
    if (object->culled == 0 && object->visible == 1) {
        BillboardObject_SetVisible(object->sprite, TRUE);
        ModelInstance_SetVisible(&object->model, TRUE);
    } else {
        BillboardObject_SetVisible(object->sprite, FALSE);
        ModelInstance_SetVisible(&object->model, FALSE);
    }
}

static void WiFiPlazaPlayerObject_ApplyLightFlags(void *sprite, WiFiPlazaPlayerObject *object) {
    NNSG3dResMdl *model = BillboardObject_GetModelNoCast(sprite);
    NNS_G3dMdlSetMdlLightEnableFlagAll(model, object->lightFlags);
}
