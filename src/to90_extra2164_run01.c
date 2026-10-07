#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct{u8 a,b,c,d;u8 pad[4];} Entry;
typedef struct{void *object;Entry *entries;u8 enabled,selected;} Work;
void ov12_0226BAFC(void*,u32,u32,u32,u32);
void ov08_02224B98(Work*,u8);
void ov08_02224B98(Work *w,u8 selected){w->selected=selected;if(w->enabled==1){u32 offset=w->selected*8;u8 *p=(u8*)w->entries;ov12_0226BAFC(w->object,p[offset],p[offset+2],p[offset+1],p[offset+3]);}}
