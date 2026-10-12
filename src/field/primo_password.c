#include "global.h"

#include "easy_chat.h"
#include "overlay_02.h"
#include "player_data.h"

// Primo's passwords: four easy chat words whose positions in the password word bank encode the player's trainer id
// (the visible half), a check value and the reward (bonus box wallpaper 0-7 or the reward of the second check, 8-10).

void PrimoPassword_RotateBitsRight(u8 *data, int size, u8 amount);
int PrimoPassword_DecodeWallpaper(WallpaperPasswordBank *bank, u16 trainerId, u16 word1, u16 word2, u16 word3, u16 word4);
int PrimoPassword_DecodeCheck2(WallpaperPasswordBank *bank, u16 trainerId, u16 word1, u16 word2, u16 word3, u16 word4);

// Rotates the bits of the byte array right by `amount`, the lowest bit of the last byte moving into the first byte.
void PrimoPassword_RotateBitsRight(u8 *data, int size, u8 amount) {
    u8 carry;
    int i;
    u8 *end;
    int last;
    u8 count = amount;
    count--;

    if (amount == 0) {
        return;
    }
    end = data + size;
    last = size - 1;
    do {
        carry = *(end - 1) & 1;
        i = last;
        while (i > 0) {
            data[i] >>= 1;
            data[i] |= (data[i - 1] & 1) << 7;
            i--;
        }
        data[i] >>= 1;
        data[i] |= carry << 7;
    } while (count-- != 0);
}

int PrimoPassword_DecodeWallpaper(WallpaperPasswordBank *bank, u16 trainerId, u16 word1, u16 word2, u16 word3, u16 word4) {
    s16 idx[4];
    u8 out[4];
    int count;
    int k;
    int val;
    int combined;

    count = WallpaperPasswordBank_GetCount(bank);
    idx[0] = WallpaperPasswordBank_GetIndexOfWord(bank, word1);
    idx[1] = WallpaperPasswordBank_GetIndexOfWord(bank, word2);
    idx[2] = WallpaperPasswordBank_GetIndexOfWord(bank, word3);
    idx[3] = WallpaperPasswordBank_GetIndexOfWord(bank, word4);

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

    PrimoPassword_RotateBitsRight(out, 4, 5);

    for (k = 0; k < 3; k++) {
        out[k] ^= (out[3] >> 4) | (out[3] & 0xf0);
    }

    PrimoPassword_RotateBitsRight(out, 3, out[3] & 0xf);

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

int PrimoPassword_DecodeCheck2(WallpaperPasswordBank *bank, u16 trainerId, u16 word1, u16 word2, u16 word3, u16 word4) {
    s16 idx[4];
    u8 out[4];
    int count;
    int k;
    int val;
    int combined;

    count = WallpaperPasswordBank_GetCount(bank);
    idx[0] = WallpaperPasswordBank_GetIndexOfWord(bank, word1);
    idx[1] = WallpaperPasswordBank_GetIndexOfWord(bank, word2);
    idx[2] = WallpaperPasswordBank_GetIndexOfWord(bank, word3);
    idx[3] = WallpaperPasswordBank_GetIndexOfWord(bank, word4);

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

    PrimoPassword_RotateBitsRight(out, 4, 5);

    for (k = 0; k < 3; k++) {
        out[k] ^= (out[3] >> 4) | (out[3] & 0xf0);
    }

    PrimoPassword_RotateBitsRight(out, 3, out[3] & 0xf);

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

int PrimoPassword_CheckWallpaper(PlayerProfile *profile, u16 word1, u16 word2, u16 word3, u16 word4, enum HeapID heapID) {
    WallpaperPasswordBank *bank = WallpaperPasswordBank_Create(heapID);
    int result = PrimoPassword_DecodeWallpaper(bank, PlayerProfile_GetTrainerID_VisibleHalf(profile), word1, word2, word3, word4);
    WallpaperPasswordBank_Delete(bank);
    return result;
}

int PrimoPassword_Check2(PlayerProfile *profile, u16 word1, u16 word2, u16 word3, u16 word4, enum HeapID heapID) {
    WallpaperPasswordBank *bank = WallpaperPasswordBank_Create(heapID);
    int result = PrimoPassword_DecodeCheck2(bank, PlayerProfile_GetTrainerID_VisibleHalf(profile), word1, word2, word3, word4);
    WallpaperPasswordBank_Delete(bank);
    return result;
}
