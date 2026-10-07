#pragma force_active on
#include "global.h"
extern void ov112_021F2204(void *,int,int),ov112_021F22B0(void *,u8,int),ov112_021F1F54(void *,int,int,int),ov112_021F1F3C(void *,int),ov112_021F22D0(void *,int,int,int),ov112_021F1624(void *,int,int);
void ov112_021F2A78(void *,u32);
#pragma force_active on
void ov112_021F2A78(void *argument,u32 count){
 u8 *work=argument;u16 value=*(u16 *)(*(u8 **)(work+0xc0+work[0x13d]*4)+0x78);int message;
 if(value>=300){ov112_021F2204(work,0,1);ov112_021F22B0(*(void **)(work+0x84),(u8)*(u32 *)(work+12),3);if(value>=4000)message=79;else if(value>=2000)message=80;else if(value>=1000)message=81;else message=82;}
 else{ov112_021F2204(work,0,1);ov112_021F1F54(work+0x98,128,72,0);ov112_021F1F3C(work+0x98,0);ov112_021F22D0(work,0,8,10);if((count>=20&&count<=24)||count<=7)message=83;else if(count>=12&&count<=16)message=84;else message=85;}
 ov112_021F1624(work,message,0);
}
