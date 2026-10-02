#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct Owner{u8 pad0[0x118];int connected;}Owner;typedef struct Work{Owner*owner;u32 p4;int state;u8 padc[0x28];void*text;u8 pad38[12];void*busy;u8 pad48[0x48];int timer,phase;}Work;
extern void ov75_022494CC(Work*,void*,int,int,int),sub_0203957C(void),ov00_021EC8D8(void),sub_0203A914(void),Sys_ClearSleepDisableFlag(int),ov70_02237F2C(void),ov75_02246BD8(Owner*);extern int ov75_02249534(void*);int ov75_0224937C(Work*);
int ov75_0224937C(Work*w){switch(w->phase){case 0:ov75_022494CC(w,w->text,15,1,0xf0f);w->phase++;break;case 1:if(!ov75_02249534(w->busy)){sub_0203957C();ov00_021EC8D8();sub_0203A914();Sys_ClearSleepDisableFlag(4);if(w->owner->connected==1){ov70_02237F2C();w->owner->connected=0;}w->phase++;}break;case 2:ov75_022494CC(w,w->text,16,1,0xf0f);w->phase++;break;case 3:if(!ov75_02249534(w->busy))w->phase++;break;default:w->timer++;if(w->timer>30){ov75_02246BD8(w->owner);w->state=29;}break;}return 0;}
