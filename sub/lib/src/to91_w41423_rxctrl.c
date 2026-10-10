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
void InitRxCtrl(void);

void InitRxCtrl(void) {
    u8 *root = ROOT;
    u8 *control = root + 0x344;
    u8 *receive = root + 0x4dc;
    u32 start;
    __VENEER_MIi_CpuClear32(0, receive, 0x50);
    HW(0x04808030) = 0x8000;
    switch (H(ROOT, 0x350)) {
    case 0:
        start = 0x794;
        break;
    case 1:
        start = 0x10c4;
        break;
    case 2:
        start = 0xbfc;
        break;
    case 3:
        start = 0x794;
        break;
    }
    HW(0x04808050) = (u16)(start + 0x04804000);
    HW(0x04808056) = (u16)(start >> 1);
    HW(0x04808052) = 0x5f60;
    HW(0x0480805a) = (u16)(start >> 1);
    H(receive, 4) = (u16)(start >> 1);
    H(receive, 0) = 0xffff;
    H(control, 0x9a) = (u16)(0x1f60 - start);
    HW(0x04808062) = 0x5f5e;
    HW(0x04808030) = 0x8001;
    HW(0x0480824c) = 0xffff;
    HW(0x0480824e) = 0xffff;
    HW(0x04805f70) = 0xffff;
    HW(0x04805f72) = 0xffff;
    HW(0x04805f7e) = 0xffff;
    HW(0x04805f76) = 0xffff;
}
