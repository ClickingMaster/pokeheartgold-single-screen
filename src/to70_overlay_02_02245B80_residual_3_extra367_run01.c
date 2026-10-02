#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct State {void *ctx;u8 *field;u8 *obj;void *task;u8 index;u8 pad;u16 step;} State;
extern u8 ov02_02253254[];extern u32 ov02_02253D80[],ov02_02253D84[];
void *TaskManager_GetEnvironment(void *),*ov02_02245E04(void *,void *),*SysTask_CreateOnMainQueue(void *,void *,int);void PlayerAvatar_CopyPositionVector(void *,s32 *),Field3dObject_SetPosEx(void *,int,int,int),ov02_02245ED8(void *,int,u32,int),ov02_022462DC(void),QueueScript(void *,int,int,int),SysTask_Destroy(void *),ov02_02245E68(void *),Heap_Free(void *),PlaySE(int);int ov02_02245FD0(void *),ov02_022461DC(void *);
int ov02_022461DC(void *manager){State *s=TaskManager_GetEnvironment(manager);switch(s->step){case 0:{s32 pos[3];s->obj=ov02_02245E04(ov02_02253254+s->index*8,s->ctx);PlayerAvatar_CopyPositionVector(*(void **)(s->field+64),pos);Field3dObject_SetPosEx(s->obj+16,pos[0],pos[1],pos[2]);PlaySE(2319);s->step++;break;}case 1:if(ov02_02245FD0(s->obj)){ov02_02245ED8(s->obj,174,ov02_02253D80[s->index*2],3);s->task=SysTask_CreateOnMainQueue(ov02_022462DC,s,0);QueueScript(manager,3,0,0);s->step++;}break;case 2:SysTask_Destroy(s->task);s->step++;case 3:if(!ov02_02245FD0(s->obj))break;ov02_02245ED8(s->obj,174,ov02_02253D84[s->index*2],3);s->step++;case 4:if(!ov02_02245FD0(s->obj))break;ov02_02245E68(s->obj);Heap_Free(s);return 1;}return 0;}
