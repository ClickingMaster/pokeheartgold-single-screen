#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct {u8 pad0[12];u8 items[0x948];s8 tick,count;u8 total,shown;} Work;
extern void ModelInstance_SetVisible(void*,int);extern int ModelInstance_IsVisible(void*);extern int ov49_02267A1C(void*,Work*,int);
int ov49_022669B0(void*,Work*);
int ov49_022669B0(void*object,Work*w){int i,result;if(w->count<w->total){int target;u32 n;u8*p;w->tick++;if(w->tick>=8){w->tick=0;w->count++;}target=(w->tick*4)/8+w->count*4;n=w->shown;if(n<(u32)target){p=w->items+n*120;do{ModelInstance_SetVisible(p,1);p+=120;}while(++n<(u32)target);}w->shown=target;}i=0;if(i<w->shown){u8*p=w->items,*q=p;do{if(ModelInstance_IsVisible(p)==1){result=ov49_02267A1C(object,w,i);if(result==1)ModelInstance_SetVisible(q,0);}p+=120;q+=120;}while(++i<w->shown);}if(w->count==w->total&&result==1)return 1;return 0;}
