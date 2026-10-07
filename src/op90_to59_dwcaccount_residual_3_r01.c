#pragma force_active on
#include <nitro/types.h>
#include <nitro/code32.h>
typedef struct {
    u32 i;
    u32 j;
    u32 s[256];
} CRYPTORC4FastContext;
void CRYPTO_RC4FastInit(CRYPTORC4FastContext *context, const void *key, u32 key_len);
// @fn CRYPTO_RC4FastInit
void CRYPTO_RC4FastInit(CRYPTORC4FastContext *context, const void *key, u32 key_len)
{
int i; u32 *s; if (key_len > 16) key_len = 16; context->i = 0; context->j = 0; s = context->s; for (i = 0; i < 256; i++) s[i] = i;
{ u32 k, j; j = 0; k = 0; for (i = 0; i < 256; i++) { u32 x = s[i]; j = (j + (x + ((const u8 *)key)[k])) & 0xff; k++; if (k == key_len) k = 0; s[i] = s[j]; s[j] = x; } }
for (i = 0; i < 256; i++) s[i] <<= 24;
}
