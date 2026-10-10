#pragma force_active on
#include <nitro/types.h>

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
#pragma force_active on
void NNS_G2dMapScrToChar256x16Pltt(u16 *, int, int, int, u32, u32);
void NNS_G2dMapScrToCharText(u16 *, int, int, int, int, int, u32, u32);
void NNSi_G2dUnpackNFT(u8 *);

void NNSi_G2dUnpackNFT(u8 *base) {
    u8 *p = base + *(u16 *)(base + 12);
    for (int i = 0; i < *(u16 *)(base + 14); i++) {
        switch (*(u32 *)p) {
        case 0x46494e46:
            *(u32 *)(p + 16) += (u32)base;
            if (*(u32 *)(p + 20)) {
                *(u32 *)(p + 20) += (u32)base;
            }
            if (*(u32 *)(p + 24)) {
                *(u32 *)(p + 24) += (u32)base;
            }
            break;
        case 0x43574448:
            if (*(u32 *)(p + 12)) {
                *(u32 *)(p + 12) += (u32)base;
            }
            break;
        case 0x434d4150:
            if (*(u32 *)(p + 16)) {
                *(u32 *)(p + 16) += (u32)base;
            }
            break;
        case 0x43474c50:
            break;
        }
        p += *(u32 *)(p + 4);
    }
}
