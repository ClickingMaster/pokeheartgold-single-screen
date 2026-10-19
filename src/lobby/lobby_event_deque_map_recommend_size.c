#include "global.h"

#include "lobby_event_deque.h"
#include "ppw_lobby_internal.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

extern u32 gLobbyEventDequeMapMaxSizeSet;
extern u32 gLobbyEventDequeMapMaxSize;
extern char sStr_CdequeLengthError[];

u32 LobbyEventDequeMap_RecommendSize(LobbyEventDequeMap *, u32, ...);

static inline u32 Count(LobbyEventDequeMap *map) {
    return map->capacity == 0 ? 0 : map->capacity - 1;
}

static inline u32 Limit(void) {
    if (!(gLobbyEventDequeMapMaxSizeSet & 1)) {
        gLobbyEventDequeMapMaxSize = 0x3ffffffe;
        gLobbyEventDequeMapMaxSizeSet |= 1;
    }
    return gLobbyEventDequeMapMaxSize;
}

static inline const u32 *Max(const u32 *a, const u32 *b) {
    return *a > *b ? a : b;
}

u32 LobbyEventDequeMap_RecommendSize(LobbyEventDequeMap *map, u32 extra, ...) {
    u32 size, limit, first, second;
    limit = Limit();
    size = Count(map);
    if (extra > limit - size) {
        LobbyEventDequeMap_LengthError();
    }
    if (size < limit / 3) {
        first = (size + 1) * 3 / 5;
        return size + *Max(&extra, &first);
    } else if (size < limit / 3 * 2) {
        second = (size + 1) / 2;
        return size + *Max(&extra, &second);
    }
    return limit;
}

void LobbyEventDequeMap_LengthError(void) {
    printf(sStr_CdequeLengthError);
    abort();
}
