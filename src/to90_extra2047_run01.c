#pragma force_active on
#include "global.h"
typedef struct {u8 p[8];u8 state,mode;u8 q[6];u8 printer,count;u8 r[7];u8 wait;u8 s[0x32];void *field;u8 t[0x338];void *left;u8 u[4];void *right;u8 v[0x34];u16 entries[1];u8 w[0xa2];void *prompt;} Context;
extern s32 ov80_02236DD4(u8),ov80_02237254(u8),ov81_0224086C(Context *,s32),ov81_02240FA4(Context *,s32,s32),YesNoPrompt_HandleInput(void *),sub_02037B38(s32);
extern void ov81_02242D88(void *,s32),ov81_022408A0(Context *,s32,s32),ov81_02241BD0(void *,void *),ov81_02242694(Context *,s32),YesNoPrompt_Reset(void *),sub_02037BEC(void),sub_02037AC0(s32);
s32 ov81_0223F1A4(Context *p);
#pragma force_active on
s32 ov81_0223F1A4(Context *p){
 s32 result=ov80_02236DD4(p->mode);
 switch(p->state){case 0:ov81_02242D88(p->left,0);ov81_02242D88(p->right,0);p->wait=0;p->state++;break;
 case 1:p->wait=0;p->state++;break;case 2:p->state++;break;case 3:p->wait=0;p->state++;break;
 case 4:ov81_022408A0(p,0,result);p->printer=ov81_0224086C(p,1);ov81_02241BD0(p->prompt,p->field);ov81_02242694(p,1);p->wait=0;p->state++;break;
 case 5:result=YesNoPrompt_HandleInput(p->prompt);switch(result){case 1:YesNoPrompt_Reset(p->prompt);ov81_02242694(p,0);p->state++;break;case 2:YesNoPrompt_Reset(p->prompt);ov81_02242694(p,0);ov81_02242D88(p->left,1);ov81_02242D88(p->right,1);p->count--;p->entries[p->count]=0;if(ov80_02237254(p->mode)==1)ov81_02240FA4(p,8,0);return 1;}break;
 case 6:if(ov80_02237254(p->mode)==0)return 1;if(ov81_02240FA4(p,8,0)==1){p->printer=ov81_0224086C(p,2);sub_02037BEC();sub_02037AC0(164);p->state++;}break;
 case 7:if(sub_02037B38(164)==1){sub_02037BEC();return 1;}break;
 }return 0;
}
