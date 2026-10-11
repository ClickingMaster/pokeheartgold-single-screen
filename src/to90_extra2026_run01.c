#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct {s16 x,y;} Pair;
typedef struct {void *object;u8 pad[4];s16 time;u16 pad2;void *other;} Work;
extern void ov45_0222AE08(void *,int *,int *),ov49_0225E420(void *,int,int,VecFx32 *),WiFiPlazaMap_PosToVecFx32(Pair *,VecFx32 *),WiFiPlazaCamera_SetTarget(void *,fx32,fx32,fx32),ov49_02259148(void *,VecFx32 *);
extern u32 ov49_02258E34(void *);
int ov49_02261234(Work *,void *,void *);
int ov49_02261234(Work *w,void *a,void *b){
VecFx32 start,end,delta;int x,y;Pair copy,p;int done,difference,fixed;u32 packed;
w->time++;if(w->time<24)done=0;else{w->time=24;done=1;}
ov45_0222AE08(w->object,&x,&y);ov49_0225E420(a,x,y,&end);packed=ov49_02258E34(w->other);p.x=packed;p.y=packed>>16;copy=p;WiFiPlazaMap_PosToVecFx32(&copy,&start);
delta.x=0;difference=end.y-start.y;fixed=(int)(w->time>0?(float)(w->time<<12)+0.5f:(float)(w->time<<12)-0.5f);delta.y=FX_Div((fx32)(((s64)fixed*difference+2048)>>12),24<<12);
if(w->time>=4){difference=end.z-start.z;fixed=(int)(w->time-4>0?(float)((w->time-4)<<12)+0.5f:(float)((w->time-4)<<12)-0.5f);delta.z=FX_Div((fx32)(((s64)fixed*difference+2048)>>12),20<<12);}else delta.z=0;
VEC_Add(&delta,&start,&delta);WiFiPlazaCamera_SetTarget(b,delta.x,delta.y,delta.z);
delta.y+=(fx32)(((s64)FX_SinCosTable_[((u16)(32767*w->time/24)>>4)*2]*65536+2048)>>12);ov49_02259148(w->other,&delta);return done;
}
