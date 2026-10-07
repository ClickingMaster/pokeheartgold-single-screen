#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct{u16 value;u8 lane,field;}Event;
extern void ov49_02267EF8(void*,int);extern int ov49_02267F40(void*);
int ov49_02267E18(void*,u32);
int ov49_02267E18(void*s,u32 value){int i=0,result=0;Event e;u32 lane,field;u8*cur;for(i=0;i<*(u16*)((u8*)s+0x3b4);i++){e=((Event*)*(void**)((u8*)s+0x3b0))[i];if(e.value==value){lane=e.lane;if(lane>=4)GF_AssertFail();field=e.field;if(field>4)GF_AssertFail();ov49_02267EF8((u8*)s+lane*236,field);result=1;}}lane=1;for(i=0,cur=s;i<4;i++,cur+=236){if(ov49_02267F40(cur)==0)lane=0;}if(*(u16*)((u8*)s+0x3b6)<=value&&lane==1)result=2;return result;}
