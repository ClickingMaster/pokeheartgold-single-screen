#pragma force_active on
#include "global.h"
typedef struct {u8 pad[0x18];u32*pc;} Work;int ov07_0221F9A8(Work*,u32*,int);
int ov07_0221F9A8(Work*w,u32*out,int count){u32 expected;int i;w->pc++;expected=*w->pc;w->pc++;if(expected!=count){if(expected!=count)GF_AssertFail();return 0;}for(i=0;i<count;i++){*out=*w->pc;w->pc++;out++;}return 1;}
