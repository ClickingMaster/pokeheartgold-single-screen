#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct{u8 p[2];s8 delta;u8 p2;s16 x,y,z,angle,p3,time;}Work;
extern void ov112_021EA9BC(void*,Work*);

void ov112_021EAA98(void*a,Work*w,int kind);

void ov112_021EAA98(void*a,Work*w,int kind){if(kind==0){w->x=128;w->y=130;w->delta=-10;w->time=0;}else if(kind==1){w->x=128;w->y=-110;w->delta=10;w->time=0;}else if(kind==2){w->x=128;w->y=130;w->delta=0;w->time=0;}w->z=0;w->angle=0;ov112_021EA9BC(a,w);}
