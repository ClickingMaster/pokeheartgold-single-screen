#include <nitro/types.h>

#include "lobby_string.h"

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

inline It Begin(Str *s) {
    return It(!s->l.isLong ? s->s.buf : s->l.ptr);
}

inline It End(Str *s) {
    char *p;
    u32 n;
    if (!s->l.isLong) {
        p = s->s.buf;
        n = s->s.len;
    } else {
        p = s->l.ptr;
        n = s->l.size;
    }
    return It(p + n);
}

extern "C" int LobbyString_ToLowerChar(int c);
extern "C" void LobbyString_CompareSubstring(Str *a, int, void *c, Str *b);

inline It Transform(It first, It last, It out, int (*f)(int)) {
    for (; first != last; ++first, ++out) {
        *out = f(*first);
    }
    return out;
}

extern "C" void LobbyString_CompareNoCase(Str *a, Str *b, void *c);

extern "C" void LobbyString_CompareNoCase(Str *a, Str *b, void *c) {
    Transform(Begin(a), End(a), Begin(a), LobbyString_ToLowerChar);
    Transform(Begin(b), End(b), Begin(b), LobbyString_ToLowerChar);
    LobbyString_CompareSubstring(a, 0, c, b);
}
