#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct {u8 pad[12];void *pics;u8 pad10[0x60];void *other;} State;
extern void NNS_G3dGeBufferOP_N(u32,const u32 *,u32),NNS_G3dGeFlushBuffer(void),NNS_G2dSetupSoftwareSpriteCamera(void),PokepicManager_HandleLoadImgAndOrPltt(void *),PokepicManager_DrawAll(void *),ov71_022474CC(void *),RequestSwap3DBuffers(int,int);
void ov71_022479C8(State *);
void ov71_022479C8(State *s){u32 pop;G3X_Reset();NNS_G3dGeBufferOP_N(0x11,NULL,0);NNS_G3dGeFlushBuffer();NNS_G2dSetupSoftwareSpriteCamera();PokepicManager_HandleLoadImgAndOrPltt(s->pics);PokepicManager_DrawAll(s->pics);pop=1;NNS_G3dGeBufferOP_N(0x12,&pop,1);ov71_022474CC(s->other);RequestSwap3DBuffers(0,0);}
