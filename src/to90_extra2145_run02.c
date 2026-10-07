#pragma force_active on
// clang-format off
#include "global.h"
#include <nitro/code32.h>
// clang-format on
#pragma force_active on
u16 WM_GetDispersionScanPeriod(void);
u16 WM_GetDispersionScanPeriod(void) {u8 mac[6];u16 sum;int i;OS_GetMacAddress(mac);i=0;sum=0;for(;i<6;i++)sum+=mac[i];sum+=*(vu32*)0x027ffc3c;sum*=13;return (sum%10)+30;}
