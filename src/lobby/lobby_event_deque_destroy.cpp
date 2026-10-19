#include <nitro/types.h>

#include "lobby_event_deque.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

extern "C" void DWC_Free(int, void *, u32);

extern "C" void LobbyEventDequeMap_Destroy(LobbyEventDequeMap *map);

struct Del {
    void operator()(void *p) const {
        if (p) {
            DWC_Free(0, p, 0);
        }
    }
};

inline u32 EndIndex(const LobbyEventDequeMap *map) {
    u32 e = 0;
    if (map->capacity != 0) {
        e = (map->start + map->size) % map->capacity;
    }
    return e;
}

extern "C" LobbyEventDeque *LobbyEventDeque_Destroy2(LobbyEventDeque *deque);

extern "C" LobbyEventDeque *LobbyEventDeque_Destroy2(LobbyEventDeque *deque) {
    Del d = Del();
    deque->size = 0;
    if (deque->map.size != 0) {
        u32 e = EndIndex(&deque->map);
        LobbyEvent **end;
        LobbyEvent **cur;
        LobbyEvent **base;
        LobbyEvent **lim;
        base = deque->map.blocks;
        end = base + e;
        lim = base + deque->map.capacity;
        cur = base + deque->map.start;
        while (cur != end) {
            d(*cur);
            if (++cur == lim) {
                cur = base;
            }
        }
    }
    LobbyEventDequeMap_Destroy(&deque->map);
    return deque;
}
