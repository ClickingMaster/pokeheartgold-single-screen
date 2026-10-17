#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

#include "lobby_chat_internal.h"

// A key without a user is a channel key; it is reported with user ID -1.
void LobbyChat_OnBroadcastKeyChanged(void *chat, char *channel, char *nick, void *key, void *value) {
    Str nickStr;
    Str channelKey;
    Str fullName;
    Str channelName;
    int userId;
    LobbyChannelCallbacks *callbacks;
    if (gLobbyChatManager && gLobbyChatManager->state == LOBBY_CHAT_STATE_ERROR) {
        return;
    }
    if (!nick) {
        userId = -1;
    } else {
        LobbyString_InitFromCStr(&nickStr, nick);
        userId = LobbyChat_FindUserIdByNick(gLobbyChatManager, &nickStr);
        LobbyString_Destroy(&nickStr);
    }
    LobbyString_InitFromCStr(&channelKey, channel);
    callbacks = LobbyChat_GetChannelCallbacks(gLobbyChatManager, &channelKey);
    LobbyString_Destroy(&channelKey);
    LobbyString_InitFromCStr(&fullName, channel);
    LobbyChat_StripChannelPrefix(&channelName, gLobbyChatManager, &fullName);
    callbacks->onBroadcastKeyChanged(StrData(&channelName), userId, key, value, callbacks->param);
    LobbyString_Destroy(&channelName);
    LobbyString_Destroy(&fullName);
}
