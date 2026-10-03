#pragma force_active on
#include "global.h"
typedef struct {u32 key,values[4];} Entry;typedef struct {u8 pad[12];int count;u8 pad2[0x140];Entry *entries;} W;u32 *ov96_021EB5EC(W *,u32,u32);
#pragma force_active on
#pragma opt_lifetimes off
#pragma opt_common_subs on
u32 *ov96_021EB5EC(W *w,u32 key,u32 slot){int i;u32 count;Entry *p;u32 base;if(slot>=4)GF_AssertFail();count=w->count;i=0;if((s32)count>0){base=(u32)w->entries;p=(Entry*)base;do{if(key==p->key)return (u32*)(base+i*sizeof(Entry)+4+slot*4);i++;p++;}while(i<(s32)count);}GF_AssertFail();return 0;}
