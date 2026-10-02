#pragma force_active on
#include "global.h"
typedef struct {void *frontier;u8 p[0x30];void *ui;} Env;typedef struct {Env *env;} W;typedef struct {u8 p[8];void *save;} Args;
extern Args *Frontier_GetLaunchArgs(void*);extern void *FrontierScript_ReadVarPtr(W*);extern void sub_0201A728(int);extern void sub_0201A748(void*);extern void Save_PrepareForAsyncWrite(void*,int);extern void FrontierScriptContext_Pause(W*,int(*)(W*));extern int Save_WriteFileAsync(void*);extern void sub_0201A774(void);extern void sub_0201A738(int);
int FrtCmd_115(W*);int ov80_0222CE80(W*);
int FrtCmd_115(W*w) {Env*e=w->env;Args*a=Frontier_GetLaunchArgs(e->frontier);FrontierScript_ReadVarPtr(w);sub_0201A728(4);sub_0201A748(e->ui);Save_PrepareForAsyncWrite(a->save,2);FrontierScriptContext_Pause(w,ov80_0222CE80);return 1;}
int ov80_0222CE80(W*w) {int v=Save_WriteFileAsync(Frontier_GetLaunchArgs(w->env->frontier)->save);if(v==2){sub_0201A774();sub_0201A738(4);return 1;}if(v==3){sub_0201A774();sub_0201A738(4);return 1;}return 0;}
