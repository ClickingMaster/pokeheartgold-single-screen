#pragma force_active on
#include "global.h"
typedef struct{u8 head[12];u8 data[0xec-12];}Entry;typedef struct{int count;Entry entries[1];}Inner;typedef struct{int x;Inner inner;}Data;extern Data*ov39_0222A2B4(void*);

int ov39_02227E8C(void*p,void**out,int max);

int ov39_02227E8C(void*p,void**out,int max){Inner*d=&ov39_0222A2B4(p)->inner;int count=d->count,i;Entry*entry;if(count>max)count=max;entry=d->entries;i=0;if(count>0)do{out[i]=entry->data;i++;entry++;}while(i<count);for(;i<max;i++)out[i]=0;return count;}
