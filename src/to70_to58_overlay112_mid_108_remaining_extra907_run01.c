#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct W {u8 pad[0x20];void *save;u8 p24[0x10cc];u32 count;u8 p10f4[0x8c86];u16 arg;u8 p9d7c[0x139dc];u32 a,b;u8 p1d760[0xce0];void*obj;} W;
extern void *Save_LocalFieldData_Get(void*),*LocalFieldData_GetCurrentPosition(void*);extern void ov112_021F328C(void*,void*),sub_02032674(void*,u32*,u32*),sub_02032650(void*,u32,u16);
void ov112_021ED530(W*),ov112_021ED590(W*);
void ov112_021ED590(W*w){u32 count;sub_02032674(w->obj,&w->a,&w->b);count=w->count;if(count<w->a)sub_02032650(w->obj,w->a,w->arg);else sub_02032650(w->obj,count,w->arg);}
