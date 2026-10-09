#pragma force_active on
#include "global.h"

extern u32 CTRDG_GetAgbGameCode(void);
extern void CTRDG_Enable(BOOL enable);
extern BOOL CTRDG_CpuCopy8(const void *src, void *dst, u32 size);
extern void CRYPTO_SetAllocator(void *(*alloc)(u32), void (*free)(void *));
extern BOOL CRYPTO_VerifySignature(const void *data, u32 size, const void *signature, const void *publicKey);
extern void *ov74_02235138(u32 size);
extern void Heap_Free(void *ptr);
extern const u8 _0223B690[];

extern u32 ov74_0223CE9C[];        // game-code table ("EB5B", "EC5B", ...), zero terminated
extern u16 ov74_0223CC5E[];        // u16 table that ends right before ov74_0223CE9C (0x11F entries)

// The original file owned the game-code table, so the literal pool holds it twice; the second entry is reached
// through the end of the preceding table (same address 0x0223CE9C).
#define GAME_CODES ((u32 *)(ov74_0223CC5E + 0x11F))

int ov74_0223514C(void);

// @fn ov74_0223514C
int ov74_0223514C(void)
{
    u8 payload[0x4A8];
    u8 signature[0x80];
    u32 n = CTRDG_GetAgbGameCode();
    u32 code = (u8)(n >> 24) | ((n << 24) | (((n >> 8) << 24) >> 8) | (((n >> 16) << 24) >> 16));
    u32 i = 0;
    if (i < ov74_0223CE9C[0]) {
        u32 *p = GAME_CODES;
        do {
            if (code == *p) {
                CTRDG_Enable(TRUE);
                CTRDG_CpuCopy8((void *)0x08100000, payload, 0x4A8);
                CTRDG_CpuCopy8((void *)0x08020000, signature, 0x80);
                CTRDG_Enable(FALSE);
                CRYPTO_SetAllocator(ov74_02235138, Heap_Free);
                if (CRYPTO_VerifySignature(payload, 0x4A8, signature, _0223B690)) {
                    return 1;
                }
            }
            i++;
            p++;
        } while (i < *p);
    }
    return 0;
}
