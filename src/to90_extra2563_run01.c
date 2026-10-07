#pragma force_active on
#include "global.h"
#include "system.h"
#include "pokemon.h"
#include "party.h"
#include "options.h"
#include "message_format.h"
#include "bg_window.h"
#include "unk_02019BA4.h"
#include "unk_02030A98.h"
#include "yes_no_prompt.h"
#include "touchscreen_list_menu.h"
typedef struct {u32 a,b;} Entry;
typedef struct {u8 pad[0x13];u8 kind;u8 pad2[0x4dc-0x14];Entry *entries;u8 pad3[0x518-0x4e0];u8 manager[0x784-0x518];void *sprites[6];u8 pad4[0x861-0x79c];u8 count;s16 page;} Work;
typedef struct Ov83PromptWrap {void*prompt;u32 active;void*bgConfig;} Ov83PromptWrap;
#pragma force_active on
typedef union Flags {u8 all;struct{u8 low:5,resume:2,high:1;}bits;}Flags;
typedef struct State{u32 unknown;FrontierSave*save;u8 state,facility,printer,unusedb,c,selection;Flags flags;u8 unusedf;u16 value;u8 unused12,action,context,oldSelection;u8 pad16[14];MessageFormat*buffer;u8 pad28[40];Window windows[6];Window messageWindow;u8 remainderC0[0x448];Options*options;SaveData*frontier;u8 pad510[0x268];void*sprite;u8 pad77c[0x28];Party*party;u8 pad7a8[0x5c];Pokemon*mon;u8 pad808[0x30];GridInputHandler*menu;u32 unknown83c;TouchscreenListMenu*list;u32 unknown844;u32 category;Ov83PromptWrap prompt;u8 pad858[4];GridInputHandler*grid;u8 item,itemCount;s16 page;u16 gridChoice;u8 animationPhase,animationTicks;u32 transfer;}State;
typedef struct Cost{u32 minimum,unused1,unused2;}Cost;
extern const Cost ov83_02247F4C[];extern const u16 ov83_02247D18[],ov83_02247D48[],ov83_02247D4E[];
void ov83_02240348(u8*s);
void ov83_02240384(u8*s);
void ov83_022402F4(u8 *p);
void ov83_02240300(u8*s);
void ov83_02240334(u8 *p);
void ov83_02240290(u8 *work);
u32 ov83_0224037C(void *p);
void ov83_02240514(u8 *p);
u32 ov83_022403B8(void *p);
void ov83_02240664(void *p);
void ov83_02240748(Work *w);
void ov83_022407FC(void *p);
void ov83_0224042C(void*);
void ov83_0224175C(u8 *p);
void ov83_02241B18(u8 *p);
void ov83_02242DAC(void *p);
void ov83_02242DFC(void *p);
void ov83_02242E88(void *p);
void ov83_02247630(void *arg0, void *arg1, u32 arg2);
void ov83_02247B04(void *p);
u32 ov83_02242AB4(u8 *p, u32 a, u32 b);
void ov83_022477B0(int value, int seq);
void ov83_02247944(Window *w,int frame);
void ov83_02240C48(u8 *p, int field, int value, int digits, int mode);
void ov83_02241770(u8 *work, void *window);
void ov83_022415F4(u8 *p, int mode, int action);
void ov83_02241BC4(void*w,u8 other,u16 action);
void ov83_02241354(void *window);
void ov83_022477EC(int,int,u32*);
void ov83_022403C0(void *work, void *arg);
void ov83_02242F18(u8 *p, u16 value);
void ov83_022408E0(void *p, u32 item);
void ov83_022416A0(u8 *p, int item, u16 value);
void ov83_02240C60(u8 *p, int field, void *mon);
void ov83_02241208(u8 *work, int delta);
void ov83_02241254(u8 *work, int delta);
int ov83_02247AD4(GridInputHandler *inputHandler);

u8 ov83_0224777C(void*s,int a,int b);
int ov83_0223FD14(void *p, int msg, u8 color);
u32 sub_0205C1F0(u32);
u32 sub_0205C268(u32);
u32 ov83_02247768(u32 unused, u32 value);
int ov83_022412DC(void*m);
BOOL ov80_02237D8C(int mode);
int ov83_02240FAC(u8 *work, u32 selected, u32 action);
BOOL ov83_02247CF0(void);
int ov83_02247BC4(GridInputHandler *inputHandler);
BOOL ov83_02242F2C(u8 *work);

u16 ov83_02240F48(void *work, int index, int mode);

