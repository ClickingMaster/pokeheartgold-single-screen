// clang-format off
#include "global.h"
#include <nitro/code32.h>
// clang-format on
#pragma force_active on
extern u8 _021D601C,_021D6020[];
void WCMi_ShelterRssi(int);
static inline u8 CalibrateRssi(int sample){if(sample&2)return (u8)(sample>>2);return (u8)((sample>>2)+25);}
void WCMi_ShelterRssi(int sample){u8 value=CalibrateRssi(sample);int index=_021D601C;_021D6020[index%16]=value;if((u32)index<16)_021D601C=index+1;else _021D601C=(index+1)%16+16;}
