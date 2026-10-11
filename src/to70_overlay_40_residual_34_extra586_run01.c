#pragma force_active on
#include "global.h"
extern void*Save_SysInfo_Get(void*);extern int Save_SysInfo_GetDwcProfileId(void*);extern void GlobalTerminalNet_Init(void*,void*);extern u8 ov40_02236158[],ov40_0223615C[],ov40_02238EB8[],ov40_02238EBC[],ov40_0223A324[],ov40_0223A360[],ov40_02242DF0[],ov40_0222FC4C[],ov40_02242E14[],ov40_02242E48[];typedef struct{int id,heap;void*save,*user;void*callbacks[10];int zero;void*last;}Config;
void ov40_0223D544(u8*w);
void ov40_0223D544(u8*w){Config c;c.id=Save_SysInfo_GetDwcProfileId(Save_SysInfo_Get(*(void**)(w+0x830)));c.heap=109;c.save=*(void**)(w+0x830);c.user=w;c.callbacks[0]=ov40_02236158;c.callbacks[1]=ov40_0223615C;c.callbacks[2]=ov40_02238EB8;c.callbacks[3]=ov40_02238EBC;c.callbacks[4]=ov40_0223A324;c.callbacks[5]=ov40_0223A360;c.callbacks[6]=ov40_02242DF0;c.callbacks[7]=ov40_0222FC4C;c.callbacks[8]=ov40_02242E14;c.callbacks[9]=ov40_02242E48;c.zero=0;c.last=w;GlobalTerminalNet_Init(w+0x68,&c);}
