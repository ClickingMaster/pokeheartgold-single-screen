#pragma force_active on
#include "global.h"
typedef struct{u16 active;u8 data[10];}Entry;typedef struct{int a,b,count;u8 pad[0x154-12];Entry*entries;}Work;extern void*ov96_021EB5B8(Entry*);extern void Sprite_SetAnimActiveFlag(void*,int);

void ov96_021EB63C(Work*w,int stopped);

void ov96_021EB63C(Work*w,int stopped){int i=0;int active,offset;if(w->count>0){offset=0;active=stopped==0?1:0;do{Entry*entry=(Entry*)((u8*)w->entries+offset);if(entry->active)Sprite_SetAnimActiveFlag(ov96_021EB5B8(entry),active);i++;offset+=12;}while(i<w->count);}}
