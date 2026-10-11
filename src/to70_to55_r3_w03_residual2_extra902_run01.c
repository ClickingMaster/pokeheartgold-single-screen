#pragma force_active on
#include "global.h"
extern void*ParticleSystem_GetEmitterParam(void*);
extern void ov07_02232BB0(void*,int*),sub_0209109C(int);
extern int SealOnCapsuleGetID(void*),SealOnCapsuleGetX(void*),SealOnCapsuleGetY(void*);
typedef struct{int a,b;void*effect;int c,d;void*seal;}Data;
typedef struct{u8 p[32];int**owner;int a;int x,y,z;}Work;
void ov07_02232C64(Work*);
void ov07_02232C64(Work*w){int pos[3];Data*d=ParticleSystem_GetEmitterParam(w);int x,y;ov07_02232BB0(d->effect,pos);sub_0209109C(SealOnCapsuleGetID(d->seal));x=SealOnCapsuleGetX(d->seal);y=SealOnCapsuleGetY(d->seal);pos[0]+=(s16)(x-190)*172;pos[1]+=(s16)(100-y)*172;w->x=pos[0]+(*w->owner)[1];w->y=pos[1]+(*w->owner)[2];w->z=pos[2]+(*w->owner)[3];}
