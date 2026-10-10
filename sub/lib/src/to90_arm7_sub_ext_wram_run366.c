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
u16 WL_ReadByte(const void *);
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
void UpdateApList(u32 strength, const void *frame, const void *ssid);

void UpdateApList(u32 strength, const void *frame, const void *ssid) {
    u8 *entry = ROOT + 0x23c;
    u32 i, empty, oldest, min;
    if (WL_ReadByte((const u8 *)ssid + 1) > 32) {
        return;
    }
    empty = 4;
    oldest = empty;
    min = 0x400;
    for (i = 0; i < 4; i++, entry += 0x32) {
        if (H(entry, 0)) {
            if (MatchMacAdrs(entry + 6, (const u8 *)frame + 0x24)) {
                goto filled;
            }
            if (H(entry, 0x30) < min) {
                min = H(entry, 0x30);
                oldest = i;
            }
        } else {
            empty = i;
        }
    }
    if (empty != 4) {
        i = empty;
    } else if (oldest != 4) {
        i = oldest;
    } else {
        return;
    }
filled:
    entry = ROOT + 0x23c + i * 0x32;
    __VENEER_MIi_CpuClear16(0, entry, 0x32);
    H(entry, 0x30) = 0x400;
    H(entry, 0) = (u16)(H(frame, 0x12) & 255);
    H(entry, 2) = (u16)strength;
    WSetMacAdrs1(entry + 6, (const u8 *)frame + 0x24);
    H(entry, 0xc) = WL_ReadByte((const u8 *)ssid + 1);
    for (i = 0; i < H(entry, 0xc); i++) {
        u32 byte = WL_ReadByte((const u8 *)ssid + 2 + i);
        WL_WriteByte(entry + 0xe + i, byte);
    }
    H(entry, 0x2e) = H(frame, 0x34);
    H(entry, 4) = H(frame, 0x36);
}
