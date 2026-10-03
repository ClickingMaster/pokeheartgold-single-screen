#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct {void *object;int remaining,delta;} Work;extern void ov41_02249B44(void *,int *,int *);extern void ov41_02249AF4(void *,int,int);void ov41_02248DC8(Work *);
void ov41_02248DC8(Work *w){int x,y;ov41_02249B44(w->object,&x,&y);y+=w->delta;ov41_02249AF4(w->object,x,y);if(--w->remaining<=0){u8 *p=(u8 *)w;p[0]=0;p[1]=0;p[2]=0;p[3]=0;p[4]=0;p[5]=0;p[6]=0;p[7]=0;p[8]=0;p[9]=0;p[10]=0;p[11]=0;}}
