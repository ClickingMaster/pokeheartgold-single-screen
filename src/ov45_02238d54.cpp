#include <nitro/types.h>

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

struct Elem {
    u32 a, b;
};

struct It {
    Elem *p;

    It(Elem *q)
        : p(q) {
    }
};

inline int Distance(It first, It last) {
    return last.p - first.p;
}

struct DIter {
    Elem **map;
    Elem **begin;
    u32 unk8;
    Elem **end;
    Elem *cur;

    DIter(const DIter &o)
        : map(o.map)
        , begin(o.begin)
        , unk8(o.unk8)
        , end(o.end)
        , cur(o.cur) {
    }
};

class Tag {};

struct Deque {
    u32 unk0;
    u32 size;
};

template <class T>
inline const T &Min(const T &a, const T &b) {
    return a < b ? a : b;
}

extern "C" DIter LobbyEventDeque_IteratorAt(Deque *, int);
extern "C" DIter LobbyEventDeque_Copy(It, It, DIter);
extern "C" void LobbyEventDeque_AppendRange(Deque *, It *, const It &, Tag);
extern "C" void LobbyEventDeque_EraseAtEnd(Deque *, u32, Tag);
extern "C" void LobbyEventDeque_Assign(Deque *, It, const It &, Tag);
#pragma force_active on

extern "C" void LobbyEventDeque_Assign(Deque *d, It first, const It &last, Tag) {
    u32 n = Distance(first, last);
    u32 m = Min(d->size, n);
    It mid = first;
    mid.p += m;
    LobbyEventDeque_Copy(first, mid, LobbyEventDeque_IteratorAt(d, 0));
    n -= m;
    if (n != 0) {
        Tag t;
        LobbyEventDeque_AppendRange(d, &mid, last, t);
    } else {
        u32 size = d->size;
        if (m < size) {
            LobbyEventDeque_EraseAtEnd(d, size - m, Tag());
        }
    }
}
