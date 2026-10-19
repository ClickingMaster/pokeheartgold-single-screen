#include <nitro/types.h>

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

struct Tag {};
struct Map;

struct LobbyEvent;

// A block pointer in the map's ring.
struct Block {
    LobbyEvent *events;
};

struct MapIterator {
    Block *cur, *first, *mid, *last;

    MapIterator(Block *c, Block *f, Block *m, Block *l)
        : cur(c)
        , first(f)
        , mid(m)
        , last(l) {
    }

    MapIterator(const MapIterator &o)
        : cur(o.cur)
        , first(o.first)
        , mid(o.mid)
        , last(o.last) {
    }

    MapIterator(Block *c, const Map &map);
    MapIterator &operator+=(int n);

    MapIterator operator+(int n) const {
        MapIterator t(*this);
        return t += n;
    }

    MapIterator operator-(int n) const {
        MapIterator t(*this);
        return t += -n;
    }

    const Block &operator*() const {
        return *cur;
    }

    MapIterator &operator++() {
        if (++cur == last) {
            cur = first;
        }
        return *this;
    }
};

extern "C" MapIterator &LobbyEventDequeMapIterator_Advance(MapIterator *it, int n);

inline MapIterator &MapIterator::operator+=(int n) {
    return LobbyEventDequeMapIterator_Advance(this, n);
}

struct Empty {};

struct SourceIterator {
    MapIterator it;
    Empty e;

    SourceIterator(const MapIterator &i)
        : it(i) {
    }
};

extern "C" int LobbyEventDequeMapIterator_Distance(const SourceIterator &a, const SourceIterator &b);

inline int MapDistance(SourceIterator first, SourceIterator last) {
    return LobbyEventDequeMapIterator_Distance(last, first);
}

inline int distance(SourceIterator first, SourceIterator last) {
    return MapDistance(first, last);
}

extern "C" MapIterator LobbyEventDequeMap_Copy(MapIterator first, MapIterator last, MapIterator dest);
extern "C" MapIterator LobbyEventDequeMap_CopyBackward(MapIterator first, MapIterator last, MapIterator dest);
extern "C" MapIterator LobbyEventDequeMap_CopyRange(SourceIterator first, SourceIterator last, MapIterator dest);

struct Alloc {
    void construct(Block *p, Block v) {
        if (p) {
            *p = v;
        }
    }

    void destroy(Block *) {
    }
};

template <class T>
inline void swap(T &a, T &b) {
    T t = a;
    a = b;
    b = t;
}

struct MapBuffer {
    Block *ptr;
    u32 start, size, cap;
    u32 *owner;
};

extern "C" u32 LobbyEventDequeMap_RecommendSize(Map *map, u32 n);
extern "C" void LobbyEventDequeMapBuffer_Allocate(MapBuffer *buffer, u32 n, Alloc);
extern "C" void LobbyEventDequeMapBuffer_AppendRange(MapBuffer *buffer, const MapIterator &first, const MapIterator &last, Alloc);
extern "C" void LobbyEventDequeMapBuffer_PrependRange(MapBuffer *buffer, const MapIterator &first, const MapIterator &last, Alloc);
extern "C" void LobbyEventDequeMapBuffer_Destroy(MapBuffer *buffer);

struct Map {
    Block *ptr;
    u32 start, size, cap;

    void clear() {
        Alloc a = Alloc();
        a.destroy(ptr);
        size = 0;
    }

    u32 capacity() const {
        return cap == 0 ? 0 : cap - 1;
    }

    u32 endIndex() const {
        const Map *self = this;
        if (self->cap == 0) {
            return 0;
        }
        return (self->start + self->size) % self->cap;
    }

    MapIterator begin();
    MapIterator end();
};

inline MapIterator::MapIterator(Block *c, const Map &map)
    : cur(c)
    , first(map.ptr)
    , mid(first + map.start)
    , last(first + map.cap) {
}

inline MapIterator Map::begin() {
    return MapIterator(ptr + start, *this);
}

inline MapIterator Map::end() {
    return MapIterator(ptr + endIndex(), *this);
}

extern "C" void LobbyEventDequeMap_InsertRange(Map *map, MapIterator pos, SourceIterator first, SourceIterator last, Tag);

extern "C" void LobbyEventDequeMap_InsertRange(Map *map, MapIterator pos, SourceIterator first, SourceIterator last, Tag) {
    u32 n = distance(first, last);
    if (n == 0) {
        return;
    }
    u32 k = pos.cur - (map->ptr + map->start);
    if (pos.cur < map->ptr + map->start) {
        k += map->cap;
    }
    u32 c = map->capacity();
    if (n <= c && map->size <= c - n) {
        MapIterator b = map->begin();
        MapIterator old = b;
        u32 after = map->size - k;
        if (k < after) {
            b += -n;
            map->start = b.cur - map->ptr;
            map->size += n;
            if (k != 0) {
                b = LobbyEventDequeMap_Copy(old, old + k, b);
            }
        } else {
            map->size += n;
            b = map->end();
            if (after != 0) {
                b = LobbyEventDequeMap_CopyBackward(b - (n + after), b - n, b);
            }
            b += -n;
        }
        LobbyEventDequeMap_CopyRange(first, last, b);
        return;
    }
    MapBuffer buffer;
    buffer.ptr = 0;
    buffer.start = 0;
    buffer.size = 0;
    buffer.cap = 0;
    buffer.owner = &map->cap;
    u32 newCapacity = LobbyEventDequeMap_RecommendSize(map, n);
    LobbyEventDequeMapBuffer_Allocate(&buffer, newCapacity, Alloc(Alloc()));
    buffer.start = k;
    Block *p = buffer.ptr + k;
    if (n != 0) {
        do {
            const Block &v = *first.it;
            Alloc a = Alloc();
            --n;
            a.construct(p, v);
            ++p;
            buffer.size++;
            ++first.it;
        } while (n != 0);
    }
    LobbyEventDequeMapBuffer_AppendRange(&buffer, pos, map->end(), Alloc());
    LobbyEventDequeMapBuffer_PrependRange(&buffer, map->begin(), pos, Alloc());
    map->clear();
    swap(map->ptr, buffer.ptr);
    swap(map->start, buffer.start);
    swap(map->size, buffer.size);
    u32 &bufferCapacity = buffer.cap;
    swap(map->cap, bufferCapacity);
    LobbyEventDequeMapBuffer_Destroy(&buffer);
}
