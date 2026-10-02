#pragma force_active on
#include "global.h"
typedef struct {u8 pad[0x438];void*bg;u8 gap[0x454-0x43c];u8 windows[12][16];void*prompt;} Work;
typedef struct{u8 data[8];} Template;extern Template ov108_021EAC18[];
extern void AddWindow(void*,void*,const Template*);extern void FillWindowPixelBuffer(void*,int);extern void AddWindowParameterized(void*,void*,int,u8,u8,int,int,int,u16);extern void*YesNoPrompt_Create(int);

void ov108_021E9D30(Work*w);

void ov108_021E9D30(Work*w){int i;u16 base;for(i=0;i<=5;i++){AddWindow(w->bg,w->windows[i],&ov108_021EAC18[i]);FillWindowPixelBuffer(w->windows[i],0);}base=0x2e2;for(i=0;i<6;i++){AddWindowParameterized(w->bg,w->windows[i+6],5,(u8)(1+(i%2)*16),(u8)(6+(i/2)*5),14,2,4,base);base-=28;}w->prompt=YesNoPrompt_Create(*(int*)w);}
