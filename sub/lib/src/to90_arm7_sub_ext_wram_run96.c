#pragma force_active on
#include <nitro/types.h>
// clang-format off
#include <nitro/code32.h>
// clang-format on

void*WL_GetThreadStruct(void);
// @fn WL_GetThreadStruct
void*WL_GetThreadStruct(void){return *(u8*volatile*)0x380fff4+24;}
