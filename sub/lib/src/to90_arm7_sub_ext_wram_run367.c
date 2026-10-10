#pragma force_active on
#include <nitro/types.h>

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
#define ROOT      (*(u8 **)0x0380fff4)
#define U32(p, o) (*(u32 *)((u8 *)(p) + (o)))
#define H(p, o)   (*(u16 *)((u8 *)(p) + (o)))
#define HW(a)     (*(volatile u16 *)(a))
void __VENEER_MIi_CpuClear32(u32, void *, u32);
void __VENEER_MIi_CpuClear16(u32, void *, u32);
void MIi_CpuClear16(u32, void *, u32);
u8 WL_ReadByte(const void *);
void WL_WriteByte(void *, u32);
int MatchMacAdrs(const void *, const void *);
void WSetMacAdrs1(void *, const void *);
u16 calc_NextCRC(u8, u16);
void __VENEER_NVRAM_ReadStatusRegister(u8 *);
void __VENEER_NVRAM_SoftwareReset(void);
void __VENEER_SPI_Lock(u32);
void __VENEER_SPI_Unlock(u32);
void __VENEER_NVRAM_ReadDataBytes(u32, u32, void *);
void FLASH_Wait(void);
void *AllocateHeapBuf(void *, u32);
void InitApList(void);

void InitApList(void) {
    MIi_CpuClear16(0, ROOT + 0x23c, 0xc8);
}
