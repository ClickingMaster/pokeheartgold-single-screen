#pragma force_active on
// clang-format off
#include <nitro/types.h>
#include <nitro/code32.h>
// clang-format on
#define H(p,o) (*(u16*)((u8*)(p)+(o)))
#define SH(p,o) (*(s16*)((u8*)(p)+(o)))
#define U(p,o) (*(u32*)((u8*)(p)+(o)))
#define P(p,o) (*(u8**)((u8*)(p)+(o)))
extern u8 wmspW[];
typedef struct {u32 operation;u8 *rx;u32 rxSize;u8 *tx;u32 txSize;u8 parameters[0x1c];u32 flags;u16 minimum,maximum,limit,extra;u8 first,second,third,pad;} W40745Request;

typedef struct{u8 pad[0x54c];u8*config;u8*status;}w37012_Context;
#define CTX ((w37012_Context*)(wmspW+0x1000))

extern void *__VENEER_WMSP_GetBuffer4Callback2Wm9(void),*WMSP_WL_ParamSetNullKeyResponseMode(void*,u32);
extern void __VENEER_WMSP_ReturnResult2Wm9(void*),WMSP_CleanSendQueue(u32),WMSP_InitSendQueue(void),WMSP_SetThreadPriorityHigh(void),WMSP_SetVAlarm(void);
extern u32 WMSP_SetMPParameterCore(void*,void*);
extern u32 __VENEER_OS_DisableInterrupts(void);extern void __VENEER_OS_RestoreInterrupts(u32);extern u64 __VENEER_OS_GetTick(void);
void WMSP_StartMP(W40745Request*request);
// @fn WMSP_StartMP
void WMSP_StartMP(W40745Request*request){
 u8*base=wmspW;u8*rx=request->rx;u32 rxSize=request->rxSize;u8*tx=request->tx;u32 txSize=request->txSize;u32 error=0;u32 flags;u8*state=*(u8**)(wmspW+0x1550);u8*settings;u32 irq;u32 buffer[128];void*cb;void*result;
 if(!H(state,0x9c)){
  u32 needed=(H(state,0x3c)+31)&~31u;
  if(txSize<needed)error=6;
  if(H(state,0x188)==0)needed=(H(state,0x3e)+12)*H(state,0xf8)+0x29;else needed=H(state,0x3e)+0x51;
  needed&=~31u;if(rxSize<needed)error=6;
 }
 if(H(state,0xe6)==2){if(!(H(state,0x1f6)&((1<<H(*(u8**)(base+0x154c),0x46))>>1)))error=6;}
 if(error){cb=__VENEER_WMSP_GetBuffer4Callback2Wm9();H(cb,0)=14;H(cb,2)=error;H(cb,4)=10;__VENEER_WMSP_ReturnResult2Wm9(cb);return;}
 {u32 clean=0;if(U(state,12)){U(state,12)=0;clean=1;}if(clean)WMSP_CleanSendQueue(0xffff);}WMSP_InitSendQueue();irq=__VENEER_OS_DisableInterrupts();WMSP_SetMPParameterCore((u8*)request+20,0);
 {flags=request->flags;settings=CTX->status;
  if(H(settings,0)!=9&&H(settings,0)!=10){u32 old=__VENEER_OS_DisableInterrupts();u8*mp=settings+0x700;u32 max=(flags&4)?request->limit:H(settings,0x5c);u32 a,b;
   if(!max)max=16;a=(flags&1)?request->minimum:H(settings,0x58);if(!a)a=16;if(a>max)a=max;
   b=(flags&2)?request->maximum:H(settings,0x5a);H(mp,0xc4)=max;if(!b)b=16;H(mp,0xc0)=a;if(b>max)b=max;H(mp,0xc2)=b;
   if(SH(settings,0x62)>(s32)max)H(settings,0x62)=max;
   H(settings,0x7ca)=(flags&0x200)?request->extra:H(settings,0x98);
   H(settings,0x7c6)=(flags&0x400)?request->first:H(settings,0x92);
   H(settings,0x7c8)=(flags&0x800)?request->second:H(settings,0x94);
   H(settings,0x7cc)=(flags&0x1000)?request->third:H(settings,0x9a);__VENEER_OS_RestoreInterrupts(old);
  }
 }
 if((u16)(H(state,0)+0xfff9)<=1){u64 tick;int i;H(state,0x84)=0;H(state,0x5e)=0;H(state,0x60)=1;H(state,0x88)=0;H(state,0x9e)=0;H(state,0xa0)=60;U(state,0x734)=0;H(state,0x8a)=0;H(state,0x8c)=0;H(state,0x8e)=0;H(state,0x90)=0;H(state,0x66)=0;P(state,0x74)=rx;H(state,0x72)=rxSize;P(state,0x78)=rx+rxSize;H(state,0x70)=0;P(state,0x7c)=tx;H(state,0x80)=txSize;H(state,0x62)=0;H(state,0x64)=0;H(state,0x68)=0;H(state,0x6a)=0;H(state,0xbe)=0xffff;H(state,0xc0)=1;
  tick=__VENEER_OS_GetTick()|1;for(i=0;i<16;i++)*(u64*)(state+0x738+i*8)=tick;
  WMSP_SetThreadPriorityHigh();H(state,0xce)=0;WMSP_SetVAlarm();if(H(state,0)==8)H(state,0)=10;else if(H(state,0)==7)H(state,0)=9;
  cb=__VENEER_WMSP_GetBuffer4Callback2Wm9();H(cb,0)=14;H(cb,2)=0;H(cb,4)=10;__VENEER_WMSP_ReturnResult2Wm9(cb);U(state,12)=1;__VENEER_OS_RestoreInterrupts(irq);
  result=WMSP_WL_ParamSetNullKeyResponseMode(buffer,1);if(H(result,4)){cb=__VENEER_WMSP_GetBuffer4Callback2Wm9();H(cb,0)=14;H(cb,2)=1;H(cb,4)=0x216;H(cb,6)=H(result,4);__VENEER_WMSP_ReturnResult2Wm9(cb);}
 }else{__VENEER_OS_RestoreInterrupts(irq);cb=__VENEER_WMSP_GetBuffer4Callback2Wm9();H(cb,0)=14;H(cb,2)=3;H(cb,4)=10;__VENEER_WMSP_ReturnResult2Wm9(cb);}
}
