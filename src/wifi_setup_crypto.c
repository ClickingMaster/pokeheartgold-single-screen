#include "global.h"

#include "wifi_setup_internal.h"

typedef struct WifiSetupBlock8 {
    u8 bytes[8];
} WifiSetupBlock8;

extern const u32 ov13_02242688[];
extern const u32 ov13_022426B0[];
extern const u32 ov13_02242AB0[];
extern const u32 ov13_02242EB0[];
extern const u32 ov13_022432B0[];
extern const u32 ov13_022436B0[];
extern const u32 ov13_02243AB0[];
extern const u32 ov13_02243EB0[];
extern const u32 ov13_022442B0[];
extern const u32 ov13_022446B0[];
extern const u32 ov13_02244AB0[];

// AES tables
#define rcon ov13_02242688
#define Td2  ov13_022426B0
#define Td3  ov13_02242AB0
#define Td4  ov13_02242EB0
#define Te0  ov13_022432B0
#define Te1  ov13_022436B0
#define Te3  ov13_02243EB0
#define Te2  ov13_02243AB0
#define Te4  ov13_022442B0
#define Td0  ov13_022446B0
#define Td1  ov13_02244AB0

#define GETU32(pt) (((u32)(pt)[0] << 24) ^ ((u32)(pt)[1] << 16) ^ ((u32)(pt)[2] << 8) ^ ((u32)(pt)[3]))
#define PUTU32(ct, st)              \
    {                               \
        (ct)[0] = (u8)((st) >> 24); \
        (ct)[1] = (u8)((st) >> 16); \
        (ct)[2] = (u8)((st) >> 8);  \
        (ct)[3] = (u8)(st);         \
    }

// Worker thread: runs the setup session, then reports the final state.
void ov13_02225320(void *arg) {
    int result = WifiSetup_RunSession();

    ov13_0224DF30.error = result;
    WifiSetupLink_DisconnectAp();
    if (result == 1) {
        ov13_0224DF30.state = 6;
    } else {
        ov13_0224DF30.state = 7;
    }
    ov13_02245A58.deadline = -1;
    ov13_02226F3C();
}

#pragma force_active on
// AES key wrap with the default initial value 0xA6A6A6A6A6A6A6A6.

#pragma opt_strength_reduction off

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
            *(WifiSetupBlock8 *)(block + 8) = *(WifiSetupBlock8 *)r;
            WifiSetup_AesEncryptBlock(ctx, mode, block, block);
            t = (s64)n * j + i;
            swapped = ((t & 0x00000000000000ffULL) << 56) | ((t & 0x000000000000ff00ULL) << 40) | ((t & 0x0000000000ff0000ULL) << 24) | ((t & 0x00000000ff000000ULL) << 8) | ((t & 0x000000ff00000000ULL) >> 8) | ((t & 0x0000ff0000000000ULL) >> 24) | ((t & 0x00ff000000000000ULL) >> 40) | ((t & 0xff00000000000000ULL) >> 56);
            ov13_022256C8(block, (const u8 *)&swapped, block);
            *(WifiSetupBlock8 *)r = *(WifiSetupBlock8 *)(block + 8);
        }
    }
    *(WifiSetupBlock8 *)out = *(WifiSetupBlock8 *)block;
    return 1;
}

#pragma opt_strength_reduction reset

#pragma opt_strength_reduction off

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
            *(WifiSetupBlock8 *)(block + 8) = *(WifiSetupBlock8 *)r;
            WifiSetup_AesDecryptBlock(ctx, mode, block, block);
            *(WifiSetupBlock8 *)r = *(WifiSetupBlock8 *)(block + 8);
        }
    }
    if (memcmp(&initial, block, 8) != 0) {
        result = 0;
    }
    return result;
}

#pragma opt_strength_reduction reset
#pragma force_active reset

void ov13_022256C8(const u8 *a, const u8 *b, u8 *out) {
    out[0] = a[0] ^ b[0];
    out[1] = a[1] ^ b[1];
    out[2] = a[2] ^ b[2];
    out[3] = a[3] ^ b[3];
    out[4] = a[4] ^ b[4];
    out[5] = a[5] ^ b[5];
    out[6] = a[6] ^ b[6];
    out[7] = a[7] ^ b[7];
}

