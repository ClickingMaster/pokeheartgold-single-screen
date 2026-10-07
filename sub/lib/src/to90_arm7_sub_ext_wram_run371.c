#pragma force_active on
// clang-format off
#include <nitro/types.h>
#include <nitro/code32.h>
// clang-format on
#define ROOT (*(u8**)0x0380fff4)
#define U32(p,o) (*(u32*)((u8*)(p)+(o)))
#define H(p,o) (*(u16*)((u8*)(p)+(o)))
#define HW(a) (*(volatile u16*)(a))
void __VENEER_MIi_CpuClear32(u32,void*,u32);void __VENEER_MIi_CpuClear16(u32,void*,u32);void MIi_CpuClear16(u32,void*,u32);
u8 WL_ReadByte(const void*);void WL_WriteByte(void*,u32);int MatchMacAdrs(const void*,const void*);void WSetMacAdrs1(void*,const void*);
u16 calc_NextCRC(u8,u16);void __VENEER_NVRAM_ReadStatusRegister(u8*);void __VENEER_NVRAM_SoftwareReset(void);void __VENEER_SPI_Lock(u32);void __VENEER_SPI_Unlock(u32);void __VENEER_NVRAM_ReadDataBytes(u32,u32,void*);void FLASH_Wait(void);void *AllocateHeapBuf(void*,u32);
void FLASH_DirectRead(u32 offset,u32 size,void*destination);
// @fn FLASH_DirectRead
void FLASH_DirectRead(u32 offset,u32 size,void*destination){__VENEER_SPI_Lock(U32(ROOT,0x314));FLASH_Wait();__VENEER_NVRAM_ReadDataBytes(offset,size,destination);__VENEER_SPI_Unlock(U32(ROOT,0x314));}
