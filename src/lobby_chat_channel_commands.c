#pragma force_active on
#include "global.h"

#include "dwc/net_common_internal.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

#include "lobby_chat_internal.h"

int LobbyRequest_GetId(u8 *a) {
    return *(int *)(a + 4);
}

int LobbyChat_LeaveChannel(const char *name) {
    Str s1;
    Str s2;
    Str s3;
    Str s4;
    LobbyChatManager *c = gLobbyChatManager;
    CTX_CHECK(c);
    LobbyString_InitFromCStr(&s1, name);
    LobbyChat_GetFullChannelName(&s2, gLobbyChatManager, &s1);
    chatLeaveChannelA(gLobbyChatManager->connection, StrData(&s2), 0);
    LobbyString_Destroy(&s2);
    LobbyString_Destroy(&s1);
    LobbyString_InitFromCStr(&s3, name);
    LobbyChat_GetFullChannelName(&s4, gLobbyChatManager, &s3);
    LobbyChat_RemoveChannel(gLobbyChatManager, &s4);
    LobbyString_Destroy(&s4);
    LobbyString_Destroy(&s3);
    return 0;
}

int LobbyChat_SendChannelMessage(const char *name, const char *msg) {
    Str s1;
    Str s2;
    LobbyChatManager *c = gLobbyChatManager;
    CTX_CHECK(c);
    if (strlen(msg) + 1 > 400) {
        return 3;
    }
    LobbyString_InitFromCStr(&s1, name);
    LobbyChat_GetFullChannelName(&s2, gLobbyChatManager, &s1);
    chatSendChannelMessageA(gLobbyChatManager->connection, StrData(&s2), msg, 3);
    LobbyString_Destroy(&s2);
    LobbyString_Destroy(&s1);
    return 0;
}

int LobbyChat_SendUserMessage(int a, const char *msg) {
    Str s;
    LobbyChatManager *c = gLobbyChatManager;
    CTX_CHECK(c);
    if (strlen(msg) + 1 > 400) {
        return 3;
    }
    LobbyChat_GetUserNick(&s, c, a);
    if (StrEqC(&s, sStr_Empty_17)) {
        LobbyString_Destroy(&s);
        return 3;
    }
    chatSendUserMessageA(gLobbyChatManager->connection, StrData(&s), msg, 3);
    LobbyString_Destroy(&s);
    return 0;
}

int LobbyChat_SetChannelKey(const char *name, int a, const char *key, const char *val) {
    Str s;
    Str tmp;
    Str s1;
    Str s2;
    LobbyChatManager *c = gLobbyChatManager;
    CTX_CHECK(c);
    if (strlen(val) + 1 > 400 || strlen(key) + 1 > 20) {
        return 3;
    }
    LobbyString_InitFromCStr(&s, sStr_Empty_17);
    if (a == -1) {
        LobbyString_AssignCStr(&s, sStr_Empty_17);
    } else {
        LobbyChat_GetUserNick(&tmp, gLobbyChatManager, a);
        LobbyString_Assign(&s, &tmp);
        LobbyString_Destroy(&tmp);
        if (StrEqC(&s, sStr_Empty_17)) {
            LobbyString_Destroy(&s);
            return 3;
        }
    }
    LobbyString_InitFromCStr(&s1, name);
    LobbyChat_GetFullChannelName(&s2, gLobbyChatManager, &s1);
    chatSetChannelKeysA(gLobbyChatManager->connection, StrData(&s2), StrData(&s), 1, &key, &val);
    LobbyString_Destroy(&s2);
    LobbyString_Destroy(&s1);
    LobbyString_Destroy(&s);
    return 0;
}
