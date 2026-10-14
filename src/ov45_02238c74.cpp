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

class Tag {};

class Less {};

struct Vec8 {
    Elem *ptr;
    u32 size, cap;

    It Begin() {
        return It(ptr);
    }

    It End() {
        return It(ptr + size);
    }
};

struct Deque {
    u32 unk0;
    u32 size;
};

struct Src {
    u32 w0, w4, w8;
    u8 c, d;
    u16 count;
    Elem items[1];
};

struct Obj {
    u8 pad[0x1c];
    u32 w1c, w20, w24;
    u8 b28, b29;
    u8 pad2a[2];
    Vec8 vec;
    Deque deq;
};

extern "C" int PPWLobbySchedule_IsSizeValid(Obj *, Src *);
extern "C" void LobbyEventVector_AssignRange(Vec8 *, Elem *, Elem *, Tag);
extern "C" void LobbySort_StableSort(It, It, Less);
extern "C" void LobbyEventDeque_Assign(Deque *, It, const It &, Tag);
extern "C" int PPWLobbySchedule_Load(Obj *, Src *);

inline void Sort(It first, It last, Less c) {
    LobbySort_StableSort(first, last, c);
}

inline void Assign(Deque *d, It first, It last) {
    Tag t;
    LobbyEventDeque_Assign(d, first, last, t);
}

#pragma force_active on

extern "C" int PPWLobbySchedule_Load(Obj *o, Src *s) {
    Less c;
    if (!PPWLobbySchedule_IsSizeValid(o, s)) {
        return 0;
    }
    o->b28 = s->c;
    o->b29 = s->d;
    o->w1c = s->w0;
    o->w20 = s->w4;
    o->w24 = s->w8;
    {
        u32 n = s->count;
        LobbyEventVector_AssignRange(&o->vec, s->items, s->items + n, Tag());
    }
    Sort(o->vec.Begin(), o->vec.End(), c);
    Assign(&o->deq, o->vec.Begin(), o->vec.End());
    return 1;
}
