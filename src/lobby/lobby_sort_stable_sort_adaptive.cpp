#include <nitro/types.h>

#include "lobby_sort.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

using namespace LobbySort;

extern "C" void LobbySort_StableSortAdaptive(Cursor first, Cursor last, ConstructingOut buf, ConstructingOut bufEnd, const Less &comp) {
    s32 len = last - first;
    if (len <= 1) {
        return;
    }
    if (len == 2) {
        --last;
        if (comp(*last, *first)) {
            IterSwap(first, last);
        }
        return;
    }
    if (len <= 20) {
        LobbySort_InsertionSort(first, last, comp);
        return;
    }
    s32 half = len - len / 2;
    Cursor middle = first + half;
    if (len <= bufEnd - buf) {
        LobbySort_StableSortToBuffer(first, middle, buf, comp);
        LobbySort_StableSortToBuffer(middle, last, buf + half, comp);
        LobbySort_MergeBuffers(buf, buf + half, buf + half, buf + len, first, comp);
    } else {
        LobbySort_StableSortAdaptive(first, middle, buf, bufEnd, comp);
        LobbySort_StableSortAdaptive(middle, last, buf, bufEnd, comp);
        LobbySort_MergeAdaptive(first, middle, last, comp, middle.position - first.position, last.position - middle.position, buf, bufEnd);
    }
}
