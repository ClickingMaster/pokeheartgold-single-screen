#include <nitro/types.h>

#include "lobby_sort.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

using namespace LobbySort;

inline Cursor CopyFromBuffer(ConstructingOut first, ConstructingOut last, Cursor output) {
    while (first != last) {
        *output = *first;
        ++first;
        ++output;
    }
    return output;
}

extern "C" Cursor LobbySort_MergeBuffers(ConstructingOut first1, ConstructingOut last1, ConstructingOut first2, ConstructingOut last2, Cursor out, const Less &comp) {
    while (first1 != last1 && first2 != last2) {
        if (comp(*first2, *first1)) {
            *out = *first2;
            ++first2;
        } else {
            *out = *first1;
            ++first1;
        }
        ++out;
    }
    if (first1 == last1) {
        return CopyFromBuffer(first2, last2, out);
    }
    return CopyFromBuffer(first1, last1, out);
}

extern "C" void LobbySort_StableSortToBuffer(Cursor first, Cursor last, ConstructingOut output, const Less &comp) {
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
        LobbySort_InsertionSortToBuffer(first, last, output, comp);
        return;
    }
    s32 half = count - count / 2;
    middle = first + half;
    LobbySort_StableSortAdaptive(first, middle, output, output + half, comp);
    LobbySort_StableSortAdaptive(middle, last, output + half, output + count, comp);
    LobbySort_Merge(first, first + half, first + half, first + count, output, comp);
}

extern "C" ConstructingOut LobbySort_Merge(Cursor first1, Cursor last1, Cursor first2, Cursor last2, ConstructingOut result, const Less &comp) {
    while (first1 != last1 && first2 != last2) {
        if (comp(*first2, *first1)) {
            *result = *first2;
            ++first2;
        } else {
            *result = *first1;
            ++first1;
        }
        ++result;
    }
    if (first1 == last1) {
        return CopyForward(first2, last2, result);
    }
    return CopyForward(first1, last1, result);
}

extern "C" ConstructingOut LobbySort_InsertionSortToBuffer(Cursor first, Cursor last, ConstructingOut result, const Less &comp) {
    bool more = first != last;
    ConstructingOut end = result;
    if (more) {
        *result = *first;
        ++first;
        ++end;
        while (first != last) {
            ConstructingOut j = end;
            Cursor k(end.position);
            while (k.position != result.position && comp(*first, *--k)) {
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

static inline Pair *Next(const Cursor &it) {
    return it.position + 1;
}

extern "C" void LobbySort_InsertionSort(Cursor first, Cursor last, const Less &comp) {
    Pair *i;
    Pair *k;

    if (first.position == last.position) {
        return;
    }
    i = Next(first);
    if (i == last.position) {
        return;
    }
    for (; i != last.position; ++i) {
        Pair *j = i;
        --j;
        if (comp(*i, *j)) {
            Pair tmp = *i;
            k = j;
            *i = *j;
            while (j != first.position && comp(tmp, *--j)) {
                *k = *j;
                --k;
            }
            *k = tmp;
        }
    }
}
