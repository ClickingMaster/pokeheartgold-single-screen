#pragma force_active on
#include <nitro/types.h>
#include "SND_work.h"
// clang-format off
#include <nitro/code32.h>
// clang-format on
typedef char SharedExtent[sizeof(struct SNDSharedWork)==0x280?1:-1];
typedef char PlayerExtent[sizeof(struct SNDPlayer)==0x24?1:-1];
s16 *GetVariablePtr(void *,int);
s16 *GetVariablePtr(void *player,int index){struct SNDSharedWork *shared=SNDi_SharedWork;if(!shared)return 0;if(index<16)return shared->players[((const u8 *)player)[1]].localVars+index;index-=16;return shared->globalVars+index;}
