#include <nitro/types.h>

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

struct Tag {};

struct LobbyEvent;

// A block pointer in the map's ring.
struct Block {
    LobbyEvent *events;
};

struct MapIterator {
    Block *cur, *first, *mid, *last;

    MapIterator() {
    }

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

    SourceIterator() {
    }

    SourceIterator(const MapIterator &i)
        : it(i) {
    }
};

inline SourceIterator operator+(SourceIterator w, int n) {
    w.it += n;
    return w;
}

inline SourceIterator Advance(const SourceIterator &w, int n) {
    return w + n;
}

extern "C" int LobbyEventDequeMapIterator_Distance(const SourceIterator &a, const SourceIterator &b);

struct Alloc {
    void construct(Block *p, Block v) {
        if (p) {
            *p = v;
        }
    }

    void destroy(Block *) {
    }

    void assign(Block *p, Block v) {
        *p = v;
    }

    void assignr(Block *p, const Block &v) {
        *p = v;
    }
};

template <class T>
inline const T &Min(const T &a, const T &b) {
    return b < a ? b : a;
}

inline Block *CopyRange(SourceIterator first, SourceIterator last, Block *d) {
    while (first.it.cur != last.it.cur) {
        const Block &v = *first.it;
        Alloc a = Alloc();
        a.assign(d, v);
        ++first.it;
        ++d;
    }
    return d;
}

extern "C" MapIterator LobbyEventDequeMap_CopyRange(SourceIterator first, SourceIterator last, MapIterator dest);

extern "C" MapIterator LobbyEventDequeMap_CopyRange(SourceIterator first, SourceIterator last, MapIterator dest) {
    if (first.it.cur != last.it.cur) {
        int n = LobbyEventDequeMapIterator_Distance(last, first);
        for (;;) {
            int room = dest.last - dest.cur;
            int chunk = Min(n, room);
            SourceIterator mid(Advance(first, chunk));
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
