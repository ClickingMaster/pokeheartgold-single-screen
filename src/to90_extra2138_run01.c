#pragma force_active on
// clang-format off
#include "global.h"
#include <nitro/code32.h>
// clang-format on
#pragma force_active on
typedef void(*Callback)(void*);typedef struct{u8 pad[0xcc];Callback callbacks[16];void*args[16];}System;
typedef struct{u16 api,error;u8 pad[6];u16 port;u8 pad2[14];u16 length;u8 pad3[4];void*arg;}Event;
typedef struct{u8 pad[0x800];u16 values[4];u16 pad8,index;u8 padc[0x10];u16 state;}Sharing;
extern System*WMi_GetSystemWork(void);extern void WmDataSharingReceiveCallback_Parent(void*),WmDataSharingReceiveCallback_Child(void*);extern u16 WM_GetAID(void);
void WmDataSharingSetDataCallback(Event*);
void WmDataSharingSetDataCallback(Event*event){System*system=WMi_GetSystemWork();Sharing*sharing=system->args[event->port];Callback callback=system->callbacks[event->port];u16 aid;if(callback!=WmDataSharingReceiveCallback_Parent&&callback!=WmDataSharingReceiveCallback_Child)return;if(!sharing)return;if(sharing!=event->arg)return;aid=WM_GetAID();if(event->error==0){if(aid!=0)return;sharing->values[sharing->index]=event->length>>1;sharing->index=(sharing->index+1)&3;}else if(event->error==10){if(aid!=0)sharing->index=(sharing->index+3)&3;sharing->state=4;}else sharing->state=5;}
