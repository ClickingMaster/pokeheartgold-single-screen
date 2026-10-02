#pragma force_active on
#include "global.h"
typedef struct {void *a,*b,*c;u8 p[0x54];int z[6];void *msg,*str,*cursor;u8 q[0x15c];u16 x,y;u8 r[12];void *arg;} W;
extern void *Heap_Alloc(int,int);extern void *ov102_021E8F68(void*);extern void *String_New(int,int);extern void *NewMsgDataFromNarc(int,int,int,int);extern void *ListMenuCursorNew(int);
W *ov102_021EA314(void*,void*,void*);
W *ov102_021EA314(void*a,void*b,void*c) {W*w=Heap_Alloc(35,500);w->a=a;w->b=b;w->c=c;w->arg=ov102_021E8F68(w->b);w->z[0]=0;w->z[1]=0;w->z[2]=0;w->z[3]=0;w->z[4]=0;w->z[5]=0;w->str=String_New(128,35);w->msg=NewMsgDataFromNarc(0,27,283,35);w->cursor=ListMenuCursorNew(35);w->x=0;w->y=0;return w;}
