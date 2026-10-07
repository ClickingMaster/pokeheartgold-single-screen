#pragma force_active on
#include "global.h"
#pragma force_active on
#pragma opt_loop_invariants off
#pragma opt_strength_reduction off
typedef struct W {u8 pad[0x20];void *save;u8 p24[0x10cc];u32 count;u8 p10f4[0x8c86];u16 arg;u8 p9d7c[0x139dc];u32 a,b;u8 p1d760[0xce0];void*obj;} W;
extern void *Save_LocalFieldData_Get(void*),*LocalFieldData_GetCurrentPosition(void*);extern void ov112_021F328C(void*,void*),sub_02032674(void*,u32*,u32*),sub_02032650(void*,u32,u16);
void ov112_021ED530(W*),ov112_021ED590(W*);
#pragma force_active on
#pragma opt_loop_invariants off
#pragma opt_strength_reduction off
void ov112_021ED530(W*w){int i;u8*p;u8*base;if(*(int*)LocalFieldData_GetCurrentPosition(Save_LocalFieldData_Get(w->save))==499)return;i=9;p=(u8*)w+0x1344;base=(u8*)w+0xb1b4;for(;i>=0;p-=0x224,i--){if((u16)(*(u16*)(p+0xb3ac)+0xfff9)<=1)ov112_021F328C(w->save,base+(i+1)*0x224);}}
