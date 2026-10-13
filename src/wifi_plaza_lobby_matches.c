#include "global.h"

#include "assert.h"
#include "wifi_plaza_internal.h"

// The three matchmaking sessions of the lobby client: the info their hosts send, the start time, the countdown.

u32 ov00_021E7144(void);

#pragma force_active on

void WiFiPlazaLobbyMatches_Set(WiFiPlazaLobbyClient *client, int index, int hostId, const WiFiPlazaLobbyMatchInfo *info) {
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

#pragma force_active reset

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
u16 WiFiPlazaLobbyMatches_CalcTimer(WiFiPlazaLobbyClient *client, int index) {
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

#pragma force_active on

// Once per frame (WiFiPlazaLobbyClient_Update; works on _022577C0): the player count of the own session, the
// countdowns.
void WiFiPlazaLobbyMatches_Update(WiFiPlazaLobbyClient *client) {
    int i;
    if (_022577C0->unk_69e == 1) {
        u32 count = ov00_021E7144();
        WiFiPlazaLobbyMatch *match;
        GF_ASSERT(count <= 4);
        match = &_022577C0->matches[_022577C0->currentMatch];
        if (match->info.playerCount != count) {
            match->info.playerCount = count;
            PPW_LobbyUpdateRecruitInfo(match);
        }
    }
    for (i = 0; i < 3; i++) {
        // steps the client pointer by one match (0x4C bytes)
        WiFiPlazaLobbyClient *p = (WiFiPlazaLobbyClient *)((u8 *)_022577C0 + i * 0x4C);
        if (*(volatile u16 *)&p->matches[0].active == 1) {
            u32 frames;
            if (*(volatile u16 *)&p->matches[0].timer) {
                (*(volatile u16 *)&p->matches[0].timer)--;
            }
            frames = WiFiPlazaLobbyMatches_CalcTimer(_022577C0, i);
            p = (WiFiPlazaLobbyClient *)((u8 *)_022577C0 + i * 0x4C);
            if (frames < *(volatile u16 *)&p->matches[0].timer) {
                *(volatile u16 *)&p->matches[0].timer = frames;
            }
        }
    }
}

#pragma force_active reset

void WiFiPlazaLobbyMatchInfo_SetStartTime(WiFiPlazaLobbyMatchInfo *info, const s64 *time) {
    info->startTime = *(const WiFiPlazaLobbyEntryTime *)time;
}

void WiFiPlazaLobbyMatchInfo_GetStartTime(const WiFiPlazaLobbyMatchInfo *info, s64 *time) {
    *(WiFiPlazaLobbyEntryTime *)time = info->startTime;
}

void ov45_02230390(int result, void *arg) {
}
