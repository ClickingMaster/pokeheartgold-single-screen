#pragma force_active on
// clang-format off
#include "global.h"
#include <nitro/code32.h>
// clang-format on
#pragma force_active on
extern u8 *_021D5FEC;
extern void WcmEditScanExParam(void *,void *,u32);
extern void WcmSetPhase(int);
extern void WcmWmcbScanEx(void *);
extern int WM_StartScanEx(void *,void *);
int WCM_BeginSearchAsync(void *,void *,u32);
#define F32(x) (*(u32 *)(_021D5FEC+(x)))
#define F16(x) (*(u16 *)(_021D5FEC+(x)))
int WCM_BeginSearchAsync(void *a,void *b,u32 c){
 OSIntrMode irq=OS_DisableInterrupts();
 if(!_021D5FEC){OS_RestoreInterrupts(irq);return 1;}
 switch(F32(0x2260)){
 case 3:break;
 case 5:WcmEditScanExParam(a,b,c);OS_RestoreInterrupts(irq);return 2;
 case 6:WcmEditScanExParam(a,b,c);OS_RestoreInterrupts(irq);return 0;
 default:OS_RestoreInterrupts(irq);return 1;
 }
 WcmEditScanExParam(a,b,c);
 DC_InvalidateRange((void *)F32(0x2288),F16(0x228c));
 F32(0x2284)++;
 switch(WM_StartScanEx((void *)WcmWmcbScanEx,_021D5FEC+0x2288)){
 case 2:WcmSetPhase(5);F16(0x2280)=3;break;
 case 8:OS_RestoreInterrupts(irq);return 4;
 case 3:default:WcmSetPhase(11);OS_RestoreInterrupts(irq);return 7;
 }
 OS_RestoreInterrupts(irq);return 3;
}
