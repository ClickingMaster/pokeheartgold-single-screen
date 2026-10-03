#pragma force_active on
// clang-format off
#include "global.h"
#include <nitro/code32.h>
// clang-format on
#pragma force_active on
typedef void (*Callback)(int,void*);
typedef struct {u8 head[0x504];void *receive;u8 pad[4];u8 sending;u8 pad2[11];u16 sendSize,receiveSize;Callback callback;u8 pad3[6];u16 ending,active,children;volatile u16 option;} Control;
typedef struct {u8 head[0x131c];u32 starting,alternate;u8 pad[0x14e8-0x1324];u32 children[15];u8 pad2[0x74c8-0x1524];u32 delay;} Business;
typedef struct {u32 pad[2];Control *control;Business *business;} Global;
typedef union {struct {u16 api,error,state,pad,substate,pad2,unused[2],aid;} fields;struct {u8 pad[8];void *data;} data;} Event;
extern Global ov13_0224CF40;extern struct {u8 pad[12];u32 option;} ov13_0224598C;
extern void MBi_OnInitializeDone(void),MBi_CheckWmErrcode(int,int),MBi_EndCommon(Event*);extern int MBi_IsSendEnabled(void);
extern int WM_SetParentParameter(void(*)(Event*),void*),WM_SetBeaconIndication(void(*)(Event*),int),WMi_StartParentEx(void(*)(Event*),u32),WM_StartMPEx(void(*)(Event*),void*,u16,void*,u16,u16,int,int,int,int,int),WM_End(void(*)(Event*)),WM_SetPortCallback(int,void*,void*),WM_SetIndCallback(void*);
void MBi_ParentCallback(Event*);
static inline void StartMP(int option){Control *c=*(Control *volatile*)&ov13_0224CF40.control;MBi_CheckWmErrcode(14,WM_StartMPEx(MBi_ParentCallback,c->receive,c->receiveSize,(u8*)c+64,c->sendSize,(u16)option,0,0,0,1,1));}
void MBi_ParentCallback(Event *event){switch(event->fields.api){case 0:if(event->fields.error){ov13_0224CF40.control->callback(256,event);return;}MBi_OnInitializeDone();return;
case 29:if(event->fields.error){ov13_0224CF40.control->callback(256,event);return;}MBi_CheckWmErrcode(7,WM_SetParentParameter(MBi_ParentCallback,ov13_0224CF40.control));return;
case 7:ov13_0224CF40.control->callback(21,event);MBi_CheckWmErrcode(25,WM_SetBeaconIndication(MBi_ParentCallback,1));return;
case 25:if(ov13_0224CF40.control->ending==0){if(event->fields.error){ov13_0224CF40.control->callback(256,event);return;}MBi_CheckWmErrcode(8,WMi_StartParentEx(MBi_ParentCallback,ov13_0224598C.option));return;}if(event->fields.error){ov13_0224CF40.control->ending=0;ov13_0224CF40.control->callback(256,event);return;}MBi_EndCommon(event);return;
case 8:if(event->fields.error){ov13_0224CF40.control->callback(256,event);return;}switch(event->fields.substate){case 0:ov13_0224CF40.control->children=0;ov13_0224CF40.control->active=0;return;
case 7:if(ov13_0224CF40.control->ending==1)return;ov13_0224CF40.control->children |=1<<event->fields.aid;ov13_0224CF40.control->callback(0,event);if(ov13_0224CF40.control->active==0 && ov13_0224CF40.business->starting==0){int option;ov13_0224CF40.business->starting=1;option=ov13_0224CF40.control->option==0;StartMP(option);return;}if(MBi_IsSendEnabled())ov13_0224CF40.control->callback(25,0);return;
case 9:ov13_0224CF40.control->children &=~(1<<event->fields.aid);ov13_0224CF40.control->callback(1,event);return;
case 2:if(ov13_0224CF40.control->ending==1)return;ov13_0224CF40.control->callback(28,event);return;
case 26:return;
default:ov13_0224CF40.control->callback(256,event);return;}
case 14:ov13_0224CF40.business->starting=0;switch(event->fields.state){case 10:ov13_0224CF40.control->active=1;if(ov13_0224CF40.control->ending==0)ov13_0224CF40.control->callback(25,0);return;case 11:ov13_0224CF40.control->callback(3,event->data.data);return;default:ov13_0224CF40.control->callback(256,event);return;}
case 15:if(ov13_0224CF40.business->delay){u32 i,count=0;for(i=0;i<15;i++){if(ov13_0224CF40.business->children[i]){count++;if(count>=2)break;}}if(count==1)OS_SpinWait(13000);}ov13_0224CF40.control->sending=0;if(event->fields.error==0){ov13_0224CF40.control->callback(2,event);if(ov13_0224CF40.control->ending==0)ov13_0224CF40.control->callback(25,0);return;}if(event->fields.error==10){ov13_0224CF40.control->callback(42,event);return;}ov13_0224CF40.control->callback(19,event);if(ov13_0224CF40.control->ending==0)ov13_0224CF40.control->callback(25,0);return;
case 1:if(ov13_0224CF40.business->alternate==0){if(event->fields.error){ov13_0224CF40.control->ending=0;ov13_0224CF40.control->callback(256,event);return;}ov13_0224CF40.control->children=0;ov13_0224CF40.control->active=0;MBi_CheckWmErrcode(2,WM_End(MBi_ParentCallback));return;}WM_SetPortCallback(1,0,0);WM_SetIndCallback(0);if(event->fields.error){ov13_0224CF40.control->ending=0;ov13_0224CF40.control->callback(256,event);return;}MBi_CheckWmErrcode(25,WM_SetBeaconIndication(MBi_ParentCallback,0));return;
case 2:if(event->fields.error){ov13_0224CF40.control->ending=0;ov13_0224CF40.control->callback(256,event);return;}MBi_EndCommon(event);return;
case 13:if(event->fields.error)return;ov13_0224CF40.control->children &=~event->fields.pad2;return;
case 128:switch(event->fields.state){case 16:ov13_0224CF40.control->callback(29,event);return;case 17:ov13_0224CF40.control->callback(31,event);return;case 18:ov13_0224CF40.control->callback(32,event);return;case 19:ov13_0224CF40.control->callback(33,event);return;case 22:OS_Terminate();return;case 23:break;default:break;}break;
default:ov13_0224CF40.control->callback(256,event);return;}}
