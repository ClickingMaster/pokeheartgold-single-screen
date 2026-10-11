#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct {int x,y;void *object,*script;int p;} Config;typedef struct {u8 p[12];int kind;Config config;void *task;} Work;
extern Config *FieldEffect_GetUserData(void *);extern int FieldEffect_GetParam(void *);extern void sub_0206121C(void *,VecFx32 *);extern void FieldEffect_SetPosition(void *,VecFx32 *);extern void *FieldEffectManager_CreateBillboardFromRecord(void *,int,VecFx32 *);int ov01_021FF394(void *,Work *);
int ov01_021FF394(void *owner,Work *w){VecFx32 pos;int a;w->config=*FieldEffect_GetUserData(owner);w->kind=FieldEffect_GetParam(owner);pos.x=(w->config.x<<16)+32768;pos.z=(w->config.y<<16)+36864;pos.y=0;sub_0206121C(w->config.object,&pos);pos.y+=4096;FieldEffect_SetPosition(owner,&pos);switch(w->kind){case 0:a=5;break;case 1:a=6;break;case 2:default:a=7;break;}w->task=FieldEffectManager_CreateBillboardFromRecord(w->config.script,a,&pos);return 1;}
