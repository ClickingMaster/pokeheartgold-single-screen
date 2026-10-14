#pragma force_active on
#include <nitro/types.h>

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

inline void *operator new(unsigned long, void *p) {
    return p;
}

struct Tag {};
struct Ring;

struct It {
    u32 *cur, *first, *mid, *last;
    inline It(Ring *r, u32 i);
};

extern "C" void LobbyEventDequeMapBuffer_Allocate(Ring *r, u32 n, Tag);
extern "C" void LobbyEventDequeMapBuffer_AppendRange(Ring *r, const It &first, const It &last, Tag);
extern "C" void LobbyEventDequeMapBuffer_Destroy(Ring *r);
extern "C" void LobbyEventDequeMapBuffer_PrependRange(Ring *r, const It &first, const It &last, Tag);
extern "C" u32 LobbyEventDequeMap_RecommendSize(Ring *r, u32 n);

inline void Swap(u32 &a, u32 &b) {
    u32 t = a;
    a = b;
    b = t;
}

inline void Swap(u32 *&a, u32 *&b) {
    u32 *t = a;
    a = b;
    b = t;
}

struct Ring {
    u32 *ptr;
    u32 start, size, cap;

    u32 capacity() const {
        return cap == 0 ? 0 : cap - 1;
    }

    u32 endIndex() const {
        if (cap == 0) {
            return 0;
        }
        return (start + size) % cap;
    }

    It begin() {
        return It(this, start);
    }

    It end() {
        return It(this, endIndex());
    }
};

inline It::It(Ring *r, u32 i)
    : cur(r->ptr + i)
    , first(r->ptr)
    , mid(first + r->start)
    , last(first + r->cap) {
}

struct RingBuf : Ring {
    u32 *alloc;

    RingBuf(u32 *a) {
        ptr = 0;
        start = 0;
        size = 0;
        cap = 0;
        alloc = a;
    }
};

struct Alloc {
    void destroy(u32 *, u32) {
    }
};

inline void Allocate(Ring *r, u32 n, Tag t) {
    LobbyEventDequeMapBuffer_Allocate(r, n, t);
}

struct A2 {
    void allocate(Ring *r, u32 n, Tag t) {
        LobbyEventDequeMapBuffer_Allocate(r, n, t);
    }
};

extern "C" void LobbyEventDequeMap_PushBack(Ring *r, const u32 &v);

extern "C" void LobbyEventDequeMap_PushBack(Ring *r, const u32 &v) {
    if (r->size + 1 < r->cap) {
        u32 *p = r->ptr + (r->start + r->size) % r->cap;
        new (p) u32(v);
        ++r->size;
        return;
    }
    RingBuf tmp(&r->cap);
    u32 n = LobbyEventDequeMap_RecommendSize(r, 1);
    Allocate(&tmp, n, Tag(Tag()));
    tmp.start = r->size;
    new (tmp.ptr + tmp.start) u32(v);
    ++tmp.size;
    LobbyEventDequeMapBuffer_PrependRange(&tmp, r->begin(), r->end(), Tag());
    Alloc a = Alloc();
    a.destroy(r->ptr, r->size);
    r->size = 0;
    u32 &c = tmp.cap;
    Swap(r->ptr, tmp.ptr);
    Swap(r->start, tmp.start);
    Swap(r->size, tmp.size);
    Swap(r->cap, c);
    LobbyEventDequeMapBuffer_Destroy(&tmp);
}
