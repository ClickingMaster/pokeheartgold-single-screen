#include <nitro/types.h>

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

#include "lobby_mem.h"

struct Tag {};

struct Guard {
    ~Guard() {
    }
};

struct LobbyEvent;

typedef LobbyEvent *Block;
struct Ring;

struct ConstMapIterator {
    Block *cur, *first, *mid, *last;

    ConstMapIterator(Block *c, Block *f, Block *m, Block *l)
        : cur(c)
        , first(f)
        , mid(m)
        , last(l) {
    }

    inline ConstMapIterator(const Ring *r, u32 i);
};
struct MapIterator;
extern "C" void LobbyEventDequeMapIterator_Advance(MapIterator *it, int n);

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

    MapIterator &operator+=(int n);

    MapIterator &operator--() {
        return *this += -1;
    }
};

inline MapIterator &MapIterator::operator+=(int n) {
    LobbyEventDequeMapIterator_Advance(this, n);
    return *this;
}

extern "C" void LobbyEventDequeMap_PushBack(Ring *r, const Block &v);
extern "C" void LobbyEventDequeMap_Reserve(Ring *r, u32 n);
extern "C" void LobbyEventDequeMap_InitCopy(Ring *r, ConstMapIterator first, const ConstMapIterator &last, Tag);
extern "C" void LobbyEventDequeMap_Destroy(Ring *r);

struct Empty {};

// The start of a map's ring: the iterator over its blocks is made from it.
struct MapStart {
    Block *blocks;
    Ring *map;
};

struct SourceIterator {
    MapIterator it;
    Empty e;

    SourceIterator(const MapIterator &i)
        : it(i) {
    }

    inline SourceIterator(const MapStart &start);

    SourceIterator(const SourceIterator &o)
        : it(o.it) {
    }
};

extern "C" void LobbyEventDequeMap_InsertRange(Ring *r, MapIterator pos, SourceIterator first, SourceIterator last, Tag);
extern "C" void LobbyEventDeque_LengthError(char *msg);
extern char gLobbyStr_DequeLengthError[];
extern u32 gLobbyDequeMaxSizeInit;
extern u32 gLobbyDequeMaxSize;

struct Alloc {
    void destroy(Block *) {
    }
};

struct RingAlloc {
    ~RingAlloc();
};

struct Ring : RingAlloc {
    Block *ptr;
    u32 start, size, cap;

    Ring()
        : ptr(0)
        , start(0)
        , size(0)
        , cap(0) {
    }

    Ring(const Ring &o)
        : ptr(0)
        , start(0)
        , size(0)
        , cap(0) {
        init(o);
    }

    void init(const Ring &o) {
        Guard g1;
        Guard g2;
        Tag tg;
        ConstMapIterator e = o.end();
        LobbyEventDequeMap_InitCopy(this, o.begin(), e, tg);
    }

    ~Ring();

    void reset() {
        ptr = 0;
        start = 0;
        size = 0;
        cap = 0;
    }

    u32 capacity() const {
        return cap == 0 ? 0 : cap - 1;
    }

    u32 endIndex() const {
        if (cap == 0) {
            return 0;
        }
        return (start + size) % cap;
    }

    ConstMapIterator begin() const {
        return ConstMapIterator(ptr + start, ptr, ptr + start, ptr + cap);
    }

    ConstMapIterator end() const {
        return ConstMapIterator(this, endIndex());
    }

    MapIterator begin() {
        MapIterator t(0, 0, 0, 0);
        t.first = ptr;
        t.cur = t.mid = t.first + start;
        t.last = t.first + cap;
        return t;
    }

    MapIterator end() {
        u32 i = endIndex();
        Block *m = ptr + start;
        return MapIterator(ptr + i, ptr, m, ptr + cap);
    }

    Block &front() {
        return ptr[start];
    }

    Block &back() {
        return ptr[(start + size - 1) % cap];
    }

    void pop_front() {
        Alloc a = Alloc();
        a.destroy(&ptr[start]);
        if (++start == cap) {
            start = 0;
        }
        --size;
    }

    void erase_end(u32 n) {
        Alloc a = Alloc();
        a.destroy(ptr);
        size -= n;
    }

