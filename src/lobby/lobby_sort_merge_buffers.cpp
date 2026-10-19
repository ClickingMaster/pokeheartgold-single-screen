#include <nitro/types.h>

#include "lobby_sort.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

using namespace LobbySort;

inline Cursor Copy(ConstructingOut first, ConstructingOut last, Cursor output) {
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
        return Copy(first2, last2, out);
    }
    return Copy(first1, last1, out);
}
