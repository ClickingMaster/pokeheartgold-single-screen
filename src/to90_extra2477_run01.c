#pragma force_active on
#pragma force_active on
#include "global.h"
typedef struct {u8 data[132];} Item;typedef struct {u8 idx,pad[3];Item*item;} Entry;typedef struct {u8 p[0xc4];u8 count,copy;u16 pad;Item*items;Entry entries[36];} Work;

void ov109_021E5D08(Work*);
void ov109_021E5D08(Work*w){u8 i;w->copy=w->count;MI_CpuFill8(w->entries,0,sizeof(w->entries));for(i=0;i<w->copy;++i){Entry*entry=&w->entries[i];entry->idx=i;entry->item=&w->items[i];}}
