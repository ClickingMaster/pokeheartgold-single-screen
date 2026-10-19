#include <nitro/types.h>

#include "lobby_sort.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

using namespace LobbySort;

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
