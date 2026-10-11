#pragma force_active on
#include "global.h"
#include <string.h>
typedef struct {u8 p[0x8c];u8 target[80];u8 output[856];u8 q[0x1a0];u8 source[936];u8 r[0xc58];int mode;} W;
extern W *OverlayManager_GetData(void*);extern void *MysteryGiftComm_GetRecvBuffer(int,int,int);extern void ov74_02235F58(void*,void*,int);
void *ov74_0222A078(void*);
void *ov74_0222A078(void*m) {W*w=OverlayManager_GetData(m);void*out=w->output;void*target=w->target;switch(w->mode){case 1:memcpy(out,MysteryGiftComm_GetRecvBuffer(0,0,0),856);ov74_02235F58(w->target,w->output,84);break;case 3:{u8 *src=w->source;memcpy(out,src+80,856);memcpy(target,src,80);break;}}return out;}