    void clear() {
        erase_end(size);
    }

    void insert(const MapIterator &pos, const SourceIterator &first, const SourceIterator &last) {
        insert_range(pos, first, last);
    }

    void insert_range(const MapIterator &pos, const SourceIterator &first, const SourceIterator &last) {
        insert_range(pos, first, last, Tag(), Tag());
    }

    void insert_range(const MapIterator &pos, const SourceIterator &first, const SourceIterator &last, Tag, Tag) {
        LobbyEventDequeMap_InsertRange(this, pos, first, last, Tag());
    }

    void push_back(const Block &v) {
        LobbyEventDequeMap_PushBack(this, v);
    }
};

inline ConstMapIterator::ConstMapIterator(const Ring *r, u32 i)
    : cur(r->ptr + i)
    , first(r->ptr)
    , mid(first + r->start)
    , last(first + r->cap) {
}

inline const u32 &Max(const u32 &a, const u32 &b) {
    return a < b ? b : a;
}

struct Deque {
    u32 start, size;
    Ring map;

    Deque()
        : start(0)
        , size(0)
        , map(Ring()) {
    }

    ~Deque();
};

inline u32 max_size() {
    if (!(gLobbyDequeMaxSizeInit & 1)) {
        gLobbyDequeMaxSize = ~0xFF;
        gLobbyDequeMaxSizeInit |= 1;
    }
    return gLobbyDequeMaxSize;
}

inline void InsertBlocks(Ring *m, Ring *src) {
    if (src->size != 0) {
        MapIterator e = src->end();
        --e;
        MapStart start;
        start.blocks = src->ptr;
        start.map = src;
        MapIterator p = m->begin();
        m->insert(p, SourceIterator(start), e);
    }
}

inline void CopyBlocks(Ring *m, Ring *src) {
    InsertBlocks(m, src);
}

extern "C" void LobbyEventDeque_Destroy2(Deque *d);
extern "C" void LobbyEventDeque_AddBackCapacity(Deque *d, u32 n);

inline SourceIterator::SourceIterator(const MapStart &start)
    : it(0, 0, 0, 0) {
    Block *c = start.blocks + start.map->start;
    it.cur = c;
    it.first = start.blocks;
    it.mid = c;
    it.last = start.blocks + start.map->cap;
}

extern "C" void LobbyEventDeque_AddBackCapacity(Deque *d, u32 n) {
    u32 blockCount = n / 0x80 + (n % 0x80 != 0);
    for (; blockCount != 0 && d->start >= 0x80; --blockCount) {
        Block p = d->map.front();
        d->map.pop_front();
        d->map.back() = p;
        d->map.push_back(0);
        d->start -= 0x80;
    }
    if (blockCount == 0) {
        return;
    }
    if (blockCount * 0x80 > max_size() || (d->map.size == 0 ? 0 : (d->map.size - 1) * 0x80) > max_size() - blockCount * 0x80) {
        LobbyEventDeque_LengthError(gLobbyStr_DequeLengthError);
    }
    if (blockCount + d->map.size <= d->map.capacity()) {
        for (; blockCount != 0; --blockCount) {
            Block p = (Block)LobbyMem_Alloc(0x400);
            d->map.back() = p;
            d->map.push_back(0);
        }
        return;
    }
    Deque tmp;
    Ring *m = &tmp.map;
    LobbyEventDequeMap_Reserve(m, Max(d->map.capacity() * 2, blockCount + d->map.size + 1));
    m->push_back(0);
    for (; blockCount != 0; --blockCount) {
        Block p = (Block)LobbyMem_Alloc(0x400);
        m->back() = p;
        m->push_back(0);
    }
    CopyBlocks(m, &d->map);
    Block *t0 = d->map.ptr;
    d->map.ptr = m->ptr;
    m->ptr = t0;
    u32 t1 = d->map.start;
    d->map.start = m->start;
    m->start = t1;
    u32 t2 = d->map.size;
    d->map.size = m->size;
    m->size = t2;
    u32 t3 = d->map.cap;
    d->map.cap = m->cap;
    m->cap = t3;
    m->clear();
}
