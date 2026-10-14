#pragma force_active on
#include <nitro/types.h>

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
extern "C" void *memset(void *p, int c, u32 n);
extern "C" void *memcpy(void *d, const void *s, u32 n);

struct Tag {};

struct Vec8 {
    u8 *ptr;
    u32 size;
    u32 cap;

    u8 *begin() {
        return ptr;
    }

    u8 *end() {
        return ptr + size;
    }
};

struct Buf {
    u8 *ptr;
    u32 size;
    u32 cap;
    u32 *alloc;
    u32 start;

    Buf(u32 *a) {
        ptr = 0;
        size = 0;
        cap = 0;
        start = 0;
        alloc = a;
    }
};
struct Buf;

struct Alloc {
    void fill(u8 *p, u8 v, u32 n) {
        memset(p, v, n);
    }

    void move(u8 *d, u8 *s, u32 n) {
        memcpy(d, s, n);
        memset(s, 0, n);
    }

    u32 moveBack(Buf *t, u8 *first, u8 *last) {
        u32 n = last - first;
        t->start -= n;
        memcpy(t->ptr + t->start, first, n);
        memset(first, 0, n);
        return n;
    }

    void destroy(u8 *, u32) {
    }
};

extern "C" u32 LobbyCharVector_RecommendSize(Vec8 *v, u32 n);
extern "C" void LobbyCharVectorBuffer_Allocate(Buf *b, u32 n, Tag t);
extern "C" void LobbyCharVectorBuffer_Destroy(Buf *b);

inline void Allocate(Buf *b, u32 n, Tag t) {
    LobbyCharVectorBuffer_Allocate(b, n, t);
}

inline void Swap(u32 &a, u32 &b) {
    u32 t = a;
    a = b;
    b = t;
}

inline void Swap(u8 *&a, u8 *&b) {
    u8 *t = a;
    a = b;
    b = t;
}

extern "C" void LobbyCharVector_AppendFillSlow(Vec8 *v, u32 n, const u8 *val);

extern "C" void LobbyCharVector_AppendFillSlow(Vec8 *v, u32 n, const u8 *val) {
    Buf tmp(&v->cap);
    u32 c = LobbyCharVector_RecommendSize(v, v->size + n - v->cap);
    Allocate(&tmp, c, Tag(Tag()));
    tmp.start = v->size;
    Alloc a = Alloc();
    a.fill(tmp.ptr + tmp.start + tmp.size, *val, n);
    tmp.size += n;
    u8 *first = v->begin();
    u8 *last = v->end();
    Alloc b = Alloc();
    u32 sz = b.moveBack(&tmp, first, last);
    tmp.size += sz;
    Alloc d = Alloc();
    d.destroy(v->ptr, v->size);
    v->size = 0;
    u32 &tc = tmp.cap;
    Swap(v->cap, tc);
    Swap(v->ptr, tmp.ptr);
    Swap(v->size, tmp.size);
    LobbyCharVectorBuffer_Destroy(&tmp);
}
