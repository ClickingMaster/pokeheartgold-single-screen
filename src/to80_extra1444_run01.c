#pragma force_active on
// clang-format off
#include "global.h"
#include <nitro/code32.h>
// clang-format on
#pragma force_active on
extern u8*mbc;
int IsChildAidValid(u16);
int MB_CommResponseRequest(u16,u32);
int MB_CommResponseRequest(u16 aid,u32 request){int response,state;OSIntrMode mode=OS_DisableInterrupts();switch(request){case 0:state=10;response=4;break;case 1:state=10;response=3;break;case 2:state=14;response=2;break;case 3:state=7;response=5;break;default:OS_RestoreInterrupts(mode);return 0;}if(mbc&&IsChildAidValid(aid)){u32 index=aid-1;u8*base=mbc;if(state==*(u32*)(base+index*4+0x14e8)){*(u16*)(base+index*2+0x1754)=response;OS_RestoreInterrupts(mode);return 1;}}OS_RestoreInterrupts(mode);return 0;}
