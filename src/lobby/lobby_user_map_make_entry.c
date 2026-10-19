#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

#include "lobby_chat_internal.h"

void LobbyUserMap_MakeEntry(LobbyUserMapEntry *entry, int userId, const Str *nick) {
    entry->userId = userId;
    LobbyString_InitCopy(&entry->nick, nick);
}

int LobbyChat_AreChannelUsersEnumerated(LobbyChatManager *manager, Str *key) {
    Iter n;
    LobbyChannelMap_Find(&n, &manager->channelMap, key);
    if (n.current == &manager->channelMap.root) {
        return 0;
    }
    return ((LobbyChannelMapNode *)n.current)->entry.channel.usersEnumerated;
}

int LobbyChat_SetChannelUsersEnumerated(LobbyChatManager *manager, Str *key) {
    Iter n;
    LobbyChannelMap_Find(&n, &manager->channelMap, key);
    if (n.current == &manager->channelMap.root) {
        return 0;
    }
    ((LobbyChannelMapNode *)n.current)->entry.channel.usersEnumerated = TRUE;
    return 1;
}

int LobbyChat_GetChannelUserLimit(LobbyChatManager *manager, Str *key) {
    Iter n;
    LobbyChannelMap_Find(&n, &manager->channelMap, key);
    if (n.current == &manager->channelMap.root) {
        return -1;
    }
    return ((LobbyChannelMapNode *)n.current)->entry.channel.userLimit;
}
