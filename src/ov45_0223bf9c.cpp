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

extern "C" It &ov45_0223CD78(It *it, int n);

inline It &It::operator+=(int n) {
    return ov45_0223CD78(this, n);
}

struct Empty {};

struct WIt {
    It it;
    Empty e;

    WIt(const It &i)
        : it(i) {
    }
};

extern "C" int ov45_0223CDBC(const WIt &a, const WIt &b);

inline int dist2(WIt first, WIt last) {
    return ov45_0223CDBC(last, first);
}

inline int distance(WIt first, WIt last) {
    return dist2(first, last);
}

extern "C" It ov45_0223C8A0(It first, It last, It dest);
extern "C" It ov45_0223C70C(It first, It last, It dest);
extern "C" It ov45_0223C53C(WIt first, WIt last, It dest);

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

extern "C" u32 ov45_0223BBFC(Ring *r, u32 n);
extern "C" void ov45_0223CB04(Tmp *t, u32 n, Alloc);
extern "C" void ov45_0223CC78(Tmp *t, const It &first, const It &last, Alloc);
extern "C" void ov45_0223CB6C(Tmp *t, const It &first, const It &last, Alloc);
extern "C" void ov45_0223CAB4(Tmp *t);

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

extern "C" void ov45_0223BF9C(Ring *r, It pos, WIt first, WIt last, Tag);

extern "C" void ov45_0223BF9C(Ring *r, It pos, WIt first, WIt last, Tag) {
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
                b = ov45_0223C8A0(old, old + k, b);
            }
        } else {
            r->size += n;
            b = r->end();
            if (after != 0) {
                b = ov45_0223C70C(b - (n + after), b - n, b);
            }
            b += -n;
        }
        ov45_0223C53C(first, last, b);
        return;
    }
    Tmp t;
    t.ptr = 0;
    t.start = 0;
    t.size = 0;
    t.cap = 0;
    t.owner = &r->cap;
    u32 nc = ov45_0223BBFC(r, n);
    ov45_0223CB04(&t, nc, Alloc(Alloc()));
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
    ov45_0223CC78(&t, pos, r->end(), Alloc());
    ov45_0223CB6C(&t, r->begin(), pos, Alloc());
    r->clear();
    swap(r->ptr, t.ptr);
    swap(r->start, t.start);
    swap(r->size, t.size);
    u32 &tc = t.cap;
    swap(r->cap, tc);
    ov45_0223CAB4(&t);
}
