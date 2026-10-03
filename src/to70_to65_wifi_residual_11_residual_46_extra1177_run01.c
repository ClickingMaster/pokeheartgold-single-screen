#pragma force_active on
// clang-format off
#include "global.h"
#include <nitro/code32.h>
// clang-format on
typedef struct{u8 pad[0x2260];u32 phase;u8 pad2[7];u8 reset;u8 pad3[0x14];u16 request;}Work;
extern Work*_021D5FEC;extern void WcmSetPhase(int),WcmWmcbCommon(void*),WcmWmcbReset(void*);extern u16*WMi_GetStatusAddress(void);extern int WM_Finish(void),WM_Disable(void(*)(void*)),WM_PowerOff(void(*)(void*)),WM_Reset(void(*)(void*));
int WCM_TerminateAsync(void);
int WCM_TerminateAsync(void){OSIntrMode irq=OS_DisableInterrupts();int code;u16*status;if(_021D5FEC==NULL){OS_RestoreInterrupts(irq);return 1;}switch(_021D5FEC->phase){case 13:OS_RestoreInterrupts(irq);return 2;case 1:OS_RestoreInterrupts(irq);return 0;case 6:WcmSetPhase(13);_021D5FEC->request=9;OS_RestoreInterrupts(irq);return 3;case 3:case 9:case 12:break;case 0:case 2:case 4:case 5:case 7:case 8:case 10:case 11:default:OS_RestoreInterrupts(irq);return 1;}if(_021D5FEC->reset==1){WcmSetPhase(13);_021D5FEC->request=9;goto accepted;}status=WMi_GetStatusAddress();DC_InvalidateRange(status,2);switch(*status){case 0:code=WM_Finish();if(code==0){WcmSetPhase(1);_021D5FEC->request=0;OS_RestoreInterrupts(irq);return 0;}break;case 1:code=WM_Disable(WcmWmcbCommon);break;case 2:code=WM_PowerOff(WcmWmcbCommon);break;default:_021D5FEC->reset=1;code=WM_Reset(WcmWmcbReset);break;}switch(code){case 2:WcmSetPhase(13);_021D5FEC->request=9;break;case 8:OS_RestoreInterrupts(irq);return 4;case 3:default:WcmSetPhase(11);OS_RestoreInterrupts(irq);return 7;}accepted:OS_RestoreInterrupts(irq);return 3;}
