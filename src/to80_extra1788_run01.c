#pragma force_active on
#include "global.h"
#pragma force_active on
extern void ov59_0223C4EC(void*,void*);extern void ParticleSystem_CreateEmitterWithParam(void*,int,void*,void*),ParticleSystem_SetPerspectiveType(void*,int);
void ov59_0223C3C0(u8*w,int value);
void ov59_0223C3C0(u8*w,int value){ParticleSystem_CreateEmitterWithParam(*(void**)(w+36),value,ov59_0223C4EC,w);ParticleSystem_SetPerspectiveType(*(void**)(w+36),1);}
