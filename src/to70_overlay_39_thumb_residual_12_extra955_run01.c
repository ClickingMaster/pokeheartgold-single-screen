#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct{u8 pad[16];u8 data[0x21c];}Entry;
typedef struct{int a,count;Entry entries[1];}Work;
extern Work *ov39_0222A2B4(void*);

int ov39_02227F14(void*w,void **out,int cap);

int ov39_02227F14(void*w,void **out,int cap){int count;int *base=&ov39_0222A2B4(w)->count;int i;Entry *e;count=*base;if(count>cap){count=cap;GF_AssertFail();}e=(Entry*)(base+1);i=0;if(i<count){void **dst=out;do{*dst++=e->data;i++;e++;}while(i<count);}if(i<cap){void **dst=out+i;do{*dst++=0;i++;}while(i<cap);}return count;}
