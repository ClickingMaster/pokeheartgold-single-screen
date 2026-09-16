#include "overlay02_slop_r3_private.h"

WIP_LOCAL int ov02_0224CAB8(WallpaperPasswordBank *bank, u16 trainerId, u16 a, u16 b, u16 c, u16 d) {
    s16 idx[4];
    u8 out[4];
    int count;
    int k;
    int val;
    int combined;

    count = WallpaperPasswordBank_GetCount(bank);
    idx[0] = WallpaperPasswordBank_GetIndexOfWord(bank, a);
    idx[1] = WallpaperPasswordBank_GetIndexOfWord(bank, b);
    idx[2] = WallpaperPasswordBank_GetIndexOfWord(bank, c);
    idx[3] = WallpaperPasswordBank_GetIndexOfWord(bank, d);

    for (k = 0; k < 4; k++) {
        if (idx[k] < 0) {
            return -1;
        }
        if (k > 0) {
            if (idx[k] >= idx[k - 1]) {
                int v = idx[k] - idx[k - 1];
                if (v > 0xff) {
                    return -1;
                }
                out[k] = v;
            } else {
                int v = count - (idx[k - 1] - idx[k]);
                if (v > 0xff) {
                    return -1;
                }
                out[k] = v;
            }
        } else {
            if (idx[0] > 0xff) {
                return -1;
            }
            out[0] = idx[0];
        }
    }

    ov02_0224CA58(out, 4, 5);

    for (k = 0; k < 3; k++) {
        out[k] ^= (out[3] >> 4) | (out[3] & 0xf0);
    }

    ov02_0224CA58(out, 3, out[3] & 0xf);

    val = out[0] & 0xf;
    if (val >= 8) {
        return -1;
    }
    out[1] ^= out[0];
    out[2] ^= out[0];
    combined = (out[1] << 8) | out[2];
    if (trainerId == combined && ((out[0] & 0xf0) >> 4) == 6 && out[3] == (u8)((out[0] + out[1]) * out[2])) {
        return val;
    }
    return -1;
}

WIP_LOCAL int ov02_0224CBF8(WallpaperPasswordBank *bank, u16 trainerId, u16 a, u16 b, u16 c, u16 d) {
    s16 idx[4];
    u8 out[4];
    int count;
    int k;
    int val;
    int combined;

    count = WallpaperPasswordBank_GetCount(bank);
    idx[0] = WallpaperPasswordBank_GetIndexOfWord(bank, a);
    idx[1] = WallpaperPasswordBank_GetIndexOfWord(bank, b);
    idx[2] = WallpaperPasswordBank_GetIndexOfWord(bank, c);
    idx[3] = WallpaperPasswordBank_GetIndexOfWord(bank, d);

    for (k = 0; k < 4; k++) {
        if (idx[k] < 0) {
            return -1;
        }
        if (k > 0) {
            if (idx[k] >= idx[k - 1]) {
                int v = idx[k] - idx[k - 1];
                if (v > 0xff) {
                    return -1;
                }
                out[k] = v;
            } else {
                int v = count - (idx[k - 1] - idx[k]);
                if (v > 0xff) {
                    return -1;
                }
                out[k] = v;
            }
        } else {
            if (idx[0] > 0xff) {
                return -1;
            }
            out[0] = idx[0];
        }
    }

    ov02_0224CA58(out, 4, 5);

    for (k = 0; k < 3; k++) {
        out[k] ^= (out[3] >> 4) | (out[3] & 0xf0);
    }

    ov02_0224CA58(out, 3, out[3] & 0xf);

    val = out[0] & 0xf;
    if ((u8)val < 8 || (u8)val >= 0xb) {
        return -1;
    }
    out[1] ^= out[0];
    out[2] ^= out[0];
    combined = (out[1] << 8) | out[2];
    if (trainerId == combined && ((out[0] & 0xf0) >> 4) == 6 && out[3] == (u8)((out[0] + out[1]) * out[2])) {
        return val;
    }
    return -1;
}
