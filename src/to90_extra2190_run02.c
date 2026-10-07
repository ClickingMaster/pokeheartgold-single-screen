#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct{u8 pad[0x188c];int value;}Work;void ov18_021F11C0(Work*,int,int),ov18_021F118C(Work*,int,int),ov18_021F3494(Work*);
void ov18_021F3494(Work*w){if(w->value==14)ov18_021F11C0(w,28,0);else{ov18_021F11C0(w,28,1);ov18_021F118C(w,28,w->value);}}
