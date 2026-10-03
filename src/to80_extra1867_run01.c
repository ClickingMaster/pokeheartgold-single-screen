#pragma force_active on
#include "global.h"
typedef struct{void *arg;u8 p[0x54];u8 ready,count;} Work;extern void ov96_021F87E0(void *,int),ov96_021F82E4(Work *,void *,int),ov96_021F8334(Work *,void *,int);extern void *ov96_021F8378(Work *,void *,int,int);extern int ov96_021F8978(void);void ov96_021F80A8(Work *,void *,u8,int);
static inline void setCount(Work *w,u32 c){w->count=c;}
void ov96_021F80A8(Work *w,void *arg,u8 count,int flag){int offset;int i,x,y;setCount(w,count);ov96_021F87E0(w->arg,flag);x=0;i=x;y=x;for(;i<4;i++){offset=x+10;void *obj=ov96_021F8378(w,arg,256,156-y);if(!ov96_021F8978())offset+=10;ov96_021F82E4(w,obj,offset);x+=10;y+=40;}if(count>4){void *obj=ov96_021F8378(w,arg,256,40);ov96_021F8334(w,obj,0);}w->ready=1;}
