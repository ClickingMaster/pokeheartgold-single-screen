#pragma force_active on
#include "global.h"
typedef struct {u8 pad[0x110];s32 shift;} Work;
typedef struct {u16 lo,hi;} Range;typedef struct {u32 count;Range*ranges;} Entry;
extern Entry ov85_021EA758[];extern void ov85_021E8530(s32*,s32);

int ov85_021E8764(Work*w,s32 position,int which);

int ov85_021E8764(Work*w,s32 position,int which){s32 value;u32 a,count; s32 b;Range*r;ov85_021E8530(&position,-w->shift);value=position;ov85_021E8530(&value,-16384);a=value/4096;value=position;ov85_021E8530(&value,20480);b=value/4096;{Entry*e=&ov85_021EA758[which];count=e->count;r=e->ranges;}while(count){if((a>=r->lo&&a<=r->hi)||((u32)b>=r->lo&&(u32)b<=r->hi))return 1;r++;count--;}return 0;}
