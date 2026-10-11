#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct {void *pocket;u16 a,b;u8 index,count;u8 q[2];} Slot;typedef struct {u32 header;void *pocket;u16 a,b;u8 index,count;} View;struct Bag {u32 header;Slot slots[8];};typedef struct {u8 p[0x234];struct Bag *bag;u8 p2[0x3c];void *sprites[8];} Work;
extern void GF_AssertFail(void);extern void ManagedSprite_SetDrawFlag(void *,int);void BagApp_ShowPocketTabSprites(Work *,int);
void BagApp_ShowPocketTabSprites(Work *w,int show){u8 flags[8];u8 *cursor;int i,j;int offset;if(show!=1&&show!=0)GF_AssertFail();MI_CpuFill8(flags,0,8);for(i=0,offset=0;i<8;i++,offset+=12){View *s=(View *)((u8 *)w->bag+offset);if(s->index>=8)GF_AssertFail();s=(View *)((u8 *)w->bag+offset);if(s->pocket!=0)flags[s->index]=1;}j=0;cursor=flags;for(;j<8;cursor++,w=(Work *)((u8 *)w+4),j++){if(*cursor)ManagedSprite_SetDrawFlag(w->sprites[0],show);else ManagedSprite_SetDrawFlag(w->sprites[0],0);}}
