#pragma force_active on
#include "global.h"
#include "task.h"
#include "pokemon.h"
#include "sound.h"
#include "heap.h"
extern int IsCryFinished(void);
typedef struct{int mode;u32 unused;int enabled;u8 pad[28];Pokemon*mon;}Env;
extern BOOL ov01_02205A60(TaskManager*);extern int ov02_02250780(void*,int);extern void ov02_022507B4(void*,int);
extern void FieldSystem_UnkSub108_AddMonMood(void*,int);
extern int(*ov02_0225373C[])(TaskManager*,void*,Env*);extern int(*ov02_02253724[])(TaskManager*,void*,Env*);
BOOL Task_FieldTeleport(TaskManager*);
BOOL Task_FieldTeleport(TaskManager*t){void*f=TaskManager_GetFieldSystem(t);Env*e=TaskManager_GetEnvironment(t);u32*s=TaskManager_GetStatePtr(t);switch(*s){case 0:if(e->enabled)TaskManager_Call(t,ov01_02205A60,0);++*s;break;
case 1:++*s;if(e->enabled){int n;if(ov02_02250780(f,14)){n=2;FieldSystem_UnkSub108_AddMonMood(*(void**)((u8*)f+0x108),1);}else n=1;ov02_022507B4(f,n);break;}
case 2:if(e->enabled){u32 species=GetMonData(e->mon,5,0);u32 form=GetMonData(e->mon,112,0);PlayCry((u16)species,(u8)form);}++*s;
case 3:{int result;do{if(e->enabled){if(IsCryFinished())result=0;else result=ov02_0225373C[e->mode](t,f,e);}else result=ov02_02253724[e->mode](t,f,e);if(result==2)Heap_Free(e);}while(result==1);}break;}return FALSE;}
