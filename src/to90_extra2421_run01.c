#pragma force_active on
#include "global.h"
#pragma force_active on
extern void*ov80_0222A4EC(void*,u16,int,u8,void*,int,int);
void ov80_0222A52C(u8*w,u16*ids,u8*types,void**params,void**outputs,int count,int a,int b);
void ov80_0222A52C(u8*w,u16*ids,u8*types,void**params,void**outputs,int count,int a,int b){int i;u32 par;u32 out;void*v;u8 type;void*param;i=0;if(count>0){out=(u32)outputs;par=(u32)params;do{type=(u8)(types==0?0:types[i]);if(params==0)param=0;else param=*(void**)par;v=ov80_0222A4EC(w,*ids,i,type,param,a,b);if(outputs)*(void**)out=v;i++;w+=56;par+=4;ids++;out+=4;}while(i<count);}}
