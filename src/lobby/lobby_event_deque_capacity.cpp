#include <nitro/types.h>

#include "lobby_event_deque.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

inline void *operator new(unsigned long, void *p) {
    return p;
}

struct Tag {};

extern "C" {
void DWC_Free(int, void *, u32);
int printf(const char *, ...);
void abort(void);

extern u32 gLobbyEventDequeMapMaxSizeSet;
extern u32 gLobbyEventDequeMapMaxSize;
extern char gLobbyStr_CdequeLengthError[];

void LobbyEventDeque_EraseAtEnd(LobbyEventDeque *d, u32 n);
void LobbyEventDeque_EraseAtBegin(LobbyEventDeque *d, u32 n);
void LobbyEventDeque_LengthError(const char *s);
void LobbyEventDequeMap_Reserve(LobbyEventDequeMap *map, u32 n);
void LobbyEventDequeMap_PushBack(LobbyEventDequeMap *map, LobbyEvent *const &block);
u32 LobbyEventDequeMap_RecommendSize(LobbyEventDequeMap *map, u32 n);
void LobbyEventDequeMap_LengthError(void);
void LobbyEventDequeMapBuffer_Allocate(LobbyEventDequeMapBuffer *buffer, u32 n, Tag);
void LobbyEventDequeMapBuffer_AppendRange(LobbyEventDequeMapBuffer *buffer, const LobbyEventDequeMapIterator &first, const LobbyEventDequeMapIterator &last, Tag);
void LobbyEventDequeMapBuffer_PrependRange(LobbyEventDequeMapBuffer *buffer, const LobbyEventDequeMapIterator &first, const LobbyEventDequeMapIterator &last, Tag);
void LobbyEventDequeMapBuffer_Destroy(LobbyEventDequeMapBuffer *buffer);
}

struct BlockAlloc {
    void deallocate(LobbyEvent *p) {
        if (p) {
            DWC_Free(0, p, 0);
        }
    }
};

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

inline void Allocate(LobbyEventDequeMapBuffer *buffer, u32 n, Tag t) {
    LobbyEventDequeMapBuffer_Allocate(buffer, n, t);
}

struct Alloc {
    void destroy(LobbyEvent **, u32) {
    }
};

inline u32 MaxSize(void) {
    if (!(gLobbyEventDequeMapMaxSizeSet & 1)) {
        gLobbyEventDequeMapMaxSize = 0x3ffffffe;
        gLobbyEventDequeMapMaxSizeSet |= 1;
    }
    return gLobbyEventDequeMapMaxSize;
}

inline const u32 *Max(const u32 *a, const u32 *b) {
    return *a > *b ? a : b;
}

extern "C" void LobbyEventDeque_EraseAtEnd(LobbyEventDeque *d, u32 n) {
    if (n == 0) {
        return;
    }
    d->size -= n;
    u32 cap = d->map.size == 0 ? 0 : (d->map.size - 1) * LOBBY_EVENT_BLOCK_SIZE;
    u32 extra = cap - (d->start + d->size);
    if (extra < 2 * LOBBY_EVENT_BLOCK_SIZE) {
        return;
    }
    do {
        BlockAlloc a = BlockAlloc();
        --d->map.size;
        LobbyEvent *&slot = d->map.blocks[(d->map.start + d->map.size - 1) % d->map.capacity];
        a.deallocate(slot);
        extra -= LOBBY_EVENT_BLOCK_SIZE;
        slot = 0;
    } while (extra >= 2 * LOBBY_EVENT_BLOCK_SIZE);
}

extern "C" void LobbyEventDeque_EraseAtBegin(LobbyEventDeque *d, u32 n) {
    volatile u8 a;
    if (!n) {
        return;
    }
    d->size -= n;
    d->start += n;
    while (d->start >= 2 * LOBBY_EVENT_BLOCK_SIZE) {
        LobbyEvent *block = d->map.blocks[d->map.start];
        if (block) {
            DWC_Free(0, block, 0);
        }
        a = 0;
        {
            u32 x = *(volatile u32 *)&d->map.start + 1;
            d->map.start = x;
            if (x == d->map.capacity) {
                d->map.start = 0;
            }
        }
        d->map.size--;
        d->start -= LOBBY_EVENT_BLOCK_SIZE;
    }
    if (!d->size) {
        d->start = LOBBY_EVENT_BLOCK_SIZE / 2;
    }
}

extern "C" void LobbyEventDeque_LengthError(const char *s) {
    printf(s);
    abort();
}

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

extern "C" void LobbyEventDequeMap_PushBack(LobbyEventDequeMap *map, LobbyEvent *const &block) {
    if (map->size + 1 < map->capacity) {
        LobbyEvent **p = map->blocks + (map->start + map->size) % map->capacity;
        new (p) LobbyEvent *(block);
        ++map->size;
        return;
    }
    LobbyEventDequeMapBuffer tmp;
    tmp.map.blocks = 0;
    tmp.map.start = 0;
    tmp.map.size = 0;
    tmp.map.capacity = 0;
    tmp.allocator = &map->capacity;
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

extern "C" u32 LobbyEventDequeMap_RecommendSize(LobbyEventDequeMap *map, u32 extra) {
    u32 size, limit, first, second;
    limit = MaxSize();
    size = Capacity(map);
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

extern "C" void LobbyEventDequeMap_LengthError(void) {
    printf(gLobbyStr_CdequeLengthError);
    abort();
}
