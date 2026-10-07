#pragma force_active on
#include "global.h"
#pragma force_active on
struct State{u32 a;void*ctx;u8 pad[20];u16 vals[4];u32 val;void*pics[2];};struct State*ov07_022324D8(void*,int);void ov07_02231FE4(void*,struct State*);int ov07_0221C468(void*);int ov07_0221C470(void*);void*ov07_0221FA48(void*,int);void Pokepic_SetAttr(void*,int,int);
extern u32 ov07_0221C4A8(void *p, int i);void ov07_02228E70(void);void ov07_02228F14(void);void ov07_0221C410(void*,void(*)(void),void*);void ov07_02228F88(void*);
void ov07_02228F88(void*x){struct State*s=ov07_022324D8(x,112);int flag;ov07_02231FE4(x,s);s->pics[0]=ov07_0221FA48(s->ctx,ov07_0221C468(s->ctx));s->pics[1]=ov07_0221FA48(s->ctx,ov07_0221C470(s->ctx));Pokepic_SetAttr(s->pics[0],11,100);flag=((s32)ov07_0221C4A8(x, 0));s->vals[0]=((s32)ov07_0221C4A8(x, 1));s->vals[1]=((s32)ov07_0221C4A8(x, 2));s->vals[2]=((s32)ov07_0221C4A8(x, 3));s->vals[3]=((s32)ov07_0221C4A8(x, 4));s->val=((s32)ov07_0221C4A8(x, 5));if(!flag)ov07_0221C410(s->ctx,ov07_02228E70,s);else ov07_0221C410(s->ctx,ov07_02228F14,s);}
