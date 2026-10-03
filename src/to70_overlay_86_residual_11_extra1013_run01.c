#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct {u8 p[0x254];void *input;u16 selected,other;} Work;
extern u8 ov86_021E8094[],ov86_021E8104[],ov86_021E7EB8[];extern void *GridInputHandler_Create(void *,void *,void *,void *,int,u8,int);extern void ov86_021E75EC(Work *,int);void ov86_021E7630(Work *);
void ov86_021E7630(Work *w){w->input= GridInputHandler_Create(ov86_021E8094,ov86_021E8104,ov86_021E7EB8,w,1,(u8)w->selected,121);ov86_021E75EC(w,w->selected);}
