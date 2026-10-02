#pragma force_active on
// clang-format off
#include "global.h"
#include <nitro/code32.h>
// clang-format on
#pragma force_active on
void SNDi_SetPlayerParam(int,int,int,int);
void SND_SetPlayerTempoRatio(int,int);
void SND_SetPlayerTempoRatio(int player,int ratio){SNDi_SetPlayerParam(player,0x1a,ratio,2);}
