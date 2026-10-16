#pragma force_active on
#include "global.h"

#include "dwc/net_common_internal.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

#include "lobby_chat_internal.h"

int LobbyChat_SetChannelMode(const char *name, int arg) {
    Str s1;
    Str s2;
    LobbyChatManager *c = gLobbyChatManager;
    if (c == NULL) {
        return 4;
    }
    if (c != NULL && c->state == 5) {
        return 7;
    }
    if (c != NULL && CtxState() != 3) {
        return 4;
    }
    LobbyString_InitFromCStr(&s1, name);
    LobbyChat_GetFullChannelName(&s2, gLobbyChatManager, &s1);
    chatSetChannelModeA(gLobbyChatManager->connection, StrData(&s2), arg);
    LobbyString_Destroy(&s2);
    LobbyString_Destroy(&s1);
    return 0;
}
