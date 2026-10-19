#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
#include <stdarg.h>

#include "lobby_chat_internal.h"

extern char gLobbyStr_Empty_18[];

// The ID in a nick only counts while the user map still has that nick for it.
int LobbyChat_FindUserIdByNick(LobbyChatManager *manager, const Str *nick) {
    int id;
    LobbyUserMapNode *node;
    id = LobbyChat_GetUserIdFromNick(nick);
    if (id == -1) {
        return -1;
    }
    LobbyUserMap_Find((Iter *)&node, &manager->userMap, &id);
    if (node == (LobbyUserMapNode *)&manager->userMap.root) {
        return -1;
    }
    if ((BOOL)(LobbyString_Equals(&node->entry.nick, nick) == 0) != FALSE) {
        return -1;
    }
    return id;
}

void LobbyChat_GetUserNick(Str *out, LobbyChatManager *manager, int userId, ...) {
    LobbyUserMapNode *node;
    LobbyUserMap_Find((Iter *)&node, &manager->userMap, &userId);
    if (node == (LobbyUserMapNode *)&manager->userMap.root) {
        LobbyString_InitFromCStr(out, gLobbyStr_Empty_18);
    } else {
        LobbyString_InitCopy(out, &node->entry.nick);
    }
}
