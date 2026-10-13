#pragma force_active on
#include "global.h"

#include "dwc/chat_internal.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
#pragma force_active on

// Stream cipher of the chat connection (RC4 style, with a reversed initial permutation) and a repeating-key xor.

void gs_prepare_key(const u8 *key, int keyLength, ChatCipher *cipher) {
    int i, k, j;
    for (i = 0; i < 256; i++) {
        cipher->state[255 - i] = i;
    }
    k = 0;
    cipher->x = k;
    j = k;
    i = k;
    cipher->y = k;
    for (i = 0; i < 256; i++) {
        int t = cipher->state[i];
        int x = key[k];
        x += t;
        j = ((j + x) % 256) & 255;
        cipher->state[i] = cipher->state[j];
        cipher->state[j] = t;
        k = ((k + 1) % keyLength) & 255;
    }
}

void gs_crypt(u8 *data, int length, ChatCipher *cipher) {
    int x = cipher->x, y = cipher->y, k;
    for (k = 0; k < length; k++) {
        int t;
        x = ((x + 1) % 256) & 255;
        t = cipher->state[x];
        y = ((y + t) % 256) & 255;
        cipher->state[x] = cipher->state[y];
        cipher->state[y] = t;
        {
            int idx = cipher->state[x];
            idx += t;
            data[k] ^= cipher->state[(idx % 256) & 255];
        }
    }
    cipher->x = x;
    cipher->y = y;
}

// xors data with key repeated (key is NUL-terminated).
void gs_xcode_buf(s8 *data, int length, const s8 *key) {
    int i;
    const s8 *p = key;
    for (i = 0; i < length; i++) {
        data[i] ^= *p;
        if (!*++p) {
            p = key;
        }
    }
}
