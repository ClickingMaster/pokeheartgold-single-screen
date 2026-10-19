#ifndef POKEHEARTGOLD_LOBBY_EVENT_DEQUE_H
#define POKEHEARTGOLD_LOBBY_EVENT_DEQUE_H

// The deque of lobby schedule events. The events are kept in blocks of 128; the deque's map is a ring
// buffer of block pointers that grows at both ends.

#include <nitro/types.h>

#define LOBBY_EVENT_BLOCK_SIZE 128

typedef struct LobbyEvent {
    s32 key;
    u32 payload;
} LobbyEvent;

typedef struct LobbyEventDequeMap {
    LobbyEvent **blocks;
    u32 start;
    u32 size;
    u32 capacity;
} LobbyEventDequeMap;

// A position in the map's ring: cur walks from start (mid) to the end of the array (last) and wraps to first.
typedef struct LobbyEventDequeMapIterator {
    LobbyEvent **cur;
    LobbyEvent **first;
    LobbyEvent **mid;
    LobbyEvent **last;
#ifdef __cplusplus
    LobbyEventDequeMapIterator() {
    }

    LobbyEventDequeMapIterator(LobbyEvent **c, LobbyEvent **f, LobbyEvent **m, LobbyEvent **l)
        : cur(c)
        , first(f)
        , mid(m)
        , last(l) {
    }

    LobbyEventDequeMapIterator(const LobbyEventDequeMapIterator &other)
        : cur(other.cur)
        , first(other.first)
        , mid(other.mid)
        , last(other.last) {
    }

    inline LobbyEventDequeMapIterator(struct LobbyEventDequeMap *map, u32 i);

    inline LobbyEventDequeMapIterator &operator+=(int n);

    LobbyEvent *const &operator*() const {
        return *cur;
    }

    LobbyEventDequeMapIterator &operator++() {
        if (++cur == last) {
            cur = first;
        }
        return *this;
    }
#endif
} LobbyEventDequeMapIterator;

#ifdef __cplusplus
extern "C" LobbyEventDequeMapIterator &LobbyEventDequeMapIterator_Advance(LobbyEventDequeMapIterator *, int);

inline LobbyEventDequeMapIterator &LobbyEventDequeMapIterator::operator+=(int n) {
    return LobbyEventDequeMapIterator_Advance(this, n);
}
#endif

#ifdef __cplusplus
inline LobbyEventDequeMapIterator::LobbyEventDequeMapIterator(LobbyEventDequeMap *map, u32 i)
    : cur(map->blocks + i)
    , first(map->blocks)
    , mid(first + map->start)
    , last(first + map->capacity) {
}
#endif

// The schedule's events in time order, before they are queued in the deque.
typedef struct LobbyEventVector {
    LobbyEvent *data;
    u32 size;
    u32 capacity;
} LobbyEventVector;

// The buffer the map grows through; allocator points at the capacity of the map it grows.
typedef struct LobbyEventDequeMapBuffer {
    LobbyEventDequeMap map;
    u32 *allocator;
} LobbyEventDequeMapBuffer;

// start is the index of the first event in the first block.
typedef struct LobbyEventDeque {
    u32 start;
    u32 size;
    LobbyEventDequeMap map;
} LobbyEventDeque;

typedef struct LobbyEventDequeIterator {
    LobbyEventDequeMapIterator block;
    LobbyEvent *cur;
#ifdef __cplusplus
    LobbyEventDequeIterator(const LobbyEventDequeIterator &other) {
        block.cur = other.block.cur;
        block.first = other.block.first;
        block.mid = other.block.mid;
        block.last = other.block.last;
        cur = other.cur;
    }

    LobbyEvent *SegmentEnd() const {
        return cur ? *block.cur + LOBBY_EVENT_BLOCK_SIZE : cur;
    }

    LobbyEventDequeIterator &operator++() {
        LobbyEvent **m = block.cur;
        if (++cur == *m + LOBBY_EVENT_BLOCK_SIZE) {
            if (++block.cur == block.last) {
                block.cur = block.first;
            }
            cur = *block.cur;
        }
        return *this;
    }

    LobbyEventDequeIterator &operator+=(int n) {
        cur += n - 1;
        return ++*this;
    }
#endif
} LobbyEventDequeIterator;

#ifdef __cplusplus
// A position in a vector of events, the source of the deque's range operations.
struct LobbyEventVectorIterator {
    LobbyEvent *p;

    LobbyEventVectorIterator() {
    }

    LobbyEventVectorIterator(LobbyEvent *q)
        : p(q) {
    }

    LobbyEvent *base() const {
        return p;
    }

    LobbyEventVectorIterator &operator+=(int n) {
        p += n;
        return *this;
    }

    bool operator!=(const LobbyEventVectorIterator &o) const {
        return p != o.p;
    }

    int operator-(const LobbyEventVectorIterator &o) const {
        return p - o.p;
    }

    LobbyEventVectorIterator operator+(int n) const {
        LobbyEventVectorIterator t(*this);
        t.p += n;
        return t;
    }
};
#endif

#endif // POKEHEARTGOLD_LOBBY_EVENT_DEQUE_H
