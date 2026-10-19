#ifndef POKEHEARTGOLD_LOBBY_TIMER_H
#define POKEHEARTGOLD_LOBBY_TIMER_H

// Lobby (overlay 45): the repeating timers of the chat session and the PPW lobby, kept in a map by id. The process
// functions run a timer's callback once its interval has passed since it last ran, and drop the timer when the
// callback returns FALSE.

#include <nitro/types.h>

typedef BOOL (*LobbyTimerCallback)(void *param);

typedef struct LobbyTimer {
    s64 lastTime;
    s64 interval;
    void *param;
    LobbyTimerCallback callback;
} LobbyTimer;

typedef struct LobbyTimerMapEntry {
    u32 id;
    LobbyTimer timer;
} LobbyTimerMapEntry;

typedef struct LobbyTimerMapNode {
    struct LobbyTimerMapNode *left;
    struct LobbyTimerMapNode *right;
    struct LobbyTimerMapNode *parent;
    u32 id;
    LobbyTimer timer;
} LobbyTimerMapNode;

// The map's own links double as its header node, whose address is the end of the map: root is the header's left
// link and leftmost (the first timer) its parent link; its right link is never used.
typedef struct LobbyTimerMap {
    u32 count;
    LobbyTimerMapNode *root;
    u8 padding_08[4];
    LobbyTimerMapNode *leftmost;
} LobbyTimerMap;

#ifdef __cplusplus
extern "C" {
#endif

void LobbyTimerMap_MakeEntry(LobbyTimerMapEntry *, int, LobbyTimer *);
void LobbyTimerMap_Find(LobbyTimerMapNode **, LobbyTimerMap *, u32 *);
LobbyTimerMapNode *LobbyTimerMap_LowerBound(LobbyTimerMap *, u32 *, LobbyTimerMapNode *, LobbyTimerMapNode *);
LobbyTimerMapNode *LobbyTimerMap_FindInsertPos(LobbyTimerMap *, const u32 *, LobbyTimerMapNode **, u8 *, u8 *);
void LobbyTimerMap_Clear(LobbyTimerMap *);
void *LobbyTimerMap_Destroy(LobbyTimerMap *);

#ifdef __cplusplus
}
#endif

#endif // POKEHEARTGOLD_LOBBY_TIMER_H
