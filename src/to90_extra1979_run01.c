#pragma force_active on
#include "global.h"
#pragma force_active on
#define U(p,o) (*(u32*)((u8*)(p)+(o)))
#define P(p,o) (*(void**)((u8*)(p)+(o)))
#define H(p,o) (*(u16*)((u8*)(p)+(o)))
#define B(p,o) (*((u8*)(p)+(o)))
typedef struct{s32 debt;u8*ctx;u64 tick;} Voice;extern Voice _0221A684;void VCT_Main(void);int ov00_021E6964(void),ov00_021E74A8(int),ov00_021E74FC(int);void ov00_021E765C(void);
void ov00_021E765C(void){u64 now=OS_GetTick();_0221A684.debt+=((u32)now-(u32)_0221A684.tick)*64000/33514-16666;if(_0221A684.debt<-10000)_0221A684.debt=0;_0221A684.tick=now;VCT_Main();while(_0221A684.debt>=16666){VCT_Main();_0221A684.debt-=16666;}if(U(_0221A684.ctx,0x1a80)){switch(U(_0221A684.ctx,0x1a7c)){case 0:PM_SetAmpGain(PM_AMPGAIN_160);break;case 1:PM_SetAmpGain(PM_AMPGAIN_80);break;case 2:PM_SetAmpGain(PM_AMPGAIN_40);break;case 3:PM_SetAmpGain(PM_AMPGAIN_20);break;}U(_0221A684.ctx,0x1a80)=0;}if(U(_0221A684.ctx,0x19e8)!=3){switch(U(_0221A684.ctx,0x19ec)){case 0:if(ov00_021E6964()==0){if(ov00_021E74A8(1)){U(_0221A684.ctx,0x19ec)=1;H(_0221A684.ctx,0x1a5a)=60;}}else if(ov00_021E6964()==1){if(ov00_021E74FC(0))U(_0221A684.ctx,0x19ec)=2;}break;case 1:H(_0221A684.ctx,0x1a5a)--;if(!H(_0221A684.ctx,0x1a5a))U(_0221A684.ctx,0x19ec)=0;break;case 2:break;}}}
