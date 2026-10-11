#pragma force_active on
#include "global.h"
#include "system.h"
typedef struct {u8 p[0x24];void*msg;u8 q[0x2e8-0x28];void*prompt;u8 r[0x318-0x2ec];int state;u8 s[0x4a1c-0x31c];int value,pad,blocked;} Work;
extern int ov73_021E746C(Work*),YesNoPrompt_HandleInput(void*);extern void ov73_021E66F0(Work*),PlaySE(int),Comm_SendCommand(int,void*,int),ov73_021E781C(Work*,int),ov73_021E6830(Work*),BufferPlayersName(void*,int,void*);extern void*sub_02034818(int);
int ov73_021E6D54(Work*,int);
int ov73_021E6D54(Work*w,int state){int choice,x;if(w->value!=ov73_021E746C(w)||w->blocked){if(gSystem.newKeys&0xc3)PlaySE(1522);ov73_021E66F0(w);return state;}choice=YesNoPrompt_HandleInput(w->prompt);if(choice){if(choice==2){x=0;w->state=0;Comm_SendCommand(117,&x,1);ov73_021E781C(w,1);}else{w->state=13;Comm_SendCommand(113,0,0);BufferPlayersName(w->msg,0,sub_02034818(0));}ov73_021E6830(w);}ov73_021E66F0(w);return state;}
