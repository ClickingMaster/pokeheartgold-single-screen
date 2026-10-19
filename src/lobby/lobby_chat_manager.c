#include "global.h"

#include "dwc/net_common_internal.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

#include "lobby_chat_internal.h"

void *LobbyChatManager_Destroy(LobbyChatManager *manager) {
    LobbyTimerMap_Clear(&manager->timerMap);
    LobbyTimerMap_Destroy(&manager->timerMap);
    LobbyRequestMap_DeleteAll(&manager->requestMap);
    LobbyRequestMap_Destroy(&manager->requestMap);
    LobbyChannelMap_Destroy(&manager->channelMap);
    LobbyUserMap_Destroy(&manager->userMap);
    LobbyString_Destroy(&manager->channelPrefix);
    LobbyString_Destroy(&manager->gameKey);
    LobbyString_Destroy(&manager->gameName);
    LobbyString_Destroy(&manager->realName);
    LobbyString_Destroy(&manager->user);
    LobbyString_Destroy(&manager->nick);
    return manager;
}

int LobbyChat_GetFullChannelName(Str *out, LobbyChatManager *manager, Str *name) {
    LobbyString_Concat(out, &manager->channelPrefix, name);
}

void LobbyChat_StripChannelPrefix(Str *out, LobbyChatManager *manager, Str *name) {
    LobbyString_Substr(out, name, StrSize(&manager->channelPrefix), -1);
}

void LobbyChat_StripChannelPrefixes(LobbyU32Vector *out, LobbyChatManager *manager, int n, u32 *arr) {
    int i;
    out->data = NULL;
    out->size = 0;
    out->capacity = 0;
    for (i = 0; i < n; i++) {
        u32 v;
        v = arr[i] + StrSize(&manager->channelPrefix);
        TagTemp();
        if (out->size < out->capacity) {
            out->size++;
            out->data[out->size - 1] = v;
        } else {
            LobbyTag t1;
            t1 = (LobbyTag) { 0 };
            LobbyU32Vector_PushBackSlow(out, &v, t1);
        }
    }
}

// Nicks are "N_<user ID>_<n>", n changing with the time so a retry gets a new nick.
void LobbyChat_FormatNick(char *buf, int userId) {
    u32 time = current_time();
    LobbyString_FormatN(buf, 0x14, sFmt_N_UnderscoreIntUnderscoreUint, userId, time % 1000);
}

int LobbyChat_GetUserIdFromNick(Str *nick) {
    LobbyStringVector parts;
    Str separator;
    int id;
    LobbyString_InitFromCStr(&separator, sStr___2);
    LobbyChat_SplitString(&parts, nick, &separator, 0);
    LobbyString_Destroy(&separator);
    if (parts.size < 2) {
        LobbyStringVector_Destroy(&parts);
        return -1;
    }
    id = atol(StrData(&parts.data[1]));
    if (id == 0) {
        LobbyStringVector_Destroy(&parts);
        return -1;
    }
    LobbyStringVector_Destroy(&parts);
    return id;
}
