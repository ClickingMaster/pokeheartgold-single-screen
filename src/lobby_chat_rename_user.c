#pragma force_active on
#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

#include "lobby_chat_internal.h"

int LobbyChat_RenameUser(LobbyChatManager *manager, const Str *oldNick, const Str *newNick) {
    Str temp;
    IntStrPair pair;
    IntStrPair entry;
    int id;
    InsertResult result;
    int ok;
    id = LobbyChat_FindUserIdByNick(manager, oldNick);
    LobbyUserMap_EraseKey((Tree *)manager->userMap, &id);
    LobbyString_InitCopy(&temp, newNick);
    LobbyUserMap_MakeEntry(&pair, id, &temp);
    {
        volatile u8 second;

        struct {
            volatile u8 x;
        } first = { 0 };

        second = 0;
        entry.key = pair.key;
        LobbyString_InitCopy(&entry.val, &pair.val);
        LobbyUserMap_Insert(&result, (Tree *)manager->userMap, &entry);
        ok = result.inserted;
        LobbyString_Destroy(&entry.val);
        LobbyString_Destroy(&pair.val);
        LobbyString_Destroy(&temp);
        return ok ? id : -1;
    }
}
