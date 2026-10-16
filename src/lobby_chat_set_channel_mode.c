#include "global.h"

#include "dwc/net_common_internal.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

#include "lobby_chat_internal.h"

int LobbyChat_SetChannelMode(const char *channel, int mode) {
    Str channelName;
    Str fullName;
    LobbyChatManager *manager = gLobbyChatManager;
    CTX_CHECK(manager);
    LobbyString_InitFromCStr(&channelName, channel);
    LobbyChat_GetFullChannelName(&fullName, gLobbyChatManager, &channelName);
    chatSetChannelModeA(gLobbyChatManager->connection, StrData(&fullName), mode);
    LobbyString_Destroy(&fullName);
    LobbyString_Destroy(&channelName);
    return LOBBY_CHAT_RESULT_OK;
}
