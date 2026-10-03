#pragma force_active on
// clang-format off
#include "global.h"
#include <nitro/code32.h>
// clang-format on
#pragma force_active on
typedef struct Event{int type;u8 pad[12];void *callback,*callback0,*arg,*payload,*user;}Event;
typedef struct Data {int n;int count;void **list;void *other;void **extra;}Data;
extern void __msl_assertion_failed(char*,char*,char*,int);
extern char ov45_02256654[],ov45_022565D0[],ov45_02255F84[],ov45_0225665C[],ov45_02256690[];
extern void ov00_021EC3D8(void*);
extern void ov45_02244D30(void*,int,void*,void*,void*,void*,int,int);
extern void ov45_02249958(void*,Event*);
void ov45_02249A0C(void*,Event*,void*);
void ov45_02249A0C(void*c,Event*e,void*response){int i,valid;Data*d;
if(!e)__msl_assertion_failed(ov45_02256654,ov45_022565D0,ov45_02255F84,0x1c1);
valid=0;if(e->type>=0&&e->type<17)valid=1;
if(!valid)__msl_assertion_failed(ov45_0225665C,ov45_022565D0,ov45_02255F84,0x1c2);
if(e->type==0){
d=e->payload;
if(e->callback0)ov45_02244D30(c,14,e->callback0,response,e->arg,e->user,0,20);
for(i=0;i<d->count;i++){ov00_021EC3D8(d->list[i]);ov00_021EC3D8(d->extra[i]);}ov00_021EC3D8(d->list);ov00_021EC3D8(d->extra);ov00_021EC3D8(d->other);
}
else if(e->type==1){
if(e->callback)ov45_02244D30(c,15,e->callback,response,e->arg,e->user,0,12);
}
else if(e->type==2){
i=((int*)response)[1];
if(e->callback)ov45_02244D30(c,16,e->callback,response,e->arg,e->user,i,12);
}
else if(e->type==3){
d=e->payload;
if(e->callback)ov45_02244D30(c,19,e->callback,response,e->arg,e->user,0,20);
for(i=0;i<d->count;i++)ov00_021EC3D8(d->list[i]);ov00_021EC3D8(d->list);ov00_021EC3D8(d->other);
}
else if(e->type==4){
d=e->payload;
if(e->callback)ov45_02244D30(c,20,e->callback,response,e->arg,e->user,0,28);
for(i=0;i<(int)d->other;i++)ov00_021EC3D8(d->extra[i]);ov00_021EC3D8(d->extra);ov00_021EC3D8((void*)d->count);ov00_021EC3D8(d->list);ov00_021EC3D8((void*)d->n);
}
else if(e->type==10){
if(e->callback)ov45_02244D30(c,21,e->callback,response,e->arg,e->user,0,16);
}
else if(e->type==11){
if(e->callback)ov45_02244D30(c,22,e->callback,response,e->arg,e->user,0,16);
}
else if(e->type==5){
if(e->callback)ov45_02244D30(c,17,e->callback,response,e->arg,e->user,0,12);
}
else if(e->type==6){
if(e->callback)ov45_02244D30(c,23,e->callback,response,e->arg,e->user,0,16);
}
else if(e->type==7){
ov00_021EC3D8(*(void**)e->payload);
}
else if(e->type==8){
d=e->payload;
if(e->callback)ov45_02244D30(c,24,e->callback,response,e->arg,e->user,0,16);
for(i=0;i<d->n;i++)ov00_021EC3D8(((void**)d->count)[i]);
}
else if(e->type==9){
if(e->callback)ov45_02244D30(c,26,e->callback,response,e->arg,e->user,0,12);
}
else if(e->type==12){
d=e->payload;
if(e->callback)ov45_02244D30(c,29,e->callback,response,e->arg,e->user,0,20);
ov00_021EC3D8(d->list);
for(i=0;i<d->n;i++)ov00_021EC3D8(((void**)d->count)[i]);
ov00_021EC3D8((void*)d->count);
}
else if(e->type==13){
d=e->payload;
if(e->callback)ov45_02244D30(c,30,e->callback,response,e->arg,e->user,0,24);
for(i=0;i<d->n;i++)ov00_021EC3D8(((void**)d->count)[i]);
ov00_021EC3D8((void*)d->count);
}
else if(e->type==14){
d=e->payload;
if(e->callback)ov45_02244D30(c,30,e->callback,response,e->arg,e->user,0,24);
for(i=0;i<d->n;i++)ov00_021EC3D8(((void**)d->count)[i]);
ov00_021EC3D8((void*)d->count);
}
else if(e->type==15){
d=e->payload;
if(e->callback)ov45_02244D30(c,27,e->callback,response,e->arg,e->user,0,16);
for(i=0;i<d->count;i++)ov00_021EC3D8(d->list[i]);ov00_021EC3D8(d->list);ov00_021EC3D8(d->other);
}
else if(e->type==16){
if(e->callback)ov45_02244D30(c,31,e->callback,response,e->arg,e->user,0,8);
}
else __msl_assertion_failed(ov45_02256690,ov45_022565D0,ov45_02255F84,0x25c);
ov45_02249958(c,e);
}
