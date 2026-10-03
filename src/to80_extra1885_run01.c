#pragma force_active on
#pragma opt_propagation off
#pragma force_active on
#include "global.h"
typedef struct{void *bg;int layer,tile;u32 palette;u8 x,y;u8 low:4;u8 high:4;u8 last;} Template;
extern void YesNoPrompt_InitFromTemplate(void *,Template *);
void ov82_0223FDC8(void *,void *,u8);
void ov82_0223FDC8(void *prompt,void *bg,u8 arg){Template t;MI_CpuFill8(&t,0,20);{int zero=0;t.tile=109;t.palette=14;t.bg=bg;t.layer=zero;t.x=24;t.y=10;t.low=arg;t.high=0;t.last=zero;}YesNoPrompt_InitFromTemplate(prompt,&t);}
