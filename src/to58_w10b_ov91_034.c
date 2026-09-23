#include "global.h"
u16 ov91_0225E1FC(void *, u32);
u16 ov91_0225E1FC(void *unused, u32 value) {
    s32 result = (s32)(((value - 750) % 50) * 0x638E) / 50;
    if (result > 0x31C7) result = 0x31C7 - result % 0x31C7;
    return result + 0x0E38;
}
#include "global.h"
void ov91_0225E23C(u8 *, u8 *);
void ov91_0225E23C(u8 *src, u8 *dst) {
    *(u16 *)(dst + 0) = *(u16 *)(src + 0);
    *(u16 *)(dst + 2) = *(u16 *)(src + 2);
    *(u16 *)(dst + 4) = *(u32 *)(src + 4);
    *(u16 *)(dst + 6) = *(u32 *)(src + 8);
    *(u16 *)(dst + 8) = *(u32 *)(src + 12);
    *(u16 *)(dst + 16) = (s64)((s64)*(s32 *)(src + 16) * 0x28F + 0x800) >> 12;
    *(u16 *)(dst + 10) = *(s32 *)(src + 20) >> 12;
    *(u16 *)(dst + 12) = *(s32 *)(src + 24) >> 12;
    *(u16 *)(dst + 14) = *(s32 *)(src + 28) >> 12;
    dst[18] = *(u32 *)(src + 32);
    dst[19] = *(u32 *)(src + 36);
}
#include "global.h"
extern s32 FX_Div(s32, s32);
void ov91_0225E294(u8 *, u8 *);
void ov91_0225E294(u8 *src, u8 *dst) {
    *(u16 *)(dst + 0) = *(u16 *)(src + 0);
    *(u16 *)(dst + 2) = *(u16 *)(src + 2);
    *(s32 *)(dst + 4) = *(s16 *)(src + 4);
    *(s32 *)(dst + 8) = *(s16 *)(src + 6);
    *(s32 *)(dst + 12) = *(s16 *)(src + 8);
    *(s32 *)(dst + 16) = FX_Div(*(s16 *)(src + 16), 0x28F);
    *(s32 *)(dst + 20) = *(s16 *)(src + 10) << 12;
    *(s32 *)(dst + 24) = *(s16 *)(src + 12) << 12;
    *(s32 *)(dst + 28) = *(s16 *)(src + 14) << 12;
    *(u32 *)(dst + 36) = src[0x13];
}
