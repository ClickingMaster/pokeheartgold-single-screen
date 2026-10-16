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

int LobbyChat_IsInChannel(char *channel) {
    Str channelName;
    Str fullName;
    int inChannel;
    if (!gLobbyChatManager) {
        return 0;
    }
    LobbyString_InitFromCStr(&channelName, channel);
    LobbyChat_GetFullChannelName(&fullName, gLobbyChatManager, &channelName);
    inChannel = chatInChannelA(gLobbyChatManager->connection, StrData(&fullName)) == 1;
    LobbyString_Destroy(&fullName);
    LobbyString_Destroy(&channelName);
    return inChannel;
}
