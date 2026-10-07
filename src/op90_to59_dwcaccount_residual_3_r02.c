#pragma force_active on
#include <nitro/types.h>
#include <nitro/code32.h>
typedef struct {
    u8 i;
    u8 j;
    u8 pad[2];
    u8 s[256];
} CRYPTORC4Context;
void CRYPTO_RC4Init(CRYPTORC4Context *context, const void *key, u32 key_len);
// @fn CRYPTO_RC4Init
void CRYPTO_RC4Init(CRYPTORC4Context *context, const void *key, u32 key_len)
{
int i; u8 *s; if (key_len > 16) key_len = 16; context->i = 0; context->j = 0; s = context->s; for (i = 0; i < 256; i++) s[i] = (u8)i;
{ u8 k, j; j = 0; k = 0; for (i = 0; i < 256; i++) { u8 x = s[i]; j = (u8)(j + (x + ((const u8 *)key)[k])); k++; if (k == (u8)key_len) k = 0; s[i] = s[j]; s[j] = x; } }
}
