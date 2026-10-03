#pragma force_active on
#include "global.h"
typedef struct{u16 mask;u8 id,pad;}Map;extern Map ov13_02245A28[];typedef struct{u8 p[4];u16 mac[3];u16 a;u16 name[16];u16 flags,present,enabled,v,pad,number;}Source;typedef struct{u32 a;u16 name[16];u32 b;u8 gap[8];u16 mac[3];u8 gap2[2];int count;u8 ids[16];int number,mode;}Target;
void ov13_02222B34(Source*s,Target*t);
#pragma force_active on
void ov13_02222B34(Source*s,Target*t){int i,count;Map*p; t->a=s->a;MIi_CpuCopy16(s->name,t->name,32);t->b=s->number;MIi_CpuCopy16(s->mac,t->mac,6);count=0;p=ov13_02245A28;i=count;for(;i<12;i++,p++){if(s->enabled&p->mask){t->ids[count]=p->id;if(s->present&p->mask)t->ids[count]|=128;count++;}}t->count=count;t->number=s->v;if((s->flags&3)==1)t->mode=1;else if((s->flags&3)==2)t->mode=2;else t->mode=0;}
