#pragma force_active on
#include "global.h"
typedef struct {u8 pad[0x14];u16 a,b;} Stats;typedef struct{u8 pad[0x18];u32*pc;u8 rest[0xa4];Stats*stats;} Work;
int ov07_0223192C(Work*,u32);void ov07_0221D174(Work*);
void ov07_0221D174(Work*w){u32 which;int result;w->pc++;which=*w->pc;w->pc++;if(which==0)result=ov07_0223192C(w,w->stats->a);else result=ov07_0223192C(w,w->stats->b);if(result==4)w->pc++;w->pc+=*w->pc;}
