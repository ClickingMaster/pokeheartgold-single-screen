#include <nitro/types.h>

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

struct Vec {
    char *ptr;
    u32 size, cap;
};

class Allocator {
public:
    u32 recommend(Vec *vector, u32 growth);
};

extern "C" u32 LobbyCharVector_RecommendSize(Vec *, u32);
extern "C" void LobbyCharVector_Reallocate(Vec *, u32, u32);

inline u32 Allocator::recommend(Vec *vector, u32 growth) {
    return LobbyCharVector_RecommendSize(vector, growth);
}

struct Iter {
    const u8 *p;
    u32 extra;

    const u8 &operator*() const {
        return *p;
    }

    Iter &operator++() {
        ++p;
        return *this;
    }

    bool operator!=(const Iter &o) const {
        return p != o.p;
    }

    bool operator==(const Iter &o) const {
        return p == o.p;
    }
};

union Ch {
    u8 u;
    char c;
};

struct Narrow {
    char operator()(u8 c) const {
        Ch x;
        x.u = c;
        return x.c;
    }
};

inline char Cast(const Iter &i) {
    Narrow n = Narrow();
    return n(*i);
}

extern "C" void LobbyCharVector_AssignIterRange(Vec *v, Iter first, Iter last);
#pragma force_active on

extern "C" void LobbyCharVector_AssignIterRange(Vec *v, Iter first, Iter last) {
    u32 n = last.p - first.p;
    u32 cap = v->cap;
    if (n > cap) {
        u32 growth = n - cap;
        Allocator a = Allocator();
        LobbyCharVector_Reallocate(v, n, a.recommend(v, growth));
    }
    {
        Iter it = first;
        char *dst = v->ptr;
        for (; it != last; ++it) {
            *dst++ = Cast(it);
        }
    }
    v->size = n;
}
