#pragma force_active on
#include <nitro/types.h>

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

struct Tag {};

struct Elem {
    u32 *x;
};

typedef Elem P;

struct It {
    P *cur, *first, *mid, *last;

    It() {
    }

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

    WIt() {
    }

    WIt(const It &i)
        : it(i) {
    }
};

inline WIt operator+(WIt w, int n) {
    w.it += n;
    return w;
}

inline WIt Advance(const WIt &w, int n) {
    return w + n;
}

extern "C" int ov45_0223CDBC(const WIt &a, const WIt &b);

inline int dist2(WIt first, WIt last) {
    return ov45_0223CDBC(last, first);
}

inline int distance(WIt first, WIt last) {
    return dist2(first, last);
}

struct Alloc {
    void construct(P *p, P v) {
        if (p) {
            *p = v;
        }
    }

    void destroy(P *) {
    }

    void assign(P *p, P v) {
        *p = v;
    }

    void assignr(P *p, const P &v) {
        *p = v;
    }
};

template <class T>
inline const T &Min(const T &a, const T &b) {
    return b < a ? b : a;
}

inline P *CopyRange(WIt first, WIt last, P *d) {
    while (first.it.cur != last.it.cur) {
        const P &v = *first.it;
        Alloc a = Alloc();
        a.assign(d, v);
        ++first.it;
        ++d;
    }
    return d;
}

extern "C" It ov45_0223C53C(WIt first, WIt last, It dest);

extern "C" It ov45_0223C53C(WIt first, WIt last, It dest) {
    if (first.it.cur != last.it.cur) {
        int n = ov45_0223CDBC(last, first);
        for (;;) {
            int room = dest.last - dest.cur;
            int chunk = Min(n, room);
            WIt mid(Advance(first, chunk));
            CopyRange(first, mid, dest.cur);
            n -= chunk;
            dest.cur += chunk - 1;
            ++dest;
            if (n == 0) {
                break;
            }
            first = mid;
        }
    }
    return dest;
}
