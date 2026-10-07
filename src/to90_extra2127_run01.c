#pragma force_active on
// clang-format off
#include "global.h"
#include <nitro/code32.h>
// clang-format on
#pragma force_active on
typedef struct{u8 pad[0x118];u32 first,second;}CacheCommon;void CARD_SetCacheFlushThreshold(u32,u32);
void CARD_SetCacheFlushThreshold(u32 first,u32 second){CacheCommon*p=(CacheCommon*)&cardi_common;p->first=first;p->second=second;}
