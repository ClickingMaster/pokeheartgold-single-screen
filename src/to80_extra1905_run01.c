#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct Actor {u8 pad[8];VecFx32 pos;int dx,dy;u8 pad1c[4];VecFx32 motion;u8 pad2c[12];u8 active,state,timer,pad3b,step,length,kind,pad3f,speed,row,pad42,wait,pad44,bounce;u16 angle;int collided;} Actor;
typedef struct CourseHeap {u8 pad[0x62c];Actor actor[2];u8 special,mode;u8 pad6c6[0x72];int ticks;u8 pad73c[7];u8 failures;} CourseHeap;
extern CourseHeap *PokeathlonCourse_GetHeapAllocPtr4(void*);
extern const u8 ov96_0221D640[],ov96_0221D514[][30],ov96_0221D5AA[][30];
extern void ov96_02213444(Actor*),ov96_02213E60(CourseHeap*,Actor*),ov96_022134D4(CourseHeap*,Actor*,VecFx32*),ov96_02212F0C(Actor*,Actor*);
extern int ov96_02213728(VecFx32*,VecFx32*,int,int,VecFx32*),ov96_0221341C(VecFx32*,int,VecFx32*,int);
extern u32 LCRandom(void);
void ov96_02212B94(void*);
void ov96_02212B94(void *course){CourseHeap *heap=PokeathlonCourse_GetHeapAllocPtr4(course);VecFx32 old,zero,out;int i;
 if(heap->actor[1].active==0&&heap->ticks<=900){heap->actor[1].state=3;heap->actor[1].active=1;}
 for(i=0;i<2;i++){Actor *a=&heap->actor[i];int speed;u8 chance;int collision;
 if(!a->active)continue;
 if(a->state==3){a->wait++;if(a->wait<8)continue;a->wait=0;a->state=0;continue;}
 if(a->state==1){a->timer--;if(a->timer!=0)continue;ov96_02213444(a);ov96_02213E60(heap,a);
  if(i==0){if(heap->ticks>=1200)chance=0;else if(heap->ticks>=600)chance=5;else if(heap->ticks>=300)chance=10;else chance=25;
  chance+=heap->failures;if(chance>100)chance=100;if((int)LCRandom()%100<chance){a->active=2;heap->failures=0;}else{a->active=1;if(heap->failures<100)heap->failures++;}}
  else a->active=1;continue;
 }
 if(a->state!=0&&a->state!=2)continue;
 speed=a->speed<<12;if(speed){
 old=a->pos;a->pos.x+=a->dx;a->pos.y+=a->dy;
 if(speed>=ov96_0221D640[a->angle]<<12){if(a->kind==2)a->state=2;else a->state=0;}else{a->state=0;a->bounce=0;}
 {VecFx32 *z=&zero;memset(z,0,sizeof(*z));}
 {int next;int step=a->step;if(a->kind==2)next=ov96_0221D514[a->row][step];else if(a->kind==1)next=ov96_0221D5AA[a->row][step];else{GF_AssertFail();next=0;}a->speed=next;}
 ov96_022134D4(heap,a,&a->motion);a->step++;
 if(a->step>=a->length){a->dx=0;a->dy=0;a->motion=zero;a->kind=0;a->speed=0;}
 else if(a->speed==0){a->motion=zero;a->step=0;a->length=0;a->kind=0;}
 a->collided=0;collision=ov96_02213728(&a->pos,&old,8,0,&out);
 if(collision){a->pos=out;a->collided=1;}
 switch(collision){case 3:case 4:a->motion.y*=-1;a->dy*=-1;break;case 1:case 2:a->motion.x*=-1;a->dx*=-1;break;}
 }else{a->state=0;a->bounce=0;a->angle=0;a->kind=0;}
 }
 if(heap->actor[0].active&&heap->actor[1].active){Actor *first=&heap->actor[0],*second=&heap->actor[1];int radius;
 if(first->state==1||first->state==3||second->state==1||second->state==3)return;
 if(first->active==1)radius=8;else if(first->active==2)radius=12;else{GF_AssertFail();return;}
 if(ov96_0221341C(&first->pos,radius<<12,&second->pos,2<<14))ov96_02212F0C(first,second);}
}
