#pragma force_active on
// clang-format off
#include "global.h"
#include <nitro/code32.h>
// clang-format on
extern void WCM_UpdateOption(u32),WcmSetPhase(int),WcmWmcbCommon(void*);
extern int WM_SetLifeTime(void(*)(void*),u16,u16,u16,u16);
typedef struct{u8 p[0x3c];u16 invalid;u8 q[0x82];}Ap;
typedef struct{u8 type,key;u8 bytes[80];}Key;
typedef struct{u8 p[0x2140];Ap ap;u8 bytes[80];u8 keytype,keyidx;u8 q[14];int phase;u8 r[28];u16 command;}Work;
extern Work*_021D5FEC;
int WCM_ConnectAsync(Ap*,Key*,u32);
int WCM_ConnectAsync(Ap*ap,Key*key,u32 options){OSIntrMode irq=OS_DisableInterrupts();if(!_021D5FEC){OS_RestoreInterrupts(irq);return 1;}switch(_021D5FEC->phase){case 3:if(!ap){OS_RestoreInterrupts(irq);return 1;}if(ap->invalid){OS_RestoreInterrupts(irq);return 1;}if(key){if(key->type>=4||key->key>=4){OS_RestoreInterrupts(irq);return 1;}_021D5FEC->keytype=key->type;_021D5FEC->keyidx=key->key;if(_021D5FEC->keytype==0)MI_CpuFill8(_021D5FEC->bytes,0,80);else MI_CpuCopy8(key->bytes,_021D5FEC->bytes,80);}else MI_CpuFill8(_021D5FEC->bytes,0,82);MI_CpuCopy8(ap,&_021D5FEC->ap,192);*(u16*)((u8*)_021D5FEC+0x2170)=*(u16*)((u8*)_021D5FEC+0x216e)|3;WCM_UpdateOption(options);break;case 8:OS_RestoreInterrupts(irq);return 2;case 9:OS_RestoreInterrupts(irq);return 0;default:OS_RestoreInterrupts(irq);return 1;}switch(WM_SetLifeTime(WcmWmcbCommon,65535,80,65535,65535)){case 2:WcmSetPhase(8);_021D5FEC->command=5;break;case 8:OS_RestoreInterrupts(irq);return 4;case 3:default:WcmSetPhase(11);OS_RestoreInterrupts(irq);return 7;}OS_RestoreInterrupts(irq);return 3;}
