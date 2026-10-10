#pragma force_active on
#include "global.h"
int WifiSetup_AesSetEncryptKey(u32 *ctx, const void *key, int bits);
void WifiSetup_AesEncryptBlock(const u32 *ctx, int mode, const u8 *in, u8 *out);
void ov13_022256C8(const u8 *a, const u8 *b, u8 *out);
void *memcpy(void *dst, const void *src, u32 n);
int ov13_02225358(u8 *out, const u8 *in, u32 length, const void *key, u32 keylen);
int WifiSetup_AesSetDecryptKey(u32 *ctx, const void *key, int bits);
void WifiSetup_AesDecryptBlock(const u32 *ctx, int mode, const u8 *in, u8 *out);
int memcmp(const void *a, const void *b, u32 n);
int ov13_02225510(u8 *out, const u8 *in, u32 length, const void *key, u32 keylen);

typedef struct Block8 {
    u8 b[8];
} Block8;

#pragma opt_strength_reduction off
// @fn ov13_02225358
int ov13_02225358(u8 *out, const u8 *in, u32 length, const void *key, u32 keylen) {
    u32 ctx[80];
    u8 block[16];
    u64 swapped;
    u64 initial = 0xA6A6A6A6A6A6A6A6ULL;
    int n, j, i, mode;
    u64 t;

    if ((length & 7) || (keylen & 7)) {
        return 0;
    }
    n = length >> 3;
    if (n < 2) {
        return 0;
    }
    mode = WifiSetup_AesSetEncryptKey(ctx, key, keylen << 3);
    memcpy(out + 8, in, length);
    {
        u8 *d = block;
        u8 *p = (u8 *)&initial;
        u32 k = 8;
        do {
            *d++ = *p++;
        } while (--k);
    }
    for (j = 0; j < 6; j++) {
        for (i = 1; i <= n; i++) {
            u8 *r = (u8 *)&((u64 *)out)[i];
            *(Block8 *)(block + 8) = *(Block8 *)r;
            WifiSetup_AesEncryptBlock(ctx, mode, block, block);
            t = (s64)n * j + i;
            swapped = ((t & 0x00000000000000ffULL) << 56) | ((t & 0x000000000000ff00ULL) << 40) | ((t & 0x0000000000ff0000ULL) << 24) | ((t & 0x00000000ff000000ULL) << 8) | ((t & 0x000000ff00000000ULL) >> 8) | ((t & 0x0000ff0000000000ULL) >> 24) | ((t & 0x00ff000000000000ULL) >> 40) | ((t & 0xff00000000000000ULL) >> 56);
            ov13_022256C8(block, (const u8 *)&swapped, block);
            *(Block8 *)r = *(Block8 *)(block + 8);
        }
    }
    *(Block8 *)out = *(Block8 *)block;
    return 1;
}
#pragma opt_strength_reduction reset

#pragma opt_strength_reduction off
// @fn ov13_02225510
int ov13_02225510(u8 *out, const u8 *in, u32 length, const void *key, u32 keylen) {
    int n, j, i, mode;
    int result = 1;
    u32 ctx[80];
    u8 block[16];
    u64 swapped;
    u64 initial = 0xA6A6A6A6A6A6A6A6ULL;
    u64 t;

    if ((length & 7) || (keylen & 7)) {
        return 0;
    }
    n = (length - 1) >> 3;
    if (n < 2) {
        return 0;
    }
    mode = WifiSetup_AesSetDecryptKey(ctx, key, keylen << 3);
    {
        u8 *d = block;
        const u8 *p = in;
        u32 k = 8;
        do {
            *d++ = *p++;
        } while (--k);
    }
    memcpy(out, in + 8, length - 1);
    for (j = 5; j >= 0; j--) {
        for (i = n; i > 0; i--) {
            u8 *r;
            t = (s64)n * j + i;
            swapped = ((t & 0x00000000000000ffULL) << 56) | ((t & 0x000000000000ff00ULL) << 40) | ((t & 0x0000000000ff0000ULL) << 24) | ((t & 0x00000000ff000000ULL) << 8) | ((t & 0x000000ff00000000ULL) >> 8) | ((t & 0x0000ff0000000000ULL) >> 24) | ((t & 0x00ff000000000000ULL) >> 40) | ((t & 0xff00000000000000ULL) >> 56);
            ov13_022256C8(block, (const u8 *)&swapped, block);
            r = (u8 *)&((u64 *)out)[i - 1];
            *(Block8 *)(block + 8) = *(Block8 *)r;
            WifiSetup_AesDecryptBlock(ctx, mode, block, block);
            *(Block8 *)r = *(Block8 *)(block + 8);
        }
    }
    if (memcmp(&initial, block, 8) != 0) {
        result = 0;
    }
    return result;
}
#pragma opt_strength_reduction reset
