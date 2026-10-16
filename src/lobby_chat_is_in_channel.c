#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

#include "lobby_chat_internal.h"

int LobbyChat_GetLastError(void) {
    return gLobbyChatManager ? gLobbyChatManager->lastError : 0;
}

void *LobbyChat_ToErrorCode(void *p) {
    return (u8 *)p + 0xa410;
}

int LobbyChat_IsInChannel(char *s) {
    Str x, y;
    int r;
    if (!gLobbyChatManager) {
        return 0;
    }
    LobbyString_InitFromCStr(&x, s);
    LobbyChat_GetFullChannelName(&y, gLobbyChatManager, &x);
    r = chatInChannelA(gLobbyChatManager->connection, StrData(&y)) == 1;
    LobbyString_Destroy(&y);
    LobbyString_Destroy(&x);
    return r;
}
