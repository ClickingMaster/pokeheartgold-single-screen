#pragma force_active on
#include <nitro/types.h>
// clang-format off
#include <nitro/code32.h>
// clang-format on
extern void CTRDG_VibPulseEdgeUpdate(void*),SND_BeginSleep(void),WVR_Shutdown(void),OS_Terminate(void);
void CTRDGi_CallbackForPulledOut(u32 tag,u32 data,int error);
// @fn CTRDGi_CallbackForPulledOut
void CTRDGi_CallbackForPulledOut(u32 tag,u32 data,int error){if((data&63)==2){CTRDG_VibPulseEdgeUpdate(0);SND_BeginSleep();WVR_Shutdown();OS_Terminate();}else OS_Terminate();}
