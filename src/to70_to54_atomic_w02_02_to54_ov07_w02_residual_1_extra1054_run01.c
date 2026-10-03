#pragma force_active on
#include "global.h"
typedef struct{u16 a,b,c,d;}Angle;extern volatile Angle ov07_02236414[];extern void*sub_02015524(void*);extern void sub_02015528(void*,int),Camera_SetAnglePos(Angle*,void*);

int ov07_02221D10(void*unused,void*w);

int ov07_02221D10(void*unused,void*w){Angle angle;u16 z,u;angle.a=ov07_02236414[4].a;angle.b=ov07_02236414[4].b;z=ov07_02236414[4].c;u=ov07_02236414[4].d;angle.c=z;angle.d=u;void*c=sub_02015524(((void**)w)[1]);sub_02015528(((void**)w)[1],1);Camera_SetAnglePos(&angle,c);return 1;}
