#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct {u32 first;u16 id;u8 rest[10];} Descriptor;
typedef struct {void *first,*second;u8 pad[12];Descriptor descriptor;u32 value;int flag;} Record;
typedef struct {u8 pad0[6];u16 count;int flag;u8 pad1[0x148];Record *records;} Scene;
typedef struct {u16 count,current;int base,state;Scene*scene;u32 unused,arg,mode,option,extra;u8 pad[48];Descriptor descriptors[1];} Request;
void ov96_021E8C70(Scene*,Record*,Descriptor*,u32,u8,u32);u32 ov96_021E9104(Record*);void sub_0200A740(void*);void ov96_021E8EE4(Scene*,u32,u32,u32,u8,u8);
int ov96_021E8FE0(Request*);
int ov96_021E8FE0(Request*p){u8 index;Record*record;switch(p->state){case 0:index=(u8)(p->base+p->current);ov96_021E8C70(p->scene,&p->scene->records[index],(Descriptor*)((u32)p+0x54+(u32)index*16),p->option,index,p->mode);p->scene->records[index].descriptor=p->descriptors[index];p->scene->records[index].value=ov96_021E9104(&p->scene->records[index]);p->scene->records[index].flag=p->scene->flag!=0&&p->descriptors[index].id!=0;if(p->scene->records[index].flag)p->scene->records[index].value+=8;if(p->mode==0){record=&p->scene->records[index];sub_0200A740(record->first);sub_0200A740(record->second);}p->current++;if(p->current>=p->count){p->current=0;p->state=1;}break;case 1:index=(u8)(p->base+p->current);ov96_021E8EE4(p->scene,p->arg,p->mode,p->extra,index,index);p->current++;p->scene->count++;if(p->current>=p->count)return 1;break;}return 0;}
