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
void FLASH_Read(u32 offset,u32 size,void*destination);
// @fn FLASH_Read
void FLASH_Read(u32 offset,u32 size,void*destination){u8*image=(u8*)U32(ROOT,0x318);u8*out=destination;const u8*source;if(image){source=(const u8*)((u32)image+offset-0x2a);while(size){u8 byte=WL_ReadByte(source);source++;WL_WriteByte(out,byte);out++;size--;}}}
