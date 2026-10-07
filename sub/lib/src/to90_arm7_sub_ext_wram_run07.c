#pragma force_active on
#include <nitro/types.h>
// clang-format off
#include <nitro/code32.h>
// clang-format on
extern void WmspKickMPChild(void *);
void WmspMPChildIntervalAlarmCallback(void *p);
// @fn WmspMPChildIntervalAlarmCallback
void WmspMPChildIntervalAlarmCallback(void *p){WmspKickMPChild(p);}
