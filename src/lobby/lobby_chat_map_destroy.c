#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

#include "lobby_chat_manager.h"

void *LobbyUserMap_Destroy(LobbyUserMap *map) {
    if (map->root) {
        LobbyUserMap_DestroyNodes(map, map->root);
    }
    return map;
}

void *LobbyRequestMap_Destroy(LobbyRequestMap *map) {
    if (map->root) {
        LobbyRequestMap_DestroyNodes(map, map->root);
    }
    return map;
}
