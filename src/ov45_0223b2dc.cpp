#include <nitro/types.h>

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

struct Tag {};

struct Guard {
    ~Guard() {
    }
};

typedef u32 *P;
struct Ring;

struct CIt {
    P *cur, *first, *mid, *last;

    CIt(P *c, P *f, P *m, P *l)
        : cur(c)
        , first(f)
        , mid(m)
        , last(l) {
    }

    inline CIt(const Ring *r, u32 i);
};
struct It;
extern "C" void LobbyEventDequeMapIterator_Advance(It *it, int n);

struct It {
    P *cur, *first, *mid, *last;

    It(P *c, P *f, P *m, P *l)
        : cur(c)
        , first(f)
        , mid(m)
        , last(l) {
    }

    It(const It &o)
        : cur(o.cur)
        , first(o.first)
        , mid(o.mid)
        , last(o.last) {
    }

    It &operator+=(int n);

    It &operator--() {
        return *this += -1;
    }
};

inline It &It::operator+=(int n) {
    LobbyEventDequeMapIterator_Advance(this, n);
    return *this;
}

extern "C" void LobbyEventDequeMap_PushBack(Ring *r, const P &v);
extern "C" void LobbyEventDequeMap_Reserve(Ring *r, u32 n);
extern "C" void LobbyEventDequeMap_InitCopy(Ring *r, CIt first, const CIt &last, Tag);
extern "C" void ov45_02238508(Ring *r);

struct Empty {};

struct SB {
    P *base;
    Ring *r;
};

struct WIt {
    It it;
    Empty e;

    WIt(const It &i)
        : it(i) {
    }

    inline WIt(const SB &s);

    WIt(const WIt &o)
        : it(o.it) {
    }
};

extern "C" void LobbyEventDequeMap_InsertRange(Ring *r, It pos, WIt first, WIt last, Tag);
extern "C" void *LobbyMem_Alloc(u32 n);
extern "C" void LobbyEventDeque_LengthError(char *msg);
extern char sStr_DequeLengthError[];
extern u32 ov45_022577C8;
extern u32 ov45_022551A8;

struct Alloc {
    void destroy(P *) {
    }
};

struct RingAlloc {
    ~RingAlloc();
};

struct Ring : RingAlloc {
    P *ptr;
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
        CIt e = o.end();
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

    CIt begin() const {
        return CIt(ptr + start, ptr, ptr + start, ptr + cap);
    }

    CIt end() const {
        return CIt(this, endIndex());
    }

    It begin() {
        It t(0, 0, 0, 0);
        t.first = ptr;
        t.cur = t.mid = t.first + start;
        t.last = t.first + cap;
        return t;
    }

    It end() {
        u32 i = endIndex();
        P *m = ptr + start;
        return It(ptr + i, ptr, m, ptr + cap);
    }

    P &front() {
        return ptr[start];
    }

    P &back() {
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

    void insert(const It &pos, const WIt &first, const WIt &last) {
        ix1(pos, first, last);
    }

    void ix1(const It &pos, const WIt &first, const WIt &last) {
        ix2(pos, first, last, Tag(), Tag());
    }

    void ix2(const It &pos, const WIt &first, const WIt &last, Tag, Tag) {
        LobbyEventDequeMap_InsertRange(this, pos, first, last, Tag());
    }

    void push_back(const P &v) {
        LobbyEventDequeMap_PushBack(this, v);
    }
};

inline CIt::CIt(const Ring *r, u32 i)
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
    if (!(ov45_022577C8 & 1)) {
        ov45_022551A8 = ~0xFF;
        ov45_022577C8 |= 1;
    }
    return ov45_022551A8;
}

inline void CopyBlocks(Ring *m, Ring *src) {
    if (src->size != 0) {
        It e = src->end();
        --e;
        SB b;
        b.base = src->ptr;
        b.r = src;
        It p = m->begin();
        m->insert(p, WIt(b), e);
    }
}

inline void CopyBlocks2(Ring *m, Ring *src) {
    CopyBlocks(m, src);
}

extern "C" void ov45_0223CA14(Deque *d);
extern "C" void LobbyEventDeque_AddBackCapacity(Deque *d, u32 n);

inline WIt::WIt(const SB &s)
    : it(0, 0, 0, 0) {
    P *c = s.base + s.r->start;
    it.cur = c;
    it.first = s.base;
    it.mid = c;
    it.last = s.base + s.r->cap;
}

extern "C" void LobbyEventDeque_AddBackCapacity(Deque *d, u32 n) {
    u32 nb = n / 0x80 + (n % 0x80 != 0);
    for (; nb != 0 && d->start >= 0x80; --nb) {
        P p = d->map.front();
        d->map.pop_front();
        d->map.back() = p;
        d->map.push_back(0);
        d->start -= 0x80;
    }
    if (nb == 0) {
        return;
    }
    if (nb * 0x80 > max_size() || (d->map.size == 0 ? 0 : (d->map.size - 1) * 0x80) > max_size() - nb * 0x80) {
        LobbyEventDeque_LengthError(sStr_DequeLengthError);
    }
    if (nb + d->map.size <= d->map.capacity()) {
        for (; nb != 0; --nb) {
            P p = (P)LobbyMem_Alloc(0x400);
            d->map.back() = p;
            d->map.push_back(0);
        }
        return;
    }
    Deque tmp;
    Ring *m = &tmp.map;
    LobbyEventDequeMap_Reserve(m, Max(d->map.capacity() * 2, nb + d->map.size + 1));
    m->push_back(0);
    for (; nb != 0; --nb) {
        P p = (P)LobbyMem_Alloc(0x400);
        m->back() = p;
        m->push_back(0);
    }
    CopyBlocks2(m, &d->map);
    P *t0 = d->map.ptr;
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
