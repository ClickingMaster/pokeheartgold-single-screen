#pragma force_active on
// clang-format off
#include "global.h"
#include <nitro/code32.h>
// clang-format on
#pragma force_active on
#pragma opt_propagation off
#define W(p,n) (*(int*)((u8*)(p)+(n)))
#define P(p,n) (*(void**)((u8*)(p)+(n)))
typedef void (*Callback)(int,void*);
extern u8 _021E36EC[];
void MicCommonCallback(PXIFifoTag,u32,BOOL);
void MicCommonCallback(PXIFifoTag tag,u32 data,BOOL err){int result,cmd;u32 high,reply;if(err){if(W(_021E36EC,4))W(_021E36EC,4)=0;if(P(_021E36EC,8)){Callback cb=(Callback)P(_021E36EC,8);P(_021E36EC,8)=0;cb(6,P(_021E36EC,12));}}high=data&0x7f00;reply=data&255;cmd=(u16)(high>>8);switch(reply){case 0:result=0;break;case 1:result=4;break;case 2:result=2;break;case 3:result=5;break;case 4:result=1;break;default:result=6;break;}if(cmd==0x51){if(P(_021E36EC,20))((Callback)P(_021E36EC,20))(result,P(_021E36EC,24));return;}if(cmd==0x40){if(P(_021E36EC,28))*(u16*)P(_021E36EC,28)=*(u16*)0x027fff94;}if(W(_021E36EC,4))W(_021E36EC,4)=0;if(P(_021E36EC,8)){Callback cb=(Callback)P(_021E36EC,8);P(_021E36EC,8)=0;cb(result,P(_021E36EC,12));}}
