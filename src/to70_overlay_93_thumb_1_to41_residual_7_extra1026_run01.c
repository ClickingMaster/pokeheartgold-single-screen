#pragma force_active on
#include "global.h"
extern void ParticleSystem_CreateEmitterWithParam(void *,int,int,void *);void ov93_0225D700(void *,int);
void ov93_0225D700(void *w,int choice){switch(choice){case 0:ParticleSystem_CreateEmitterWithParam(*(void**)((u8*)w+0xa4),0,0,w);break;case 1:ParticleSystem_CreateEmitterWithParam(*(void**)((u8*)w+0xa4),1,0,w);break;case 2:ParticleSystem_CreateEmitterWithParam(*(void**)((u8*)w+0xa4),2,0,w);break;default:GF_AssertFail();break;}}
