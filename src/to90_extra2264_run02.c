#pragma force_active on
#include "global.h"
typedef struct{u16 a,b,c,d;} Angle;extern Angle ov07_02236414[];extern void*ParticleSystem_GetCamera(void*);extern void ParticleSystem_SetPerspectiveType(void*,int);extern void Camera_SetAnglePos(Angle*,void*);

int ov07_02221E24(void*unused,void*w);
int ov07_02221E60(void*unused,void*w);

static inline void copyAngle(volatile Angle *dst,const volatile Angle *src){u16 third,fourth;dst->a=src->a;dst->b=src->b;third=src->c;fourth=src->d;dst->c=third;dst->d=fourth;}
#pragma force_active on
int ov07_02221E60(void*unused,void*w){Angle a;void*c;copyAngle(&a,&ov07_02236414[1]);c=ParticleSystem_GetCamera(((void**)w)[1]);ParticleSystem_SetPerspectiveType(((void**)w)[1],1);Camera_SetAnglePos(&a,c);return 1;}
