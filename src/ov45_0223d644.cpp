#pragma force_active on
#include <nitro/types.h>

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

struct It {
    char *p;

    It(char *q)
        : p(q) {
    }

    char &operator*() const {
        return *p;
    }

    It &operator++() {
        ++p;
        return *this;
    }

    bool operator!=(const It &o) const {
        return p != o.p;
    }
};

union Str {
    struct {
        u32 flag : 1;
        u32 capacity : 31;
        u32 size;
        char *data;
    } l;

    struct {
        u8 flag : 1;
        u8 size : 7;
        char data[11];
    } s;

    u32 words[3];

    It begin() {
        return It(!l.flag ? s.data : l.data);
    }

    It end() {
        char *p;
        u32 n;
        if (!l.flag) {
            p = s.data;
            n = s.size;
        } else {
            p = l.data;
            n = l.size;
        }
        return It(p + n);
    }
};

extern "C" int ov45_0223D7B4(int c);
extern "C" void ov45_0223DB54(Str *a, int, void *c, Str *b);

inline It Transform(It first, It last, It out, int (*f)(int)) {
    for (; first != last; ++first, ++out) {
        *out = f(*first);
    }
    return out;
}

extern "C" void ov45_0223D644(Str *a, Str *b, void *c);

extern "C" void ov45_0223D644(Str *a, Str *b, void *c) {
    Transform(a->begin(), a->end(), a->begin(), ov45_0223D7B4);
    Transform(b->begin(), b->end(), b->begin(), ov45_0223D7B4);
    ov45_0223DB54(a, 0, c, b);
}
