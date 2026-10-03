#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct{u8 pad[0x13];u8 flag:1,rest:7;}Work;
int ov81_02240F28(Work*w);
int ov81_02240F28(Work*w){if(w->flag)return 1;return 0;}