int WifiSetup_AesSetEncryptKey(u32 rk[/*4*(Nr + 1)*/], const u8 cipherKey[], int keyBits) {
    int i = 0;
    u32 temp;

    rk[0] = GETU32(cipherKey);
    rk[1] = GETU32(cipherKey + 4);
    rk[2] = GETU32(cipherKey + 8);
    rk[3] = GETU32(cipherKey + 12);
    if (keyBits == 128) {
        for (;;) {
            temp = rk[3];
            rk[4] = rk[0] ^ (Te4[(temp >> 16) & 0xff] & 0xff000000) ^ (Te4[(temp >> 8) & 0xff] & 0x00ff0000) ^ (Te4[(temp) & 0xff] & 0x0000ff00) ^ (Te4[(temp >> 24)] & 0x000000ff) ^ rcon[i];
            rk[5] = rk[1] ^ rk[4];
            rk[6] = rk[2] ^ rk[5];
            rk[7] = rk[3] ^ rk[6];
            if (++i >= 10) {
                return 10;
            }
            rk += 4;
        }
    }
    rk[4] = GETU32(cipherKey + 16);
    rk[5] = GETU32(cipherKey + 20);
    if (keyBits == 192) {
        for (;;) {
            temp = rk[5];
            rk[6] = rk[0] ^ (Te4[(temp >> 16) & 0xff] & 0xff000000) ^ (Te4[(temp >> 8) & 0xff] & 0x00ff0000) ^ (Te4[(temp) & 0xff] & 0x0000ff00) ^ (Te4[(temp >> 24)] & 0x000000ff) ^ rcon[i];
            rk[7] = rk[1] ^ rk[6];
            rk[8] = rk[2] ^ rk[7];
            rk[9] = rk[3] ^ rk[8];
            if (++i >= 8) {
                return 12;
            }
            rk[10] = rk[4] ^ rk[9];
            rk[11] = rk[5] ^ rk[10];
            rk += 6;
        }
    }
    rk[6] = GETU32(cipherKey + 24);
    rk[7] = GETU32(cipherKey + 28);
    if (keyBits == 256) {
        for (;;) {
            temp = rk[7];
            rk[8] = rk[0] ^ (Te4[(temp >> 16) & 0xff] & 0xff000000) ^ (Te4[(temp >> 8) & 0xff] & 0x00ff0000) ^ (Te4[(temp) & 0xff] & 0x0000ff00) ^ (Te4[(temp >> 24)] & 0x000000ff) ^ rcon[i];
            rk[9] = rk[1] ^ rk[8];
            rk[10] = rk[2] ^ rk[9];
            rk[11] = rk[3] ^ rk[10];
            if (++i >= 7) {
                return 14;
            }
            temp = rk[11];
            rk[12] = rk[4] ^ (Te4[(temp >> 24)] & 0xff000000) ^ (Te4[(temp >> 16) & 0xff] & 0x00ff0000) ^ (Te4[(temp >> 8) & 0xff] & 0x0000ff00) ^ (Te4[(temp) & 0xff] & 0x000000ff);
            rk[13] = rk[5] ^ rk[12];
            rk[14] = rk[6] ^ rk[13];
            rk[15] = rk[7] ^ rk[14];

            rk += 8;
        }
    }
    return 0;
}

int WifiSetup_AesSetDecryptKey(u32 rk[/*4*(Nr + 1)*/], const u8 cipherKey[], int keyBits) {
    int Nr, i, j;
    u32 temp;

    /* expand the cipher key: */
    Nr = WifiSetup_AesSetEncryptKey(rk, cipherKey, keyBits);
    /* invert the order of the round keys: */
    for (i = 0, j = 4 * Nr; i < j; i += 4, j -= 4) {
        temp = rk[i];
        rk[i] = rk[j];
        rk[j] = temp;
        temp = rk[i + 1];
        rk[i + 1] = rk[j + 1];
        rk[j + 1] = temp;
        temp = rk[i + 2];
        rk[i + 2] = rk[j + 2];
        rk[j + 2] = temp;
        temp = rk[i + 3];
        rk[i + 3] = rk[j + 3];
        rk[j + 3] = temp;
    }
    /* apply the inverse MixColumn transform to all round keys but the first and the last: */
    for (i = 1; i < Nr; i++) {
        rk += 4;
        rk[0] = Td0[Te4[(rk[0] >> 24)] & 0xff] ^ Td1[Te4[(rk[0] >> 16) & 0xff] & 0xff] ^ Td2[Te4[(rk[0] >> 8) & 0xff] & 0xff] ^ Td3[Te4[(rk[0]) & 0xff] & 0xff];
        rk[1] = Td0[Te4[(rk[1] >> 24)] & 0xff] ^ Td1[Te4[(rk[1] >> 16) & 0xff] & 0xff] ^ Td2[Te4[(rk[1] >> 8) & 0xff] & 0xff] ^ Td3[Te4[(rk[1]) & 0xff] & 0xff];
        rk[2] = Td0[Te4[(rk[2] >> 24)] & 0xff] ^ Td1[Te4[(rk[2] >> 16) & 0xff] & 0xff] ^ Td2[Te4[(rk[2] >> 8) & 0xff] & 0xff] ^ Td3[Te4[(rk[2]) & 0xff] & 0xff];
        rk[3] = Td0[Te4[(rk[3] >> 24)] & 0xff] ^ Td1[Te4[(rk[3] >> 16) & 0xff] & 0xff] ^ Td2[Te4[(rk[3] >> 8) & 0xff] & 0xff] ^ Td3[Te4[(rk[3]) & 0xff] & 0xff];
    }
    return Nr;
}

