#pragma force_active on
#include "global.h"
#pragma force_active on
struct State{u8 pad[0x90];void*data;u8 p94[5];u8 a;u8 p9A[2];u8 kind,b;};struct Output{void*value,*arg;};void*ov96_02214EB4(void*,u8,int,void*,void*);void*ov96_02214C3C(void*,u8,int,void*,void*);void*ov96_02215184(void*,u8,int,void*,void*,struct State*);void ov96_022155A0(void*,struct State*,struct Output*);
void ov96_022155A0(void*c,struct State*s,struct Output*out){u8 n=s->b+s->a;int kind=s->kind;switch(kind){case 0:out->value=ov96_02214EB4(c,n,kind,out->arg,s->data);break;case 1:out->value=ov96_02214C3C(c,n,kind,out->arg,s->data);break;case 2:out->value=ov96_02215184(c,n,kind,out->arg,s->data,s);break;default:GF_AssertFail();break;}}
