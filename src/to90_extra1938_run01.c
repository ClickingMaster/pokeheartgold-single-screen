#pragma force_active on
#include "global.h"
typedef struct{u32 x;u8 ids[4];volatile u8 count;}Data;typedef struct{u8 a[4],b[4];void*objs[4];}Out;extern void*ov90_022588CC(Data*,int);extern u8 sub_020348A8(int),sub_020348CC(int);
void ov90_0225A204(Out*out,Data*d);
int ov90_0225A258(Data*d,u8*ids);
#pragma force_active on
int ov90_0225A258(Data*d,u8*ids){int i,result;result=0;i=0;for(;i<d->count-1;i++){int b=ids[i+1],a=ids[i];if(a==b && a!=0 && b!=0){}else result=1;}return result;}
