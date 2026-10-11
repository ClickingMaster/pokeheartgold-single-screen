#pragma force_active on
#include "global.h"
#pragma force_active on
typedef union {u32 raw;struct {u32 points:12,mid:8,id1:2,id2:2,hi:8;} b;} Flags;
typedef struct Work {void *course;u8 pad[12];struct Work *other;u8 pad2[0x58-0x14];u8 mode,pad3[2],timer;u32 unk;Flags flags;} Work;
extern void PokeathlonCourse_AddMonCounter(void*,u8,u8,int,int),ov96_02218578(Work*,int);
void ov96_02219030(Work*);
void ov96_02219030(Work*w){Work*other;int points;w->timer=60;w->mode=0;other=w->other;if(other==NULL){points=w->flags.b.points-10;if(points<0)points=0;w->flags.b.points=points;PokeathlonCourse_AddMonCounter(w->course,w->flags.b.id1,w->flags.b.id2,8,1);}else{points=other->flags.b.points+10;if(points>200)points=200;other->flags.b.points=points;PokeathlonCourse_AddMonCounter(other->course,other->flags.b.id1,other->flags.b.id2,3,1);w->other=NULL;}PokeathlonCourse_AddMonCounter(w->course,w->flags.b.id1,w->flags.b.id2,5,1);PokeathlonCourse_AddMonCounter(w->course,w->flags.b.id1,w->flags.b.id2,1,1);ov96_02218578(w,3);}
