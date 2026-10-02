#pragma force_active on
#include "global.h"
typedef struct {u8 pad[0xa];u8 printer;u8 p1[2];u8 kind;u8 p2;u8 ready;u8 p3[0x14];void *formatter;u8 p4[0x20];void *text;u8 window[0x40];void *obj;u8 count;u8 p5[11];void *options;u8 p6[0x164];void *a;void *b;u8 p7[12];void *c;} Work;
extern void ov82_0223FCB0(void *,int),ov82_0223FD78(void *,int),BufferTypeName(void *,int,int),ov82_0223EFB4(Work *,int,int),ov82_0223FCFC(void *,int),ov82_0223FDC8(void *,void *,u8);
extern int Options_GetFrame(void *),ov80_02237920(u8),ov82_0223F6C4(u8),sub_02030BD0(int,void *),ov82_0223F6E4(Work *),ov82_0223EF7C(Work *,int,int);
void ov82_0223F84C(Work *);
void ov82_0223F84C(Work *w){u8 n;int id;ov82_0223FCB0(w->b,0);ov82_0223FD78(w->window,Options_GetFrame(w->options));BufferTypeName(w->formatter,0,ov80_02237920(w->kind));n=sub_02030BD0(ov82_0223F6C4(w->kind),w->c)+1;if(n>10)n=10;ov82_0223EFB4(w,1,n);if(ov82_0223F6E4(w)==1)id=31;else id=24;w->printer=ov82_0223EF7C(w,id,1);ov82_0223FCFC(w->a,0);ov82_0223FDC8(w->obj,w->text,w->count);w->ready=1;}
