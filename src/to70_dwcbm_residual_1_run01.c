#pragma force_active on
#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
#pragma force_active on

struct WifiInfo {
    u64 first, second;
    u16 count, tail;
};

extern u8 Wifi[];
extern u8 _021D43C8[];
extern void DWCi_BACKUPlConvWifiInfo(struct WifiInfo *);
extern BOOL readNvram(u32, u32, void *);
extern void writeNvram(u32, u32, const void *);
extern BOOL verify(const void *, u32, u32, void *);
extern BOOL writeDisable(void);

extern u16 MATH_CalcCRC16(const void *, const void *, u32);
void DWCi_BM_GetWiFiInfo(struct WifiInfo *);
BOOL DWCi_BM_SetWiFiInfo(struct WifiInfo *, u8 *);

void DWCi_BM_GetWiFiInfo(struct WifiInfo *out) {
    MI_CpuCopy8(Wifi, out, 6);
    out->first &= 0x7ffffffffffULL;
    MI_CpuCopy8(Wifi + 5, &out->second, 6);
    out->second >>= 3;
    out->second &= 0x7ffffffffffULL;
    MI_CpuCopy8(Wifi + 10, &out->count, 2);
    out->count >>= 6;
    out->count &= 0x3ff;
    MI_CpuCopy8(Wifi + 12, &out->tail, 2);
}

BOOL DWCi_BM_SetWiFiInfo(struct WifiInfo *info, u8 *work) {
    int i;
    u32 start = *(u32 *)(_021D43C8 + 8);
    DWCi_BACKUPlConvWifiInfo(info);
    MATHi_CRC16InitTableRev((MATHCRC16Table *)(work + 0x200), 0xa001);
    for (i = 0; i < 2; i++, start += 0x100) {
        if (!readNvram(start, 0x100, work)) {
            OS_Terminate();
            return FALSE;
        }
        MI_CpuCopy8(Wifi, work + 0xf0, 14);
        *(u16 *)(work + 0xfe) = MATH_CalcCRC16(work + 0x200, work, 0xfe);
        do {
            writeNvram(start, 0x100, work);
        } while (!verify(work, start, 0x100, work + 0x100));
    }
    return writeDisable() != 0;
}
