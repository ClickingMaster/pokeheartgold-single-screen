#pragma force_active on
#include "global.h"
typedef struct{u8 p[16];int count;void*objects[18];int ready[11];u32 active;}Work;
extern int ParticleSystem_GetActiveEmitterCount(void*);extern void BattleAnimSystem_FreeParticleSystem(void*),SysTask_Destroy(void*);void ov07_02232A5C(void*,Work*);
void ov07_02232A5C(void*task,Work*w){int busy=0,i;if(w->count>9)GF_AssertFail();for(i=0;i<w->count;i++){if(w->objects[i]){if(ParticleSystem_GetActiveEmitterCount(w->objects[i])==0&&w->ready[i]){BattleAnimSystem_FreeParticleSystem(w->objects[i]);w->objects[i]=0;w->ready[i]=0;}else busy=1;}}if(busy==0){w->active=0;SysTask_Destroy(task);}}
