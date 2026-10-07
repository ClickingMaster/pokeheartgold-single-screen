#include "global.h"
#include "spl.h"
#include "particle_internal.h"
#pragma force_active on
typedef struct {void *context;ParticleContext *particles;u32 reserved;u32 arguments[6];u32 first,second;u32 tail[3];} State;
extern int ov07_02231924(void *,int);
extern int sub_02015530(ParticleContext *);
void ov07_02221550(void *,void *);
void ov07_02221550(void *emitterArg,void *stateArg){
 SPLEmitter *emitter=emitterArg;u8 *state=stateArg;
 int value;fx32 offset;
 ov07_02231924(*(void**)state,*(u32*)(state+0x24));
 value=ov07_02231924(*(void**)state,*(u32*)(state+0x28));
 switch(*(u32*)(state+0xc)){case 0:break;case 1:offset=value==0?256:-5120;break;case 2:offset=value==0?0:-5376;break;case 3:break;}
 sub_02015530(*(void**)(state+4));
 SPL_SetEmitterPositionZ(emitter,offset);
}
