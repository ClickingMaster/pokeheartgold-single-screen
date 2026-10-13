#pragma force_active on
#include "wl_internal.h"
// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

// -ipa file would otherwise inline the small functions of this file into their callers here.
#pragma dont_inline on

void ClearTimeOut(void) {
    OS_CancelAlarm(&wlWorkPlain->timeOutAlarm);
}

void DMA_Read(void *dest, const void *src, u32 size) {
    u32 first, rest;
    size = (size + 1) & ~1U;
    if ((u32)src + size > 0x04805F60) {
        first = 0x04805F60 - (u32)src;
        rest = size - first;
    } else {
        first = size;
        rest = 0;
    }
    __VENEER_MIi_CpuCopy16(src, dest, first);
    if (rest) {
        u32 wrapped = (u32)src + first - wlWorkPlain->bss.rxBufSize;
        __VENEER_MIi_CpuCopy16((void *)wrapped, (u8 *)dest + first, rest);
    }
}

void DMA_Write(void *dest, const void *src, u32 size) {
    MIi_CpuCopy16(src, dest, (size + 1) & ~1U);
}

void DMA_WriteHeaderData(void *dest, const void *header, const void *data, u32 size) {
    __VENEER_MIi_CpuCopy16(header, dest, 36);
    if (size) {
        __VENEER_MIi_CpuCopy16(data, (u8 *)dest + 36, (size + 1) & ~1U);
    }
}

void DMA_WepWriteHeaderData(void *dest, const void *header, const void *data, u32 size) {
    __VENEER_MIi_CpuCopy16(header, dest, 36);
    if (size) {
        __VENEER_MIi_CpuCopy16(data, (u8 *)dest + 40, (size + 1) & ~1U);
    }
}

// The MAC memory only takes 16-bit accesses.
void WL_WriteByte(void *dest, u32 value) {
    u32 addr = (u32)dest;
    if (addr & 1) {
        vu16 *p = (vu16 *)(addr - 1);
        *p = (*p & 0xFF) | (value << 8);
    } else {
        vu16 *p = dest;
        *p = (*p & 0xFF00) | (value & 0xFF);
    }
}

u8 WL_ReadByte(const void *src) {
    u8 value;
    if ((u32)src & 1) {
        value = *(vu16 *)((u32)src - 1) >> 8;
    } else {
        value = *(vu16 *)src;
    }
    return value & 0xFF;
}
