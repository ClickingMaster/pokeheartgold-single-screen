#include <nitro/types.h>

#include "lobby_sort.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

using namespace LobbySort;

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
