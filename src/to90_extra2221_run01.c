#pragma force_active on
#include "global.h"
#include "system.h"
#pragma force_active on
typedef struct {u8 pad[0x94];u8 timer;u8 pad2[2];u8 cancel:1;u8 unused:7;u8 pad3[8];u16 *result;u8 pad4[12];void *menu;} Work;
extern int IsPaletteFadeFinished(void),Handle2dMenuInput(void *),System_GetTouchNew(void),Get2dMenuSelection(void *);
extern void ov80_0222E8FC(Work *),ov80_0222E88C(Work *);
void ov80_0222E7C8(void *,Work *);
void ov80_0222E7C8(void *task,Work *w){
u32 choice;if(w->timer){w->timer--;return;}
if(IsPaletteFadeFinished()){
choice=Handle2dMenuInput(w->menu);if(choice==-1&&System_GetTouchNew()==1)choice=Get2dMenuSelection(w->menu);
if((gSystem.newAndRepeatedKeys&64)||(gSystem.newAndRepeatedKeys&128)||(gSystem.newAndRepeatedKeys&32)||(gSystem.newAndRepeatedKeys&16))ov80_0222E8FC(w);
if(*w->result==0xeedd){ov80_0222E88C(w);return;}
switch(choice){case -2:if(w->cancel==1){*w->result=0xfffe;ov80_0222E88C(w);}break;case -1:break;default:*w->result=choice;ov80_0222E88C(w);break;}
}
}
