#pragma force_active on
#include <nitro/types.h>
#include <nitro/code32.h>
void SPI_SendWait_03803D98(u32 data);
void NVRAM_PageErase(u32 address);
void NVRAM_SectorErase(u32 address);
// @fn NVRAM_PageErase
void NVRAM_PageErase(u32 address)
{
    u16 high = (u16)((address & 0xff0000) >> 16);
    u16 mid = (u16)((address & 0xff00) >> 8);
    u16 low = (u16)(address & 0xff);
    while (*(vu16 *)0x040001C0 & 0x80) {}
    *(vu16 *)0x040001C0 = 0x8900;
    *(vu16 *)0x040001C2 = 0xdb;
    while (*(vu16 *)0x040001C0 & 0x80) {}
    SPI_SendWait_03803D98(high);
    SPI_SendWait_03803D98(mid);
    *(vu16 *)0x040001C0 = 0x8100;
    SPI_SendWait_03803D98(low);
}
// @fn NVRAM_SectorErase
void NVRAM_SectorErase(u32 address)
{
    u16 high = (u16)((address & 0xff0000) >> 16);
    u16 mid = (u16)((address & 0xff00) >> 8);
    u16 low = (u16)(address & 0xff);
    while (*(vu16 *)0x040001C0 & 0x80) {}
    *(vu16 *)0x040001C0 = 0x8900;
    *(vu16 *)0x040001C2 = 0xd8;
    while (*(vu16 *)0x040001C0 & 0x80) {}
    SPI_SendWait_03803D98(high);
    SPI_SendWait_03803D98(mid);
    *(vu16 *)0x040001C0 = 0x8100;
    SPI_SendWait_03803D98(low);
}
