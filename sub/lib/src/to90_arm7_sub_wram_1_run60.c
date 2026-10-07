#pragma force_active on
#include <nitro/types.h>
// clang-format off
#include <nitro/code32.h>
// clang-format on
typedef struct W35229Shared {u8 pad[0x260];s16 globals[16];} W35229Shared;
extern W35229Shared *SNDi_SharedWork;
void SND_SetPlayerGlobalVariable(int i,s16 v);
// @fn SND_SetPlayerGlobalVariable
void SND_SetPlayerGlobalVariable(int i,s16 v){SNDi_SharedWork->globals[i]=v;}
