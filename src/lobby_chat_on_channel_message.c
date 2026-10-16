#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

#include "lobby_chat_internal.h"

void LobbyChat_OnInvited(void) {
    if (gLobbyChatManager && gLobbyChatManager->state == LOBBY_CHAT_STATE_ERROR) {
        return;
    }
}

void LobbyChat_OnChannelMessage(void *chat, char *channel, char *nick, void *message, int type) {
    Str nickStr;
    Str channelKey;
    Str fullName;
    Str channelName;
    int userId;
    LobbyChannelCallbacks *callbacks;
    if (gLobbyChatManager && gLobbyChatManager->state == LOBBY_CHAT_STATE_ERROR) {
        return;
    }
    if (type != 3) {
        return;
    }
    LobbyString_InitFromCStr(&nickStr, nick);
    userId = LobbyChat_FindUserIdByNick(gLobbyChatManager, &nickStr);
    LobbyString_Destroy(&nickStr);
    LobbyString_InitFromCStr(&channelKey, channel);
    callbacks = LobbyChat_GetChannelCallbacks(gLobbyChatManager, &channelKey);
    LobbyString_Destroy(&channelKey);
    LobbyString_InitFromCStr(&fullName, channel);
    LobbyChat_StripChannelPrefix(&channelName, gLobbyChatManager, &fullName);
    callbacks->onChannelMessage(StrData(&channelName), userId, message, callbacks->param);
    LobbyString_Destroy(&channelName);
    LobbyString_Destroy(&fullName);
}

void LobbyChat_OnKicked(void *chat, char *channel) {
    Str channelStr;
    if (gLobbyChatManager && gLobbyChatManager->state == LOBBY_CHAT_STATE_ERROR) {
        return;
    }
    LobbyString_InitFromCStr(&channelStr, channel);
    LobbyChat_RemoveChannel(gLobbyChatManager, &channelStr);
    LobbyString_Destroy(&channelStr);
}
