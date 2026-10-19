#ifndef POKEHEARTGOLD_LOBBY_SORT_H
#define POKEHEARTGOLD_LOBBY_SORT_H

// Stable merge sort of the lobby schedule's key/payload pairs (overlay 45), ordered by key. The sort merges through a
// temporary buffer when one can be allocated and falls back to rotations when it is too small.

#include <nitro/types.h>
#include <size_t.h>

inline void *operator new(std::size_t, void *storage) {
    return storage;
}

namespace LobbySort {
struct Pair {
    s32 key;
    u32 payload;
};

struct Less {
    bool operator()(const Pair &a, const Pair &b) const {
        return a.key < b.key;
    }
};

// The temporary buffer: its size and how many of its pairs have been constructed so far.
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

// A position in the temporary buffer. Storing through it constructs the pairs past the buffer's count.
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

    Pair *base() const {
        return position;
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

    ConstructingOut &operator+=(s32 n) {
        position += n;
        return *this;
    }

    ConstructingOut operator+(s32 n) const {
        return ConstructingOut(*this) += n;
    }

    s32 operator-(const ConstructingOut &o) const {
        return position - o.position;
    }

    bool operator==(const ConstructingOut &other) const {
        return position == other.position;
    }
};

inline bool operator!=(const ConstructingOut &a, const ConstructingOut &b) {
    return !(a == b);
}

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

inline void IterSwap(const Cursor &a, const Cursor &b) {
    Pair t = *a;
    *a = *b;
    *b = t;
}
}

extern "C" {
void LobbySort_InsertionSort(LobbySort::Cursor, LobbySort::Cursor, const LobbySort::Less &);
LobbySort::ConstructingOut LobbySort_InsertionSortToBuffer(LobbySort::Cursor, LobbySort::Cursor, LobbySort::ConstructingOut, const LobbySort::Less &);
LobbySort::ConstructingOut LobbySort_Merge(LobbySort::Cursor, LobbySort::Cursor, LobbySort::Cursor, LobbySort::Cursor, LobbySort::ConstructingOut, const LobbySort::Less &);
LobbySort::Cursor LobbySort_MergeBuffers(LobbySort::ConstructingOut, LobbySort::ConstructingOut, LobbySort::ConstructingOut, LobbySort::ConstructingOut, LobbySort::Cursor, const LobbySort::Less &);
LobbySort::Cursor LobbySort_MergeBufferForward(LobbySort::ConstructingOut, LobbySort::ConstructingOut, LobbySort::Cursor, LobbySort::Cursor, LobbySort::Cursor, const LobbySort::Less &);
LobbySort::Cursor LobbySort_MergeBackward(LobbySort::Cursor, LobbySort::Cursor, LobbySort::ConstructingOut, LobbySort::ConstructingOut, LobbySort::Cursor, const LobbySort::Less &);
LobbySort::Cursor LobbySort_UpperBound(LobbySort::Cursor, LobbySort::Cursor, const LobbySort::Pair &, const LobbySort::Less &);
LobbySort::Cursor LobbySort_LowerBound(LobbySort::Cursor, LobbySort::Cursor, const LobbySort::Pair &, const LobbySort::Less &);
LobbySort::Cursor LobbySort_Rotate(LobbySort::Cursor, LobbySort::Cursor, LobbySort::Cursor);
void LobbySort_MergeAdaptive(LobbySort::Cursor, LobbySort::Cursor, LobbySort::Cursor, const LobbySort::Less &, s32, s32, LobbySort::ConstructingOut, LobbySort::ConstructingOut);
void LobbySort_StableSortToBuffer(LobbySort::Cursor, LobbySort::Cursor, LobbySort::ConstructingOut, const LobbySort::Less &);
void LobbySort_StableSortAdaptive(LobbySort::Cursor, LobbySort::Cursor, LobbySort::ConstructingOut, LobbySort::ConstructingOut, const LobbySort::Less &);
}

#endif // POKEHEARTGOLD_LOBBY_SORT_H
