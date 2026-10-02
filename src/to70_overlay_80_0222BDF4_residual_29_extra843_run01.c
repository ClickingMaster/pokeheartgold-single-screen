#pragma force_active on
#include "global.h"
extern void *FrontierSystem_GetFrontierMap(void *);
extern u16 FrontierScriptContext_ReadHalfWord(void *);
extern void ov80_022398E4(void *,s16 *,s16 *);
extern void ov80_0223A00C(u16,void *,void *,void *,void *,void *,s16,s16);
extern void Sound_SetSceneAndPlayBGM(int,int,int);
extern int ov80_0222E05C(void *);
extern void FrontierScriptContext_Pause(void *,int (*)(void *));
typedef struct{void *system;u8 p[0x74];int state;} Context;
typedef struct{void *a,*b;u8 p[0x2c];void *c,*d;} Map;
int FrtCmd_071(Context *ctx);
int FrtCmd_071(Context *ctx){Map *m=FrontierSystem_GetFrontierMap(ctx->system);u16 id=FrontierScriptContext_ReadHalfWord(ctx);struct{s16 y,x;} pos;ov80_022398E4(m,&pos.x,&pos.y);ov80_0223A00C(id,m->a,m->c,m->d,m->b,&ctx->state,pos.x,pos.y);Sound_SetSceneAndPlayBGM(5,1147,1);FrontierScriptContext_Pause(ctx,ov80_0222E05C);return 1;}
