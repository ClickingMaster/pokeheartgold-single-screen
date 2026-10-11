#pragma force_active on
#include <nitro/types.h>

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

struct Tag {};
struct Ring;

struct It {
    u32 *cur, *first, *mid, *last;
    inline It(Ring *r, u32 i);
};

extern "C" void ov45_0223CB04(Ring *r, u32 n, Tag);
extern "C" void ov45_0223CC78(Ring *r, const It &first, const It &last, Tag);
extern "C" void ov45_0223CAB4(Ring *r);

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

    RingBuf(u32 *a)
        : alloc(a) {
        ptr = 0;
        start = 0;
        size = 0;
        cap = 0;
    }
};

struct Alloc {
    void destroy(u32 *, u32) {
    }
};

extern "C" void ov45_0223B8E0(Ring *r, u32 n);

extern "C" void ov45_0223B8E0(Ring *r, u32 n) {
    if (n <= r->capacity()) {
        return;
    }
    RingBuf tmp(&r->cap);
    ov45_0223CB04(&tmp, n, Tag());
    ov45_0223CC78(&tmp, r->begin(), r->end(), Tag());
    Alloc a = Alloc();
    a.destroy(r->ptr, r->size);
    r->size = 0;
    u32 &c = tmp.cap;
    Swap(r->ptr, tmp.ptr);
    Swap(r->start, tmp.start);
    Swap(r->size, tmp.size);
    Swap(r->cap, c);
    ov45_0223CAB4(&tmp);
}
