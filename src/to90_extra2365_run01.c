#pragma force_active on
#include "global.h"
#pragma force_active on
extern int sub_0203769C(void),sub_02037454(void),sub_020378AC(int);
static inline void swap(int*a,int*b){int t=*a;*a=*b;*b=t;}
void ov12_0223A664(u8*,u8*);
void ov12_0223A664(u8*w,u8*input){int ids[4],values[4];int local,count,i,j;if(*(u32*)(w+0x240c)&16){w[0x23fc]=1;return;}for(i=0;i<4;i++,input+=4){ids[i]=i;values[i]=*(int*)(input+0x17c);}local=sub_0203769C();count=sub_02037454();for(i=0;i<count-1;i++){for(j=i+1;j<count;j++){if(values[i]<values[j]){swap(&ids[i],&ids[j]);swap(&values[i],&values[j]);}}}if(ids[0]==local){w[0x23fc]=1;return;}if(!(*(u32*)(w+0x2c)&128)){if(*(u32*)(w+0x2c)&8){int side=sub_020378AC(local);switch(sub_020378AC(ids[0])){case 0:case 2:if(side&1)*(u32*)(w+0x240c)|=32;break;case 1:case 3:if(!(side&1))*(u32*)(w+0x240c)|=32;break;}}else *(u32*)(w+0x240c)|=32;}}
