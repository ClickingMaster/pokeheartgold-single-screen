#include <string.h>

#include "global.h"

#include "assert.h"
#include "wifi_plaza_internal.h"

// The callbacks the lobby library calls (ov45_0222E810 hands them over): channel joined, players entering, leaving
// and updating, messages, the location list, match updates and the results of the library's requests.

// TRUE while the channel has room (ov45_02254C30 per channel).
BOOL ov45_0222FBD8(WiFiPlazaLobbyClient *client, int channel) {
    u32 libChannel;
    int count;
    int i;

    GF_ASSERT(channel < 4);
    libChannel = ov45_0222F7B0(channel);
    count = 0;
    for (i = 0; i < client->idLists[0].capacity; i++) {
        int id = ov45_0222FB5C(client, i, 0);
        if (id != -1 && libChannel == client->records[i].channel) {
            count++;
        }
    }
    return ov45_02254C30[channel] > count;
}

// Entered the lobby (libChannel 0) or a channel.
void ov45_0222FC44(int result, u32 libChannel) {
    WiFiPlazaLobbyClient *client;

    if (libChannel == 0) {
        _022577C0->unk_5b4 = sizeof(_022577C0->unk_1b4);
        GF_ASSERT(WiFiPlazaLobbyClient_CheckResult(ov45_02232170(&_022577C0->unk_1b4, &_022577C0->unk_5b4)));
        ov45_02232304(-1, 0, ov45_02254EF0, 0);
        return;
    }
    client = _022577C0;
    if (client->channel == ov45_0222F7A8(libChannel)) {
        if (result == 1) {
            client->unk_12f = 1;
            ov45_0222FB84(_022577C0, _022577C0->channel);
            ov45_0222FA74(_022577C0, ov45_02232580(), 1);
        } else {
            client->unk_12f = 2;
        }
    } else {
        GF_AssertFail();
        ov45_0222EB94();
    }
}

void ov45_0222FCE0(int id, const WiFiPlazaLobbyPlayerRecord *record, const void *data, u32 size) {
    BOOL isOwn;
    WiFiPlazaLobbyClient *client;
    WiFiPlazaPlayer *player;

    if (id == ov45_02232580()) {
        ov45_022320C4(&_022577C0->time);
        isOwn = TRUE;
    } else {
        isOwn = FALSE;
    }
    ov45_0222FA74(_022577C0, id, 0);
    ov45_0222F898(_022577C0, id, data, size);
    ov45_0222F928(_022577C0, id, record);
    client = _022577C0;
    player = ov45_0222F8D8(client, id);
    _022577C0->callbacks.onPlayerEnter(id, player, client->callbackArg, isOwn);
}

void ov45_0222FD50(int id) {
    int i;

    if (ov45_0222FB24(_022577C0, id, 0) == -1) {
        return;
    }
    for (i = 0; i < 3; i++) {
        if (id == _022577C0->matches[i].hostId) {
            ov45_02230008(id, &_022577C0->matches[i].info);
        }
    }
    _022577C0->callbacks.onPlayerLeave(id, _022577C0->callbackArg);
    ov45_0222F978(_022577C0, id);
    ov45_0222F8FC(_022577C0, id);
    ov45_0222FAF8(_022577C0, id, 0);
    ov45_0222FAF8(_022577C0, id, 1);
}

void ov45_0222FDD4(void) {
}

#pragma force_active on

// A message of the given type from a player: calls the receive callback of the lobby (libChannel 0 / 5) or channel
// table.
void ov45_0222FDD8(int id, u32 libChannel, u32 type, const void *data, u32 size) {
    int table;

    if (ov45_0222FB24(_022577C0, id, 0) == -1) {
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
        if (_022577C0->channel != ov45_0222F7A8(libChannel)) {
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
void ov45_0222FE84(int a0, int a1, int a2, int a3, const char *key, const void *data) {
    if (strcmp(key, ov45_02254EF0) == 0) {
        memcpy(_022577C0->locations, data, sizeof(_022577C0->locations));
        if (_022577C0->unk_130) {
            _022577C0->unk_130 = 0;
        }
    }
}

#pragma force_active on

// A player's record changed: follows the player into or out of the own channel.
void ov45_0222FEC4(int id, const WiFiPlazaLobbyPlayerRecord *record) {
    if (ov45_0222FB24(_022577C0, id, 0) != -1) {
        if (_022577C0->channel != 4) {
            int current, old = ov45_0222F954(_022577C0, id)->channel;
            current = record->channel;
            {
                int own = ov45_0222F7B0(_022577C0->channel);
                if (old == 5 && current == own) {
                    ov45_0222FA74(_022577C0, id, 1);
                } else if (old == own && current != own) {
                    ov45_0222FAF8(_022577C0, id, 1);
                }
            }
        }
        ov45_0222F928(_022577C0, id, record);
    }
}

#pragma force_active reset

void ov45_0222FF40(int id, const void *data) {
    if (ov45_0222FB24(_022577C0, id, 0) != -1) {
        ov45_0222F88C(_022577C0, id, data);
        _022577C0->callbacks.onPlayerUpdate(id, (WiFiPlazaPlayer *)data, _022577C0->callbackArg);
    }
}

#pragma force_active on

static inline BOOL IsSameId(int id, int other) {
    return id == other;
}

// Match info from a player.
void ov45_0222FF7C(int id, WiFiPlazaLobbyMatchInfo *info) {
    s64 current;
    s64 received;
    int slot = ov45_0222FB24(_022577C0, id, 0);
    if (slot != -1) {
        if (IsSameId(WiFiPlazaLobbyClient_GetOwnId(), id)) {
        } else if (_022577C0->unk_69e && _022577C0->currentMatch == info->index) {
            ov45_02230384(&_022577C0->matches[_022577C0->currentMatch].info, &current);
            ov45_02230384(info, &received);
            if (current < received) {
                return;
            }
            _022577C0->unk_69f = 1;
            ov45_0222F154();
        }
        ov45_02230164(_022577C0, info->index, id, info);
    }
}

#pragma force_active reset

void ov45_02230008(int id, const WiFiPlazaLobbyMatchInfo *info) {
    int result;
    if (ov45_0222FB24(_022577C0, id, 0) == -1) {
        return;
    }
    result = ov45_022301E0(_022577C0, info->index, id);
    if (_022577C0->currentMatch == info->index && result == 1) {
        _022577C0->unk_69f = 1;
    }
}

void ov45_02230050(u32 event) {
    _022577C0->callbacks.onLobbyEvent(event, _022577C0->callbackArg);
}

void ov45_02230064(int result, const WiFiPlazaPlayer *data, u32 size) {
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
        ov45_0223040C(&_022577C0->unk_6a0, data, size);
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

void ov45_02230144(WiFiPlazaLobbyClient *client) {
    memset(client->matches, 0, sizeof(client->matches));
    client->currentMatch = 3;
}
