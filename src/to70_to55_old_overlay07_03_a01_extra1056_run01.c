#pragma force_active on
#include "global.h"
typedef struct {u8 p[0x84];void *task;int active;} W;
extern void ov07_0221C69C(void);extern void ov07_02232AFC(W*,void*);extern void ov07_02232A5C(void*,void*);extern void ov07_02232C28(void);extern void *SysTask_CreateOnMainQueue(void(*)(void*,void*),void*,int);
void ov07_02232A14(W*,void*);void ov07_02232A44(W*);
void ov07_02232A14(W*w,void*a) {w->active=1;ov07_0221C69C();ov07_02232AFC(w,a);w->task=SysTask_CreateOnMainQueue(ov07_02232A5C,w,1000);}
void ov07_02232A44(W*w) {ov07_02232A14(w,(void*)ov07_02232C28);}
