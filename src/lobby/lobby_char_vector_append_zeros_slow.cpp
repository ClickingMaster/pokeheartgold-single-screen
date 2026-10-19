#include <nitro/types.h>

#include "lobby_vector.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
extern "C" void *memset(void *, int, unsigned);
extern "C" void *memcpy(void *, const void *, unsigned);

class TrueType {};

static inline void InitBuffer(LobbyCharVectorBuffer *b, u32 *allocator) {
    b->data = 0;
    b->size = 0;
    b->capacity.value = 0;
    b->capacity.allocator = allocator;
    b->start = 0;
}

inline u32 &Value(LobbyVectorBufferCapacity &c) {
    return c.value;
}

inline u32 &EndCapacity(LobbyCharVectorBuffer &b) {
    return Value(b.capacity);
}

extern "C" u32 LobbyCharVector_RecommendSize(LobbyCharVector *, u32);

inline void Allocate(LobbyCharVectorBuffer &b, u32 n, LobbyTag t) {
    LobbyCharVectorBuffer_Allocate(&b, n, t);
}

inline void AllocA(LobbyCharVectorBuffer &b, u32 n) {
    Allocate(b, n, LobbyTag());
}

extern "C" void LobbyCharVector_AppendZerosSlow(LobbyCharVector *, u32);

template <class T>
inline void Swap(T &a, T &b) {
    T t = a;
    a = b;
    b = t;
}

inline void FillN(char *p, u32 n, char c) {
    TrueType f = TrueType();
    (void)f;
    memset(p, c, n);
}

inline char *CopyT(const char *first, const char *last, char *out) {
    u32 n = (u32)(last - first);
    memcpy(out, first, n);
    return out + n;
}

inline void MoveBack(LobbyCharVectorBuffer &b, LobbyCharVector *v) {
    char *first = v->data;
    char *last = first + v->size;
    TrueType f = TrueType();
    (void)f;
    u32 len = (u32)(last - first);
    b.start -= len;
    CopyT(first, last, b.data + b.start);
    memset(first, 0, len);
    b.size += len;
}

inline void Clear(LobbyCharVector *v) {
    TrueType f = TrueType();
    (void)f;
    v->size = 0;
}

extern "C" void LobbyCharVector_AppendZerosSlow(LobbyCharVector *v, u32 n) {
    LobbyCharVectorBuffer b;
    InitBuffer(&b, &v->capacity);
    u32 len = LobbyCharVector_RecommendSize(v, v->size + n - v->capacity);
    AllocA(b, len);
    b.start = v->size;
    FillN(b.data + b.start + b.size, n, 0);
    b.size += n;
    MoveBack(b, v);
    Clear(v);
    Swap(v->capacity, EndCapacity(b));
    Swap(v->data, b.data);
    Swap(v->size, b.size);
    LobbyCharVectorBuffer_Destroy(&b);
}
