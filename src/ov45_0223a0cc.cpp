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

    Cursor operator+(s32 n) const {
        Cursor t(*this);
        t.position += n;
        return t;
    }

    s32 operator-(const Cursor &other) const {
        return (s32)((u32)position - (u32)other.position) / (s32)sizeof(Pair);
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
        return *this;
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

    ConstructingOut &operator+=(s32 n) {
        position += n;
        return *this;
    }

    ConstructingOut operator+(s32 n) const {
        return ConstructingOut(*this) += n;
    }
};

inline ConstructingOut CopyForward(Cursor first, Cursor last, ConstructingOut output) {
    while (first != last) {
        *output = *first;
        ++first;
        ++output;
    }
    return output;
}

inline ConstructingOut CopyReverse(Cursor first, Cursor last, ConstructingOut output) {
    while (first != last) {
        --last;
        *output = *last;
        ++output;
    }
    return output;
}
}

using namespace NativeSort;
extern "C" ConstructingOut ov45_0223A7E4(Cursor, Cursor, ConstructingOut, void *);
extern "C" ConstructingOut ov45_0223A4C8(Cursor, Cursor, Cursor, Cursor, ConstructingOut, void *);
extern "C" void ov45_02239294(Cursor, Cursor, ConstructingOut, ConstructingOut, void *);
#pragma force_active on
extern "C" void ov45_0223A0CC(Cursor, Cursor, ConstructingOut, void *);

extern "C" void ov45_0223A0CC(Cursor first, Cursor last, ConstructingOut output, void *context) {
    Cursor middle;
    s32 count = last - first;
    if (count <= 0) {
        return;
    }
    if (count == 1) {
        *output = *first;
        return;
    }
    if (count == 2) {
        Cursor prev = last;
        if ((*--prev).key < (*first).key) {
            CopyReverse(first, last, output);
        } else {
            CopyForward(first, last, output);
        }
        return;
    }
    if (count <= 20) {
        ov45_0223A7E4(first, last, output, context);
        return;
    }
    s32 half = count - count / 2;
    middle = first + half;
    ov45_02239294(first, middle, output, output + half, context);
    ov45_02239294(middle, last, output + half, output + count, context);
    ov45_0223A4C8(first, first + half, first + half, first + count, output, context);
}

#pragma force_active off
