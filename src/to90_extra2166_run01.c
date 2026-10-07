#pragma force_active on
#include "global.h"
#pragma force_active on
typedef union{u32 word;u8 value[4];s8 signedValue[4];}Time;
void ov45_0222D594(const Time*,const Time*,Time*);
void ov45_0222D594(const Time*first,const Time*second,Time*out){Time a=*first,b=*second;int delta;u8 value;value=b.value[2];delta=a.signedValue[2]-(s8)value;if(delta<0){int borrow=(-delta)/60+1;a.value[2]=(u8)(a.value[2]+borrow*60);a.value[1]=(u8)(a.value[1]-borrow);}out->value[2]=(u8)(a.value[2]-value);value=b.value[1];delta=a.signedValue[1]-(s8)value;if(delta<0){int borrow=(-delta)/60+1;a.value[1]=(u8)(a.value[1]+borrow*60);a.value[0]=(u8)(a.value[0]-borrow);}out->value[1]=(u8)(a.value[1]-value);delta=(s8)a.value[0]-(s8)b.value[0];if(delta<0){int borrow=(-delta)/24+1;a.value[0]=(u8)(a.value[0]+borrow*24);}out->value[0]=(u8)(a.value[0]-b.value[0]);}
