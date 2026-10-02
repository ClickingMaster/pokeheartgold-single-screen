#pragma force_active on
#include "global.h"
#pragma force_active on
int ov45_0222AD90(void *);
int ov45_0222ADA0(void *);
void ov49_0225E3B8(void *,int,int);
void ov49_02269178(u8 *,int);
void ov49_02269178(u8 *w,int arg){
 int a=ov45_0222AD90(*(void **)(w+4));
 int b=ov45_0222ADA0(*(void **)(w+4));
 fx32 ratio=FX_Div(*(fx32*)(w+0xc0),8192);
 float bf,af;
 fx32 scaled,res;
 if(b>0)bf=0.5f+(float)(b<<12);else bf=(float)(b<<12)-0.5f;
 if(a>0)af=0.5f+(float)(a<<12);else af=(float)(a<<12)-0.5f;
 scaled=(fx32)(((s64)(int)af*ratio+2048)>>12);
 res=FX_Div(scaled,(int)bf);
 res=(fx32)((((s64)res<<13)+2048)>>12);
 res+=*(int*)(w+0xbc);
 ov49_0225E3B8(*(void **)(w+8),arg,res);
}
