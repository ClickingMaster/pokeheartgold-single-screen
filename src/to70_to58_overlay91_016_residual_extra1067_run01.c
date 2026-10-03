#pragma force_active on
#include "global.h"
typedef struct{u8 state;u8 rest[0x43];}Entry;typedef struct{u8 pad[0x4c];Entry entries[96];}Work;extern void ov91_0225E23C(void*,void*),ov91_0225E400(void*);

int ov91_0225D32C(Work*w,void*dst);

int ov91_0225D32C(Work*w,void*dst){int i;for(i=0;i<96;i++){if(w->entries[i].state==4){ov91_0225E23C((void*)((u32)((u8*)w+0x50)+i*0x44),dst);ov91_0225E400((void*)((u32)((u8*)w+0x4c)+i*0x44));return 1;}}return 0;}
