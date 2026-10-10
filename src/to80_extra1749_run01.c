#pragma force_active on
#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
#pragma force_active on
extern void long_from_char__sha1(u32 *, const u8 *, int);
extern u32 R(int, u32 *);
void CPSi_sha1_block(u32 *, const u8 *);
#define ROT5(a)                      (((a) << 5) | ((a) >> 27))
#define ROT30(a)                     (((a) << 30) | ((a) >> 2))
#define CH(b, c, d)                  ((d) ^ ((b) & ((c) ^ (d))))
#define PAR(b, c, d)                 ((b) ^ (c) ^ (d))
#define MAJ(b, c, d)                 (((b) & ((c) | (d))) | ((c) & (d)))
#define STEP(a, b, c, d, e, w, k, f) ((e) += ROT5(a) + f(b, c, d) + (w) + (k), (b) = ROT30(b))

void CPSi_sha1_block(u32 *state, const u8 *data) {
    u32 a = state[0], b = state[1], c = state[2], d = state[3], e = state[4], words[16];
    int i, j;
    long_from_char__sha1(words, data, 64);
    for (i = 0, j = 0; j < 3; i += 5, j++) {
        STEP(a, b, c, d, e, words[i], 0x5a827999, CH);
        STEP(e, a, b, c, d, words[i + 1], 0x5a827999, CH);
        STEP(d, e, a, b, c, words[i + 2], 0x5a827999, CH);
        STEP(c, d, e, a, b, words[i + 3], 0x5a827999, CH);
        STEP(b, c, d, e, a, words[i + 4], 0x5a827999, CH);
    }
    STEP(a, b, c, d, e, words[15], 0x5a827999, CH);
    STEP(e, a, b, c, d, R(0, words), 0x5a827999, CH);
    STEP(d, e, a, b, c, R(1, words), 0x5a827999, CH);
    STEP(c, d, e, a, b, R(2, words), 0x5a827999, CH);
    STEP(b, c, d, e, a, R(3, words), 0x5a827999, CH);
    i = 4;
    for (j = 0; j < 4; j++) {
        STEP(a, b, c, d, e, R(i, words), 0x6ed9eba1, PAR);
        STEP(e, a, b, c, d, R(i + 1, words), 0x6ed9eba1, PAR);
        i = (i + 2) & 15;
        STEP(d, e, a, b, c, R(i, words), 0x6ed9eba1, PAR);
        STEP(c, d, e, a, b, R(i + 1, words), 0x6ed9eba1, PAR);
        STEP(b, c, d, e, a, R(i + 2, words), 0x6ed9eba1, PAR);
        i += 3;
    }
    for (j = 0; j < 4; j++) {
        STEP(a, b, c, d, e, R(i, words), 0x8f1bbcdc, MAJ);
        STEP(e, a, b, c, d, R(i + 1, words), 0x8f1bbcdc, MAJ);
        STEP(d, e, a, b, c, R(i + 2, words), 0x8f1bbcdc, MAJ);
        i = (i + 3) & 15;
        STEP(c, d, e, a, b, R(i, words), 0x8f1bbcdc, MAJ);
        STEP(b, c, d, e, a, R(i + 1, words), 0x8f1bbcdc, MAJ);
        i += 2;
    }
    for (j = 0; j < 4; j++) {
        STEP(a, b, c, d, e, R(i, words), 0xca62c1d6, PAR);
        STEP(e, a, b, c, d, R(i + 1, words), 0xca62c1d6, PAR);
        STEP(d, e, a, b, c, R(i + 2, words), 0xca62c1d6, PAR);
        STEP(c, d, e, a, b, R(i + 3, words), 0xca62c1d6, PAR);
        i = (i + 4) & 15;
        STEP(b, c, d, e, a, R(i, words), 0xca62c1d6, PAR);
        i++;
    }
    state[0] += a;
    state[1] += b;
    state[2] += c;
    state[3] += d;
    state[4] += e;
}
