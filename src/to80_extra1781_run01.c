#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct{u8 pad[0x11];u8 kind;}Work;extern int(*ov45_02254A60[])(void*,Work*);
int ov45_0222C828(void*unused,Work*w,void*unused2,void*arg);
int ov45_0222C828(void*unused,Work*w,void*unused2,void*arg){return ov45_02254A60[w->kind](arg,w);}
