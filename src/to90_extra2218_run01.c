#pragma force_active on
#include "global.h"
#pragma force_active on
#define W(p,o) (*(int*)((u8*)(p)+(o)))
#define P(p,o) (*(void**)((u8*)(p)+(o)))
#define H(p,o) (*(u16*)((u8*)(p)+(o)))
#define B(p,o) (*(u8*)((u8*)(p)+(o)))
extern int ov00_021EC5B4(void),ov00_021EC724(void),ov00_021EC11C(int*,int*);extern void ov00_021EC60C(void),ov00_021EC210(void),ov00_021EC8D8(void),WirelessStatusIcon_Destroy(void),sub_0203957C(void),Sys_ClearSleepDisableFlag(int),ov70_02237F2C(void),ov75_02247878(void*),ov00_021EC0FC(int*),ov00_021EC9E0(void*);int ov75_02248800(void*);
int ov75_02248800(void*w){struct{int error,b,a;u8 info[60];}frame;ov00_021EC60C();if(ov00_021EC5B4()){switch(ov00_021EC724()){case 7:W(w,20)=ov00_021EC11C(&frame.a,&frame.b);W(w,24)=frame.a;W(w,28)=frame.b;ov00_021EC210();ov00_021EC8D8();WirelessStatusIcon_Destroy();sub_0203957C();Sys_ClearSleepDisableFlag(4);if(W(P(w,0),0x118)==1){ov70_02237F2C();W(P(w,0),0x118)=0;}ov75_02247878(w);W(w,8)=27;break;case 0:case 1:case 2:case 3:case 5:case 6:case 8:default:ov00_021EC0FC(&frame.error);ov75_02247878(w);W(w,8)=32;W(w,16)=-2;break;case 4:ov00_021EC9E0(frame.info);W(w,8)=14;break;}}return 0;}
