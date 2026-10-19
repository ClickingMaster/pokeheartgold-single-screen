#include <nitro/types.h>

#include "lobby_event_deque.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

inline void *operator new(unsigned long, void *p) {
    return p;
}

struct Tag {};

extern "C" void LobbyEventDequeMapBuffer_Allocate(LobbyEventDequeMapBuffer *buffer, u32 n, Tag);
extern "C" void LobbyEventDequeMapBuffer_Destroy(LobbyEventDequeMapBuffer *buffer);
extern "C" void LobbyEventDequeMapBuffer_PrependRange(LobbyEventDequeMapBuffer *buffer, const LobbyEventDequeMapIterator &first, const LobbyEventDequeMapIterator &last, Tag);
extern "C" u32 LobbyEventDequeMap_RecommendSize(LobbyEventDequeMap *map, u32 n);

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
    buffer->map.blocks = 0;
    buffer->map.start = 0;
    buffer->map.size = 0;
    buffer->map.capacity = 0;
    buffer->allocator = allocator;
}

struct Alloc {
    void destroy(LobbyEvent **, u32) {
    }
};

inline void Allocate(LobbyEventDequeMapBuffer *buffer, u32 n, Tag t) {
    LobbyEventDequeMapBuffer_Allocate(buffer, n, t);
}

extern "C" void LobbyEventDequeMap_PushBack(LobbyEventDequeMap *map, LobbyEvent *const &block);

extern "C" void LobbyEventDequeMap_PushBack(LobbyEventDequeMap *map, LobbyEvent *const &block) {
    if (map->size + 1 < map->capacity) {
        LobbyEvent **p = map->blocks + (map->start + map->size) % map->capacity;
        new (p) LobbyEvent *(block);
        ++map->size;
        return;
    }
    LobbyEventDequeMapBuffer tmp;
    InitBuffer(&tmp, &map->capacity);
    u32 n = LobbyEventDequeMap_RecommendSize(map, 1);
    Allocate(&tmp, n, Tag(Tag()));
    tmp.map.start = map->size;
    new (tmp.map.blocks + tmp.map.start) LobbyEvent *(block);
    ++tmp.map.size;
    LobbyEventDequeMapBuffer_PrependRange(&tmp, Begin(map), End(map), Tag());
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
