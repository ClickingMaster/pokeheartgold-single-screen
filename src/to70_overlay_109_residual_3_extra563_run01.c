#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct {u8 pad[0x19];u8 page,pages,col,row;u8 p[0xc5-0x1d];u8 count;u8 q[6];u8 entries[1][8];} State;
extern u8 ov109_021E7890[];
void ov109_021E5DEC(State *,u8);
void ov109_021E5DEC(State *s,u8 selected){u8 i;int entry;s->count--;if(selected<s->count){for(i=selected;i<s->count;i++)MI_CpuCopy8(s->entries[i+1],s->entries[i],8);}MI_CpuFill8(s->entries[s->count],0,8);s->pages=s->count/12;if(s->count%12)s->pages++;if(s->count==0)s->pages=1;if(selected>=s->count){if(s->count==0){s->col=3;s->row=3;s->page=0;return;}if(selected!=0)selected--;s->page=selected/12;entry=ov109_021E7890[selected%12];s->col=entry%4;s->row=(u32)entry>>2;}}
