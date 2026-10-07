#pragma force_active on
#include <nitro/types.h>
// clang-format off
#include <nitro/code32.h>
// clang-format on
extern void CTRDG_VibPulseEdgeUpdate(void*);
void CTRDGi_CallbackForCTREx(u32 tag,u32 data,int error);
// @fn CTRDGi_CallbackForCTREx
void CTRDGi_CallbackForCTREx(u32 tag,u32 data,int error){CTRDG_VibPulseEdgeUpdate((void*)data);}
