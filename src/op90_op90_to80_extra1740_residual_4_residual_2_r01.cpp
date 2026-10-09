#pragma force_active on
#include <nitro/types.h>
#include <nitro/code32.h>

struct Elem {
    int key;
    u32 val;
};

struct It {
    Elem *p;
};

struct Less {
    bool operator()(const Elem &a, const Elem &b) const { return a.key < b.key; }
};

static inline Elem *Next(const It &it) {
    return it.p + 1;
}

extern "C" void ov45_0223A9C0(It first, It last, Less comp);

// @fn ov45_0223A9C0
extern "C" void ov45_0223A9C0(It first, It last, Less comp) {
    Elem *i;
    Elem *k;

    if (first.p == last.p) {
        return;
    }
    i = Next(first);
    if (i == last.p) {
        return;
    }
    for (; i != last.p; ++i) {
        Elem *j = i;
        --j;
        if (comp(*i, *j)) {
            Elem tmp = *i;
            k = j;
            *i = *j;
            while (j != first.p && comp(tmp, *--j)) {
                *k = *j;
                --k;
            }
            *k = tmp;
        }
    }
}
