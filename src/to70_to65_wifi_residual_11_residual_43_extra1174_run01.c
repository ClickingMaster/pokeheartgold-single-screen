#pragma force_active on
// clang-format off
#include "global.h"
#include <nitro/code32.h>
// clang-format on
extern void WcmConfigure(void*,void*),WcmSetPhase(int),WcmWmcbIndication(void*),WcmWmcbCommon(void*);
extern int WM_Init(void*,u16),WM_GetAllowedChannel(void),WM_Finish(void),WM_SetIndCallback(void(*)(void*)),WM_Enable(void(*)(void*));
typedef struct{u8 p[0x2260];int phase;u8 r[8];u32 dma;u8 q[16];u16 command;}Work;
extern Work*_021D5FEC;
int WCM_StartupAsync(void*,void*);
int WCM_StartupAsync(void*cfg,void*cb){OSIntrMode irq=OS_DisableInterrupts();if(!_021D5FEC){OS_RestoreInterrupts(irq);return 1;}switch(_021D5FEC->phase){case 1:WcmConfigure(cfg,cb);break;case 2:OS_RestoreInterrupts(irq);return 2;case 3:OS_RestoreInterrupts(irq);return 0;default:OS_RestoreInterrupts(irq);return 1;}switch(WM_Init(_021D5FEC,(u16)_021D5FEC->dma)){case 0:break;case 3:WcmSetPhase(11);OS_RestoreInterrupts(irq);return 7;case 4:OS_RestoreInterrupts(irq);return 5;case 1:case 2:case 5:case 6:default:WcmSetPhase(11);OS_RestoreInterrupts(irq);return 7;}if(WM_GetAllowedChannel()==0){if(WM_Finish()!=0){WcmSetPhase(11);OS_RestoreInterrupts(irq);return 7;}OS_RestoreInterrupts(irq);return 5;}if(WM_SetIndCallback(WcmWmcbIndication)!=0){WcmSetPhase(11);OS_RestoreInterrupts(irq);return 7;}switch(WM_Enable(WcmWmcbCommon)){case 2:WcmSetPhase(2);_021D5FEC->command=1;break;case 8:WcmSetPhase(12);OS_RestoreInterrupts(irq);return 1;case 3:default:WcmSetPhase(11);OS_RestoreInterrupts(irq);return 7;}OS_RestoreInterrupts(irq);return 3;}
