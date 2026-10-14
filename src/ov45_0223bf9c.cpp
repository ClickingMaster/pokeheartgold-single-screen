#pragma force_active on
#include <nitro/types.h>

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

struct Tag {};
struct Ring;

struct Elem {
    u32 *x;
};

typedef Elem P;

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

    It(P *c, const Ring &r);
    It &operator+=(int n);

    It operator+(int n) const {
        It t(*this);
        return t += n;
    }

    It operator-(int n) const {
        It t(*this);
        return t += -n;
    }

    const P &operator*() const {
        return *cur;
    }

    It &operator++() {
        if (++cur == last) {
            cur = first;
        }
        return *this;
    }
};

extern "C" It &LobbyEventDequeMapIterator_Advance(It *it, int n);

inline It &It::operator+=(int n) {
    return LobbyEventDequeMapIterator_Advance(this, n);
}

struct Empty {};

struct WIt {
    It it;
    Empty e;

    WIt(const It &i)
        : it(i) {
    }
};

extern "C" int LobbyEventDequeMapIterator_Distance(const WIt &a, const WIt &b);

inline int dist2(WIt first, WIt last) {
    return LobbyEventDequeMapIterator_Distance(last, first);
}

inline int distance(WIt first, WIt last) {
    return dist2(first, last);
}

extern "C" It LobbyEventDequeMap_Copy(It first, It last, It dest);
extern "C" It LobbyEventDequeMap_CopyBackward(It first, It last, It dest);
extern "C" It LobbyEventDequeMap_CopyRange(WIt first, WIt last, It dest);

struct Alloc {
    void construct(P *p, P v) {
        if (p) {
            *p = v;
        }
    }

    void destroy(P *) {
    }
};

template <class T>
inline void swap(T &a, T &b) {
    T t = a;
    a = b;
    b = t;
}

struct Tmp {
    P *ptr;
    u32 start, size, cap;
    u32 *owner;
};

extern "C" u32 LobbyEventDequeMap_RecommendSize(Ring *r, u32 n);
extern "C" void LobbyEventDequeMapBuffer_Allocate(Tmp *t, u32 n, Alloc);
extern "C" void LobbyEventDequeMapBuffer_AppendRange(Tmp *t, const It &first, const It &last, Alloc);
extern "C" void LobbyEventDequeMapBuffer_PrependRange(Tmp *t, const It &first, const It &last, Alloc);
extern "C" void LobbyEventDequeMapBuffer_Destroy(Tmp *t);

struct Ring {
    P *ptr;
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
        const Ring *self = this;
        if (self->cap == 0) {
            return 0;
        }
        return (self->start + self->size) % self->cap;
    }

    It begin();
    It end();
};

inline It::It(P *c, const Ring &r)
    : cur(c)
    , first(r.ptr)
    , mid(first + r.start)
    , last(first + r.cap) {
}

inline It Ring::begin() {
    return It(ptr + start, *this);
}

inline It Ring::end() {
    return It(ptr + endIndex(), *this);
}

extern "C" void LobbyEventDequeMap_InsertRange(Ring *r, It pos, WIt first, WIt last, Tag);

extern "C" void LobbyEventDequeMap_InsertRange(Ring *r, It pos, WIt first, WIt last, Tag) {
    u32 n = distance(first, last);
    if (n == 0) {
        return;
    }
    u32 k = pos.cur - (r->ptr + r->start);
    if (pos.cur < r->ptr + r->start) {
        k += r->cap;
    }
    u32 c = r->capacity();
    if (n <= c && r->size <= c - n) {
        It b = r->begin();
        It old = b;
        u32 after = r->size - k;
        if (k < after) {
            b += -n;
            r->start = b.cur - r->ptr;
            r->size += n;
            if (k != 0) {
                b = LobbyEventDequeMap_Copy(old, old + k, b);
            }
        } else {
            r->size += n;
            b = r->end();
            if (after != 0) {
                b = LobbyEventDequeMap_CopyBackward(b - (n + after), b - n, b);
            }
            b += -n;
        }
        LobbyEventDequeMap_CopyRange(first, last, b);
        return;
    }
    Tmp t;
    t.ptr = 0;
    t.start = 0;
    t.size = 0;
    t.cap = 0;
    t.owner = &r->cap;
    u32 nc = LobbyEventDequeMap_RecommendSize(r, n);
    LobbyEventDequeMapBuffer_Allocate(&t, nc, Alloc(Alloc()));
    t.start = k;
    P *p = t.ptr + k;
    if (n != 0) {
        do {
            const P &v = *first.it;
            Alloc a = Alloc();
            --n;
            a.construct(p, v);
            ++p;
            t.size++;
            ++first.it;
        } while (n != 0);
    }
    LobbyEventDequeMapBuffer_AppendRange(&t, pos, r->end(), Alloc());
    LobbyEventDequeMapBuffer_PrependRange(&t, r->begin(), pos, Alloc());
    r->clear();
    swap(r->ptr, t.ptr);
    swap(r->start, t.start);
    swap(r->size, t.size);
    u32 &tc = t.cap;
    swap(r->cap, tc);
    LobbyEventDequeMapBuffer_Destroy(&t);
}
