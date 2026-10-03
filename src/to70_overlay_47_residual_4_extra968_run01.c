#pragma force_active on
#include "global.h"
typedef struct{void *messages[9],*fmt,*string,*tmp,*arg;} Work;
extern int ov47_02259E9C[];extern void *NewMsgDataFromNarc(int,int,int,int);extern void *MessageFormat_New(int);extern void *String_New(int,int);
void ov47_02258C70(Work *,void *,int);
void ov47_02258C70(Work *w,void *arg,int heap){int *source=ov47_02259E9C;void **cursor;int i;w->arg=arg;i=0;cursor=w->messages;for(;i<9;i++)*cursor++=NewMsgDataFromNarc(0,27,*source,heap),source++;w->fmt=MessageFormat_New(heap);w->string=String_New(256,heap);w->tmp=String_New(256,heap);}