void WifiSetup_AesEncryptBlock(const u32 rk[/*4*(Nr + 1)*/], int Nr, const u8 pt[16], u8 ct[16]) {
    u32 s0, s1, s2, s3, t0, t1, t2, t3;
#ifndef FULL_UNROLL
    int r;
#endif /* ?FULL_UNROLL */

    /*
     * map byte array block to cipher state
     * and add initial round key:
     */
    s0 = GETU32(pt) ^ rk[0];
    s1 = GETU32(pt + 4) ^ rk[1];
    s2 = GETU32(pt + 8) ^ rk[2];
    s3 = GETU32(pt + 12) ^ rk[3];
#ifdef FULL_UNROLL
    /* round 1: */
    t0 = Te0[s0 >> 24] ^ Te1[(s1 >> 16) & 0xff] ^ Te2[(s2 >> 8) & 0xff] ^ Te3[s3 & 0xff] ^ rk[4];
    t1 = Te0[s1 >> 24] ^ Te1[(s2 >> 16) & 0xff] ^ Te2[(s3 >> 8) & 0xff] ^ Te3[s0 & 0xff] ^ rk[5];
    t2 = Te0[s2 >> 24] ^ Te1[(s3 >> 16) & 0xff] ^ Te2[(s0 >> 8) & 0xff] ^ Te3[s1 & 0xff] ^ rk[6];
    t3 = Te0[s3 >> 24] ^ Te1[(s0 >> 16) & 0xff] ^ Te2[(s1 >> 8) & 0xff] ^ Te3[s2 & 0xff] ^ rk[7];
    /* round 2: */
    s0 = Te0[t0 >> 24] ^ Te1[(t1 >> 16) & 0xff] ^ Te2[(t2 >> 8) & 0xff] ^ Te3[t3 & 0xff] ^ rk[8];
    s1 = Te0[t1 >> 24] ^ Te1[(t2 >> 16) & 0xff] ^ Te2[(t3 >> 8) & 0xff] ^ Te3[t0 & 0xff] ^ rk[9];
    s2 = Te0[t2 >> 24] ^ Te1[(t3 >> 16) & 0xff] ^ Te2[(t0 >> 8) & 0xff] ^ Te3[t1 & 0xff] ^ rk[10];
    s3 = Te0[t3 >> 24] ^ Te1[(t0 >> 16) & 0xff] ^ Te2[(t1 >> 8) & 0xff] ^ Te3[t2 & 0xff] ^ rk[11];
    /* round 3: */
    t0 = Te0[s0 >> 24] ^ Te1[(s1 >> 16) & 0xff] ^ Te2[(s2 >> 8) & 0xff] ^ Te3[s3 & 0xff] ^ rk[12];
    t1 = Te0[s1 >> 24] ^ Te1[(s2 >> 16) & 0xff] ^ Te2[(s3 >> 8) & 0xff] ^ Te3[s0 & 0xff] ^ rk[13];
    t2 = Te0[s2 >> 24] ^ Te1[(s3 >> 16) & 0xff] ^ Te2[(s0 >> 8) & 0xff] ^ Te3[s1 & 0xff] ^ rk[14];
    t3 = Te0[s3 >> 24] ^ Te1[(s0 >> 16) & 0xff] ^ Te2[(s1 >> 8) & 0xff] ^ Te3[s2 & 0xff] ^ rk[15];
    /* round 4: */
    s0 = Te0[t0 >> 24] ^ Te1[(t1 >> 16) & 0xff] ^ Te2[(t2 >> 8) & 0xff] ^ Te3[t3 & 0xff] ^ rk[16];
    s1 = Te0[t1 >> 24] ^ Te1[(t2 >> 16) & 0xff] ^ Te2[(t3 >> 8) & 0xff] ^ Te3[t0 & 0xff] ^ rk[17];
    s2 = Te0[t2 >> 24] ^ Te1[(t3 >> 16) & 0xff] ^ Te2[(t0 >> 8) & 0xff] ^ Te3[t1 & 0xff] ^ rk[18];
    s3 = Te0[t3 >> 24] ^ Te1[(t0 >> 16) & 0xff] ^ Te2[(t1 >> 8) & 0xff] ^ Te3[t2 & 0xff] ^ rk[19];
    /* round 5: */
    t0 = Te0[s0 >> 24] ^ Te1[(s1 >> 16) & 0xff] ^ Te2[(s2 >> 8) & 0xff] ^ Te3[s3 & 0xff] ^ rk[20];
    t1 = Te0[s1 >> 24] ^ Te1[(s2 >> 16) & 0xff] ^ Te2[(s3 >> 8) & 0xff] ^ Te3[s0 & 0xff] ^ rk[21];
    t2 = Te0[s2 >> 24] ^ Te1[(s3 >> 16) & 0xff] ^ Te2[(s0 >> 8) & 0xff] ^ Te3[s1 & 0xff] ^ rk[22];
    t3 = Te0[s3 >> 24] ^ Te1[(s0 >> 16) & 0xff] ^ Te2[(s1 >> 8) & 0xff] ^ Te3[s2 & 0xff] ^ rk[23];
    /* round 6: */
    s0 = Te0[t0 >> 24] ^ Te1[(t1 >> 16) & 0xff] ^ Te2[(t2 >> 8) & 0xff] ^ Te3[t3 & 0xff] ^ rk[24];
    s1 = Te0[t1 >> 24] ^ Te1[(t2 >> 16) & 0xff] ^ Te2[(t3 >> 8) & 0xff] ^ Te3[t0 & 0xff] ^ rk[25];
    s2 = Te0[t2 >> 24] ^ Te1[(t3 >> 16) & 0xff] ^ Te2[(t0 >> 8) & 0xff] ^ Te3[t1 & 0xff] ^ rk[26];
    s3 = Te0[t3 >> 24] ^ Te1[(t0 >> 16) & 0xff] ^ Te2[(t1 >> 8) & 0xff] ^ Te3[t2 & 0xff] ^ rk[27];
    /* round 7: */
    t0 = Te0[s0 >> 24] ^ Te1[(s1 >> 16) & 0xff] ^ Te2[(s2 >> 8) & 0xff] ^ Te3[s3 & 0xff] ^ rk[28];
    t1 = Te0[s1 >> 24] ^ Te1[(s2 >> 16) & 0xff] ^ Te2[(s3 >> 8) & 0xff] ^ Te3[s0 & 0xff] ^ rk[29];
    t2 = Te0[s2 >> 24] ^ Te1[(s3 >> 16) & 0xff] ^ Te2[(s0 >> 8) & 0xff] ^ Te3[s1 & 0xff] ^ rk[30];
    t3 = Te0[s3 >> 24] ^ Te1[(s0 >> 16) & 0xff] ^ Te2[(s1 >> 8) & 0xff] ^ Te3[s2 & 0xff] ^ rk[31];
    /* round 8: */
    s0 = Te0[t0 >> 24] ^ Te1[(t1 >> 16) & 0xff] ^ Te2[(t2 >> 8) & 0xff] ^ Te3[t3 & 0xff] ^ rk[32];
    s1 = Te0[t1 >> 24] ^ Te1[(t2 >> 16) & 0xff] ^ Te2[(t3 >> 8) & 0xff] ^ Te3[t0 & 0xff] ^ rk[33];
    s2 = Te0[t2 >> 24] ^ Te1[(t3 >> 16) & 0xff] ^ Te2[(t0 >> 8) & 0xff] ^ Te3[t1 & 0xff] ^ rk[34];
    s3 = Te0[t3 >> 24] ^ Te1[(t0 >> 16) & 0xff] ^ Te2[(t1 >> 8) & 0xff] ^ Te3[t2 & 0xff] ^ rk[35];
    /* round 9: */
    t0 = Te0[s0 >> 24] ^ Te1[(s1 >> 16) & 0xff] ^ Te2[(s2 >> 8) & 0xff] ^ Te3[s3 & 0xff] ^ rk[36];
    t1 = Te0[s1 >> 24] ^ Te1[(s2 >> 16) & 0xff] ^ Te2[(s3 >> 8) & 0xff] ^ Te3[s0 & 0xff] ^ rk[37];
    t2 = Te0[s2 >> 24] ^ Te1[(s3 >> 16) & 0xff] ^ Te2[(s0 >> 8) & 0xff] ^ Te3[s1 & 0xff] ^ rk[38];
    t3 = Te0[s3 >> 24] ^ Te1[(s0 >> 16) & 0xff] ^ Te2[(s1 >> 8) & 0xff] ^ Te3[s2 & 0xff] ^ rk[39];
    if (Nr > 10) {
        /* round 10: */
        s0 = Te0[t0 >> 24] ^ Te1[(t1 >> 16) & 0xff] ^ Te2[(t2 >> 8) & 0xff] ^ Te3[t3 & 0xff] ^ rk[40];
        s1 = Te0[t1 >> 24] ^ Te1[(t2 >> 16) & 0xff] ^ Te2[(t3 >> 8) & 0xff] ^ Te3[t0 & 0xff] ^ rk[41];
        s2 = Te0[t2 >> 24] ^ Te1[(t3 >> 16) & 0xff] ^ Te2[(t0 >> 8) & 0xff] ^ Te3[t1 & 0xff] ^ rk[42];
        s3 = Te0[t3 >> 24] ^ Te1[(t0 >> 16) & 0xff] ^ Te2[(t1 >> 8) & 0xff] ^ Te3[t2 & 0xff] ^ rk[43];
        /* round 11: */
        t0 = Te0[s0 >> 24] ^ Te1[(s1 >> 16) & 0xff] ^ Te2[(s2 >> 8) & 0xff] ^ Te3[s3 & 0xff] ^ rk[44];
        t1 = Te0[s1 >> 24] ^ Te1[(s2 >> 16) & 0xff] ^ Te2[(s3 >> 8) & 0xff] ^ Te3[s0 & 0xff] ^ rk[45];
        t2 = Te0[s2 >> 24] ^ Te1[(s3 >> 16) & 0xff] ^ Te2[(s0 >> 8) & 0xff] ^ Te3[s1 & 0xff] ^ rk[46];
        t3 = Te0[s3 >> 24] ^ Te1[(s0 >> 16) & 0xff] ^ Te2[(s1 >> 8) & 0xff] ^ Te3[s2 & 0xff] ^ rk[47];
        if (Nr > 12) {
            /* round 12: */
            s0 = Te0[t0 >> 24] ^ Te1[(t1 >> 16) & 0xff] ^ Te2[(t2 >> 8) & 0xff] ^ Te3[t3 & 0xff] ^ rk[48];
            s1 = Te0[t1 >> 24] ^ Te1[(t2 >> 16) & 0xff] ^ Te2[(t3 >> 8) & 0xff] ^ Te3[t0 & 0xff] ^ rk[49];
            s2 = Te0[t2 >> 24] ^ Te1[(t3 >> 16) & 0xff] ^ Te2[(t0 >> 8) & 0xff] ^ Te3[t1 & 0xff] ^ rk[50];
            s3 = Te0[t3 >> 24] ^ Te1[(t0 >> 16) & 0xff] ^ Te2[(t1 >> 8) & 0xff] ^ Te3[t2 & 0xff] ^ rk[51];
            /* round 13: */
            t0 = Te0[s0 >> 24] ^ Te1[(s1 >> 16) & 0xff] ^ Te2[(s2 >> 8) & 0xff] ^ Te3[s3 & 0xff] ^ rk[52];
            t1 = Te0[s1 >> 24] ^ Te1[(s2 >> 16) & 0xff] ^ Te2[(s3 >> 8) & 0xff] ^ Te3[s0 & 0xff] ^ rk[53];
            t2 = Te0[s2 >> 24] ^ Te1[(s3 >> 16) & 0xff] ^ Te2[(s0 >> 8) & 0xff] ^ Te3[s1 & 0xff] ^ rk[54];
            t3 = Te0[s3 >> 24] ^ Te1[(s0 >> 16) & 0xff] ^ Te2[(s1 >> 8) & 0xff] ^ Te3[s2 & 0xff] ^ rk[55];
        }
    }
    rk += Nr << 2;
#else  /* !FULL_UNROLL */
    /*
     * Nr - 1 full rounds:
     */
    r = Nr >> 1;
    for (;;) {
        t0 = Te0[(s0 >> 24)] ^ Te1[(s1 >> 16) & 0xff] ^ Te2[(s2 >> 8) & 0xff] ^ Te3[(s3) & 0xff] ^ rk[4];
        t1 = Te0[(s1 >> 24)] ^ Te1[(s2 >> 16) & 0xff] ^ Te2[(s3 >> 8) & 0xff] ^ Te3[(s0) & 0xff] ^ rk[5];
        t2 = Te0[(s2 >> 24)] ^ Te1[(s3 >> 16) & 0xff] ^ Te2[(s0 >> 8) & 0xff] ^ Te3[(s1) & 0xff] ^ rk[6];
        t3 = Te0[(s3 >> 24)] ^ Te1[(s0 >> 16) & 0xff] ^ Te2[(s1 >> 8) & 0xff] ^ Te3[(s2) & 0xff] ^ rk[7];

        rk += 8;
        if (--r == 0) {
            break;
        }

        s0 = Te0[(t0 >> 24)] ^ Te1[(t1 >> 16) & 0xff] ^ Te2[(t2 >> 8) & 0xff] ^ Te3[(t3) & 0xff] ^ rk[0];
        s1 = Te0[(t1 >> 24)] ^ Te1[(t2 >> 16) & 0xff] ^ Te2[(t3 >> 8) & 0xff] ^ Te3[(t0) & 0xff] ^ rk[1];
        s2 = Te0[(t2 >> 24)] ^ Te1[(t3 >> 16) & 0xff] ^ Te2[(t0 >> 8) & 0xff] ^ Te3[(t1) & 0xff] ^ rk[2];
        s3 = Te0[(t3 >> 24)] ^ Te1[(t0 >> 16) & 0xff] ^ Te2[(t1 >> 8) & 0xff] ^ Te3[(t2) & 0xff] ^ rk[3];
    }
#endif /* ?FULL_UNROLL */
       /*
        * apply last round and
        * map cipher state to byte array block:
        */
    s0 = (Te4[(t0 >> 24)] & 0xff000000) ^ (Te4[(t1 >> 16) & 0xff] & 0x00ff0000) ^ (Te4[(t2 >> 8) & 0xff] & 0x0000ff00) ^ (Te4[(t3) & 0xff] & 0x000000ff) ^ rk[0];
    PUTU32(ct, s0);
    s1 = (Te4[(t1 >> 24)] & 0xff000000) ^ (Te4[(t2 >> 16) & 0xff] & 0x00ff0000) ^ (Te4[(t3 >> 8) & 0xff] & 0x0000ff00) ^ (Te4[(t0) & 0xff] & 0x000000ff) ^ rk[1];
    PUTU32(ct + 4, s1);
    s2 = (Te4[(t2 >> 24)] & 0xff000000) ^ (Te4[(t3 >> 16) & 0xff] & 0x00ff0000) ^ (Te4[(t0 >> 8) & 0xff] & 0x0000ff00) ^ (Te4[(t1) & 0xff] & 0x000000ff) ^ rk[2];
    PUTU32(ct + 8, s2);
    s3 = (Te4[(t3 >> 24)] & 0xff000000) ^ (Te4[(t0 >> 16) & 0xff] & 0x00ff0000) ^ (Te4[(t1 >> 8) & 0xff] & 0x0000ff00) ^ (Te4[(t2) & 0xff] & 0x000000ff) ^ rk[3];
    PUTU32(ct + 12, s3);
}

