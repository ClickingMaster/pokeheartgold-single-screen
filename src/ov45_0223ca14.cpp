#pragma force_active on
#include <nitro/types.h>

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

struct Tag {};

extern "C" void DWC_Free(int, void *, u32);

struct Buf {
    void **ptr;
    u32 start, size, cap;
};

struct Ring {
    u32 unk0;
    u32 unk4;
    Buf buf;
};

extern "C" void ov45_02238508(Buf *b);

struct Del {
    void operator()(void *p) const {
        if (p) {
            DWC_Free(0, p, 0);
        }
    }
};

inline u32 EndIndex(const Buf *b) {
    u32 e = 0;
    if (b->cap != 0) {
        e = (b->start + b->size) % b->cap;
    }
    return e;
}

extern "C" Ring *ov45_0223CA14(Ring *r);

extern "C" Ring *ov45_0223CA14(Ring *r) {
    Del d = Del();
    r->unk4 = 0;
    if (r->buf.size != 0) {
        u32 e = EndIndex(&r->buf);
        void **end;
        void **cur;
        void **base;
        void **lim;
        base = r->buf.ptr;
        end = base + e;
        lim = base + r->buf.cap;
        cur = base + r->buf.start;
        while (cur != end) {
            d(*cur);
            if (++cur == lim) {
                cur = base;
            }
        }
    }
    ov45_02238508(&r->buf);
    return r;
}
