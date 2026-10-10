#pragma force_active on
#pragma dollar_identifiers on
#include <nitro/types.h>

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
#define BASE    (*(u8 **)0x0380fff4)
#define H(p, o) (*(u16 *)((u8 *)(p) + (o)))
#define P(p, o) (*(u8 **)((u8 *)(p) + (o)))
#define HW(a)   (*(volatile u16 *)(a))
void __VENEER_MIi_CpuCopy16(const void *, void *, u32);
u32 WL_ReadByte(const void *);
void WL_WriteByte(void *, u32);
int WSetGameInfo(u32 length, const u8 *input);

int WSetGameInfo(u32 length, const u8 *input) {
    u8 *s = BASE + 0x344;
    if (length > 128) {
        return 4;
    }
    if (length) {
        if (H(s, 0xa2) & 1) {
            u8 *dest = P(s, 0x9c);
            u32 i;
            WL_WriteByte(dest, 255);
            dest++;
            for (i = 0; i < length; i++) {
                WL_WriteByte(dest, WL_ReadByte(input));
                dest++;
                input++;
            }
        } else {
            __VENEER_MIi_CpuCopy16(input, P(s, 0x9c), length + 1);
        }
    }
    H(s, 0xa0) = length;
    H(s, 0xa4) = 1;
    return 0;
}
