#pragma force_active on
// clang-format off
#include "global.h"
#include <nitro/code32.h>
// clang-format on
#pragma force_active on
void PushCommand_impl(int,int,int,int,int);
void SND_SetMasterVolume(int);
void SND_SetMasterVolume(int value){PushCommand_impl(0x17,value,0,0,0);}
