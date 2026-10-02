#pragma force_active on
#include "global.h"
typedef struct {void*task;u8 a,b,c,d;u16 e;u8 f,pad;}Task;
typedef struct {u8 pad[0xb0];Task task;}Work;
typedef struct {void**arg;}Script;
extern Work*sub_0209680C(void*);extern void SysTask_Destroy(void*),ov80_0222F53C(void*,void*);
extern u16 FrontierScript_ReadVar(void*);
extern void*SysTask_CreateOnMainQueue(void(*)(void*,void*),void*,u32);
int FrtCmd_078(Script*);
int FrtCmd_078(Script*s){Work*w=sub_0209680C(*s->arg);Task*t=&w->task;if(t->task){GF_AssertFail();SysTask_Destroy(t->task);}MI_CpuFill8(t,0,12);t->a=FrontierScript_ReadVar(s);t->b=FrontierScript_ReadVar(s);t->c=FrontierScript_ReadVar(s);t->d=FrontierScript_ReadVar(s);t->e=FrontierScript_ReadVar(s);t->f=FrontierScript_ReadVar(s);t->task=SysTask_CreateOnMainQueue(ov80_0222F53C,t,300);return 0;}
