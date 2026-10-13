#pragma force_active on
#include <nitro/types.h>

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
#include <size_t.h>

inline void *operator new(std::size_t, void *storage) {
    return storage;
}

namespace NativeSort {
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

    Pair *operator->() const {
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

    ConstructingOut operator*() const {
        return ConstructingOut(position, buffer);
    }

    operator Pair &() const {
        return *position;
    }

    ConstructingOut &operator++() {
        ++position;
        return *this;
    }

    bool operator==(const ConstructingOut &other) const {
        return position == other.position;
    }
};

inline bool operator!=(const ConstructingOut &a, const ConstructingOut &b) {
    return !(a == b);
}

inline bool Less(const Pair &a, const Pair &b) {
    return a.key < b.key;
}

inline Cursor Copy(ConstructingOut first, ConstructingOut last, Cursor output) {
    while (first != last) {
        *output = *first;
        ++first;
        ++output;
    }
    return output;
}
}

using namespace NativeSort;
extern "C" Cursor LobbySort_MergeBuffers(ConstructingOut first1, ConstructingOut last1, ConstructingOut first2, ConstructingOut last2, Cursor out, void *ctx);

extern "C" Cursor LobbySort_MergeBuffers(ConstructingOut first1, ConstructingOut last1, ConstructingOut first2, ConstructingOut last2, Cursor out, void *ctx) {
    while (first1 != last1 && first2 != last2) {
        if (Less(*first2, *first1)) {
            *out = *first2;
            ++first2;
        } else {
            *out = *first1;
            ++first1;
        }
        ++out;
    }
    if (first1 == last1) {
        return Copy(first2, last2, out);
    }
    return Copy(first1, last1, out);
}
