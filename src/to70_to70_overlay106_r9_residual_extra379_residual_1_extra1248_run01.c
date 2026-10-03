#pragma force_active on
#include "global.h"
typedef struct{u8 p[0x1c];int first,start,second;u32 b;int end,total;} Blend;
typedef struct{u8 p[0x410];int next,count;Blend *blend;} Work;
int ov106_021E68F4(Work *);
int ov106_021E68F4(Work *w){struct{volatile int end,count;} local;Blend *b=w->blend;int count=w->count,total=b->total,value;local.count=count;if(count==total){value=b->end;G2x_SetBlendAlpha_(0x4000050,b->first,b->second,value,abs(15-value));w->count=0;return w->next;}else{int end=b->end;local.end=end;int start=b->start;u32 delta=(abs(start-end)<<8)/total;delta=(local.count*delta)>>8;if(start>local.end)value=start-delta;else value=delta+start;G2x_SetBlendAlpha_(0x4000050,b->first,b->second,value,abs(15-value));w->count++;return 3;}}
