#pragma force_active on
#include "global.h"
extern void ov48_0225AA50(void*),ov48_0225ACAC(void*),ov48_0225AA38(void*,int,int),ov48_0225A790(u8*,int,u32*,void*),ov48_0225ACD8(void*,u8,u8,u8);extern int ov48_0225A30C(u8*,u32*,int),ov48_0225B038(u32);
void ov48_0225A354(u8*,void*);
void ov48_0225A354(u8*w,void*arg){struct{u32 a,b,c;}value;int which,i,index,previous;u8*p;memset(w+0x40,0,72);*(u16*)(w+0x3c)=0;*(u16*)(w+0x3e)=0;p=w+0xa0;for(i=0;i<5;i++,p+=40)ov48_0225AA50(p);ov48_0225ACAC(w+0x1b8);previous=-1;for(i=0;i<3;i++){which=2-i;if(ov48_0225A30C(w,&value.a,i)==1){index=ov48_0225B038(value.a);ov48_0225AA38(w+0xa0+index*40,which,1);ov48_0225A790(w,which,&value.a,arg);*(u32*)(w+0x88+which*4)=index;ov48_0225ACD8(w+0x1b8,which,value.a,previous);*(u16*)(w+0x3e)+=6;previous=value.a;}else{*(u32*)(w+0x88+which*4)=5;previous=-1;}}}
