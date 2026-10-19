#include <nitro/types.h>

#include "lobby_sort.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

using namespace LobbySort;

inline ConstructingOut Copy(Cursor first, Cursor last, ConstructingOut result) {
    for (; first != last; ++first, ++result) {
        *result = *first;
    }
    return result;
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
        return Copy(first2, last2, result);
    }
    return Copy(first1, last1, result);
}
