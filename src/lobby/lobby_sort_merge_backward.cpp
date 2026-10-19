#include <nitro/types.h>

#include "lobby_sort.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

using namespace LobbySort;

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
        *result = *last.base();
    }
    return result;
}

extern "C" Cursor LobbySort_MergeBackward(Cursor first1, Cursor last1, ConstructingOut first2, ConstructingOut last2, Cursor result, const Less &comp) {
    if (first1 == last1) {
        return CopyBackB(first2, last2, result);
    }
    if (first2 == last2) {
        return CopyBackA(first1, last1, result);
    }
    --last1;
    --last2;
    while (true) {
        if (comp(*last2.base(), *last1)) {
            --result;
            *result = *last1;
            if (first1 == last1) {
                return CopyBackB(first2, ++last2, result);
            }
            --last1;
        } else {
            --result;
            *result = *last2.base();
            if (first2 == last2) {
                return CopyBackA(first1, ++last1, result);
            }
            --last2;
        }
    }
}
