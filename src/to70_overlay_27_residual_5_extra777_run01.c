#pragma force_active on
#include "global.h"
typedef struct {u8 p[0x6c];u32 ready;u8 q[0xd2-0x70];u8 state:6;u8 other:2;}Data;
typedef struct {u8 pad[16];Data*data;}Work;
extern void ov27_0225A7B0(Work*),ov27_0225A7DC(Work*),ov27_0225B404(Work*);
extern int IsPaletteFadeFinished(void),ov27_0225B4D8(Work*);
void ov27_0225A7FC(Work*);
void ov27_0225A7FC(Work*w){switch(w->data->state){case 0:ov27_0225A7B0(w);break;case 1:ov27_0225A7DC(w);w->data->state=2;break;case 2:if(w->data->ready&&IsPaletteFadeFinished()&&ov27_0225B4D8(w))ov27_0225B404(w);break;case 3:case 4:default:GF_AssertFail();break;}}
