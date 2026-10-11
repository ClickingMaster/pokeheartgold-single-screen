#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct{u16 x,y,z,w;} Angle;typedef struct{u8 pad[24];Angle angle;} Data;extern const Data ov07_02236414;extern void*ParticleSystem_GetCamera(void*);extern void ParticleSystem_SetPerspectiveType(void*,int),Camera_SetAnglePos(const Angle*,void*);int ov07_02221D88(void*,void*);
int ov07_02221D88(void*unused,void*w){Angle angle=ov07_02236414.angle;void*camera=ParticleSystem_GetCamera(*(void**)((u8*)w+4));ParticleSystem_SetPerspectiveType(*(void**)((u8*)w+4),1);Camera_SetAnglePos(&angle,camera);return 1;}
