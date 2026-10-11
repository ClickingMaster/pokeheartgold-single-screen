#include <nitro/types.h>

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
#include <size_t.h>

inline void *operator new(std::size_t, void *p) {
    return p;
}
struct Str;
extern "C" Str *ov45_02237A24(Str *, const Str *);

struct Str {
    u32 words[3];

    Str(const Str &o) {
        ov45_02237A24(this, &o);
    }
};

class Tag {};

class FalseType {};

struct Vec {
    Str *ptr;
    u32 size, cap;
};

struct CapPair {
    u32 cap;
    u32 *alloc;

    CapPair(u32 *a)
        : cap(0)
        , alloc(a) {
    }

    u32 &First() {
        return cap;
    }
};

struct Buf {
    Str *ptr;
    u32 size;
    CapPair pair;
    u32 start;

    Buf(u32 *a)
        : ptr(0)
        , size(0)
        , pair(a)
        , start(0) {
    }

    u32 &EndCap() {
        return pair.First();
    }

    void Allocate(u32 n, Tag t);

    Tag get_tag() const {
        return Tag();
    }

    void Allocate2(u32 n);
    void Allocate3(u32 n);
    void Allocate4(u32 n);
    void Allocate5(u32 n, Tag t);
    void AllocA(u32 n);
    void AllocB(u32 n);
    void AllocC(u32 n, const Tag &t);
};

extern "C" u32 ov45_022385A4(Vec *, u32);
extern "C" void ov45_02238A3C(Buf *, u32, Tag);
extern "C" void ov45_0223895C(Buf *);
extern "C" void ov45_02238674(Vec *, u32, const Str &);

template <class T>
inline void Swap(T &a, T &b) {
    T t = a;
    a = b;
    b = t;
}

inline Buf *Self(FalseType, Buf &b) {
    return &b;
}

inline void Buf::Allocate(u32 n, Tag t) {
    ov45_02238A3C(this, n, t);
}

inline void Buf::Allocate2(u32 n) {
    ov45_02238A3C(this, n, get_tag());
}

inline void Buf::Allocate3(u32 n) {
    Tag t = get_tag();
    ov45_02238A3C(this, n, t);
}

inline void Buf::Allocate4(u32 n) {
    Tag t = get_tag();
    (void)t;
    ov45_02238A3C(this, n, Tag());
}

inline void Buf::Allocate5(u32 n, Tag t) {
    Tag u = get_tag();
    (void)u;
    ov45_02238A3C(this, n, t);
}

inline Tag MakeTag() {
    return Tag();
}

inline void Buf::AllocA(u32 n) {
    Allocate(n, Tag());
}

inline void Buf::AllocB(u32 n) {
    Allocate(n, MakeTag());
}

inline void Buf::AllocC(u32 n, const Tag &t) {
    Allocate(n, t);
}

inline const Tag &TagRef(const Tag &t) {
    return t;
}

inline void FillN(Buf &b, u32 n, const Str &x) {
    FalseType f = FalseType();
    (void)f;
    Str *p = b.ptr + b.start + b.size;
    for (; n != 0; n--, b.size++, p++) {
        new (p) Str(x);
    }
}

inline void MoveBack(Buf &b, Vec *v) {
    Str *first = v->ptr;
    Str *last = first + v->size;
    FalseType f = FalseType();
    (void)f;
    Str *out = b.ptr + b.start;
    while (last > first) {
        --last;
        --out;
        new (out) Str(*last);
        b.start--;
        b.size++;
    }
}

inline void Release(Vec *) {
    FalseType f = FalseType();
    (void)f;
}

#pragma force_active on

extern "C" void ov45_02238674(Vec *v, u32 n, const Str &x) {
    Buf b(&v->cap);
    u32 len = ov45_022385A4(v, v->size + n - v->cap);
    b.AllocA(len);
    b.start = v->size;
    FillN(b, n, x);
    MoveBack(b, v);
    Release(v);
    Swap(v->cap, b.EndCap());
    Swap(v->ptr, b.ptr);
    Swap(v->size, b.size);
    ov45_0223895C(&b);
}
