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

    ConstructingOut &operator=(const Pair &value) {
        if (position - buffer->data < buffer->count) {
            *position = value;
        } else {
            if (position) {
                new (position) Pair(value);
            }
            ++buffer->count;
        }
        return *this;
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
}

using namespace NativeSort;
extern "C" ConstructingOut LobbySort_InsertionSortToBuffer(Cursor first, Cursor last, ConstructingOut result, void *ctx);

extern "C" ConstructingOut LobbySort_InsertionSortToBuffer(Cursor first, Cursor last, ConstructingOut result, void *ctx) {
    bool more = first != last;
    ConstructingOut end = result;
    if (more) {
        *result = *first;
        ++first;
        ++end;
        while (first != last) {
            ConstructingOut j = end;
            Cursor k(end.position);
            while (k.position != result.position && Less(*first, *--k)) {
                *j = *k;
                --j;
            }
            *j = *first;
            ++first;
            ++end;
        }
    }
    return ConstructingOut(end.position, end.buffer);
}
