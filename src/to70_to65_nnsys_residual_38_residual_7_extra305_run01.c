#pragma force_active on
#include <nitro/types.h>
#include <nitro/code32.h>
typedef struct Render {u8 *command;u32 reserved,flags;u8 pad[12];void (*callback)(struct Render *);u8 pad2[115];u8 timing;} Render;
extern void NNS_G3dGeBufferOP_N(u32,const u32 *,u32);
void NNSi_G3dFuncSbc_MTX(Render *);
static inline u32 Timing(Render *render){return render->callback?render->timing:0;}
void NNSi_G3dFuncSbc_MTX(Render *render){if(!(render->flags&512) && (render->flags&1)){u32 timing=Timing(render);u32 skip;if(timing==1){render->flags&=~64;render->callback(render);timing=Timing(render);skip=render->flags&64;}else skip=0;if(!skip){u32 param=render->command[1];if(!(render->flags&256))NNS_G3dGeBufferOP_N(20,&param,1);}if(timing==3){render->flags&=~64;render->callback(render);}}render->command+=2;}
