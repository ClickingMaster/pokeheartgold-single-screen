#pragma force_active on
#include "global.h"

#include "dwc/net_common_internal.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

#include "lobby_chat_internal.h"

void *LobbyChatManager_Destroy(LobbyChatManager *manager) {
    LobbyTimerMap_Clear(manager->timerMap);
    LobbyTimerMap_Destroy(manager->timerMap);
    LobbyRequestMap_DeleteAll((Tree *)&manager->requestMap);
    LobbyRequestMap_Destroy(&manager->requestMap);
    LobbyChannelMap_Destroy(manager->channelMap);
    LobbyUserMap_Destroy(manager->userMap);
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

void LobbyChat_StripChannelPrefixes(Vec *out, LobbyChatManager *manager, int n, u32 *arr) {
    int i;
    out->ptr = NULL;
    out->size = 0;
    out->cap = 0;
    for (i = 0; i < n; i++) {
        u32 v;
        v = arr[i] + StrSize(&manager->channelPrefix);
        TagTemp();
        if (out->size < out->cap) {
            out->size++;
            ((u32 *)out->ptr)[out->size - 1] = v;
        } else {
            Tag t1;
            t1 = (Tag) { 0 };
            LobbyU32Vector_PushBackSlow(out, &v, t1);
        }
    }
}

void LobbyChat_FormatNick(char *buf, int arg) {
    u32 t = current_time();
    LobbyString_FormatN(buf, 0x14, sFmt_N_UnderscoreIntUnderscoreUint, arg, t % 1000);
}

int LobbyChat_GetUserIdFromNick(Str *s) {
    Vec v;
    Str sep;
    int id;
    LobbyString_InitFromCStr(&sep, sStr___2);
    LobbyChat_SplitString(&v, s, &sep, 0);
    LobbyString_Destroy(&sep);
    if (v.size < 2) {
        LobbyStringVector_Destroy(&v);
        return -1;
    }
    id = atol(StrData((Str *)v.ptr + 1));
    if (id == 0) {
        LobbyStringVector_Destroy(&v);
        return -1;
    }
    LobbyStringVector_Destroy(&v);
    return id;
}
