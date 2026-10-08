#pragma force_active on
#include "global.h"
typedef struct Agent {void *parts[22];int x,y;u8 pad60[0x1c];int destx,desty,destz,speedx,speedy,speedz;u8 pad94[0x10],f4,f5,f6,pada7[3],fa,fb,padac[5],part;} Agent;
typedef struct Entry {Agent *agent;u16 x,y;} Entry;
typedef struct Obstacle {int active,x,y;} Obstacle;
typedef struct SceneCell {int x,y,a,b;s16 score,index;} SceneCell;
typedef struct Other {void *ptr;u8 pad[12];u16 x,y;} Other;
typedef struct Scene {Entry agents[4];Obstacle obstacles[20],bonus[10];Other *other;u32 pad18c;SceneCell cells[48];u8 pad550[8],count;} Scene;
typedef struct XY {int y,x;} XY;
extern void ov96_021EB0A4(void *,int,int,int *,int *),ov96_021EB03C(void *,int,int,int *,int *);
extern u32 LCRandom(void);
extern int ov96_022090A8(SceneCell *,u16,u16),ov96_022090D8(SceneCell *);
extern SceneCell *ov96_02208FB8(Scene *);
void ov96_02208BB8(Scene *,int);
// @fn ov96_02208BB8
void ov96_02208BB8(Scene *s,int who){Obstacle obstacles[20],bonus[10];Entry agents[4];Entry *self;Other *other;Agent *a;XY raw,screen,chosen;SceneCell *best;int i,j,k;
 a=s->agents[who].agent;
 if(a->fb||a->f6==1||a->f4)return;
 for(i=0;i<4;i++){Agent *p;agents[i]=s->agents[i];p=agents[i].agent;ov96_021EB0A4(p->parts[p->part],p->x/4096,p->y/4096,&raw.x,&raw.y);ov96_021EB03C(p->parts[p->part],raw.x<<12,raw.y<<12,&screen.x,&screen.y);agents[i].x=screen.x/4096;agents[i].y=screen.y/4096;}
 self=&agents[who];for(i=0;i<20;i++)obstacles[i]=s->obstacles[i];for(i=0;i<10;i++)bonus[i]=s->bonus[i];other=s->other;
 for(i=0;i<48;i++){s->cells[i].index=i;s->cells[i].x=self->x-80+(i%6)*32;s->cells[i].y=self->y-48+(i/6)*32;s->cells[i].a=self->x-96+(i%6)*32+(int)LCRandom()%32;s->cells[i].b=self->y-128+(i/6)*32+(int)LCRandom()%32;}
 for(i=0;i<48;i++)s->cells[i].score=0;
 for(k=0;k<2;k++)for(j=0;j<4;j++){u8 id=j+19;s->cells[id].score+=2;}
 for(i=0;i<48;i++){SceneCell *c=&s->cells[i];{Other*o=other;for(j=0;j<5;j++,o++)if(o->ptr&&ov96_022090A8(c,o->x,o->y))c->score+=7;}for(j=0;j<4;j++)if(j!=who&&ov96_022090A8(c,agents[j].x,agents[j].y))c->score-=3;
  for(j=0;j<20;j++){if(!obstacles[j].active)break;if(ov96_022090A8(c,(u16)obstacles[j].x,(u16)obstacles[j].y))c->score-=2;}
  for(j=0;j<10;j++){if(!bonus[j].active)break;if(ov96_022090A8(c,(u16)bonus[j].x,(u16)bonus[j].y))c->score+=2;}
  if(!ov96_022090D8(c))c->score-=100;
 }
 {u16 y=a->y/4096;if(y>256){for(i=0;i<6;i++)s->cells[i+30].score+=3;}else if(y<256){for(i=0;i<6;i++)s->cells[i].score+=3;}}
 if(a->fa){for(i=0;i<6;i++)s->cells[i+30].score+=a->fa;{u16 x=a->x/4096;if(x>256){for(i=0;i<3;i++)s->cells[i+30].score+=2;}else if(x<256){for(i=0;i<3;i++)s->cells[i+33].score+=2;}}}
 best=ov96_02208FB8(s);if((u32)who>=s->count){Agent *p=self->agent;ov96_021EB0A4(p->parts[p->part],self->x,self->y,&chosen.x,&chosen.y);self->agent->destx=chosen.x<<12;self->agent->desty=chosen.y<<12;self->agent->destz=0;self->agent->speedx=best->a<<12;self->agent->speedy=best->b<<12;self->agent->speedz=0;self->agent->f5=1;}
}
