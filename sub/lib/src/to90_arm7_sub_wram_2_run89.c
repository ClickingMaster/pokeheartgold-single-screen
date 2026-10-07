#pragma force_active on
#include <nitro/types.h>
// clang-format off
#include <nitro/code32.h>
// clang-format on
extern u32 lock_id[];extern void OS_Terminate(void);
void CTRDGi_CallbackForInitModuleInfo(u32 tag,u32 data,int error);
// @fn CTRDGi_CallbackForInitModuleInfo
void CTRDGi_CallbackForInitModuleInfo(u32 tag,u32 data,int error){if((data&63)==1){lock_id[6]=data;lock_id[10]=1;}else OS_Terminate();}
