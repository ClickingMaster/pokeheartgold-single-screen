#pragma force_active on
#include "global.h"
#include "yes_no_prompt.h"
#pragma force_active on
typedef struct W{u8 p[8];u8 state,choice;u8 pa[6];u8 printer;u8 p11[0x3b];void*arg;u8 p50[0x41c];YesNoPrompt*prompt;}W;
extern u8 ov81_0224086C(W*,int);extern void ov81_02241BD0(YesNoPrompt*,void*),ov81_02242694(W*,int),ov81_0223F754(W*),ov81_02242E08(void*,int);extern int ov80_02237254(u8);int ov81_0223F6A8(W*);
int ov81_0223F6A8(W*w){switch(w->state){case 0:w->printer=ov81_0224086C(w,10);ov81_02241BD0(w->prompt,w->arg);ov81_02242694(w,1);w->state++;break;case 1:switch(YesNoPrompt_HandleInput(w->prompt)){case 1:YesNoPrompt_Reset(w->prompt);ov81_02242694(w,0);if(ov80_02237254(w->choice)==1)w->printer=ov81_0224086C(w,2);w->state++;break;case 2:YesNoPrompt_Reset(w->prompt);ov81_02242694(w,0);ov81_0223F754(w);ov81_02242E08(*(void**)((u8*)w+0x388),0);w->state++;break;}break;case 2:return 1;}return 0;}
