#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct W{u8 pad[4];void*arg;u8 gap[0x5c8-8];int state,timer,flag;}W;typedef struct G{u8 pad[16];int state;}G;extern G ov74_0223D0A8;extern void sub_02038D28(void*),sub_02038D64(void);extern int sub_02038D80(void);
void ov74_0222CE10(W*),ov74_0222CE6C(int);
void ov74_0222CE10(W*w){switch(w->state){case 28:sub_02038D28(w->arg);w->timer=120;w->state=30;break;case 29:break;case 30:if(sub_02038D80()&1)w->flag=1;w->timer--;if(w->timer==0){sub_02038D64();w->state=29;}break;}}
void ov74_0222CE6C(int choice){switch(choice){case 0:ov74_0223D0A8.state=40;break;case 1:ov74_0223D0A8.state=41;break;case 2:ov74_0223D0A8.state=42;break;case 3:if(ov74_0223D0A8.state==42)ov74_0223D0A8.state=44;else ov74_0223D0A8.state=47;break;case 4:ov74_0223D0A8.state=48;break;}}
