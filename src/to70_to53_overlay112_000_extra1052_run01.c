#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct{u32 p[4];u32 count,p2,ready;} State;
extern State _021FF9E0;
u16 ov112_021E5900(u8*,u32);
void ov112_021E5938(void);
u16 ov112_021E5900(u8*bytes,u32 len){u32 sum=2,index;for(index=0;index<len;index++){u8 b=*bytes++;if(index&1)sum+=b;else sum+=b<<8;}sum=(sum>>16)+(u16)sum;sum+=sum>>16;return sum;}
void ov112_021E5938(void){_021FF9E0.ready=1;_021FF9E0.count++;}
