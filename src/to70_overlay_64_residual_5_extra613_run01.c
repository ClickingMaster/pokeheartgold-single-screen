#pragma force_active on
#include "global.h"
extern void *NewMsgDataFromNarc(int,int,int,int);
extern void *MessageFormat_New(int);
extern void *String_New(int,int);
typedef struct {u8 pad[0x118]; void *a,*b,*c,*format,*s1,*s2; u8 gap[0x58]; void *x,*y; u8 gap2[0x14]; void *s3,*s4;} Work;
void ov64_021E607C(Work*);
void ov64_021E607C(Work *w) {
w->a=NewMsgDataFromNarc(0,27,181,59); w->b=NewMsgDataFromNarc(1,27,237,59);w->c=NewMsgDataFromNarc(1,27,750,59);w->format=MessageFormat_New(59);w->s1=String_New(256,59);w->s2=String_New(256,59);w->s3=String_New(256,59);w->s4=String_New(256,59);w->x=w->s3;w->y=w->s4;
}
