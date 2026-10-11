#include <nitro/types.h>

// The code below is compiled as Thumb (16-bit) code.
#include <nitro/code16.h>

// BIOS calls (SWI) of the ARM7: memory copy/fill, square root, CRC-16, decompression and the sound tables.
// Declared without parameters: each one passes its arguments through in r0-r3.

void SVC_CpuSet(void);
void SVC_CpuSetFast(void);
void SVC_Sqrt(void);
void SVC_GetCRC16(void);
void IsMmemExpanded(void);
void SVC_UnpackBits(void);
void SVC_UncompressLZ8(void);
void SVC_UncompressLZ16FromDevice(void);
void SVC_UncompressHuffmanFromDevice(void);
void SVC_UncompressRL8(void);
void SVC_UncompressRL16FromDevice(void);
void SVC_GetSinTable(void);
void SVC_GetPitchTable(void);
void SVC_GetVolumeTable(void);

#pragma force_active on

// clang-format off
asm void SVC_CpuSet(void)
{
    swi 0xb
    bx lr
}
// clang-format on

// clang-format off
asm void SVC_CpuSetFast(void)
{
    swi 0xc
    bx lr
}
// clang-format on

// clang-format off
asm void SVC_Sqrt(void)
{
    swi 0xd
    bx lr
}
// clang-format on

// clang-format off
asm void SVC_GetCRC16(void)
{
    swi 0xe
    bx lr
}
// clang-format on

// clang-format off
asm void IsMmemExpanded(void)
{
    swi 0xf
    bx lr
}
// clang-format on

// clang-format off
asm void SVC_UnpackBits(void)
{
    swi 0x10
    bx lr
}
// clang-format on

// clang-format off
asm void SVC_UncompressLZ8(void)
{
    swi 0x11
    bx lr
}
// clang-format on

// clang-format off
asm void SVC_UncompressLZ16FromDevice(void)
{
    swi 0x12
    bx lr
}
// clang-format on

// clang-format off
asm void SVC_UncompressHuffmanFromDevice(void)
{
    swi 0x13
    bx lr
}
// clang-format on

// clang-format off
asm void SVC_UncompressRL8(void)
{
    swi 0x14
    bx lr
}
// clang-format on

// clang-format off
asm void SVC_UncompressRL16FromDevice(void)
{
    swi 0x15
    bx lr
}
// clang-format on

// clang-format off
asm void SVC_GetSinTable(void)
{
    swi 0x1a
    bx lr
}
// clang-format on

// clang-format off
asm void SVC_GetPitchTable(void)
{
    swi 0x1b
    bx lr
}
// clang-format on

// clang-format off
asm void SVC_GetVolumeTable(void)
{
    swi 0x1c
    bx lr
}
// clang-format on
