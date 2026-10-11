#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct {u8 p[0x2c];void *input;u8 p2[0x2c0];void *widget;} Data;typedef struct {u8 p[0x34];Data *data;} Work;
extern u32 GridInputHandler_GetNextInput(void *);extern int PCBox_ShowSlotMonInfo(Work *,u32);extern void PCBox_SlideInSummaryButton(void *),PCBox_ClearMonInfo(Work *),ov14_021F1F24(Work *),PCBox_WaitForUpdateFunc(Work *,void *,int);extern u8 ov14_021EAC24[];void ov14_021F1C04(Work *);
void ov14_021F1C04(Work *w){u32 input=GridInputHandler_GetNextInput(w->data->input);if(input<36){if(PCBox_ShowSlotMonInfo(w,input)==1)PCBox_SlideInSummaryButton(w->data->widget);}else PCBox_ClearMonInfo(w);ov14_021F1F24(w);PCBox_WaitForUpdateFunc(w,ov14_021EAC24,134);}
