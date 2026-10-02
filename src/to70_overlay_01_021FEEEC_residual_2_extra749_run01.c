#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct {int x,y;void *object,*script;int p;} Config;typedef struct {u8 p[12];int kind;Config config;void *task;} Work;
extern Config *sub_02068D98(void *);extern int sub_02068D90(void *);extern void sub_0206121C(void *,VecFx32 *);extern void sub_02068DA8(void *,VecFx32 *);extern void *ov01_021F1740(void *,int,VecFx32 *);int ov01_021FF394(void *,Work *);
int ov01_021FF394(void *owner,Work *w){VecFx32 pos;int a;w->config=*sub_02068D98(owner);w->kind=sub_02068D90(owner);pos.x=(w->config.x<<16)+32768;pos.z=(w->config.y<<16)+36864;pos.y=0;sub_0206121C(w->config.object,&pos);pos.y+=4096;sub_02068DA8(owner,&pos);switch(w->kind){case 0:a=5;break;case 1:a=6;break;case 2:default:a=7;break;}w->task=ov01_021F1740(w->config.script,a,&pos);return 1;}
