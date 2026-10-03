#pragma force_active on
#include "global.h"
typedef struct{u8 pad[0x10];u16 state;u8 gap[0x2f0-0x12];void*bg;}Inner;typedef struct{u8 pad[0x34];Inner*inner;}Work;extern int ov14_021E9434(Work*),sub_02019978(void*,int);

int ov14_021E95C8(Work*w);

int ov14_021E95C8(Work*w){Inner*i=w->inner;switch(i->state){case 0:if(ov14_021E9434(w)==0)i->state++;break;case 1:if(sub_02019978(i->bg,12)==0){i->state=0;return 0;}break;}return 1;}
