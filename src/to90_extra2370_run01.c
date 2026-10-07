#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct{u8 x,y,width,height;u8 pad[32];}Layout;
extern int ov18_021E8B24(int),ov18_021E8B0C(int),ov18_021E8B5C(int);
void ov18_021F41C4(u8*,int,s16*,s16*,u8*);
void ov18_021F41C4(u8*w,int id,s16*outX,s16*outY,u8*kind){int x,y,width,height;if(ov18_021E8B24(id)==1){x=32;y=4;width=1;height=3;}else if(ov18_021E8B0C(id)==18){x=(*(Layout**)(w+0x190c))[id].x;y=(*(Layout**)(w+0x190c))[id].y;width=1;height=2;}else if(ov18_021E8B5C(id)==1){x=35;y=8;width=2;height=1;}else{x=(*(Layout**)(w+0x190c))[id].x;y=(*(Layout**)(w+0x190c))[id].y;width=(*(Layout**)(w+0x190c))[id].width;height=(*(Layout**)(w+0x190c))[id].height;}if(width==1){if(height==1)*kind=0;else if(height==2)*kind=6;else if(height==3)*kind=7;else if(height==4)*kind=8;else if(height==5)*kind=9;}else if(width==2){if(height==1)*kind=1;else *kind=10;}else if(width==3){if(height==1)*kind=2;else *kind=11;}else if(width==4)*kind=3;else if(width==5)*kind=4;else if(width==6)*kind=5;else *kind=0;*outX=(s16)((s32)((u32)(x-22*(*(s8*)(w+0x18c8)))<<3)+(s32)((u32)width<<3)/2+64);*outY=(s16)((s32)((u32)y<<3)+(s32)((u32)height<<3)/2+40);}
