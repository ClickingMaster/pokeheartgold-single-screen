#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct {u8 p[0x254];void *input;u16 selected,other;} Work;
extern u8 ov86_021E7F94[],ov86_021E7FE4[],ov86_021E7EA8[];extern void *GridInputHandler_Create(void *,void *,void *,void *,int,u8,int);extern void ov86_021E781C(Work *,int);void ov86_021E7860(Work *);
void ov86_021E7860(Work *w){int value=w->other;w->input= GridInputHandler_Create(ov86_021E7F94,ov86_021E7FE4,ov86_021E7EA8,w,1,(u8)value,121);ov86_021E781C(w,value);}
