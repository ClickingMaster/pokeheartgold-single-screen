#pragma force_active on
#include "global.h"
typedef struct{u8 x[4],y[4];u8 a:3,b:3,c:2;}Out;typedef struct{u16 x,y;u32 z;}Item;typedef struct{u16 count;Item data[1];}Input;
void ov37_021E78E0(Out*out,Input*in,int a,int b);
void ov37_021E78E0(Out*out,Input*in,int a,int b){int i;if(in->count){out->x[0]=*(u16*)((u8*)in+2);out->y[0]=*(u16*)((u8*)in+4);u8*last=(u8*)in+(in->count-1)*8;out->x[1]=*(u16*)(last+2);out->y[1]=*(u16*)(last+4);for(i=0;i<2;i++){if(out->x[i]+out->y[i]==0)in->count=0;}}if(in->count>=2)out->b=2;else out->b=(u8)in->count;out->a=(u8)a;out->c=(u8)b;}
