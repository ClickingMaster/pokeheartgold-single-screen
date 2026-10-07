#pragma force_active on
#include "global.h"
#include "system.h"
typedef struct {void *x0,*frontier;u8 state,facility,printer,xb,xc,selected,level;u8 low:3;u8 mode:5;u8 result,x11,apply,option,page,original;u8 pad16[0x2a2];void *options,*facilityData;u8 pad2c0[0x280];void *object;u8 pad544[8];u8 *blocked,*upgrades,*items3,*items4;void *party;u8 pad560[0x60];void *box;u8 pad5c4[0x2c];void *menu;u32 x5f4;void *touch;u8 pad5fc[8];void *prompt;}Work;
extern void ov83_02245094(void *),ov83_02244BEC(Work *),ov83_02244C9C(Work *),ov83_02247630(void *,int,int),ov83_02244C4C(Work *),ov83_02244C58(Work *),ov83_02247B04(void *),ov83_022469D8(Work *,int,int),ov83_022477B0(u32,int),ov83_02246CC0(Work *),ov83_02244C88(Work *),ov83_022453C0(Work *),ov83_02244A98(Work *,int,int,int,int),ov83_02244A74(Work *),ov83_02244AB0(Work *,int,void *),ov83_02244A88(Work *),ov83_022478B4(void **),ov83_02244A90(Work *),ov83_02245554(Work *,int),ov83_02244CCC(Work *),ov83_02246D40(Work *),ov83_02247944(void *,int),ov83_02246114(Work *,int),ov83_022448E4(Work *,void *),ov83_022459A0(Work *),ov83_02246988(Work *),ov83_022453DC(void *, void *),ov83_02245748(Work *,int),ov83_02245838(void *, int, int);
extern void ov83_02245ACC(void *,u8,int);
extern u32 ov83_02247AD4(void *),TouchscreenListMenu_HandleInput(void *);
extern int ov83_02247768(int,int),ov83_022448AC(Work *,int,int),YesNoPrompt_HandleInput(void *),sub_0205C1F0(int),sub_0205C268(int),ov80_02237D8C(int),ov83_02245068(int),ov83_0224777C(void *,int,int),Options_GetFrame(void *),ov83_02245A40(Work *,int,int),ov83_02247CF0(void),ov83_02244E24(Work *,int,int);
extern u32 FrontierSave_GetStat(void *,int,int);
extern void *Party_GetMonByIndex(void *,int),*Mon_GetBoxMon(void *);
extern void ov80_02237FA4(void *,int,int),PlaySE(int);
int ov83_022433F8(Work *);
#define WINDOW ((u8 *)w+0xc0)
#define GETSTAT() FrontierSave_GetStat(w->frontier,sub_0205C1F0(w->facility),sub_0205C268(sub_0205C1F0(w->facility)))
#define TEXT(n) w->printer=ov83_022448AC(w,(n),1)
int ov83_022433F8(Work *w) {
 u32 input,stat;void *mon;
 switch(w->state){
 case 0:w->xb=0;w->state=1;if(w->mode==1){ov83_02245094(WINDOW);ov83_02244BEC(w);}else if(w->mode==2){ov83_02245094(WINDOW);ov83_02244C9C(w);ov83_02247630(w->object,200,105);w->state=6;}else if(w->mode==3)w->state=14;w->mode=0;break;
 case 1:switch(ov83_02247AD4(w->menu)){case 0:case 1:case 2:case 3:ov83_02244C4C(w);ov83_02244C58(w);w->state=2;break;case 4:PlaySE(1500);return 1;case (u32)-2:if(w->selected!=w->original){ov83_02247B04(w->menu);ov83_022469D8(w,4,w->selected);}break;}break;
 case 2:input=TouchscreenListMenu_HandleInput(w->touch);ov83_022477B0(input,1500);ov83_02246CC0(w);switch(input){
 case 0:w->option=input;if(w->blocked[ov83_02247768(w->page,w->selected)]==0){ov83_02244C88(w);ov83_022453C0(w);ov83_02244A98(w,0,1,4,0);TEXT(16);ov83_02244A74(w);w->state=3;}else{ov83_02244C88(w);mon=Party_GetMonByIndex(w->party,ov83_02247768(w->page,w->selected));ov83_022453C0(w);ov83_02244AB0(w,0,Mon_GetBoxMon(mon));TEXT(20);w->state=16;}break;
 case 1:w->option=input;ov83_02244C88(w);ov83_022453C0(w);ov83_02244AB0(w,0,w->box);TEXT(21);ov83_02244A88(w);w->state=4;break;
 case 2:ov83_02244C88(w);ov83_02244C9C(w);w->state=6;break;
 case 6:case (u32)-2:ov83_02244C88(w);ov83_02244BEC(w);w->state=0;break;
 case 3:case 4:case 5:case (u32)-1:break;
 }break;
 case 3:switch(YesNoPrompt_HandleInput(w->prompt)){
 case 1:ov83_022478B4(&w->prompt);ov83_02245094(WINDOW);stat=GETSTAT();if(stat<1){ov83_022453C0(w);TEXT(28);w->state=16;}else{if(ov80_02237D8C(w->facility)==0){ov80_02237FA4(w->frontier,w->facility,1);ov83_022453DC(w,(u8 *)w+0x50);ov83_02245748(w,w->selected);ov83_02246988(w);ov83_02246114(w,0);w->state=11;}else{w->result=1;return 1;}}break;
 case 2:ov83_022478B4(&w->prompt);ov83_02245094(WINDOW);ov83_02244C58(w);w->state=2;break;
 }break;
 case 4:input=TouchscreenListMenu_HandleInput(w->touch);ov83_022477B0(input,1500);switch(input){
 case 2:case (u32)-2:ov83_02245094(WINDOW);ov83_02244A90(w);ov83_02244C58(w);w->state=2;break;
 case 0:GETSTAT();ov83_02244A90(w);if(w->upgrades[ov83_02247768(w->page,w->selected)]==1){ov83_022453C0(w);TEXT(29);w->state=16;}else{ov83_02245554(w,1);w->state=5;}break;
 case 1:GETSTAT();ov83_02244A90(w);if(w->upgrades[ov83_02247768(w->page,w->selected)]==2){ov83_022453C0(w);TEXT(30);w->state=16;}else{ov83_02245554(w,2);w->state=5;}break;
 case (u32)-1:break;
 }break;
 case 5:switch(YesNoPrompt_HandleInput(w->prompt)){
 case 1:ov83_022478B4(&w->prompt);stat=GETSTAT();if(stat<(u32)ov83_02245068(w->level)){ov83_022453C0(w);TEXT(28);w->state=16;return 0;}w->apply=w->level;if(ov80_02237D8C(w->facility)==0){ov83_02245094(WINDOW);ov80_02237FA4(w->frontier,w->facility,ov83_02245068(w->level));ov83_022453DC(w,(u8 *)w+0x50);ov83_02245838(w,w->selected,w->level);w->state=12;}else{w->result=1;return 1;}break;
 case 2:ov83_022478B4(&w->prompt);ov83_02245094(WINDOW);ov83_02244C58(w);w->state=2;break;
 }break;
 case 6:input=TouchscreenListMenu_HandleInput(w->touch);ov83_022477B0(input,1500);ov83_02246D40(w);switch(input){
 case (u32)-2:ov83_02245094(WINDOW);ov83_02244CCC(w);ov83_02244C58(w);w->state=2;break;
 case 3:w->option=input;ov83_02244CCC(w);if(w->items3[ov83_02247768(w->page,w->selected)]==0){ov83_022453C0(w);ov83_02244A98(w,0,2,4,0);TEXT(43);ov83_02244A74(w);w->state=7;}else w->state=17;break;
 case 4:w->option=input;ov83_02244CCC(w);if(ov83_0224777C(w->facilityData,w->facility,2)==1){TEXT(42);w->state=15;return 0;}if(w->items4[ov83_02247768(w->page,w->selected)]==0){ov83_022453C0(w);ov83_02244A98(w,0,5,4,0);TEXT(79);ov83_02244A74(w);w->state=8;}else w->state=18;break;
 case 5:if(ov83_0224777C(w->facilityData,w->facility,2)==2){ov83_02245094(WINDOW);ov83_02244CCC(w);ov83_02244C58(w);w->state=2;}else{w->option=input;ov83_02244CCC(w);GETSTAT();ov83_02244A98(w,0,50,4,0);TEXT(91);ov83_02244A74(w);w->state=9;}break;
 case (u32)-1:break;
 }break;
 case 7:switch(YesNoPrompt_HandleInput(w->prompt)){case 1:ov83_022478B4(&w->prompt);if(ov83_02245A40(w,2,46)==1)return 1;break;case 2:ov83_022478B4(&w->prompt);ov83_02244C9C(w);w->state=6;break;}break;
 case 8:switch(YesNoPrompt_HandleInput(w->prompt)){case 1:ov83_022478B4(&w->prompt);if(ov83_02245A40(w,5,82)==1)return 1;break;case 2:ov83_022478B4(&w->prompt);ov83_02244C9C(w);w->state=6;break;}break;
 case 9:switch(YesNoPrompt_HandleInput(w->prompt)){case 1:ov83_022478B4(&w->prompt);stat=GETSTAT();ov83_0224777C(w->facilityData,w->facility,2);if(stat<50){ov83_02247944(WINDOW,Options_GetFrame(w->options));TEXT(82);w->state=15;}else if(ov80_02237D8C(w->facility)==0){ov83_02245ACC(w,w->selected,5);w->state=10;}else{w->result=1;return 1;}break;case 2:ov83_022478B4(&w->prompt);ov83_02244C9C(w);w->state=6;break;}break;
 case 10:if(ov83_02247CF0()==1){ov83_02244C9C(w);ov83_02246114(w,0);ov83_02247630(w->object,200,105);w->state=6;}break;
 case 11:ov83_022448E4(w,(u8 *)w+0x80);w->state=12;
 case 12:if(ov83_02244E24(w,w->selected,w->option)==1)w->state=16;break;
 case 13:if(ov83_02244E24(w,w->selected,w->option)==1)w->state=14;break;
 case 14:if(w->option==3)w->state=17;else w->state=18;break;
 case 15:if(ov83_02247CF0()==1){PlaySE(1500);ov83_02244C9C(w);w->state=6;}break;
 case 16:if(ov83_02247CF0()==1){PlaySE(1500);ov83_02245094(WINDOW);ov83_02244BEC(w);w->state=0;}break;
 case 17:if(gSystem.newKeys&32)break;if(gSystem.newKeys&16)break;if(ov83_02247CF0()==1){PlaySE(1500);ov83_022459A0(w);ov83_02244C58(w);w->state=2;}break;
 case 18:if(gSystem.newKeys&32)break;if(gSystem.newKeys&16)break;if(ov83_02247CF0()==1){PlaySE(1500);ov83_022459A0(w);ov83_02244C58(w);w->state=2;}break;
 }return 0;
}
