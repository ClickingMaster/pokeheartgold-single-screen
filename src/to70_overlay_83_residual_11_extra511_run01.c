#pragma force_active on
#include "global.h"
typedef struct {u32 a,b;} Entry;
typedef struct {u8 pad[0x13];u8 kind;u8 pad2[0x4dc-0x14];Entry *entries;u8 pad3[0x518-0x4e0];u8 manager[0x784-0x518];void *sprites[6];u8 pad4[0x861-0x79c];u8 count;s16 page;} Work;
extern int ov83_02240F48(Work *,u16,int);extern void ov83_02247264(void *,u32,int);extern void ov83_022472A0(void *,u32,int);extern void ov83_02242894(u32,u16 *,u16 *);extern void ov83_02247568(void *,u16,u16);extern void ov83_0224755C(void *,int);
void ov83_02240748(Work *);
void ov83_02240748(Work *w){u32 offset=0,base;u32 i=0;u32 index=(u16)(w->page*6);Work *cursor=w;base=index*8;for(;i<6;i++,index++,offset+=8,cursor=(Work *)((u8 *)cursor+4)){if(index<w->count){int value=ov83_02240F48(w,(u16)*(u32 *)(offset+(base+(u32)w->entries)+4),w->kind);struct {u16 y,x;} coords;ov83_02247264(w->manager,i+4,value);ov83_022472A0(w->manager,i+4,value);ov83_02242894(i,&coords.x,&coords.y);ov83_02247568(cursor->sprites[0],coords.x,coords.y);ov83_0224755C(cursor->sprites[0],1);}else ov83_0224755C(cursor->sprites[0],0);}}
