#pragma force_active on
#include <nitro/types.h>
// clang-format off
#include <nitro/code32.h>
// clang-format on
extern void WmspKickMPParent(u16);
void WmspMPParentIntervalAlarmCallback(void *p);
// @fn WmspMPParentIntervalAlarmCallback
void WmspMPParentIntervalAlarmCallback(void *p){WmspKickMPParent((u16)(u32)p);}
