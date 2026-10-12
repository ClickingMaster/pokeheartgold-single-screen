#pragma force_active on
#include "global.h"
typedef struct {u16 state;u8 p[2];int arg;u8 q[4];void *obj;u8 r[12];int mode;} W;
extern W *TaskManager_GetEnvironment(void*);extern int (*ov01_022069D0[])(W*,void*);extern int (*ov01_022069F0[])(W*,void*);extern void FollowMon_MoveToPlayer(void*,u8);extern void *FollowMon_GetMapObject(void*);extern void sub_02069DC8(void*,int);extern void FollowMon_SetFacingDirection(void*,int);extern void ov01_021F30F4(W*);
int ov01_021F2628(void*);
int ov01_021F2628(void*task) {W*w=TaskManager_GetEnvironment(task);int v;do{if(w->mode==1)v=ov01_022069D0[w->state](w,task);else v=ov01_022069F0[w->state](w,task);}while(v==2);if(v==1){FollowMon_MoveToPlayer(w->obj,(u8)w->arg);sub_02069DC8(FollowMon_GetMapObject(w->obj),1);FollowMon_SetFacingDirection(w->obj,1);ov01_021F30F4(w);}return v;}
