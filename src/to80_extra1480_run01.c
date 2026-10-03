#pragma force_active on
// clang-format off
#include "global.h"
#include <nitro/code32.h>
// clang-format on
#pragma force_active on
#pragma opt_strength_reduction off
extern s32 _021D7008[];
extern s16 sImpulseResponse[],sDelayLine[];
extern s32 sTap[],sH[];
u32 HPFFilter(s16*,u32);
void InitFIRFilter(void);
void InitFIRFilter(void){s16*impulse;int i,j;_021D7008[3]=0;_021D7008[2]=413;impulse=sImpulseResponse;for(i=0;i<2048;i++)sDelayLine[i]=0;for(j=0;j<6;j++){sTap[j]=0;sH[j]=impulse[j];}}
