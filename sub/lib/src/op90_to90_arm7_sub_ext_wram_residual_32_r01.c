#pragma force_active on
#include <nitro/types.h>
// clang-format off
#include <nitro/code32.h>
// clang-format on
#define BASE (*(u8 **)0x0380fff4)
#define H(p,o) (*(u16 *)((u8*)(p)+(o)))
typedef struct {u16 unknown[6],mode,power;u16 gap10[1],reset;u16 gap14[40],flags64;u16 gap66[4],interval;u16 gap70[6],control;} W40744State;
#define HW(a) (*(volatile u16 *)(a))
void WStop(void);void RND_Init(u32,u32);void InitCAM(void);void InitApList(void);void InitTxCtrl(void);void InitRxCtrl(void);u64 __VENEER__ll_udiv(u64,u64);void WSetStaState(u32);void StartBeaconFrame(void);void WDisableTmpttPowerSave(void);int WSetPowerState(u32);void WaitLoop_Rxpe(void);
void WStart(void);
#pragma opt_propagation off
static inline void ProgramNextBeacon(const W40744State *state){
 union {u64 value;u16 part[4];} stamp;u16 *words=stamp.part;u64 period;
 words[0]=HW(0x048080f8);words[1]=HW(0x048080fa);words[2]=HW(0x048080fc);words[3]=HW(0x048080fe);
 period=(u32)state->interval<<10;stamp.value=__VENEER__ll_udiv(stamp.value,(u64)period)+1;stamp.value*=period;
 HW(0x048080f6)=words[3];HW(0x048080f4)=words[2];HW(0x048080f2)=words[1];HW(0x048080f0)=words[0]|1;}
// @fn WStart
void WStart(void){
 u8*w=BASE;W40744State*s=(W40744State*)(w+0x344);u8*param=w+0x31c;u32 a,b,c;struct {volatile u16 tx,rx;}*enable;
 WStop();a=HW(0x04808044);b=HW(0x04808044);c=HW(0x04808044);RND_Init(a+(b<<8),c);
 s->control=1;if(((struct {u16 low:2,preamble:1,high:13;}*)(param+0x1e))->preamble==1)s->control|=0x20;if(H(param,0x18))s->control|=0x10;
 s->reset=0;HW(0x04808032)=0x8000;HW(0x04808134)=65535;HW(0x0480802a)=0;HW(0x04808028)=0;HW(0x04808038)=15;
 InitCAM();InitApList();InitTxCtrl();InitRxCtrl();HW(0x04808030)=0x8000;HW(0x04808010)=65535;HW(0x048081ae)=0x1fff;
 if(H(BASE,0x690)&8)HW(0x048081aa)=0x400;else HW(0x048081aa)=0;
 enable=(void*)0x04808008;enable->tx=0;enable->rx=0;
 switch(s->mode){
 case 0:HW(0x04808012)=0x3f;HW(0x048080d0)=65535;HW(0x048080e0)=8;enable->tx=0;enable->rx=0;HW(0x048080e8)=0;HW(0x04808004)=1;break;
 case 1:{
 HW(0x04808012)=0x703f;HW(0x048081ae)=0x1fff;HW(0x048080d0)=0x301;HW(0x048080e0)=13;enable->tx=0xe000;HW(0x04808004)=1;
 ProgramNextBeacon(s);HW(0x048080e8)=1;HW(0x048080ea)=1;WSetStaState(64);StartBeaconFrame();HW(0x048080ae)=2;break;}
 case 2:HW(0x04808012)=0xe0bf;if(H(BASE,0x690)&0x20){HW(0x04808012)|=0x40;HW(0x048081aa)|=0x68;}if(s->flags64&1)HW(0x048080d0)=0x581;else HW(0x048080d0)=0x181;HW(0x048080e0)=11;HW(0x04808004)=1;HW(0x048080e8)=1;HW(0x048080ea)=1;WSetStaState(32);break;
 case 3:HW(0x04808010)=65535;HW(0x04808012)=0xc03f;if(s->flags64&1)HW(0x048080d0)=0x401;else HW(0x048080d0)=1;HW(0x048080e0)=11;HW(0x04808004)=1;HW(0x048080e8)=1;HW(0x048080ea)=1;HW(0x04808048)=0;WSetStaState(32);break;
 case 4:HW(0x04808012)=0;HW(0x048081ae)=0;HW(0x04808004)=1;WSetStaState(32);break;}
 HW(0x04808048)=0;WDisableTmpttPowerSave();HW(0x048080ae)=2;if(s->power==1)WSetPowerState(2);WaitLoop_Rxpe();
}
#pragma opt_propagation reset
