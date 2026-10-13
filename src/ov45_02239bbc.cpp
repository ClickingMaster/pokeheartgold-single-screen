#pragma force_active on
#include <nitro/types.h>

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

struct Pair {
    s32 key;
    u32 payload;
};

struct Buffer {
    s32 capacity, count;
    Pair *data;
};

struct Cursor {
    Pair *position;

    explicit Cursor(Pair *p)
        : position(p) {
    }

    Cursor() {
    }

    Pair &operator*() const {
        return *position;
    }

    Pair *base() const {
        return position;
    }

    Cursor &operator++() {
        ++position;
        return *this;
    }

    Cursor &operator--() {
        --position;
        return *this;
    }

    bool operator==(const Cursor &other) const {
        return position == other.position;
    }
};

inline bool operator!=(const Cursor &a, const Cursor &b) {
    return !(a == b);
}

struct ConstructingOut {
    Pair *position;
    Buffer *buffer;

    ConstructingOut(Pair *p, Buffer *b)
        : position(p)
        , buffer(b) {
    }

    ConstructingOut() {
    }

    operator Pair *() const {
        return position;
    }

    ConstructingOut &operator++() {
        ++position;
        return *this;
    }

    ConstructingOut &operator--() {
        --position;
        return *this;
    }
};

inline bool Less(const Pair &a, const Pair &b) {
    return a.key < b.key;
}

inline Cursor CopyBackA(Cursor first, Cursor last, Cursor result) {
    Pair *f = first.base();
    Pair *r = result.base();
    Pair *l = last.base();
    while (f < l) {
        *--r = *--l;
    }
    return Cursor(r);
}

inline Cursor CopyBackB(ConstructingOut first, ConstructingOut last, Cursor result) {
    while (last != first) {
        --last;
        --result;
        *result = *last;
    }
    return result;
}

extern "C" Cursor LobbySort_MergeBackward(Cursor first1, Cursor last1, ConstructingOut first2, ConstructingOut last2, Cursor result, void *ctx);

extern "C" Cursor LobbySort_MergeBackward(Cursor first1, Cursor last1, ConstructingOut first2, ConstructingOut last2, Cursor result, void *ctx) {
    if (first1 == last1) {
        return CopyBackB(first2, last2, result);
    }
    if (first2 == last2) {
        return CopyBackA(first1, last1, result);
    }
    --last1;
    --last2;
    while (true) {
        if (Less(*last2, *last1)) {
            --result;
            *result = *last1;
            if (first1 == last1) {
                return CopyBackB(first2, ++last2, result);
            }
            --last1;
        } else {
            --result;
            *result = *last2;
            if (first2 == last2) {
                return CopyBackA(first1, ++last1, result);
            }
            --last2;
        }
    }
}
