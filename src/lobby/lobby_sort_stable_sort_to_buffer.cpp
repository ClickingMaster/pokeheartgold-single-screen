#include <nitro/types.h>

#include "lobby_sort.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

using namespace LobbySort;

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