extern void ov80_02237FA4(void*,int,int);
extern void ov83_022478B4(Ov83PromptWrap*);extern BOOL PlaySE(u16);
void StopSE(u16,int);
void ov83_022428A8(void*,u32);

int ov83_0223E14C(void*);
#define MESSAGE(id,next) do{s->printer=ov83_0223FD14(s,id,1);s->state=next;}while(0)
#define FRAME() ov83_02247944(&s->messageWindow,Options_GetFrame(s->options))
#define COST_DIALOG(cost) do{ov83_02240C48((u8*)s,0,cost,3,0);s->printer=ov83_0223FD14(s,0x37,1);ov83_02240514((u8*)s);s->state=11;}while(0)
#define OPEN_CHOICE(first) do{if(first)ov83_02240348((u8*)s);else ov83_02240384((u8*)s);s->state=first?2:8;}while(0)
#define PAGEREFRESH() do{ov83_02240664(s);ov83_02240748((Work*)s);ov83_022407FC(s);ov83_022408E0(s,GridInputHandler_GetNextInput(s->grid));s->state=9;}while(0)
#define SELECTED_COST() ov83_02240EC4(s,s->item,s->action)
#define START_SELECTED() do{ov83_0224042C(s);FRAME();ov80_02237FA4(s->save,s->facility,SELECTED_COST());ov83_02241770((u8*)s,s->windows);ov83_022416A0((u8*)s,s->selection,ov83_02240F48(s,s->item,s->action));s->state=19;}while(0)
#define MULTI_SELECTED() do{s->value=ov83_02240F48(s,s->item,s->action);ov83_0224042C(s);ov83_02241354(&s->messageWindow);s->flags.all|=2;return 1;}while(0)
u16 ov83_02240EC4(void*,int,int);
int ov83_0223E14C(void*work){State*s=work;u32 choice;int amount,index,stat;Pokemon*mon;
 switch(s->state){
 case 0:index=s->flags.bits.resume;if(index==1){ov83_02240348((u8*)s);ov83_02247630(s->sprite,(void*)204,100);s->state=2;s->flags.all&=~0x60;return 0;}if(index==2){ov83_02240384((u8*)s);ov83_02247630(s->sprite,(void*)211,106);s->state=8;s->flags.all&=~0x60;return 0;}
  choice=ov83_02247AD4(s->menu);switch(choice){case 0:case 1:case 2:case 3:ov83_022402F4((u8*)s);ov83_02240300((u8*)s);s->state=1;break;case 4:PlaySE(1500);return 1;case (u32)-2:if(s->selection!=s->oldSelection){ov83_02247B04(s->menu);ov83_02242AB4((u8*)s,4,s->selection);}break;}break;
 case 1:choice=TouchscreenListMenu_HandleInput(s->list);ov83_022477B0(choice,1500);ov83_02242DAC(s);switch(choice){case (u32)-1:break;case 0:ov83_02240334((u8*)s);OPEN_CHOICE(1);break;case 5:ov83_02240334((u8*)s);OPEN_CHOICE(0);break;case 11:case (u32)-2:ov83_02240334((u8*)s);ov83_02240290((u8*)s);s->state=0;break;}break;
 case 2:choice=TouchscreenListMenu_HandleInput(s->list);ov83_022477B0(choice,1500);ov83_02242DFC(s);switch(choice){case (u32)-1:break;case 1:case 2:case 3:s->action=choice;ov83_0224037C(s);FRAME();amount=ov83_0224777C(s->frontier,s->facility,0);if((u32)amount<ov83_02247F4C[s->category].minimum){MESSAGE(33,7);break;}ov83_02240C48((u8*)s,0,ov83_02247D18[s->category],3,0);s->printer=ov83_0223FD14(s,55,1);ov83_02240514((u8*)s);s->state=3;break;
  case 4:amount=ov83_0224777C(s->frontier,s->facility,0);if(amount==3){StopSE(1500,0);PlaySE(1523);break;}s->action=choice;ov83_0224037C(s);index=sub_0205C1F0(s->facility);stat=FrontierSave_GetStat(s->save,index,sub_0205C268(sub_0205C1F0(s->facility)));ov83_02240C48((u8*)s,0,ov83_02247D48[amount],4,0);s->printer=ov83_0223FD14(s,38,1);ov83_02240514((u8*)s);s->state=4;break;
  case 11:case (u32)-2:ov83_0224037C(s);ov83_02240300((u8*)s);s->state=1;break;}break;
 case 3:choice=YesNoPrompt_HandleInput(s->prompt.prompt);switch(choice){case 1:ov83_022478B4(&s->prompt);mon=Party_GetMonByIndex(s->party,ov83_02247768(s->context,s->selection));index=sub_0205C1F0(s->facility);stat=FrontierSave_GetStat(s->save,index,sub_0205C268(sub_0205C1F0(s->facility)));amount=ov83_0224777C(s->frontier,s->facility,0);index=s->category;
  if((u32)amount<ov83_02247F4C[index].minimum){FRAME();MESSAGE(33,7);break;}if((u32)stat<ov83_02247D18[index]){FRAME();MESSAGE(32,7);break;}
  if(index==0){u32 current=GetMonData(mon,163,0);if(current==GetMonData(mon,164,0)){MESSAGE(37,7);break;}}else if(index==1){if(!ov83_022412DC(mon)){MESSAGE(37,7);break;}}else{u32 current=GetMonData(mon,163,0);if(current==GetMonData(mon,164,0)&&!ov83_022412DC(mon)){MESSAGE(37,7);break;}}
  if(!ov80_02237D8C(s->facility)){ov80_02237FA4(s->save,s->facility,ov83_02247D18[s->action-1]);ov83_02241770((u8*)s,s->windows);ov83_022415F4((u8*)s,s->selection,s->action);s->state=19;}else{s->flags.all|=2;return 1;}break;case 2:ov83_022478B4(&s->prompt);OPEN_CHOICE(1);break;}break;
 case 4:choice=YesNoPrompt_HandleInput(s->prompt.prompt);switch(choice){case 1:ov83_022478B4(&s->prompt);index=sub_0205C1F0(s->facility);stat=FrontierSave_GetStat(s->save,index,sub_0205C268(sub_0205C1F0(s->facility)));amount=ov83_0224777C(s->frontier,s->facility,0);if((u32)stat<ov83_02247D48[amount]){FRAME();MESSAGE(41,7);break;}if(!ov80_02237D8C(s->facility)){ov83_02241BC4(s,s->selection,4);s->state=5;}else{s->flags.all|=2;return 1;}break;case 2:ov83_022478B4(&s->prompt);OPEN_CHOICE(1);break;}break;
 case 5:if(ov83_02240FAC((u8*)s,s->selection,s->action)==1)s->state=6;break;
 case 6:if(ov83_02247CF0()==1){PlaySE(1500);ov83_02240348((u8*)s);ov83_02247630(s->sprite,(void*)204,100);s->state=2;}break;
 case 7:if(ov83_02247CF0()==1){PlaySE(1500);ov83_02241354(&s->messageWindow);OPEN_CHOICE(1);}break;
 case 8:ov83_022477EC(2,0,&s->transfer);choice=TouchscreenListMenu_HandleInput(s->list);ov83_022477B0(choice,1500);ov83_02242E88(s);switch(choice){case (u32)-1:break;case (u32)-2:ov83_022403B8(s);ov83_02240300((u8*)s);s->state=1;break;case 6:s->action=choice;ov83_022403B8(s);ov83_022403C0(s,(void*)6);s->state=9;break;
  case 7:s->action=choice;ov83_022403B8(s);if(ov83_0224777C(s->frontier,s->facility,1)==1){MESSAGE(54,15);return 0;}ov83_022403C0(s,(void*)7);s->state=9;break;
  case 8:amount=ov83_0224777C(s->frontier,s->facility,1);if(amount==3){StopSE(1500,0);PlaySE(1523);break;}s->action=choice;ov83_022403B8(s);ov83_02240C48((u8*)s,0,ov83_02247D4E[amount],4,0);s->printer=ov83_0223FD14(s,38,1);ov83_02240514((u8*)s);s->state=12;break;}break;
 case 9:choice=ov83_02247BC4(s->grid);switch(choice){case 0:case 1:case 2:case 3:case 4:case 5:s->item=choice+s->page*6;if(s->item<s->itemCount){PlaySE(1501);ov83_02242F18((u8*)s,(u16)choice);s->state=10;}break;case 6:PlaySE(1504);ov83_02242F18((u8*)s,6);s->state=10;break;case 7:PlaySE(1504);ov83_02242F18((u8*)s,7);s->state=10;break;case (u32)-3:PlaySE(1500);break;case 8:case (u32)-2:PlaySE(1501);ov83_02242F18((u8*)s,8);s->state=10;break;}break;
 case 10:if(ov83_02242F2C((u8*)s)==1)break;choice=s->gridChoice;switch(choice){case 0:case 1:case 2:case 3:case 4:case 5:ov83_022428A8(s,choice);FRAME();COST_DIALOG(SELECTED_COST());break;
  case 6:s->page=(s16)((u32)s->page-1u);if(s->page<0)s->page=(s->itemCount-1)/6;PAGEREFRESH();break;case 7:s->page=(s16)((u32)s->page+1u);if((s->itemCount-1)/6<s->page)s->page=0;PAGEREFRESH();break;case 8:ov83_0224042C(s);OPEN_CHOICE(0);break;}break;
 case 11:choice=YesNoPrompt_HandleInput(s->prompt.prompt);switch(choice){case 1:ov83_022478B4(&s->prompt);index=sub_0205C1F0(s->facility);stat=FrontierSave_GetStat(s->save,index,sub_0205C268(sub_0205C1F0(s->facility)));if((u32)stat<(u32)SELECTED_COST()){FRAME();MESSAGE(32,16);break;}mon=s->mon;if(!GetMonData(mon,6,0)){if(!ov80_02237D8C(s->facility)){START_SELECTED();}else{MULTI_SELECTED();}}else{ov83_02240C60((u8*)s,0,Mon_GetBoxMon(mon));BufferItemNameWithIndefArticle(s->buffer,1,GetMonData(mon,6,0));FRAME();MESSAGE(60,17);}break;case 2:ov83_022478B4(&s->prompt);ov83_0224175C((u8*)s);s->state=9;break;}break;
 case 12:choice=YesNoPrompt_HandleInput(s->prompt.prompt);switch(choice){case 1:ov83_022478B4(&s->prompt);index=sub_0205C1F0(s->facility);stat=FrontierSave_GetStat(s->save,index,sub_0205C268(sub_0205C1F0(s->facility)));amount=ov83_0224777C(s->frontier,s->facility,1);if((u32)stat<ov83_02247D4E[amount]){FRAME();MESSAGE(41,15);break;}if(!ov80_02237D8C(s->facility)){ov83_02241BC4(s,s->selection,8);s->state=13;}else{s->flags.all|=2;return 1;}break;case 2:ov83_022478B4(&s->prompt);OPEN_CHOICE(0);break;}break;
 case 13:if(ov83_02240FAC((u8*)s,s->selection,s->action)==1)s->state=14;break;
 case 14:if(ov83_02247CF0()==1){PlaySE(1500);ov83_02240384((u8*)s);ov83_02247630(s->sprite,(void*)211,106);s->state=8;}break;
 case 15:if(ov83_02247CF0()==1){PlaySE(1500);ov83_02241354(&s->messageWindow);OPEN_CHOICE(0);}break;
 case 16:if(ov83_02247CF0()==1){PlaySE(1500);ov83_0224175C((u8*)s);s->state=9;}break;
 case 17:if(ov83_02247CF0()==1){PlaySE(1500);s->printer=ov83_0223FD14(s,61,1);ov83_02240514((u8*)s);s->state=18;}break;
 case 18:choice=YesNoPrompt_HandleInput(s->prompt.prompt);switch(choice){case 1:ov83_022478B4(&s->prompt);ov83_02240664(s);if(!ov80_02237D8C(s->facility)){START_SELECTED();}else{MULTI_SELECTED();}break;case 2:ov83_022478B4(&s->prompt);ov83_0224175C((u8*)s);s->state=9;break;}break;
 case 19:ov83_022477EC(2,0,&s->transfer);if(ov83_02240FAC((u8*)s,s->selection,s->action)==1)s->state=20;break;
 case 20:if(ov83_02247CF0()==1){PlaySE(1500);ov83_02241354(&s->messageWindow);ov83_02240290((u8*)s);s->state=0;}break;
 case 21:if(gSystem.newKeys&32)ov83_02241208((u8*)s,-1);else if(gSystem.newKeys&16)ov83_02241208((u8*)s,1);else if(ov83_02247CF0()==1){PlaySE(1500);ov83_02241B18((u8*)s);ov83_02240300((u8*)s);s->state=1;}break;
 case 22:if(gSystem.newKeys&32)ov83_02241254((u8*)s,-1);else if(gSystem.newKeys&16)ov83_02241254((u8*)s,1);else if(ov83_02247CF0()==1){PlaySE(1500);ov83_02241B18((u8*)s);ov83_02240300((u8*)s);s->state=1;}break;
 }
 return 0;
}
