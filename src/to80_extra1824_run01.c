#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct{u8 pad[0xe8];u8 values[1];}Data;typedef struct{u8 pad[0xc0];Data*data;}Work;
u8 ov07_0221FA2C(Work*w,int index);
u8 ov07_0221FA2C(Work*w,int index){return w->data->values[index];}
