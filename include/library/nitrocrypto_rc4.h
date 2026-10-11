#ifndef POKEHEARTGOLD_NITROCRYPTO_RC4_H
#define POKEHEARTGOLD_NITROCRYPTO_RC4_H

// RC4 stream cipher of the nitrocrypto library: byte-state and word-state ("fast") variants.

#include <nitro/types.h>

typedef struct {
    u8 i;
    u8 j;
    u8 pad[2];
    u8 s[256];
} CRYPTORC4Context;

// the permutation is kept in the top byte of each word
typedef struct {
    u32 i;
    u32 j;
    u32 s[256];
} CRYPTORC4FastContext;

void CRYPTO_RC4Init(CRYPTORC4Context *context, const void *key, u32 key_len);
void CRYPTO_RC4Encrypt(void *context, const void *in, u32 length, void *out);
void CRYPTO_RC4FastInit(CRYPTORC4FastContext *context, const void *key, u32 key_len);
void CRYPTO_RC4FastEncrypt(void *context, const void *in, u32 length, void *out);

#endif // POKEHEARTGOLD_NITROCRYPTO_RC4_H
