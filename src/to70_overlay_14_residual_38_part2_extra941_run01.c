#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct {u8 p[0x2c];void *input;u8 p2[0x2c0];void *widget;} Data;typedef struct {u8 p[0x34];Data *data;} Work;
extern u32 GridInputHandler_GetNextInput(void *);extern int ov14_021E7588(Work *,u32);extern void ov14_021E8620(void *),ov14_021E765C(Work *),ov14_021F1F24(Work *),ov14_021F0234(Work *,void *,int);extern u8 ov14_021EAC24[];void ov14_021F1C04(Work *);
void ov14_021F1C04(Work *w){u32 input=GridInputHandler_GetNextInput(w->data->input);if(input<36){if(ov14_021E7588(w,input)==1)ov14_021E8620(w->data->widget);}else ov14_021E765C(w);ov14_021F1F24(w);ov14_021F0234(w,ov14_021EAC24,134);}
