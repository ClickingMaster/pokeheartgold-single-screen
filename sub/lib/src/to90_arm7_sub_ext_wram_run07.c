#pragma force_active on
#include <nitro/types.h>

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
extern void WmspKickMPChild(void *);
void WmspMPChildIntervalAlarmCallback(void *p);

void WmspMPChildIntervalAlarmCallback(void *p) {
    WmspKickMPChild(p);
}
