#include <nitro/types.h>

#include "lobby_vector.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

class FalseType {};

class TrueType {};

// The one-byte tag object the string vector buffer hands on when it erases its strings.
struct ByteTag {
    u8 value;
};

extern "C" {
void *memset(void *, int, unsigned);
void *memcpy(void *, const void *, unsigned);
void *LobbyMem_Alloc(u32);
void DWC_Free(int, void *, u32);
void _ZNSt15__vector_commonILb1EE18throw_length_errorEv(void);
Str *LobbyString_InitCopy(Str *, const Str *);
int LobbyString_Destroy(Str *);
u32 LobbyStringVector_RecommendSize(LobbyStringVector *, u32);
u32 LobbyCharVector_RecommendSize(LobbyCharVector *, u32);
void LobbyStringVector_AppendCopiesSlow(LobbyStringVector *, u32, const Str &);
void LobbyCharVector_AppendZerosSlow(LobbyCharVector *, u32);
void LobbyStringVectorBuffer_EraseAtEnd(LobbyStringVectorBuffer *, u32, ByteTag, u32);
static LobbyCharVectorBuffer *LobbyCharVectorBuffer_Deallocate(LobbyCharVectorBuffer *buffer);
}

template <class T>
inline void Swap(T &a, T &b) {
    T t = a;
    a = b;
    b = t;
}

inline u32 &Value(LobbyVectorBufferCapacity &c) {
    return c.value;
}

static inline void InitBuffer(LobbyStringVectorBuffer *b, u32 *allocator) {
    b->data = 0;
    b->size = 0;
    b->capacity.value = 0;
    b->capacity.allocator = allocator;
    b->start = 0;
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

static inline void InitBuffer(LobbyCharVectorBuffer *b, u32 *allocator) {
    b->data = 0;
    b->size = 0;
    b->capacity.value = 0;
    b->capacity.allocator = allocator;
    b->start = 0;
}

inline u32 &EndCapacity(LobbyCharVectorBuffer &b) {
    return Value(b.capacity);
}

inline void Allocate(LobbyCharVectorBuffer &b, u32 n, LobbyTag t) {
    LobbyCharVectorBuffer_Allocate(&b, n, t);
}

inline void AllocA(LobbyCharVectorBuffer &b, u32 n) {
    Allocate(b, n, LobbyTag());
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

extern "C" void LobbyStringVector_EraseAtEnd(LobbyStringVector *p, u32 n, LobbyTag tag) {
    Str *end = p->data + p->size;
    p->size -= n;
    if (!n) {
        return;
    }
    do {
        end--;
        LobbyString_Destroy(end);
    } while (--n);
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

extern "C" LobbyStringVectorBuffer *LobbyStringVectorBuffer_Destroy(LobbyStringVectorBuffer *p) {
    Str *first = p->data + p->start, *end = first + p->size;
    while (end > first) {
        end--;
        LobbyString_Destroy(end);
    }
    p->size = 0;
    LobbyStringVectorBuffer_Deallocate(p);
    return p;
}

extern "C" LobbyCharVectorBuffer *LobbyCharVectorBuffer_Destroy(LobbyCharVectorBuffer *p) {
    p->size = 0;
    LobbyCharVectorBuffer_Deallocate(p);
    return p;
}

extern "C" void LobbyCharVectorBuffer_Allocate(LobbyCharVectorBuffer *p, u32 n, LobbyTag tag) {
    p->data = (char *)LobbyMem_Alloc(n);
    p->capacity.value = n;
}

static LobbyCharVectorBuffer *LobbyCharVectorBuffer_Deallocate(LobbyCharVectorBuffer *p) {
    if (p->data) {
        u32 old = p->size;

        struct {
            volatile u8 zero;
        } init = { 0 };

        p->size -= old;
        if (p->data) {
            DWC_Free(0, p->data, 0);
        }
    }
    return p;
}

extern "C" void LobbyStringVectorBuffer_Allocate(LobbyStringVectorBuffer *p, u32 n, LobbyTag tag) {
    if (n > 0x15555555) {
        _ZNSt15__vector_commonILb1EE18throw_length_errorEv();
    }
    p->data = (Str *)LobbyMem_Alloc(n * sizeof(Str));
    p->capacity.value = n;
}

extern "C" LobbyStringVectorBuffer *LobbyStringVectorBuffer_Deallocate(LobbyStringVectorBuffer *p) {
    if (p->data) {
        u32 current = p->size;
        ByteTag t = { 0 };
        LobbyStringVectorBuffer_EraseAtEnd(p, current, t, 0);
        if (p->data) {
            DWC_Free(0, p->data, 0);
        }
    }
    return p;
}
