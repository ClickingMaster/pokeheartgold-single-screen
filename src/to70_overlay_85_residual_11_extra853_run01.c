#pragma force_active on
#include "global.h"
typedef struct{int state,x,y,timer;u8 gap[8];int flag;u8 gap2[0xb4];u32*data;}Work;extern int sub_02037B38(int);extern void sub_020398D4(int,int),sub_02038C1C(int),sub_02037FF0(void);
int ov85_021E6694(Work*w);
int ov85_021E6694(Work*w){if(w->flag==0){if(sub_02037B38(202)){sub_020398D4(0,0);sub_02038C1C(1);sub_02037FF0();w->state=50;}}else{w->timer++;if(w->timer>=120){w->timer=0;if(w->data[15]==0){sub_020398D4(0,0);sub_02038C1C(1);sub_02037FF0();}w->state=50;}}return 0;}
