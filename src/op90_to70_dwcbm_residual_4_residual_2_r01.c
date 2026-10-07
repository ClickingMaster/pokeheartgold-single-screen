#pragma force_active on
#include "global.h"
#include <nitro/code32.h>
extern u8 Wifi[];
typedef struct WiInfo {
    u64 id;
    s64 password;
    u16 counter;
    u8 salt[2];
} WiInfo;
void *DWCi_BACKUPlConvWifiInfo(WiInfo *info);
// @fn DWCi_BACKUPlConvWifiInfo
void *DWCi_BACKUPlConvWifiInfo(WiInfo *info)
{
    u64 v = info->password;
    MI_CpuCopy8(info, Wifi, 5);
    Wifi[5] = ((info->id >> 40) & 7) | ((v & 0x1f) << 3);
    v >>= 5;
    MI_CpuCopy8(&v, Wifi + 6, 4);
    Wifi[10] = ((v >> 32) & 0x3f) | ((info->counter & 3) << 6);
    Wifi[11] = info->counter >> 2;
    MI_CpuCopy8(info->salt, Wifi + 12, 2);
    return Wifi;
}
