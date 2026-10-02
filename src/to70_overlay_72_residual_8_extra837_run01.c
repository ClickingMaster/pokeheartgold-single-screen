#pragma force_active on
#include "global.h"
typedef struct{u8 p[0xbd4];void *msg;u8 p2[8];void *a,*b,*c,*d,*e;} Work;
extern void *String_New(int,int),*NewString_ReadMsgData(void *,int);
void ov72_02239040(Work *);
void ov72_02239040(Work *w){w->a=String_New(180,67);w->c=String_New(4,67);w->d=String_New(3,67);w->b=NewString_ReadMsgData(w->msg,12);w->e=String_New(256,67);}
