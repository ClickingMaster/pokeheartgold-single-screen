#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct{int x;void*a,*b;u8 gap[0x9e];struct {s16 value,pad;} thresholds[3];}Work;typedef struct{int x,y,z;}Vec;extern int ov45_0222AE34(void*,int);extern void ov49_0225E420(void*,int,int,Vec*),ov45_0222AE24(void*,int,int);
void ov49_022695C4(void*parent,int index);
void ov49_022695C4(void*parent,int index){Work*w=parent;struct {int index,base;} input;input.index=index;int i=0,j;int twice=(s32)((u32)index*2);input.base=(s32)((u32)input.index+(u32)twice);Vec v;for(i=0;i<3;i++){j=ov45_0222AE34(w->a,(s32)((u32)i+(u32)input.base));ov49_0225E420(w->b,input.index,i,&v);if(j<3){u8*p=(u8*)w+j*4;do{if((*(s16*)(p+0xaa)+16)*4096>v.x)ov45_0222AE24(w->a,(s32)((u32)i+(u32)input.base),j);j++;p+=4;}while(j<3);}}}
