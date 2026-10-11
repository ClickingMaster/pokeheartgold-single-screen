#pragma force_active on
#include "global.h"
struct OverlayManager;
#include <string.h>
#pragma force_active on
typedef struct Payload {u16 kind,pad;u32 id;u8 rest[0x350];} Payload;
typedef struct State {void *bg,*strings;u8 pad08[0x10],w18[0x30],w48[0x10],w58[0x10],pad68[0x18];int choice,mode;u32 pad88;u8 message[0x4e],flags,paddb;Payload payload;int counter,selection,timer,result;u8 pad444[0x184];int error;u8 pad5cc[0x100c];int *states;int ticks;u8 pad15e0[0x1408];void (*callback)(struct State *);int flag;u32 pad29f0;int busy;} State;
typedef struct Work {u8 pad0[12];void *value;u8 pad10[0x3090];int *status;} Work;
extern State *OverlayManager_GetData(void *);
extern BOOL CTRDG_IsExisting(void);
extern void OS_ResetSystem(u32);
extern void ov74_0222CE10(State *),ov74_022352A0(int),ov74_0222AB70(void *,State *),ov74_02229F04(State *),ov74_0223539C(int,int,int *,int),ov74_022353FC(int *),ov74_0222AB0C(void *,int *,int (*)(void *)),ov74_0222AC1C(void *,int *),ov74_0222CEE0(State *),ov74_02229E28(State *,int),ov74_02229F28(State *,int);
extern int ov74_0222A6C0(void *),ov74_0222A5AC(void *),ov74_0222A2A4(void *),ov74_0222A538(void *),ov74_0222A494(void *),ov74_0222AD6C(void *);
extern u32 ov74_0223D0A8[];
extern void *ov74_0222A078(void *);
extern int ov74_0222FD98(void *,void *),WindowIsInUse(void *),ov74_0222A0E4(void *);
extern void AddWindowParameterized(void *,void *,u8,u8,u8,u8,u8,u8,u16);
extern void *ov74_02236988(void);
extern void ov74_0222AAAC(void *,void *,void *),ov74_0222AA18(void *,void *,int),ov74_0222A43C(void *),ov74_0222CEC0(void),ov74_0222A174(void *),ov74_02229F60(void *,int),ov74_02235DC4(void *,void *),ov74_02229F78(void *,int),BgClearTilemapBufferAndCommit(void *,int);
extern void *Heap_Alloc(int,u32),*ov74_02235708(void);
extern void ov74_0222C04C(void *,void *);
extern void *SysTask_CreateOnMainQueue(void (*)(void *,void *),void *,u32);
extern int ov74_0222ADBC(void *,void *,int,...),ov74_0223615C(void),ov74_02229D6C(State *),ov74_02229DBC(void);
extern void ov74_02236128(void),ov74_02236140(void),PlaySE(int),GfGfx_EngineATogglePlanes(int,int),GfGfx_EngineBTogglePlanes(int,int),ov74_02229DF8(void),WirelessStatusIcon_Destroy(void),sub_020398D4(int,int),sub_02037AC0(int),ov74_0222A240(void *,int),ov74_0222A94C(void *,int,int),ov74_02235258(void *,int),ov74_0222AE3C(State *),ov74_02229E68(struct OverlayManager *),ov74_02235390(int),ov74_0222A7A0(void *),ov74_0222FC50(void *,void *,int),ov74_0222EC08(void *,void *,int),ov74_022358C8(int);
extern int Comm_GetLocalNetId(void),Comm_IsNetIdConnected(u16),sub_02037B38(int);
extern u32 gSystem[];
int ov74_0222C2EC(void *,int *);
#define ADDWINDOW() AddWindowParameterized(s->bg,s->w58,0,3,2,26,4,0,452)
#define CANCEL() do{ov74_02229DF8();WirelessStatusIcon_Destroy();ov74_02229F28(s,-1);ov74_02229E28(s,0);}while(0)
#define CLEAR() do{ov74_02236140();s->flags&=~4;ov74_02229E28(s,0);*state=ov74_0222ADBC(manager,s->w18,27,56);}while(0)
int ov74_0222C2EC(void *manager,int *state){State *s=OverlayManager_GetData(manager);void *p;int n;CTRDG_IsExisting();if(s->error!=29)ov74_0222CE10(s);
 switch(*state){
 case 0:ov74_022352A0(84);s->states=state;*state=1;break;
 case 1:ov74_0222AB70(manager,s);ov74_02229F04(s);ov74_0223539C(1,3,state,2);break;
 case 2:ov74_022353FC(state);break;
 case 3:ov74_0222AB0C(manager,state,ov74_0222A6C0);break;
 case 4:ov74_0222AB0C(manager,state,ov74_0222A5AC);break;
 case 5:ov74_0222AB0C(manager,state,ov74_0222A2A4);break;
 case 27:ov74_0222AC1C(manager,state);break;
 case 32:ov74_0222CEE0(s);if(ov74_0223D0A8[4]==45){ov74_02229E28(s,0);ov74_0222A078(manager);s->choice=ov74_0222FD98(s->strings,s->message);if(s->choice==1){ov74_02229E28(s,0);*state=49;}else{if(!WindowIsInUse(s->w58))ADDWINDOW();ov74_0222AAAC(manager,s->w58,ov74_02236988());ov74_0222AA18(manager,s->w18,4);ov74_0222A43C(manager);*state=31;}}if((u32)(ov74_0223D0A8[4]-46)<=1){ov74_0222CEC0();*state=17;}break;
 case 31:ov74_0222AB0C(manager,state,ov74_0222A538);ov74_0222CEE0(s);break;
 case 33:p=ov74_0222A078(manager);ov74_02229E28(s,1);ov74_02229F28(s,1);if(ov74_0222A0E4(p)==1){Work *w;ov74_0222A174(manager);s->busy=1;ov74_02229F28(s,-1);ov74_02229F60(s->w58,0);w=Heap_Alloc(84,0x30a4);memset(w,0,0x30a4);w->status=&s->busy;w->value=ov74_02235708();SysTask_CreateOnMainQueue(ov74_0222C04C,w,5);}else{ov74_02235DC4(s->bg,p);ov74_0222A174(manager);}*state=34;break;
 case 35:if(!s->busy){*state=ov74_0222ADBC(manager,s->w18,8,56);GfGfx_EngineATogglePlanes(1,1);}break;
 case 34:n=ov74_0223615C();if(n==4)ov74_02236128();if(n==2){ov74_02229E28(s,0);ov74_02229F28(s,-1);ov74_02229F78(s->w18,0);BgClearTilemapBufferAndCommit(s->bg,0);GfGfx_EngineATogglePlanes(1,0);p=&s->payload;if(ov74_0222A0E4(p)==1){ov74_02235DC4(s->bg,p);s->busy=2;}else PlaySE(1571);*state=35;}else if(n==3){ov74_02229E28(s,0);ov74_02229F28(s,-1);s->flags&=~4;*state=ov74_0222ADBC(manager,s->w18,27,56);}break;
 case 36:ov74_0222CEE0(s);n=ov74_0223D0A8[4];if((u32)(n-45)<=2){if(n==47)ov74_0222CEC0();*state=ov74_0222A494(manager);}break;
 case 7:s->selection=ov74_02229D6C(s);if(s->selection!=-1){ov74_02229E28(s,0);s->choice=ov74_0222FD98(s->strings,s->message);if(!WindowIsInUse(s->w58))ADDWINDOW();ov74_0222AAAC(manager,s->w58,s->message);ov74_0222AA18(manager,s->w18,4);ov74_0222A240(manager,0);*state=8;}if((gSystem[18]&2)||--s->counter==0){ov74_02229E28(s,0);ov74_02229DF8();WirelessStatusIcon_Destroy();*state=17;}break;
 case 8:ov74_0222AB0C(manager,state,ov74_0222A494);break;
 case 9:n=Comm_GetLocalNetId();if(n&&Comm_IsNetIdConnected((u16)n)){if(sub_02037B38(171)==1){ov74_02229E28(s,0);sub_020398D4(1,1);ov74_0222AA18(manager,s->w18,7);ov74_02229F28(s,1);ov74_02229E28(s,1);s->counter=600;*state=10;}}else if(n&&!Comm_IsNetIdConnected((u16)n)){CANCEL();*state=16;break;}if((gSystem[18]&2)||--s->counter==0){CANCEL();*state=17;}break;
 case 10:if(ov74_02229DBC())*state=11;n=Comm_GetLocalNetId();if((gSystem[18]&2)||--s->counter==0||(n&&!Comm_IsNetIdConnected((u16)n))){ov74_02229E28(s,0);ov74_02229DF8();WirelessStatusIcon_Destroy();*state=17;}break;
 case 11:p=ov74_0222A078(manager);ov74_02235DC4(s->bg,p);sub_020398D4(0,0);ov74_0222A174(manager);*state=12;break;
 case 12:n=Comm_GetLocalNetId();if(n&&!Comm_IsNetIdConnected((u16)n)){ov74_02229DF8();s->flag=1;ov74_02236140();ov74_02229E28(s,0);WirelessStatusIcon_Destroy();ov74_02229F28(s,-1);*state=14;}else if(ov74_0223615C()==4){sub_02037AC0(147);*state=13;s->timer=120;}else if(ov74_0223615C()==3){CLEAR();}break;
 case 13:if(sub_02037B38(147)==1){ov74_02236128();s->timer=10;*state=15;}else if(--s->timer==0){CLEAR();}break;
 case 14:ov74_02229F60(s->w58,0);s->flags&=~4;*state=ov74_0222ADBC(manager,s->w18,29,56);break;
 case 15:if(--s->timer==0){ov74_02229E28(s,0);ov74_02229DF8();WirelessStatusIcon_Destroy();PlaySE(1571);ov74_02229F28(s,-1);*state=ov74_0222ADBC(manager,s->w18,8,56);}break;
 case 16:PlaySE(1500);ov74_0222AA18(manager,s->w18,25);ov74_02229F60(s->w58,0);*state=18;break;
 case 17:if(s->ticks)s->ticks--;else{ov74_02229E28(s,0);PlaySE(1500);ov74_0222AA18(manager,s->w18,24);ov74_02229F60(s->w58,0);*state=18;}break;
 case 18:if(gSystem[18]){PlaySE(1500);ov74_0222A94C(manager,196,0);*state=3;}break;
 case 19:if(s->ticks)s->ticks--;else{if(s->mode!=27)ov74_02235258(s->message,936);s->choice=ov74_0222FD98(s->strings,s->message);if(s->choice==1){ov74_02229E28(s,0);*state=49;}else{if(!WindowIsInUse(s->w58))ADDWINDOW();ov74_0222AAAC(manager,s->w58,s->message);ov74_0222AA18(manager,s->w18,4);ov74_0222A240(manager,0);*state=20;}}break;
 case 20:ov74_0222AB0C(manager,state,ov74_0222A494);break;
 case 21:*state=22;break;
 case 22:p=ov74_0222A078(manager);ov74_02235DC4(s->bg,p);ov74_0222AA18(manager,s->w18,7);ov74_02229F28(s,1);ov74_02229E28(s,1);s->timer=60;*state=23;break;
 case 23:if(--s->timer==0){ov74_0222A174(manager);*state=24;}break;
 case 24:if(ov74_0223615C()==4)ov74_02236128();if(ov74_0223615C()==2){s->timer=1;ov74_02229E28(s,0);ov74_02229F28(s,-1);PlaySE(1571);*state=ov74_0222ADBC(manager,s->w18,8,25);}else if(ov74_0223615C()==3){ov74_02229F28(s,-1);s->flags&=~4;*state=ov74_0222ADBC(manager,s->w18,27,56);}break;
 case 25:if(--s->timer==0){s->timer=65536;*state=56;}break;
 case 49:if(s->choice!=5)ov74_0222AE3C(s);*state=ov74_0222AD6C(manager);ov74_02229F60(s->w48,0);break;
 case 50:*state=ov74_0222ADBC(manager,NULL,0,*state);break;
 case 51:if(gSystem[18]){ov74_02229F60(s->w58,0);*state=ov74_0222A5AC(manager);}break;
 case 52:ov74_0222A240(manager,1);*state=8;s->choice=0;break;
 case 53:ov74_02229E68(manager);s->result=0;return 1;
 case 54:ov74_02229E68(manager);s->result=1;return 1;
 case 55:ov74_02229E68(manager);s->result=2;return 1;
 case 56:if(gSystem[18]){PlaySE(1500);if((((u32)s->flags<<29)>>31)==1)ov74_0223539C(0,57,s->states,2);else{ov74_02235390(1);ov74_0223539C(0,58,s->states,2);}}break;
 case 57:ov74_0222A7A0(s->bg);GfGfx_EngineBTogglePlanes(1,0);GfGfx_EngineBTogglePlanes(2,1);if((((u32)s->flags<<29)>>31)==1&&s->payload.kind==3&&s->payload.id==533)ov74_0222FC50(s->bg,&s->payload,84);else ov74_0222EC08(s->bg,&s->payload,84);ov74_0223539C(1,56,s->states,2);s->flags&=~4;break;
 case 58:OS_ResetSystem(0);break;
 default:break;
 }
 if(s->callback)s->callback(s);ov74_022358C8(s->busy);return 0;
}
