#pragma force_active on
#include "global.h"
#include "heap.h"
typedef struct {enum HeapID heap;void *bg;u32 pad;void *system,*manager;} Context;
typedef struct {void *owner,*sprite,*extra;u8 windows[6][16];u8 pad[6];s16 x,y;u16 end;} Result;
typedef struct {s16 x,y;u8 width,height,pal,pad;u16 base;} Window;
extern const Window ov111_021E6C68[];
extern void *ov111_021E6330(void *,void *,s16,s16,s32,s32),*ov111_021E6380(void *,void *,s16,s16,u8);
extern void AddWindowParameterized(void *,void *,u8,u8,u8,u8,u8,u8,u16);
Result *ov111_021E63D0(Context *p,u16 x,u16 y,s32 mode,void *owner);
#pragma force_active on
Result *ov111_021E63D0(Context *p,u16 x,u16 y,s32 mode,void *owner){Result *r;u8 bg=mode==0?2:3;r=Heap_Alloc(p->heap,120);s32 i;MI_CpuFill8(r,0,120);r->owner=owner;r->x=x;r->y=y;r->sprite=ov111_021E6330(p->system,p->manager,r->x,r->y,0,1);r->extra=ov111_021E6380(p->system,p->manager,r->x-44,r->y-8,mode);for(i=0;i<6;i++)AddWindowParameterized(p->bg,r->windows[i],bg,(u8)((u8)(x>>3)+ov111_021E6C68[i].x),(u8)((u8)(y>>3)+ov111_021E6C68[i].y),ov111_021E6C68[i].width,ov111_021E6C68[i].height,ov111_021E6C68[i].pal,ov111_021E6C68[i].base);return r;}
