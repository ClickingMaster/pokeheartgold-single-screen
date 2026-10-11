#pragma force_active on
#include "global.h"
typedef struct {u8 pad[12];int ready;u8 pad2[20];void*bg;void*obj;} Work;
extern void BgSetPosTextAndCommit(void*,int,int,int);extern void TradeSequence_AdjustCameraAngle(void*,s16*);extern void SysTask_Destroy(void*);

void ov71_02249C98(void*task,Work*w);

void ov71_02249C98(void*task,Work*w){s16 v[3];if(w->ready){w->ready=0;v[0]=0x1150;v[1]=0;v[2]=0;BgSetPosTextAndCommit(w->bg,3,3,0);BgSetPosTextAndCommit(w->bg,7,3,256);TradeSequence_AdjustCameraAngle(w->obj,v);*(volatile u16*)0x4000304|=0x8000;SysTask_Destroy(task);}}
