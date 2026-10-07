#pragma force_active on
#include <nitro/types.h>
// clang-format off
#include <nitro/code32.h>
// clang-format on
extern u32 CTRDGi_Work;extern u8 CTRDGi_EnableFlag[];extern void __VENEER_SVC_CpuSet(const void*,void*,u32);extern int OS_GetLockID(void);
void CTRDGi_InitCommon(void);
// @fn CTRDGi_InitCommon
void CTRDGi_InitCommon(void){u32 zero=0;__VENEER_SVC_CpuSet(&zero,&CTRDGi_Work,0x05000001);*(u16*)(CTRDGi_EnableFlag+6)=OS_GetLockID();}
