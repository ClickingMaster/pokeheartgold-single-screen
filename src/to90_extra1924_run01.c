#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct{int a,b;void*p;} Entry;typedef struct{s16 count,pad;u16 other,lock;int x,y;Entry*entries;} State;typedef struct{u8 pad[0xf0];void*p;u8 gap[12];State*state;} Work;

void ov01_021FA668(Work*w,int value,int unused);
void ov01_021FA6A4(Work*w,int value);

void ov01_021FA6A4(Work*w,int value){State*s=w->state;int i;Entry*p=s->entries;int count;s->lock=1;count=s->count;for(i=0;i<count;i++,p++){if(p->p!=0 && p->b==value){p->p=0;p->a=0;break;}}s->lock=0;}
