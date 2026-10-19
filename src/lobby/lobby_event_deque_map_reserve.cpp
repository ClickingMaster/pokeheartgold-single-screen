#include <nitro/types.h>

#include "lobby_event_deque.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

struct Tag {};

extern "C" void LobbyEventDequeMapBuffer_Allocate(LobbyEventDequeMapBuffer *buffer, u32 n, Tag);
extern "C" void LobbyEventDequeMapBuffer_AppendRange(LobbyEventDequeMapBuffer *buffer, const LobbyEventDequeMapIterator &first, const LobbyEventDequeMapIterator &last, Tag);
extern "C" void LobbyEventDequeMapBuffer_Destroy(LobbyEventDequeMapBuffer *buffer);

inline void Swap(u32 &a, u32 &b) {
    u32 t = a;
    a = b;
    b = t;
}

inline void Swap(LobbyEvent **&a, LobbyEvent **&b) {
    LobbyEvent **t = a;
    a = b;
    b = t;
}

inline u32 Capacity(const LobbyEventDequeMap *map) {
    return map->capacity == 0 ? 0 : map->capacity - 1;
}

inline u32 EndIndex(const LobbyEventDequeMap *map) {
    if (map->capacity == 0) {
        return 0;
    }
    return (map->start + map->size) % map->capacity;
}

inline LobbyEventDequeMapIterator Begin(LobbyEventDequeMap *map) {
    return LobbyEventDequeMapIterator(map, map->start);
}

inline LobbyEventDequeMapIterator End(LobbyEventDequeMap *map) {
    return LobbyEventDequeMapIterator(map, EndIndex(map));
}

inline void InitBuffer(LobbyEventDequeMapBuffer *buffer, u32 *allocator) {
    buffer->allocator = allocator;
    buffer->map.blocks = 0;
    buffer->map.start = 0;
    buffer->map.size = 0;
    buffer->map.capacity = 0;
}

struct Alloc {
    void destroy(LobbyEvent **, u32) {
    }
};

extern "C" void LobbyEventDequeMap_Reserve(LobbyEventDequeMap *map, u32 n);

extern "C" void LobbyEventDequeMap_Reserve(LobbyEventDequeMap *map, u32 n) {
    if (n <= Capacity(map)) {
        return;
    }
    LobbyEventDequeMapBuffer tmp;
    InitBuffer(&tmp, &map->capacity);
    LobbyEventDequeMapBuffer_Allocate(&tmp, n, Tag());
    LobbyEventDequeMapBuffer_AppendRange(&tmp, Begin(map), End(map), Tag());
    Alloc a = Alloc();
    a.destroy(map->blocks, map->size);
    map->size = 0;
    u32 &c = tmp.map.capacity;
    Swap(map->blocks, tmp.map.blocks);
    Swap(map->start, tmp.map.start);
    Swap(map->size, tmp.map.size);
    Swap(map->capacity, c);
    LobbyEventDequeMapBuffer_Destroy(&tmp);
}
