#pragma force_active on
#include "global.h"
#pragma force_active on
extern u8 ov13_0224DF30[],ov13_02245A58[],ov13_02245A6C[];
int ov13_02223CA0(void*,int,int),ov13_02223E6C(void),SOC_Startup(void*);u32 ov13_02226CBC(void);void ov13_02223ED4(void*);int ov13_02223FDC(void);
#define I(w,o) (*(int*)((w)+(o)))
int ov13_02223FDC(void){OSAlarm alarm;int active=1,result=-2,event;u8*p=(u8*)((u32)*(u8**)(ov13_0224DF30+0x5c)+(u32)I(ov13_0224DF30,0x18)*192);if(!p)return 0;if(!ov13_02223CA0(p,0,0x30000))return -2;OS_CreateAlarm(&alarm);OS_SetAlarm(&alarm,0x3fec42,ov13_02223ED4,(void*)18);do{if(ov13_02226CBC()>=*(u32*)(ov13_02245A58+12)){result=-3;break;}if(I(ov13_0224DF30,16)){result=-8;break;}OS_Sleep(10);event=ov13_02223E6C();while(event){switch(event){case 4:case 5:case 18:case 19:break;case 12:active=0;result=1;break;case 13:if(I(ov13_0224DF30,16)){active=0;result=-8;}else if(!ov13_02223CA0(p,0,0x30000))return result;break;default:active=0;break;}event=ov13_02223E6C();}}while(active);OS_CancelAlarm(&alarm);while(ov13_02223E6C()){}if(result>0){I(ov13_0224DF30,32)=1;if(SOC_Startup(ov13_02245A6C)<0)result=-2;else I(ov13_0224DF30,36)=1;}return result;}
