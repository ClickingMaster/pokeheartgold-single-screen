#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

#include "lobby_chat_internal.h"

int LobbyChat_RenameUser(LobbyChatManager *manager, const Str *oldNick, const Str *newNick) {
    Str temp;
    LobbyUserMapEntry pair;
    LobbyUserMapEntry entry;
    int id;
    LobbyUserMapInsertResult result;
    int ok;
    id = LobbyChat_FindUserIdByNick(manager, oldNick);
    LobbyUserMap_EraseKey(&manager->userMap, &id);
    LobbyString_InitCopy(&temp, newNick);
    LobbyUserMap_MakeEntry(&pair, id, &temp);
    {
        volatile u8 second;

        struct {
            volatile u8 x;
        } first = { 0 };

        second = 0;
        entry.userId = pair.userId;
        LobbyString_InitCopy(&entry.nick, &pair.nick);
        LobbyUserMap_Insert(&result, &manager->userMap, &entry);
        ok = result.inserted;
        LobbyString_Destroy(&entry.nick);
        LobbyString_Destroy(&pair.nick);
        LobbyString_Destroy(&temp);
        return ok ? id : -1;
    }
}
