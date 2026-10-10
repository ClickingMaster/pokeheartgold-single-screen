#pragma force_active on
#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
void DWCi_BM_GetWiFiInfo(void *);

typedef struct __attribute__((packed, aligned(4))) Info {
    u64 first, second;
    u16 tail[2];
} Info;

int DWC_Auth_CheckPseudoWiFiID(void);
#pragma force_active on

int DWC_Auth_CheckPseudoWiFiID(void) {
    Info info;
    DWCi_BM_GetWiFiInfo(&info);
    if (info.second == 0 && info.first == 0) {
        return 1;
    }
    return 0;
}
