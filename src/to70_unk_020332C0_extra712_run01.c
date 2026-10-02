#pragma force_active on
#include "global.h"
typedef struct {u8 p[0x1338];u32 rng;} Data;
typedef struct {u32 p;Data *data;} Global;
extern Global _021D4124;
s16 sub_02033494(u16);
s16 sub_02033494(u16 mask){s16 result=0,i;u16 count=0;for(i=0;i<16;i++){if(mask&(1<<i)){result=i+1;count++;}}if(count>1){u16 selected;_021D4124.data->rng=_021D4124.data->rng*0x10dcd+0x3039;selected=(count*((u8)_021D4124.data->rng))>>8;for(i=0;i<16;i++){if(mask&1){if(selected==0)return i+1;selected--;}mask>>=1;}return 0;}return result;}
