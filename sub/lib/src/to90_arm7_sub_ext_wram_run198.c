#pragma force_active on
#include <nitro/types.h>

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
#define H(a)      (*(volatile u16 *)(a))
#define WL        (*(u8 **)0x0380fff4)
#define R16(p, o) (*(u16 *)((u8 *)(p) + (o)))
#define R32(p, o) (*(u32 *)((u8 *)(p) + (o)))
#define P(p, o)   (*(u8 **)((u8 *)(p) + (o)))
extern u32 CheckKeyTxEndMain(void *);
u32 CheckKeyTxEnd(void);

u32 CheckKeyTxEnd(void) {
    u8 *slots = WL + 0x42c;
    u32 result = CheckKeyTxEndMain(slots + 0x50);
    return result | CheckKeyTxEndMain(slots + 0x64);
}
