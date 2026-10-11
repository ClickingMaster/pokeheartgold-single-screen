#pragma force_active on
#include <nitro/types.h>

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
#include <size_t.h>

inline void *operator new(std::size_t, void *p) {
    return p;
}

struct Elem {
    u32 a, b;
};

struct It {
    Elem *p;

    It() {
    }

    It(Elem *q)
        : p(q) {
    }

    Elem *base() const {
        return p;
    }

    It &operator+=(int n) {
        p += n;
        return *this;
    }

    bool operator!=(const It &o) const {
        return p != o.p;
    }

    int operator-(const It &o) const {
        return p - o.p;
    }

    It operator+(int n) const {
        It t(*this);
        t.p += n;
        return t;
    }
};

struct DIter {
    Elem **map;
    Elem **begin;
    u32 unk8;
    Elem **end;
    Elem *cur;

    DIter(const DIter &o)
        : map(o.map)
        , begin(o.begin)
        , unk8(o.unk8)
        , end(o.end)
        , cur(o.cur) {
    }

    Elem *SegEnd() const {
        return cur ? *map + 128 : cur;
    }

    DIter &operator++() {
        Elem **m = map;
        if (++cur == *m + 128) {
            if (++map == end) {
                map = begin;
            }
            cur = *map;
        }
        return *this;
    }

    DIter &operator+=(int n) {
        cur += n - 1;
        return ++*this;
    }
};

template <class T>
inline const T &Min(const T &a, const T &b) {
    return b < a ? b : a;
}

struct FalseType {};

extern "C" DIter ov45_02238F18(It, It, DIter, FalseType, u32 *);
extern "C" DIter ov45_02239090(It, It, DIter);

inline Elem *UninitCopy(It first, It last, Elem *out, u32 *count) {
    for (; first != last; ++first.p, ++out) {
        new (out) Elem(*first.p);
        ++*count;
    }
    return out;
}

inline Elem *CopyRange(const It &f, const It &l, Elem *out) {
    Elem *p = f.base();
    Elem *o = out;
    Elem *e = l.base();
    for (; p < e; ++p, ++o) {
        *o = *p;
    }
    return o;
}

inline Elem *Copy(It first, It last, Elem *out) {
    return CopyRange(first, last, out);
}

extern "C" DIter ov45_02239090(It first, It last, DIter result) {
    if (first != last) {
        int n = last - first;
        for (;;) {
            int m;
            It mid;
            int room = result.SegEnd() - result.cur;
            m = Min(n, room);
            mid = first + m;
            Copy(first, mid, result.cur);
            n -= m;
            result += m;
            if (n == 0) {
                break;
            }
            first = mid;
        }
    }
    return result;
}
