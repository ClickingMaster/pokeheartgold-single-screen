#include <nitro/types.h>

#include "lobby_vector.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

class FalseType {};

extern "C" Str *LobbyString_InitCopy(Str *, const Str *);
extern "C" u32 LobbyStringVector_RecommendSize(LobbyStringVector *, u32);
extern "C" void LobbyStringVector_AppendCopiesSlow(LobbyStringVector *, u32, const Str &);

static inline void InitBuffer(LobbyStringVectorBuffer *b, u32 *allocator) {
    b->data = 0;
    b->size = 0;
    b->capacity.value = 0;
    b->capacity.allocator = allocator;
    b->start = 0;
}

inline u32 &Value(LobbyVectorBufferCapacity &c) {
    return c.value;
}

inline u32 &EndCapacity(LobbyStringVectorBuffer &b) {
    return Value(b.capacity);
}

inline void Allocate(LobbyStringVectorBuffer &b, u32 n, LobbyTag t) {
    LobbyStringVectorBuffer_Allocate(&b, n, t);
}

inline void AllocA(LobbyStringVectorBuffer &b, u32 n) {
    Allocate(b, n, LobbyTag());
}

template <class T>
inline void Swap(T &a, T &b) {
    T t = a;
    a = b;
    b = t;
}

inline void FillN(LobbyStringVectorBuffer &b, u32 n, const Str &x) {
    FalseType f = FalseType();
    (void)f;
    Str *p = b.data + b.start + b.size;
    for (; n != 0; n--, b.size++, p++) {
        if (p) {
            LobbyString_InitCopy(p, &x);
        }
    }
}

inline void MoveBack(LobbyStringVectorBuffer &b, LobbyStringVector *v) {
    Str *first = v->data;
    Str *last = first + v->size;
    FalseType f = FalseType();
    (void)f;
    Str *out = b.data + b.start;
    while (last > first) {
        --last;
        --out;
        if (out) {
            LobbyString_InitCopy(out, last);
        }
        b.start--;
        b.size++;
    }
}

inline void Release(LobbyStringVector *) {
    FalseType f = FalseType();
    (void)f;
}

extern "C" void LobbyStringVector_AppendCopiesSlow(LobbyStringVector *v, u32 n, const Str &x) {
    LobbyStringVectorBuffer b;
    InitBuffer(&b, &v->capacity);
    u32 len = LobbyStringVector_RecommendSize(v, v->size + n - v->capacity);
    AllocA(b, len);
    b.start = v->size;
    FillN(b, n, x);
    MoveBack(b, v);
    Release(v);
    Swap(v->capacity, EndCapacity(b));
    Swap(v->data, b.data);
    Swap(v->size, b.size);
    LobbyStringVectorBuffer_Destroy(&b);
}
