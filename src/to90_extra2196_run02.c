#pragma force_active on
#include "global.h"
#pragma force_active on
extern u8 ov18_021FA304[];
void ManagedSprite_GetPositionXY(void*,s16*,s16*);void ManagedSprite_SetPositionXY(void*,s16,s16);
u32 ov18_021F6230(void*,int,void*,int,int);u32 ov18_021F6244(void*,int,void*,int,int);
BOOL ov18_021F63DC(u8*,int,u32);
BOOL ov18_021F63DC(u8*c,int unused,u32 y){s16 x,oldY;u32 min,max,segments,width,position,index,lower,upper;
ManagedSprite_GetPositionXY(*(void**)(c+0x684),&x,&oldY);min=ov18_021F6230(c,5,ov18_021FA304,96,86);if(y<min)y=min;max=ov18_021F6244(c,5,ov18_021FA304,96,86);if(y>max)y=max;ManagedSprite_SetPositionXY(*(void**)(c+0x684),x,(s16)y);min=ov18_021F6230(c,5,ov18_021FA304,96,86);max=ov18_021F6244(c,5,ov18_021FA304,96,86);max-=min;segments=(*(u32*)(c+0x1900))-1;width=(max<<8)/segments;position=(y-min)<<8;index=0;lower=index;upper=index;for(;index<=segments;index++){if(position>=lower&&position<upper+width){if(*(s8*)(c+0x18ca)!=index){*(u8*)(c+0x18ca)=index;return 1;}break;}lower+=width;upper+=width;}return 0;}
