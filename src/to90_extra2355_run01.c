#pragma force_active on
#include "global.h"
typedef struct{int value;void*object;int*out;}Entry;typedef struct{s16 count;u8 pad[6];Entry*entries;}Inner;typedef struct{u8 pad[256];Inner*inner;}Work;extern void ov01_021FA2B8(void*,int);

void ov01_021FA75C(Work*w,void*p,int*out,int value);


#pragma force_active on
#pragma opt_lifetimes off
void ov01_021FA75C(Work*w,void*p,int*out,int value){int i=0;Inner*inner=w->inner;s16 count=inner->count;Entry*entry=inner->entries;for(;i<count;i++,entry++){if(entry->object==0){entry->object=p;entry->out=out;entry->value=value;*entry->out=0;ov01_021FA2B8(p,1);return;}}GF_AssertFail();}
