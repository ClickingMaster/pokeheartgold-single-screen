#include <nitro/types.h>

#include "lobby_vector.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
extern "C" void *memset(void *p, int c, u32 n);
extern "C" void *memcpy(void *d, const void *s, u32 n);

static inline void InitBuffer(LobbyCharVectorBuffer *b, u32 *allocator) {
    b->data = 0;
    b->size = 0;
    b->capacity.value = 0;
    b->start = 0;
    b->capacity.allocator = allocator;
}

struct Alloc {
    void fill(char *p, u8 v, u32 n) {
        memset(p, v, n);
    }

    void move(char *d, char *s, u32 n) {
        memcpy(d, s, n);
        memset(s, 0, n);
    }

    u32 moveBack(LobbyCharVectorBuffer *t, char *first, char *last) {
        u32 n = last - first;
        t->start -= n;
        memcpy(t->data + t->start, first, n);
        memset(first, 0, n);
        return n;
    }

    void destroy(char *, u32) {
    }
};

extern "C" u32 LobbyCharVector_RecommendSize(LobbyCharVector *v, u32 n);

inline void Allocate(LobbyCharVectorBuffer *b, u32 n, LobbyTag t) {
    LobbyCharVectorBuffer_Allocate(b, n, t);
}

inline void Swap(u32 &a, u32 &b) {
    u32 t = a;
    a = b;
    b = t;
}

inline void Swap(char *&a, char *&b) {
    char *t = a;
    a = b;
    b = t;
}

extern "C" void LobbyCharVector_AppendFillSlow(LobbyCharVector *v, u32 n, const u8 *val);

extern "C" void LobbyCharVector_AppendFillSlow(LobbyCharVector *v, u32 n, const u8 *val) {
    LobbyCharVectorBuffer tmp;
    InitBuffer(&tmp, &v->capacity);
    u32 c = LobbyCharVector_RecommendSize(v, v->size + n - v->capacity);
    Allocate(&tmp, c, LobbyTag(LobbyTag()));
    tmp.start = v->size;
    Alloc a = Alloc();
    a.fill(tmp.data + tmp.start + tmp.size, *val, n);
    tmp.size += n;
    char *first = v->data;
    char *last = v->data + v->size;
    Alloc b = Alloc();
    u32 sz = b.moveBack(&tmp, first, last);
    tmp.size += sz;
    Alloc d = Alloc();
    d.destroy(v->data, v->size);
    v->size = 0;
    u32 &tc = tmp.capacity.value;
    Swap(v->capacity, tc);
    Swap(v->data, tmp.data);
    Swap(v->size, tmp.size);
    LobbyCharVectorBuffer_Destroy(&tmp);
}
