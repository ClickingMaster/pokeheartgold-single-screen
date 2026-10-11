#pragma force_active on
#include "global.h"
#include "system.h"
typedef struct {int id;u16 next,choice,timer;u8 state,ticks;void*mon;u8 tail[8];} C;
extern void *WiFiPlazaRoom_GetObjManager(void*),*WiFiPlazaRoom_GetModels(void*),*ov49_0225A008(void*),*WiFiPlazaRoom_GetLobby(void*),*ov49_0225EF84(void*),*ov49_0225EF40(void*,int),*WiFiPlazaObj_FindById(void*,int),*ov49_0225A010(void*);
extern int ov49_0225EF88(void*),ov49_0225A040(void*),ov45_0222A374(void*),ov45_0222A330(void*),ov45_0222ADB8(void*,int,int),ov49_022611F4(C*,void*),ov49_02261234(C*,void*,void*),ov49_022613AC(C*,void*),WiFiPlazaObj_GetParam(void*,int),WiFiPlazaRoom_IsMessageFinished(void*);
extern void* WiFiPlazaRoom_GetMsgString(void*,int,int);
extern void ov49_0225EF8C(void*,int),ov49_0225A09C(void*,void*),ov49_0225A0BC(void*),ov45_0222ADC8(void*,int),ov49_0225A0CC(void*),ov45_0222AE44(void*),ov45_0222A5E8(void*,int),ov49_0225A044(void*),WiFiPlazaCamera_ClearFollowObject(void*),ov49_02259130(void*,int),ov49_0225916C(void*,int),PlaySE(int),ov49_0225A510(void*),WiFiPlazaObj_SetDirection(void*,int),ov49_022591B4(void*,int),ov49_02261434(C*,void*,void*),ov49_022611D4(C*,void*),ov49_0225A53C(void*,int),WiFiPlazaObj_GetPosition(void*,int*),ov49_02261460(C*,void*,void*),ov49_02258EAC(void*,void*,int,int),WiFiPlazaRoom_PrintMessage(void*,void*),WiFiPlazaRoom_CloseMessage(void*),ov49_0225A034(void*,int),ov49_0225A038(void*,int),WiFiPlazaObj_SetHandler(void *, void *, int),ov49_0225EF68(void*),ov49_0225EF98(void*,int,const void*,int);
extern const u8 ov49_02269B38[];
int ov49_02260E2C(void*,void*,int);
int ov49_02260E2C(void*owner,void*ui,int index){void*map=WiFiPlazaRoom_GetObjManager(ui);void*extra=WiFiPlazaRoom_GetModels(ui);void*ptr=ov49_0225A008(ui);void*net=WiFiPlazaRoom_GetLobby(ui);C*c=ov49_0225EF84(owner);int v[3];
switch(ov49_0225EF88(owner)){
case 0:c=ov49_0225EF40(owner,24);c->mon=WiFiPlazaObj_FindById(map,index);switch(ov49_0225A040(ui)){case 33:c->choice=2;break;case 34:c->choice=1;break;case 35:c->choice=0;break;default:GF_AssertFail();break;}ov49_0225EF8C(owner,1);break;
case 1:if(!ov45_0222A374(net)){c->next=8;ov49_0225EF8C(owner,6);}else if(ov45_0222A330(net)==1){c->next=10;ov49_0225EF8C(owner,6);}else{c->id=ov45_0222ADB8(net,index,c->choice);if(c->id!=-1){ov49_0225EF8C(owner,2);ov49_0225A09C(ui,WiFiPlazaRoom_GetMsgString(ui,0,31));ov49_0225A0BC(ui);}else{c->next=9;ov49_0225EF8C(owner,6);}}break;
case 2:if((gSystem.newKeys&2)||(gSystem.newKeys&128)){ov45_0222ADC8(net,index);ov49_0225A0CC(ui);c->state=6;c->next=15;ov49_0225EF8C(owner,12);}else if(ov49_022611F4(c,extra)==1){ov49_0225A0CC(ui);ov45_0222AE44(net);c->timer=0;ov45_0222A5E8(WiFiPlazaRoom_GetLobby(ui),10);ov49_0225A044(ui);WiFiPlazaCamera_ClearFollowObject(ptr);ov49_02259130(c->mon,0);ov49_0225916C(c->mon,1);PlaySE(1473);ov49_0225A510(ui);c->state=3;ov49_0225EF8C(owner,12);}break;
case 3:if(ov49_02261234(c,extra,ptr)==1){ov49_0225916C(c->mon,0);WiFiPlazaObj_SetDirection(c->mon,2);ov49_022591B4(c->mon,8);ov49_0225EF8C(owner,4);c->timer=0;}break;
case 4:ov49_02261434(c,extra,ui);ov49_022611D4(c,ptr);if(ov49_022613AC(c,extra)==1){ov49_0225EF8C(owner,5);ov49_0225A53C(ui,0);}break;
case 5:ov49_02261434(c,extra,ui);ov49_022611D4(c,ptr);WiFiPlazaObj_GetPosition(c->mon,v);ov49_02261460(c,extra,ui);if(v[0]<(6<<16)){c->ticks=0;ov49_0225EF8C(owner,13);}break;
case 6:ov49_02258EAC(map,c->mon,2,1);ov49_0225EF8C(owner,7);break;
case 7:if(!WiFiPlazaObj_GetParam(c->mon,5))ov49_0225EF8C(owner,c->next);break;
case 8:WiFiPlazaRoom_PrintMessage(ui,WiFiPlazaRoom_GetMsgString(ui,0,30));c->next=15;ov49_0225EF8C(owner,11);break;
case 9:WiFiPlazaRoom_PrintMessage(ui,WiFiPlazaRoom_GetMsgString(ui,0,32));c->next=15;ov49_0225EF8C(owner,11);break;
case 10:WiFiPlazaRoom_PrintMessage(ui,WiFiPlazaRoom_GetMsgString(ui,0,33));c->next=15;ov49_0225EF8C(owner,11);break;
case 11:if(WiFiPlazaRoom_IsMessageFinished(ui)==1)ov49_0225EF8C(owner,c->next);break;
case 12:WiFiPlazaRoom_CloseMessage(ui);ov49_0225EF8C(owner,c->state);break;
case 13:ov49_02261434(c,extra,ui);ov49_02261460(c,extra,ui);c->ticks++;if(c->ticks>120){ov49_0225A034(ui,1);ov49_0225A038(ui,0);WiFiPlazaRoom_CloseMessage(ui);ov45_0222A5E8(WiFiPlazaRoom_GetLobby(ui),11);ov49_0225EF8C(owner,14);}break;
case 14:ov49_02261434(c,extra,ui);ov49_02261460(c,extra,ui);break;
case 15:{void*obj=ov49_0225A010(ui);WiFiPlazaRoom_CloseMessage(ui);WiFiPlazaObj_SetHandler(map, (void*)(c->mon),1);ov49_0225EF68(owner);ov49_0225EF98(obj,index,ov49_02269B38,0);break;}}
return 0;}
