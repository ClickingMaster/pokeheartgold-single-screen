#pragma force_active on
#include "global.h"
typedef struct {int x[12];} State;
typedef struct {void *bg;u8 pad[24];int layer;} Work;
typedef struct {Work*w;State state;int flag,ticks,dx,dy,x,y;} Env;
extern void ov41_02249F0C(void*,void*);extern void *CreateSysTaskAndEnvironment(void(*)(void*,void*),int,int,int);extern void *SysTask_GetData(void*);extern int Bg_GetXpos(void*,int);extern int Bg_GetYpos(void*,int);extern void ov41_02249E60(State*,int,int);

void ov41_02249DB4(Work*w,State*s,int dx,int dy,int ticks,int flag);

void ov41_02249DB4(Work*w,State*s,int dx,int dy,int ticks,int flag){Env*e=SysTask_GetData(CreateSysTaskAndEnvironment(ov41_02249F0C,76,0,13));e->w=w;e->state=*s;e->flag=flag;e->ticks=ticks;e->dx=dx/ticks;e->dy=dy/ticks;e->x=Bg_GetXpos(w->bg,w->layer);e->y=Bg_GetYpos(w->bg,w->layer);s->x[10]=128;s->x[9]=5;s->x[5]-=dx;s->x[6]-=dy;s->x[11]=14;ov41_02249E60(s,0,0);}
