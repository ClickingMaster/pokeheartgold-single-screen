#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct Work {
u32 field0;
u8 pad4[6600];
u16 vals[2];
u8 pad19d0[228];
u8 f1ab4;
u8 pad1ab5[1155];
u32 state1;
u32 pad1f3c;u32 counter1;u8 pad1f44[16];
u32 state;
u32 counter;
u8 pad1f5c[24];
u8 f1f74;
u8 pad1f75[559];
void *f21a4;
u8 pad21a8[25732];
u8 f862c;
u8 pad862d[344];
u8 f8785;
u8 pad8786[6];u8 f878c;u8 pad878d[87];
u8 f87e4;
} Work;
extern void ov91_0225E6B0(Work*),ov91_0225EE9C(Work*),ov91_0225DBE4(void*),ov90_02258CB0(void*),ov91_02261B10(void*),ov91_02261890(void*),ov91_0225E40C(Work*),ov91_0225E648(Work*),ov91_0225DD50(void*,u16),ov91_022601C8(void*,void*),ov91_022601F4(void*,void*),ov91_022601AC(void*,void*,u32),ov91_0225F7A8(Work*,void*),ov91_02261928(void*);
extern int ov91_0225DBF4(void*),ov91_0225E6D0(Work*),ov91_0225DC24(void*);
extern void StartBrightnessTransition(int,int,int,int,int);extern int IsBrightnessTransitionActive(int);
int ov91_0225D078(Work*,int);
extern void ov91_0225DBC0(void*,void*),ov91_02260218(void*,void*),ov91_0225F23C(void*),sub_02037AC0(u32),ov91_0226031C(void*,int),ov91_0225DB7C(void*,void*),ov91_0225DB18(void*,void*),ov91_0225DB5C(void*,void*),ov91_0225DD20(void*,int),PlaySE(int);
extern int ov91_0225F25C(void*,Work*),sub_02037B38(u32),ov91_0225DB44(void*,void*),ov91_0225DBAC(void*,void*);
int ov91_0225CEB4(Work*,int);
int ov91_0225CEB4(Work*w,int ready){int result=1;if(w->state1>=4){int angle=w->f8785?-120:120;ov91_0225DD50(w->vals,(u16)(w->vals[1]+angle));}ov91_0225DBC0(&w->f1ab4,w->vals);ov91_022601F4(&w->f1f74,w->vals);ov91_02260218(&w->f1f74,w->vals);switch(w->state1){case 0:ov91_0225F23C(&w->f878c);w->state1++;break;case 1:if(ov91_0225F25C(&w->f878c,w)==1){w->state1++;sub_02037AC0(2);}break;case 2:if(sub_02037B38(2)){w->state1++;w->counter1=65;}break;case 3:w->counter1--;if(w->counter1==50)PlaySE(0x619);if(w->counter1==20)PlaySE(0x63d);ov91_0226031C(&w->f1f74,4096);if(!w->counter1){ov91_0225DB7C(&w->state1,&w->f1ab4);w->state1=4;}break;case 4:if(ready==1)w->state1=5;break;case 5:ov91_0225DB18(&w->state1,&w->f1ab4);PlaySE(0x5dd);w->state1=6;break;case 6:if(!ov91_0225DB44(&w->state1,&w->f1ab4))w->state1=7;break;case 7:if(!ov91_0225DBAC(&w->state1,&w->f1ab4))w->state1=8;break;case 8:ov91_0225DB5C(&w->state1,&w->f1ab4);ov91_0225DD20(w->vals,1);result=0;break;}ov91_022601AC(&w->f1f74,w->vals,w->field0);ov91_0225F7A8(w,&w->f1ab4);return result;}
int ov91_0225D078(Work*w,int ready){int result=1;switch(w->state){case 0:ov91_0225E6B0(w);ov91_0225EE9C(w);StartBrightnessTransition(24,-14,0,63,2);ov91_0225DBE4(&w->f1ab4);ov90_02258CB0(w->f21a4);ov91_02261B10(&w->f87e4);ov91_02261890(&w->f862c);w->state++;break;case 1:{int done=1;if(!ov91_0225DBF4(&w->f1ab4))done=0;ov91_0225E40C(w);ov91_0225E648(w);if(ov91_0225E6D0(w)==1)done=0;if(!ov91_0225DC24(&w->f1ab4))done=0;if(!IsBrightnessTransitionActive(2))done=0;if(done==1 && ready==1){w->counter=0;w->state++;}break;}case 2:w->counter++;if(w->counter>=64)w->state++;break;case 3:result=0;break;}int angle=w->f8785?-120:120;ov91_0225DD50(w->vals,(u16)(w->vals[1]+angle));ov91_022601C8(&w->f1f74,w->vals);ov91_022601F4(&w->f1f74,w->vals);ov91_022601AC(&w->f1f74,w->vals,w->field0);ov91_0225F7A8(w,&w->f1ab4);ov91_02261928(&w->f862c);return result;}
