#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct {u16 initial;u16 pad;u32 other;int x,y,dx,dy;union {u32 flags;struct {u32 xdir:1,ydir:1,tick:30;};};} Motion;
typedef struct {u8 p[0xc];Motion*motion;u8 p2[0x43c];u8 id;} Sub;
typedef struct {u8 p[0x34];Sub*sub;} Work;
extern void ov14_021E7A08(Work*,int,int,int,int);
extern void ov14_021F396C(Sub*,int,int);
extern void ov14_021F39A0(Sub*,int,int);
extern void ov14_021F3B5C(Sub*);
extern void ov14_021F395C(Sub*,s16,s16);
void ov14_021E7AE4(Work*,int,int);
int ov14_021E7B04(Work*,int);
void ov14_021E7AE4(Work*w,int a,int b){ov14_021E7A08(w,w->sub->id,a,b,0);}
int ov14_021E7B04(Work*w,int flag){Sub*s=w->sub;Motion*m=s->motion;u32 f=m->flags;if((f>>2)==8){if(flag==1)ov14_021F396C(s,m->initial,m->other);else ov14_021F39A0(s,m->initial,m->other);ov14_021F3B5C(w->sub);return 0;}if(m->xdir==0)m->x+=m->dx;else m->x-=m->dx;if(m->ydir==0)m->y+=m->dy;else m->y-=m->dy;ov14_021F395C(w->sub,(s16)(m->x>>8),(s16)(m->y>>8));ov14_021F3B5C(w->sub);m->tick++;return 1;}
