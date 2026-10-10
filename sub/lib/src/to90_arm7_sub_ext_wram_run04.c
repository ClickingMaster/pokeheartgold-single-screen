#pragma force_active on
#include <nitro/types.h>

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
extern void WmspKickMPParent(u16);
void WmspMPParentIntervalAlarmCallback(void *p);

void WmspMPParentIntervalAlarmCallback(void *p) {
    WmspKickMPParent((u16)(u32)p);
}
