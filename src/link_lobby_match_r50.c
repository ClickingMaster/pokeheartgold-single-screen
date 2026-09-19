#include "link_lobby_match_r50_private.h"

BOOL sub_02057C94(void) {
    u16 coords2[4];
    u16 coords4[8];
    int mapping[4];
    int count;
    int result;
    int n;
    int i;
    int j;
    const u16 *coords;

    coords2[0] = _020FC7A0[0];
    {
        const u16 *src = _020FC7A8;

        coords2[1] = _020FC7A0[1];
        coords2[2] = _020FC7A0[2];
        coords2[3] = _020FC7A0[3];
        u32 copyCount;
        u16 *dst;

        dst = coords4;
        copyCount = 8;

        do {
            *dst = *src;
            src++;
            dst++;
        } while (--copyCount != 0);
    }
    sub_0203993C();
    n = sub_0203774C();
    count = 0;
    result = FALSE;
    coords = n == 2 ? coords2 : coords4;
    for (i = 0; i < n; i++, coords += 2) {
        for (j = 0; j < n; j++) {
            if (coords[0] == sub_02057A34(j) && coords[1] == sub_02057A88(j)) {
                count++;
                mapping[j] = i;
                if (j == sub_0203769C()) {
                    result = TRUE;
                }
                break;
            }
        }
    }
    if (count == n) {
        for (i = 0; i < n; i++) {
            sub_02037894(mapping[i], i);
        }
    }
    return result;
}
