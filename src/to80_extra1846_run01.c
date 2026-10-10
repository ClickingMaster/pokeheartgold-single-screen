#pragma force_active on
#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
#define R32(a) (*(volatile u32 *)(a))
#define R16(a) (*(volatile u16 *)(a))
void GXx_SetMasterBrightness_(volatile u16 *, int);
void ov13_02240648(void);
void ov13_02241C68(void);
void ov13_02241DFC(void);
void ov13_0223FADC(void);
void ov13_0222B620(void);

static inline void subReset(volatile u32 *reg) {
    u32 value = *reg;
    value &= ~0x1f00;
    *reg = value;
    value = *reg;
    value &= ~0xe000;
    *reg = value;
    GXx_SetMasterBrightness_((volatile u16 *)((u8 *)reg + 0x6c), 0);
}

void ov13_0222B620(void) {
    GX_VBlankIntr(0);
    GX_SetBankForBG((GXVRamBG)1);
    GX_SetBankForOBJ((GXVRamOBJ)2);
    GX_SetGraphicsMode((GXDispMode)1, (GXBGMode)0, (GXBG0As)0);
    R32(0x4000000) &= ~0x1f00;
    R32(0x4000000) &= ~0xe000;
    GXx_SetMasterBrightness_((volatile u16 *)0x400006c, 0);
    R32(0x4000000) = (R32(0x4000000) & 0xffcfffef) | 0x10 | 0x200000;
    R16(0x4000008) &= ~64;
    R16(0x400000a) &= ~64;
    R16(0x400000c) &= ~64;
    R16(0x400000e) &= ~64;
    R32(0x4000010) = 0;
    R32(0x4000014) = 0;
    R32(0x4000018) = 0;
    R32(0x400001c) = 0;
    G2x_SetBlendBrightness_((volatile u16 *)0x4000050, 63, 16);
    GX_SetBankForSubBG((GXVRamSubBG)128);
    GX_SetBankForSubOBJ((GXVRamSubOBJ)256);
    GXS_SetGraphicsMode((GXBGMode)0);
    subReset((volatile u32 *)0x4001000);
    R32(0x4001000) = (R32(0x4001000) & 0xffcfffef) | 16;
    R16(0x4001008) &= ~64;
    R16(0x400100a) &= ~64;
    R16(0x400100c) &= ~64;
    R16(0x400100e) &= ~64;
    R32(0x4001010) = 0;
    R32(0x4001014) = 0;
    R32(0x4001018) = 0;
    R32(0x400101c) = 0;
    G2x_SetBlendBrightness_((volatile u16 *)0x4001050, 63, 16);
    R16(0x4000008) = (R16(0x4000008) & 0x43) | 0xc00;
    R16(0x400000a) = (R16(0x400000a) & 0x43) | 0x108 | 0xc00;
    R16(0x400000c) = (R16(0x400000c) & 0x43) | 0xe10;
    R16(0x400000e) = (R16(0x400000e) & 0x43) | 0xf10;
    R16(0x4001008) = (R16(0x4001008) & 0x43) | 0xc00;
    R16(0x400100a) = (R16(0x400100a) & 0x43) | 0xd00;
    R16(0x400100c) = (R16(0x400100c) & 0x43) | 0xe00;
    R16(0x400100e) = (R16(0x400100e) & 0x43) | 0xf00;
    R32(0x4000000) &= ~0x38000000;
    R32(0x4000000) &= ~0x7000000;
    R16(0x4000304) &= ~0x8000;
    ov13_02240648();
    ov13_02241C68();
    ov13_02241DFC();
    ov13_0223FADC();
    GX_DispOn();
    R32(0x4001000) |= 0x10000;
    GX_VBlankIntr(1);
}
