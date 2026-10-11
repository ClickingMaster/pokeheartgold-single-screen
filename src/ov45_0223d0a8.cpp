#pragma force_active on
#include <nitro/types.h>

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
extern "C" void *memmove(void *, const void *, u32);

struct RIt {
    u32 *cur, *first, *mid, *last;

    RIt &operator++() {
        if (++cur == last) {
            cur = first;
        }
        return *this;
    }

    RIt &operator+=(int n) {
        cur += n - 1;
        return ++*this;
    }

    bool second() const {
        return cur >= mid;
    }
};

static inline u32 *Copy(u32 *d, u32 *s, u32 n) {
    memmove(d, s, n * sizeof(u32));
    return d + n;
}

extern "C" u32 *ov45_0223D0A8(RIt src, RIt end, u32 *dest);

extern "C" u32 *ov45_0223D0A8(RIt src, RIt end, u32 *dest) {
    int len;
    u32 n;
    u32 *p;
    while (src.cur != end.cur) {
        len = ((src.cur >= src.mid) == (end.cur >= end.mid)) ? end.cur - src.cur : src.last - src.cur;
        p = src.cur;
        p += len;
        n = p - src.cur;
        dest = Copy(dest, src.cur, n);
        src += len;
    }
    return dest;
}
