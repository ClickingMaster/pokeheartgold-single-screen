#pragma force_active on
#include "global.h"
typedef struct {u32 x[37];} Data;typedef struct {u8 pad[32];Data data;} Work;

void ov45_0222BB00(Work*w,Data*d,u32 size);

void ov45_0222BB00(Work*w,Data*d,u32 size){w->data=*d;if(size<=148)MI_CpuCopy8(d,&w->data,size);else MI_CpuCopy8(d,&w->data,148);MI_CpuCopy8((u8*)d+8,(u8*)w+16,16);MI_CpuCopy8(w,(u8*)w+40,16);}
