#pragma force_active on
#include "global.h"
typedef struct{u8 pad[8];int delay;void*str;u8 window[16];void*prompt;u8 rest[8];}Work;
extern void*Save_PlayerData_GetOptionsAddr(void*),*String_New(int,int),*YesNoPrompt_Create(int);extern int Options_GetTextFrameDelay(void*);extern void AddWindowParameterized(void*,void*,int,int,int,int,int,int,int),FillWindowPixelBuffer(void*,int),ov48_0225B0D4(void*,int,void*);
void ov48_02259EAC(Work*,void**,void*,void*,int);
void ov48_02259EAC(Work*w,void**bg,void*msg,void*save,int heap){memset(w,0,44);w->delay=Options_GetTextFrameDelay(Save_PlayerData_GetOptionsAddr(save));w->str=String_New(128,heap);AddWindowParameterized(*bg,w->window,1,2,1,27,4,1,52);FillWindowPixelBuffer(w->window,15);ov48_0225B0D4(msg,5,w->str);w->prompt=YesNoPrompt_Create(heap);}
