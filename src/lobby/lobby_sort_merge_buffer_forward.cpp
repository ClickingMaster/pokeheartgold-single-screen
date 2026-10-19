#include <nitro/types.h>

#include "lobby_sort.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

using namespace LobbySort;

inline Cursor CopyA(Cursor first, Cursor last, Cursor result) {
    Pair *l = last.base();
    Pair *r = result.base();
    Pair *f = first.base();
    for (; f < l; ++f, ++r) {
        *r = *f;
    }
    return Cursor(r);
}

inline Cursor CopyB(ConstructingOut first, ConstructingOut last, Cursor result) {
    for (; first != last; ++first, ++result) {
        *result = *first.base();
    }
    return result;
}

extern "C" Cursor LobbySort_MergeBufferForward(ConstructingOut first1, ConstructingOut last1, Cursor first2, Cursor last2, Cursor result, const Less &comp) {
    while (first1 != last1 && first2 != last2) {
        if (comp(*first2, *first1.base())) {
            *result = *first2;
            ++first2;
        } else {
            *result = *first1.base();
            ++first1;
        }
        ++result;
    }
    if (first1 == last1) {
        return CopyA(first2, last2, result);
    }
    return CopyB(first1, last1, result);
}
