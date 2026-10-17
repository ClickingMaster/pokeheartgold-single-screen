#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

#include "lobby_chat_internal.h"

void LobbyChat_OnUserParted(int chat, char *channel, char *nick) {
    Str nickStr;
    Str channelKey;
    Str fullName;
    Str channelName;
    Str userNick;
    Str userChannel;
    int userId;
    LobbyChannelCallbacks *callbacks;
    if (gLobbyChatManager && gLobbyChatManager->state == LOBBY_CHAT_STATE_ERROR) {
        return;
    }
    LobbyString_InitFromCStr(&nickStr, nick);
    userId = LobbyChat_FindUserIdByNick(gLobbyChatManager, &nickStr);
    LobbyString_Destroy(&nickStr);
    if (userId == -1) {
        return;
    }
    LobbyString_InitFromCStr(&channelKey, channel);
    callbacks = LobbyChat_GetChannelCallbacks(gLobbyChatManager, &channelKey);
    LobbyString_Destroy(&channelKey);
    LobbyString_InitFromCStr(&fullName, channel);
    LobbyChat_StripChannelPrefix(&channelName, gLobbyChatManager, &fullName);
    callbacks->onUserParted(StrData(&channelName), userId, callbacks->param);
    LobbyString_Destroy(&channelName);
    LobbyString_Destroy(&fullName);
    LobbyString_InitFromCStr(&userNick, nick);
    LobbyString_InitFromCStr(&userChannel, channel);
    LobbyChat_RemoveChannelUser(gLobbyChatManager, &userNick, &userChannel);
    LobbyString_Destroy(&userChannel);
    LobbyString_Destroy(&userNick);
}
