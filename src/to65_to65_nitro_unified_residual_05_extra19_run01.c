#pragma force_active on
// clang-format off
#include "global.h"
#include <nitro/code32.h>
// clang-format on
#pragma force_active on
void PushCommand_impl(int,void*,int,int,int);
void SND_ReadDriverInfo(void*);
void SND_ReadDriverInfo(void *info){PushCommand_impl(0x21,info,0,0,0);}
