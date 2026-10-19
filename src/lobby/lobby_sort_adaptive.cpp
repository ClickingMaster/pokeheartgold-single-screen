#include <nitro/types.h>

#include "lobby_sort.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

using namespace LobbySort;

inline s32 Distance(Cursor first, Cursor last) {
    return last - first;
}

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

extern "C" void LobbySort_MergeAdaptive(Cursor first, Cursor middle, Cursor last, const Less &comp, s32 len1, s32 len2, ConstructingOut buf, ConstructingOut bufEnd) {
    Cursor first_cut, second_cut;
    s32 len11, len22;
    if (len1 == 0 || len2 == 0) {
        return;
    }
    if (len1 + len2 == 2) {
        if (comp(*middle, *first)) {
            IterSwap(first, middle);
        }
        return;
    }
    s32 cap = bufEnd - buf;
    if (len1 <= len2 && len1 <= cap) {
        LobbySort_MergeBufferForward(buf, CopyForward(first, middle, buf), middle, last, first, comp);
    } else if (len2 <= cap) {
        LobbySort_MergeBackward(first, middle, buf, CopyForward(middle, last, buf), last, comp);
    } else {
        if (len1 <= len2) {
            len22 = len2 / 2;
            second_cut = middle + len22;
            first_cut = LobbySort_UpperBound(first, middle, *second_cut, comp);
            len11 = Distance(first, first_cut);
        } else {
            len11 = len1 / 2;
            first_cut = first + len11;
            second_cut = LobbySort_LowerBound(middle, last, *first_cut, comp);
            len22 = Distance(middle, second_cut);
        }
        middle = LobbySort_Rotate(first_cut, middle, second_cut);
        LobbySort_MergeAdaptive(first, first_cut, middle, comp, len11, len22, buf, bufEnd);
        LobbySort_MergeAdaptive(middle, second_cut, last, comp, len1 - len11, len2 - len22, buf, bufEnd);
    }
}
