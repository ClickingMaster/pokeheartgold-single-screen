#pragma force_active on
#include "global.h"
typedef struct{int state,mode;u8 p[0x2c];void *arg;} Work;
extern Work *TaskManager_GetEnvironment(void *);extern int (*ov01_022069A8[])(Work *);extern int (*ov01_022069BC[])(Work *);extern void FollowMon_MoveToPlayer(void *,u8);extern void ov01_021F30F4(Work *);
int ov01_021F2944(void *);
int ov01_021F2944(void *task){Work *w=TaskManager_GetEnvironment(task);int result;do {if(w->mode==0)result=ov01_022069A8[w->state](w);else result=ov01_022069BC[w->state](w);}while(result==2);if(result==1){FollowMon_MoveToPlayer(w->arg,(u8)w->mode);ov01_021F30F4(w);return 1;}return 0;}
