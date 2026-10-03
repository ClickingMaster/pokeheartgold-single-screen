#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct{u8 pad[0x34];void*items;}Work;extern void ListMenuItems_AddItem(void*,void*,int);
void ov49_0225AEF8(Work*w,void*text,int value);
void ov49_0225AEF8(Work*w,void*text,int value){ListMenuItems_AddItem(w->items,text,value);}
