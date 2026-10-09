#pragma force_active on
#include <nitro/types.h>
#include <nitro/code16.h>
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
// @fn SVC_CpuSet
asm void SVC_CpuSet(void)
{
    swi 0xb
    bx lr
}
// @fn SVC_CpuSetFast
asm void SVC_CpuSetFast(void)
{
    swi 0xc
    bx lr
}
// @fn SVC_Sqrt
asm void SVC_Sqrt(void)
{
    swi 0xd
    bx lr
}
// @fn SVC_GetCRC16
asm void SVC_GetCRC16(void)
{
    swi 0xe
    bx lr
}
// @fn IsMmemExpanded
asm void IsMmemExpanded(void)
{
    swi 0xf
    bx lr
}
// @fn SVC_UnpackBits
asm void SVC_UnpackBits(void)
{
    swi 0x10
    bx lr
}
// @fn SVC_UncompressLZ8
asm void SVC_UncompressLZ8(void)
{
    swi 0x11
    bx lr
}
// @fn SVC_UncompressLZ16FromDevice
asm void SVC_UncompressLZ16FromDevice(void)
{
    swi 0x12
    bx lr
}
// @fn SVC_UncompressHuffmanFromDevice
asm void SVC_UncompressHuffmanFromDevice(void)
{
    swi 0x13
    bx lr
}
// @fn SVC_UncompressRL8
asm void SVC_UncompressRL8(void)
{
    swi 0x14
    bx lr
}
// @fn SVC_UncompressRL16FromDevice
asm void SVC_UncompressRL16FromDevice(void)
{
    swi 0x15
    bx lr
}
// @fn SVC_GetSinTable
asm void SVC_GetSinTable(void)
{
    swi 0x1a
    bx lr
}
// @fn SVC_GetPitchTable
asm void SVC_GetPitchTable(void)
{
    swi 0x1b
    bx lr
}
// @fn SVC_GetVolumeTable
asm void SVC_GetVolumeTable(void)
{
    swi 0x1c
    bx lr
}
