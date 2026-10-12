#pragma force_active on
#include "global.h"
#pragma force_active on
#define P(p,o) (*(void **)((u8 *)(p)+(o)))
#define U(p,o) (*(u32 *)((u8 *)(p)+(o)))
#define B(p,o) (*((u8 *)(p)+(o)))
extern void *TaskManager_GetEnvironment(void *); extern int (*ov01_02206994[])(void *); extern void FollowMon_MoveToPlayer(void *,u8); extern void ov01_021F30F4(void *);
int ov01_021F2DD0(void *task);
int ov01_021F2DD0(void *task){int r;void *env=TaskManager_GetEnvironment(task); do{r=ov01_02206994[U(env,0)](env);}while(r==2);if(r==1){FollowMon_MoveToPlayer(P(env,0x34),(u8)U(env,4));ov01_021F30F4(env);}return r;}
