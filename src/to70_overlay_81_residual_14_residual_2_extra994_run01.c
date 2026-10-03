#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct{u8 p[17];u8 count;u8 gap[0x3c8-18];u16 order[6];u8 gap2[0x464-0x3d4];void *input;u8 gap3[20];void *menu;}Work;
extern u32 GridInputHandler_GetNextInput(void*);
extern void ov81_02241C84(int,void*,void*,void*);

void ov81_02241CA0(Work*w,void*a,void*b);

void ov81_02241CA0(Work*w,void*a,void*b){u32 value=GridInputHandler_GetNextInput(w->input);u16 i;if(value<6){for(i=0;i<w->count;i++)if(value==w->order[i])break;ov81_02241C84(i,w->menu,a,b);}}
