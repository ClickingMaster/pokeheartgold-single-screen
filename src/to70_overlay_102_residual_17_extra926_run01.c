#pragma force_active on
#include "global.h"
typedef struct{u8 p[0x18];void *a;u8 p1c[0x1d0];void *view,*other;} Owner;
typedef struct{Owner *owner;u8 p4[16];int state;} Work;
extern int ov102_021E9044(void *);extern void ov102_021EBFA0(void *,int);extern int ov102_021EC05C(void *);extern void ov102_021EC290(void *,int);extern void ov102_021E94A4(Work *);
void ov102_021EA00C(void *,Work *);
void ov102_021EA00C(void *unused,Work *w){Owner *o=w->owner;switch(w->state){case 0:ov102_021EBFA0(o->view,ov102_021E9044(o->a));w->state++;break;case 1:if(ov102_021EC05C(o->view)){ov102_021EC290(o->other,1);ov102_021E94A4(w);}break;}}
