#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

typedef struct LobbyMap {
    int count;
    void *root;
    void *leftmost;
} LobbyMap;

void *LobbyUserMap_Destroy(LobbyMap *map);
void *LobbyRequestMap_Destroy(LobbyMap *map);
void LobbyUserMap_DestroyNodes(LobbyMap *map, void *node);
void LobbyRequestMap_DestroyNodes(LobbyMap *map, void *node);

void *LobbyUserMap_Destroy(LobbyMap *map) {
    if (map->root) {
        LobbyUserMap_DestroyNodes(map, map->root);
    }
    return map;
}

void *LobbyRequestMap_Destroy(LobbyMap *map) {
    if (map->root) {
        LobbyRequestMap_DestroyNodes(map, map->root);
    }
    return map;
}
