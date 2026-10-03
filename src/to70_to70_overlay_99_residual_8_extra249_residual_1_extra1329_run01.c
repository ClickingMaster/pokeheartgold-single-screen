#pragma force_active on
#include "global.h"
typedef struct{u8 pad[0x14];u16 mons[494];u16*list;u32 pages:5;u32 count:9;u32 rest:18;}Work;
extern u16*ov99_021E728C(Work*,u32*);extern int ov99_021E7180(Work*,u16);
void ov99_021E72C0(Work*);
void ov99_021E72C0(Work*w){u32 count,i;u16 id;w->list=ov99_021E728C(w,&count);for(i=0;i<count;i++){id=w->list[i];if(id==0||id>493)GF_AssertFail();if(ov99_021E7180(w,id)){w->mons[w->count]=id;w->count++;}}if(w->count){w->pages=w->count/30;if(w->count%30==0)w->pages--;}}
