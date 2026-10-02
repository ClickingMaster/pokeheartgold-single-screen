#pragma force_active on
#include "global.h"
typedef struct {void**arg;}Script;typedef struct {u32 a,b;void*save;}Args;
extern void*FrontierSystem_GetFrontierMap(void*),*SaveArray_Party_Get(void*),*Party_GetMonByIndex(void*,u32);extern Args*Frontier_GetLaunchArgs(void*);
extern u16 FrontierScript_ReadVar(void*),FrontierScriptContext_ReadHalfWord(void*);
extern void ov80_0222F030(void*,void*,int,int,int,int,int,int,int,int);
int FrtCmd_069(Script*);
int FrtCmd_069(Script*s){void*map=FrontierSystem_GetFrontierMap(s->arg);u32 n=FrontierScript_ReadVar(s);int a=FrontierScript_ReadVar(s),b=FrontierScript_ReadVar(s);int c=FrontierScriptContext_ReadHalfWord(s);void*mon;FrontierScriptContext_ReadHalfWord(s);FrontierScriptContext_ReadHalfWord(s);mon=Party_GetMonByIndex(SaveArray_Party_Get(Frontier_GetLaunchArgs(*s->arg)->save),n);ov80_0222F030(map,mon,11,c,a,b,0,0,8,0);return 0;}
