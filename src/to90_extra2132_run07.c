#pragma force_active on
// clang-format off
#include "global.h"
#include <nitro/code32.h>
// clang-format on
#pragma force_active on

typedef struct {u8 pad[16];u16 index,samples;TPData*buffer;u16 capacity;} AutoPoint;
extern AutoPoint _021E36B0;
void TP_GetLatestRawPointInAuto(TPData*out){int i,index,latest;TPData*p;out->validity=3;latest=_021E36B0.index;if(_021E36B0.samples==1||_021E36B0.capacity==1){*out=_021E36B0.buffer[latest];return;}for(i=0;i<_021E36B0.samples&&i<_021E36B0.capacity-1;i++){index=latest-i;if(index<0)index+=_021E36B0.capacity;p=&_021E36B0.buffer[index];if(!p->touch){*out=*p;return;}if((out->validity&1)&&!(p->validity&1)){out->x=p->x;if(i)out->validity&=~1;}if((out->validity&2)&&!(p->validity&2)){out->y=p->y;if(i)out->validity&=~2;}if(!out->validity){out->touch=1;return;}}out->touch=1;}
