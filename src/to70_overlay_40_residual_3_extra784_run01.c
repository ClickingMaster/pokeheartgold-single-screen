#pragma force_active on
#include "global.h"
typedef struct{u8 p[0x6d8];int count;u8 q[0x818-0x6dc];void*current;void*stack[7];u32 a,b;u32 arr[8];}Work;
void ov40_0222C480(Work*);void ov40_0222C4B8(Work*);
void ov40_0222C480(Work*w){if(w->count>0)w->count--;else if(w->count!=0){w->count=0;GF_AssertFail();}w->current=w->stack[w->count];}
void ov40_0222C4B8(Work*w){int i;w->a=0;w->b=255;for(i=0;i<8;i++)w->arr[i]=0;}
