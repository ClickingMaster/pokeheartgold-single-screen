#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

#include "lobby_chat_manager.h"

int LobbyChannelMap_EraseKey(LobbyChannelMap *map, Str *key) {
    Iterator it, out;
    LobbyChannelMap_Find(&it, map, key);
    if (it.current == &map->root) {
        return 0;
    }
    LobbyChannelMap_Erase(&out, map, (Iterator) { it.current });
    return 1;
}