void WifiSetup_AesDecryptBlock(const u32 rk[/*4*(Nr + 1)*/], int Nr, const u8 ct[16], u8 pt[16]) {
    u32 s0, s1, s2, s3, t0, t1, t2, t3;
#ifndef FULL_UNROLL
    int r;
#endif /* ?FULL_UNROLL */

    /*
     * map byte array block to cipher state
     * and add initial round key:
     */
    s0 = GETU32(ct) ^ rk[0];
    s1 = GETU32(ct + 4) ^ rk[1];
    s2 = GETU32(ct + 8) ^ rk[2];
    s3 = GETU32(ct + 12) ^ rk[3];
#ifdef FULL_UNROLL
    /* round 1: */
    t0 = Td0[s0 >> 24] ^ Td1[(s3 >> 16) & 0xff] ^ Td2[(s2 >> 8) & 0xff] ^ Td3[s1 & 0xff] ^ rk[4];
    t1 = Td0[s1 >> 24] ^ Td1[(s0 >> 16) & 0xff] ^ Td2[(s3 >> 8) & 0xff] ^ Td3[s2 & 0xff] ^ rk[5];
    t2 = Td0[s2 >> 24] ^ Td1[(s1 >> 16) & 0xff] ^ Td2[(s0 >> 8) & 0xff] ^ Td3[s3 & 0xff] ^ rk[6];
    t3 = Td0[s3 >> 24] ^ Td1[(s2 >> 16) & 0xff] ^ Td2[(s1 >> 8) & 0xff] ^ Td3[s0 & 0xff] ^ rk[7];
    /* round 2: */
    s0 = Td0[t0 >> 24] ^ Td1[(t3 >> 16) & 0xff] ^ Td2[(t2 >> 8) & 0xff] ^ Td3[t1 & 0xff] ^ rk[8];
    s1 = Td0[t1 >> 24] ^ Td1[(t0 >> 16) & 0xff] ^ Td2[(t3 >> 8) & 0xff] ^ Td3[t2 & 0xff] ^ rk[9];
    s2 = Td0[t2 >> 24] ^ Td1[(t1 >> 16) & 0xff] ^ Td2[(t0 >> 8) & 0xff] ^ Td3[t3 & 0xff] ^ rk[10];
    s3 = Td0[t3 >> 24] ^ Td1[(t2 >> 16) & 0xff] ^ Td2[(t1 >> 8) & 0xff] ^ Td3[t0 & 0xff] ^ rk[11];
    /* round 3: */
    t0 = Td0[s0 >> 24] ^ Td1[(s3 >> 16) & 0xff] ^ Td2[(s2 >> 8) & 0xff] ^ Td3[s1 & 0xff] ^ rk[12];
    t1 = Td0[s1 >> 24] ^ Td1[(s0 >> 16) & 0xff] ^ Td2[(s3 >> 8) & 0xff] ^ Td3[s2 & 0xff] ^ rk[13];
    t2 = Td0[s2 >> 24] ^ Td1[(s1 >> 16) & 0xff] ^ Td2[(s0 >> 8) & 0xff] ^ Td3[s3 & 0xff] ^ rk[14];
    t3 = Td0[s3 >> 24] ^ Td1[(s2 >> 16) & 0xff] ^ Td2[(s1 >> 8) & 0xff] ^ Td3[s0 & 0xff] ^ rk[15];
    /* round 4: */
    s0 = Td0[t0 >> 24] ^ Td1[(t3 >> 16) & 0xff] ^ Td2[(t2 >> 8) & 0xff] ^ Td3[t1 & 0xff] ^ rk[16];
    s1 = Td0[t1 >> 24] ^ Td1[(t0 >> 16) & 0xff] ^ Td2[(t3 >> 8) & 0xff] ^ Td3[t2 & 0xff] ^ rk[17];
    s2 = Td0[t2 >> 24] ^ Td1[(t1 >> 16) & 0xff] ^ Td2[(t0 >> 8) & 0xff] ^ Td3[t3 & 0xff] ^ rk[18];
    s3 = Td0[t3 >> 24] ^ Td1[(t2 >> 16) & 0xff] ^ Td2[(t1 >> 8) & 0xff] ^ Td3[t0 & 0xff] ^ rk[19];
    /* round 5: */
    t0 = Td0[s0 >> 24] ^ Td1[(s3 >> 16) & 0xff] ^ Td2[(s2 >> 8) & 0xff] ^ Td3[s1 & 0xff] ^ rk[20];
    t1 = Td0[s1 >> 24] ^ Td1[(s0 >> 16) & 0xff] ^ Td2[(s3 >> 8) & 0xff] ^ Td3[s2 & 0xff] ^ rk[21];
    t2 = Td0[s2 >> 24] ^ Td1[(s1 >> 16) & 0xff] ^ Td2[(s0 >> 8) & 0xff] ^ Td3[s3 & 0xff] ^ rk[22];
    t3 = Td0[s3 >> 24] ^ Td1[(s2 >> 16) & 0xff] ^ Td2[(s1 >> 8) & 0xff] ^ Td3[s0 & 0xff] ^ rk[23];
    /* round 6: */
    s0 = Td0[t0 >> 24] ^ Td1[(t3 >> 16) & 0xff] ^ Td2[(t2 >> 8) & 0xff] ^ Td3[t1 & 0xff] ^ rk[24];
    s1 = Td0[t1 >> 24] ^ Td1[(t0 >> 16) & 0xff] ^ Td2[(t3 >> 8) & 0xff] ^ Td3[t2 & 0xff] ^ rk[25];
    s2 = Td0[t2 >> 24] ^ Td1[(t1 >> 16) & 0xff] ^ Td2[(t0 >> 8) & 0xff] ^ Td3[t3 & 0xff] ^ rk[26];
    s3 = Td0[t3 >> 24] ^ Td1[(t2 >> 16) & 0xff] ^ Td2[(t1 >> 8) & 0xff] ^ Td3[t0 & 0xff] ^ rk[27];
    /* round 7: */
    t0 = Td0[s0 >> 24] ^ Td1[(s3 >> 16) & 0xff] ^ Td2[(s2 >> 8) & 0xff] ^ Td3[s1 & 0xff] ^ rk[28];
    t1 = Td0[s1 >> 24] ^ Td1[(s0 >> 16) & 0xff] ^ Td2[(s3 >> 8) & 0xff] ^ Td3[s2 & 0xff] ^ rk[29];
    t2 = Td0[s2 >> 24] ^ Td1[(s1 >> 16) & 0xff] ^ Td2[(s0 >> 8) & 0xff] ^ Td3[s3 & 0xff] ^ rk[30];
    t3 = Td0[s3 >> 24] ^ Td1[(s2 >> 16) & 0xff] ^ Td2[(s1 >> 8) & 0xff] ^ Td3[s0 & 0xff] ^ rk[31];
    /* round 8: */
    s0 = Td0[t0 >> 24] ^ Td1[(t3 >> 16) & 0xff] ^ Td2[(t2 >> 8) & 0xff] ^ Td3[t1 & 0xff] ^ rk[32];
    s1 = Td0[t1 >> 24] ^ Td1[(t0 >> 16) & 0xff] ^ Td2[(t3 >> 8) & 0xff] ^ Td3[t2 & 0xff] ^ rk[33];
    s2 = Td0[t2 >> 24] ^ Td1[(t1 >> 16) & 0xff] ^ Td2[(t0 >> 8) & 0xff] ^ Td3[t3 & 0xff] ^ rk[34];
    s3 = Td0[t3 >> 24] ^ Td1[(t2 >> 16) & 0xff] ^ Td2[(t1 >> 8) & 0xff] ^ Td3[t0 & 0xff] ^ rk[35];
    /* round 9: */
    t0 = Td0[s0 >> 24] ^ Td1[(s3 >> 16) & 0xff] ^ Td2[(s2 >> 8) & 0xff] ^ Td3[s1 & 0xff] ^ rk[36];
    t1 = Td0[s1 >> 24] ^ Td1[(s0 >> 16) & 0xff] ^ Td2[(s3 >> 8) & 0xff] ^ Td3[s2 & 0xff] ^ rk[37];
    t2 = Td0[s2 >> 24] ^ Td1[(s1 >> 16) & 0xff] ^ Td2[(s0 >> 8) & 0xff] ^ Td3[s3 & 0xff] ^ rk[38];
    t3 = Td0[s3 >> 24] ^ Td1[(s2 >> 16) & 0xff] ^ Td2[(s1 >> 8) & 0xff] ^ Td3[s0 & 0xff] ^ rk[39];
    if (Nr > 10) {
        /* round 10: */
        s0 = Td0[t0 >> 24] ^ Td1[(t3 >> 16) & 0xff] ^ Td2[(t2 >> 8) & 0xff] ^ Td3[t1 & 0xff] ^ rk[40];
        s1 = Td0[t1 >> 24] ^ Td1[(t0 >> 16) & 0xff] ^ Td2[(t3 >> 8) & 0xff] ^ Td3[t2 & 0xff] ^ rk[41];
        s2 = Td0[t2 >> 24] ^ Td1[(t1 >> 16) & 0xff] ^ Td2[(t0 >> 8) & 0xff] ^ Td3[t3 & 0xff] ^ rk[42];
        s3 = Td0[t3 >> 24] ^ Td1[(t2 >> 16) & 0xff] ^ Td2[(t1 >> 8) & 0xff] ^ Td3[t0 & 0xff] ^ rk[43];
        /* round 11: */
        t0 = Td0[s0 >> 24] ^ Td1[(s3 >> 16) & 0xff] ^ Td2[(s2 >> 8) & 0xff] ^ Td3[s1 & 0xff] ^ rk[44];
        t1 = Td0[s1 >> 24] ^ Td1[(s0 >> 16) & 0xff] ^ Td2[(s3 >> 8) & 0xff] ^ Td3[s2 & 0xff] ^ rk[45];
        t2 = Td0[s2 >> 24] ^ Td1[(s1 >> 16) & 0xff] ^ Td2[(s0 >> 8) & 0xff] ^ Td3[s3 & 0xff] ^ rk[46];
        t3 = Td0[s3 >> 24] ^ Td1[(s2 >> 16) & 0xff] ^ Td2[(s1 >> 8) & 0xff] ^ Td3[s0 & 0xff] ^ rk[47];
        if (Nr > 12) {
            /* round 12: */
            s0 = Td0[t0 >> 24] ^ Td1[(t3 >> 16) & 0xff] ^ Td2[(t2 >> 8) & 0xff] ^ Td3[t1 & 0xff] ^ rk[48];
            s1 = Td0[t1 >> 24] ^ Td1[(t0 >> 16) & 0xff] ^ Td2[(t3 >> 8) & 0xff] ^ Td3[t2 & 0xff] ^ rk[49];
            s2 = Td0[t2 >> 24] ^ Td1[(t1 >> 16) & 0xff] ^ Td2[(t0 >> 8) & 0xff] ^ Td3[t3 & 0xff] ^ rk[50];
            s3 = Td0[t3 >> 24] ^ Td1[(t2 >> 16) & 0xff] ^ Td2[(t1 >> 8) & 0xff] ^ Td3[t0 & 0xff] ^ rk[51];
            /* round 13: */
            t0 = Td0[s0 >> 24] ^ Td1[(s3 >> 16) & 0xff] ^ Td2[(s2 >> 8) & 0xff] ^ Td3[s1 & 0xff] ^ rk[52];
            t1 = Td0[s1 >> 24] ^ Td1[(s0 >> 16) & 0xff] ^ Td2[(s3 >> 8) & 0xff] ^ Td3[s2 & 0xff] ^ rk[53];
            t2 = Td0[s2 >> 24] ^ Td1[(s1 >> 16) & 0xff] ^ Td2[(s0 >> 8) & 0xff] ^ Td3[s3 & 0xff] ^ rk[54];
            t3 = Td0[s3 >> 24] ^ Td1[(s2 >> 16) & 0xff] ^ Td2[(s1 >> 8) & 0xff] ^ Td3[s0 & 0xff] ^ rk[55];
        }
    }
    rk += Nr << 2;
#else  /* !FULL_UNROLL */
    /*
     * Nr - 1 full rounds:
     */
    r = Nr >> 1;
    for (;;) {
        t0 = Td0[(s0 >> 24)] ^ Td1[(s3 >> 16) & 0xff] ^ Td2[(s2 >> 8) & 0xff] ^ Td3[(s1) & 0xff] ^ rk[4];
        t1 = Td0[(s1 >> 24)] ^ Td1[(s0 >> 16) & 0xff] ^ Td2[(s3 >> 8) & 0xff] ^ Td3[(s2) & 0xff] ^ rk[5];
        t2 = Td0[(s2 >> 24)] ^ Td1[(s1 >> 16) & 0xff] ^ Td2[(s0 >> 8) & 0xff] ^ Td3[(s3) & 0xff] ^ rk[6];
        t3 = Td0[(s3 >> 24)] ^ Td1[(s2 >> 16) & 0xff] ^ Td2[(s1 >> 8) & 0xff] ^ Td3[(s0) & 0xff] ^ rk[7];

        rk += 8;
        if (--r == 0) {
            break;
        }

        s0 = Td0[(t0 >> 24)] ^ Td1[(t3 >> 16) & 0xff] ^ Td2[(t2 >> 8) & 0xff] ^ Td3[(t1) & 0xff] ^ rk[0];
        s1 = Td0[(t1 >> 24)] ^ Td1[(t0 >> 16) & 0xff] ^ Td2[(t3 >> 8) & 0xff] ^ Td3[(t2) & 0xff] ^ rk[1];
        s2 = Td0[(t2 >> 24)] ^ Td1[(t1 >> 16) & 0xff] ^ Td2[(t0 >> 8) & 0xff] ^ Td3[(t3) & 0xff] ^ rk[2];
        s3 = Td0[(t3 >> 24)] ^ Td1[(t2 >> 16) & 0xff] ^ Td2[(t1 >> 8) & 0xff] ^ Td3[(t0) & 0xff] ^ rk[3];
    }
#endif /* ?FULL_UNROLL */
    /*
     * apply last round and
     * map cipher state to byte array block:
     */
    s0 = (Td4[(t0 >> 24)] & 0xff000000) ^ (Td4[(t3 >> 16) & 0xff] & 0x00ff0000) ^ (Td4[(t2 >> 8) & 0xff] & 0x0000ff00) ^ (Td4[(t1) & 0xff] & 0x000000ff) ^ rk[0];
    PUTU32(pt, s0);
    s1 = (Td4[(t1 >> 24)] & 0xff000000) ^ (Td4[(t0 >> 16) & 0xff] & 0x00ff0000) ^ (Td4[(t3 >> 8) & 0xff] & 0x0000ff00) ^ (Td4[(t2) & 0xff] & 0x000000ff) ^ rk[1];
    PUTU32(pt + 4, s1);
    s2 = (Td4[(t2 >> 24)] & 0xff000000) ^ (Td4[(t1 >> 16) & 0xff] & 0x00ff0000) ^ (Td4[(t0 >> 8) & 0xff] & 0x0000ff00) ^ (Td4[(t3) & 0xff] & 0x000000ff) ^ rk[2];
    PUTU32(pt + 8, s2);
    s3 = (Td4[(t3 >> 24)] & 0xff000000) ^ (Td4[(t2 >> 16) & 0xff] & 0x00ff0000) ^ (Td4[(t1 >> 8) & 0xff] & 0x0000ff00) ^ (Td4[(t0) & 0xff] & 0x000000ff) ^ rk[3];
    PUTU32(pt + 12, s3);
}

