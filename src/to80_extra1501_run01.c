#pragma force_active on
#include "global.h"
struct Vec{int x,y,z;};
struct Chunk{u8 p[0x854];void *model;u8 q[8];int index,ready;void *props;};
struct Slot{struct Chunk *chunks[2];u8 p[8];u8 job[16];u8 order[2],next,pad;int done,last,other;};
struct Movement{struct Vec pos;struct Vec *target;int *high,*low;int moving;u8 direction,pad[3];int enabled;};
struct Manager;
struct Vtable{void*unused;void(*load)(int,void*,void*,int,int,struct Manager*,struct Slot*);};
struct Manager{u32 first;struct Slot slots[2];u8 q[8];int pending;u8 dir;u8 gap[15];int flags[4];u8 gaps[16];u8 count,unused,next,pad;u8 raw[16];int state;void *area;u8 unknown[4];void *a;int width,height;u32 cc;struct Movement movement;u8 z[8];struct Vtable *table;u8 ext[8];void(*callback)(void*,int,void*);void*cbarg;};
extern void ov01_021F61DC(int,int,int*,int*),ov01_021F5F64(int,int,struct Manager*),ov01_021F613C(u8,struct Manager*),ov01_021F477C(struct Slot*),ov01_021EA3B0(void*),ov01_021F5D20(struct Manager*),ov01_021F5D38(u8,struct Manager*),ov01_021F5CB4(struct Manager*);
extern int CommState_SetError(int),ov01_021F5024(void*),ov01_021FBA00(void*),ov01_021F5D10(struct Manager*);

void MapLoadManager_Tick(struct Manager*);
static inline u32 *firstEntry(u8*dict,u8 index){if(dict&&index<(u32)dict[1])return (u32*)(dict+*(u16*)(dict+6)+4);return 0;}
static inline void *firstModel(void *arg){u8*p=arg;if(p){u32*off=firstEntry(p+8,0);if(off)return p+*off;}return 0;}
static inline void copyVec(struct Vec*src,struct Vec*dst){*dst=*src;}
void MapLoadManager_Tick(struct Manager*m){struct Slot*s=&m->slots[m->next];struct Movement*v=&m->movement;int x,z;struct Chunk*c;void *model;if(v->enabled==1&&v->target){if(v->moving){if(*v->low>*v->high)GF_AssertFail();if(*v->high-*v->low>=0x10000){ov01_021F61DC(v->target->x,v->target->z,&x,&z);ov01_021F5F64(x,z,m);{struct Vec *dst=&v->pos;struct Vec *src=v->target;*dst=*src;}if(v->pos.z%0x8000&&CommState_SetError(1))return;if(v->pos.z%0x8000)GF_AssertFail();v->moving=0;v->high=0;v->low=0;}}else{if(v->pos.x!=v->target->x&&v->pos.z!=v->target->z){GF_AssertFail();}else if(v->pos.x!=v->target->x||v->pos.z!=v->target->z){v->moving=1;if(v->pos.x!=v->target->x){if(v->pos.x>v->target->x){v->high=&v->pos.x;v->low=&v->target->x;v->direction=3;}else{v->high=&v->target->x;v->low=&v->pos.x;v->direction=1;}if(v->pos.x%0x8000&&CommState_SetError(1))return;if(v->pos.x%0x8000)GF_AssertFail();}else{if(m->movement.pos.z>m->movement.target->z){v->high=&v->pos.z;v->low=&v->target->z;v->direction=4;}else{v->high=&v->target->z;v->low=&v->pos.z;v->direction=2;}if(v->pos.z%0x8000&&CommState_SetError(1))return;if(v->pos.z%0x8000)GF_AssertFail();}ov01_021F613C(v->direction,m);}}}
switch(m->state){case 0:if(m->flags[s->order[s->next]]!=1)m->table->load(s->next,m->area,m->a,m->width,m->height,m,s);s->next++;m->state=1;break;case 1:if(m->flags[s->order[s->next-1]]==1){ov01_021F477C(s);m->flags[s->order[s->next-1]]=0;}if(s->next<=2&&ov01_021F5024(s->job)==1){if(s->next>=2)s->done=1;else m->state=0;c=s->chunks[s->next-1];if(c->ready==1){model=firstModel(NNS_G3dGetMdlSet(c->model));if(ov01_021FBA00(m->area)==1)ov01_021EA3B0(model);}if(m->callback){c=s->chunks[s->next-1];if(c->index>=0&&c->index<m->width*m->height)m->callback(m->cbarg,c->index,c->props);}}break;case 2:break;case 3:if(ov01_021F5024(s->job)==1)m->count=0;break;}
if(m->count==0){m->next=0;m->unused=0;m->state=2;return;}if(ov01_021F5D10(m)==1){m->slots[m->next].last=0;ov01_021F5D20(m);m->count--;m->next=(m->next+1)%2;if(m->count)m->state=0;else{m->next=0;m->unused=0;m->state=2;}if(m->pending==1){m->pending=0;ov01_021F5D38(m->dir,m);ov01_021F5CB4(m);}}}
