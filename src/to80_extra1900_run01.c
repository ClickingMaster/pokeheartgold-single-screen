#pragma force_active on
#pragma force_active on
#include "global.h"
typedef struct {int threshold;u8 p[20];} Entry;
typedef struct {u32 count;Entry *entries;} List;
typedef struct {int timer,enabled;u8 p[0x28];List *list;u16 tick;volatile u8 index;u8 changed;} Work;
int ov91_0225CCF4(Work *w);
int ov91_0225CCF4(Work *w){int i;u32 count,walk;u8 previous;List *list;if(w->timer>0&&w->enabled==1)w->timer--;if(w->timer<=0)return 0;previous=w->index;list=w->list;count=list->count;i=previous;walk=previous;if(previous<count){Entry *e=&list->entries[previous];u16 tick=w->tick;do{if(e->threshold>=tick)break;i++;e++;walk=i;}while(i<count);}if(walk==count)i--;if(i!=previous){w->index=i;w->changed=1;}return 1;}