void WifiSetup_Md5Init(WifiSetupMd5Context *ctx) {
    ctx->bitsHi = 0;
    ctx->bitsLo = 0;
    ctx->state[0] = 0x67452301;
    ctx->state[1] = 0xefcdab89;
    ctx->state[2] = 0x98badcfe;
    ctx->state[3] = 0x10325476;
}

void WifiSetup_Md5Update(WifiSetupMd5Context *ctx, const u8 *src, u32 len) {
    u32 idx = (ctx->bitsLo >> 3) & 0x3f;
    u32 old = ctx->bitsLo;
    u32 part;
    u32 i;
    ctx->bitsLo += len << 3;
    if (ctx->bitsLo < (len << 3)) {
        ctx->bitsHi++;
    }
    ctx->bitsHi += len >> 29;
    part = 64 - idx;
    if (len >= part) {
        WifiSetup_CopyBytes(ctx->buffer + idx, src, part);
        WifiSetup_Md5Transform(ctx, ctx->buffer);
        for (i = part; i + 63 < len; i += 64) {
            WifiSetup_Md5Transform(ctx, src + i);
        }
        idx = 0;
    } else {
        i = 0;
    }
    WifiSetup_CopyBytes(ctx->buffer + idx, src + i, len - i);
}

void WifiSetup_Md5Final(u8 *out, WifiSetupMd5Context *ctx) {
    u8 bits[8];
    u32 idx;
    u32 pad;
    WifiSetup_EncodeLittleEndian(bits, &ctx->bitsLo, 8);
    idx = (ctx->bitsLo >> 3) & 0x3f;
    pad = idx < 56 ? 56 - idx : 120 - idx;
    WifiSetup_Md5Update(ctx, ov13_02245AD8, pad);
    WifiSetup_Md5Update(ctx, bits, 8);
    WifiSetup_EncodeLittleEndian(out, ctx->state, 16);
    WifiSetup_FillBytes(ctx, 0, 0x58);
}
