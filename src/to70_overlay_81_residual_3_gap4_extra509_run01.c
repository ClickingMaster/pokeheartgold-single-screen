#pragma force_active on
#include "global.h"
typedef struct {u8 pad[8];u8 state,kind;u8 pad2[6];u8 text;u8 pad3[2];u8 flags;u8 pad4[0x38];void *data;u8 pad5[0x46c-0x50];void *prompt;u8 pad6[0x388-0x470+0x1000];} Work;
extern int ov81_0224086C(Work *,int);extern void ov81_02241BD0(void *,void *);extern void ov81_02242694(Work *,int);extern int YesNoPrompt_HandleInput(void *);extern void YesNoPrompt_Reset(void *);extern int ov80_02237254(int);extern void ov81_0223FC60(Work *);extern void ov81_02242E08(void *,int);
int ov81_0223FBAC(Work *);
int ov81_0223FBAC(Work *w){switch(w->state){case 0:w->text=ov81_0224086C(w,10);ov81_02241BD0(w->prompt,w->data);ov81_02242694(w,1);w->state++;break;case 1:switch(YesNoPrompt_HandleInput(w->prompt)){case 1:YesNoPrompt_Reset(w->prompt);ov81_02242694(w,0);w->flags&=~8;if(ov80_02237254(w->kind)==1)w->text=ov81_0224086C(w,2);w->state++;break;case 2:YesNoPrompt_Reset(w->prompt);ov81_02242694(w,0);ov81_0223FC60(w);ov81_02242E08(*(void **)((u8 *)w+0x388),0);w->state++;break;}break;case 2:return 1;}return 0;}
