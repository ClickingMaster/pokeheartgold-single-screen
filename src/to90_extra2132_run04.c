#pragma force_active on
// clang-format off
#include "global.h"
#include <nitro/code32.h>
// clang-format on
#pragma force_active on
typedef struct {u32 pad[2];TPData raw;u8 pad10[40];u16 busy;} Touch;
extern Touch _021E36B0;void TP_WaitBusy(u32);
u32 TP_WaitRawResult(TPData*result){TP_WaitBusy(1);if(_021E36B0.busy&1)return 1;*result=_021E36B0.raw;return 0;}
