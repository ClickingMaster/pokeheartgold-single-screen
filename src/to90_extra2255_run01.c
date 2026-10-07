#pragma force_active on
#include "global.h"
typedef struct {u16 id,valid;} Entry;
typedef struct {Entry *entries;u16 pad;s16 page;u8 type,count;u16 end;} Group;
typedef struct {u32 a;Group groups[8];u8 selected;} Lists;
typedef struct {u8 pad[0x234];Lists *list;u8 gap[0xc0];void *normal,*special;u8 rest[0x50];void *dest[6];u8 tail[0x33c];u16 ids[6];} Context;
extern const u8 ov15_022008B0[];
extern void ov15_021F9D9C(void *,void *,u16,s32),ov15_021F9D8C(void *,void *,u16,s32);
void ov15_021F9F08(Context *p);
#pragma force_active on
void ov15_021F9F08(Context *p){Group *g=&p->list->groups[p->list->selected];u32 i;s32 page;
 if(g->type==3){for(i=0;i<ov15_022008B0[g->type];i++){if(g->entries[i].id==0||g->entries[i].valid==0)break;ov15_021F9D9C(p->special,p->dest[i],g->entries[i].id,6);p->ids[i]=g->entries[i].id;}g->count=i;}else{for(i=0;i<ov15_022008B0[g->type];i++){if(g->entries[i].id==0||g->entries[i].valid==0)break;ov15_021F9D8C(p->normal,p->dest[i],g->entries[i].id,6);p->ids[i]=g->entries[i].id;}g->count=i;}
 if(g->count==0)page=0;else page=((g->count-1)/6)*6;if(g->page>page)g->page=page;}
