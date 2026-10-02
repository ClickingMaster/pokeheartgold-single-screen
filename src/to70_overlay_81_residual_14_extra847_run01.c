#pragma force_active on
#include "global.h"
typedef struct{u8 a[0x11];u8 count;u8 b[0x1ac-0x12];void*pics[16];u8 gap[0x3c8-0x1ec];u16 selected[16];u8 gap2[0x464-0x3e8];void*input;} Work;
extern u32 GridInputHandler_GetNextInput(void*);extern void Pokepic_SetAttr(void*,int,int),ov81_02241E68(Work*,int,int,int),ov81_02241CEC(Work*);

void ov81_02241C0C(Work*w);

void ov81_02241C0C(Work*w){u32 input=GridInputHandler_GetNextInput(w->input);if(input<6){int count=w->count;u16 i=0;for(;i<count;i++){if(input==w->selected[i])break;}if(count!=0 && i!=count)Pokepic_SetAttr(w->pics[count],6,1);else{ov81_02241E68(w,count,input,0);Pokepic_SetAttr(w->pics[w->count],6,0);}ov81_02241CEC(w);}}
