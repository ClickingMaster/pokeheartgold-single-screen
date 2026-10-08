#pragma force_active on
// clang-format off
#include "global.h"
#include <nitro/code32.h>
// clang-format on
#include "WM.h"
int WMi_CheckStateEx(int,...);u16 WM_GetAID(void),WM_GetConnectedAIDs(void);u32 MATH_CountPopulation(u32);
void WmDataSharingReceiveCallback_Parent(void*),WmDataSharingReceiveCallback_Child(void*),WmDataSharingSetDataCallback(void*);
int WM_SetPortCallback(u16,void(*)(void*),void*);
int WM_SetMPDataToPortEx(void(*)(void*),void*,void*,u16,u16,u16,u16);
int WM_StartDataSharing(WMDataSharingInfo*,u16,u16,u16,BOOL);
// @fn WM_StartDataSharing
int WM_StartDataSharing(WMDataSharingInfo*info,u16 port,u16 aid,u16 length,BOOL doubleMode){WMDataSet*dataset;int result,i;u16 participants;u32 population;u16 self,connected;connected=1;result=WMi_CheckStateEx(2,9,10);if(result)return result;if(!info)return 6;if(port>=16)return 6;if(!aid)return 6;self=WM_GetAID();if(!self)connected=WM_GetConnectedAIDs();MIi_CpuClearFast(0,(u32*)info,0x820);info->writeIndex=0;info->sendIndex=0;info->readIndex=0;info->dataLength=length;info->port=port;info->aidBitmap=0;info->doubleMode=doubleMode?1:0;participants=aid|(1u<<self);info->aidBitmap=participants;population=MATH_CountPopulation(participants);info->stationNumber=population;info->dataSetLength=length*population;if(info->dataSetLength>508){info->aidBitmap=0;return 6;}info->dataSetLength+=4;info->state=1;
 if(!self){u32 members=connected|1;for(i=0;i<4;i++)info->ds[i].aidBitmap=info->aidBitmap&members;WM_SetPortCallback(port,WmDataSharingReceiveCallback_Parent,info);dataset=info->ds;for(i=0;i<(info->doubleMode==1?2:1);dataset++,i++){info->writeIndex=(info->writeIndex+1)&3;result=WM_SetMPDataToPortEx(WmDataSharingSetDataCallback,info,dataset,info->dataSetLength,(u16)(info->aidBitmap&connected),info->port,1);if(result==7){info->seqNum[i]=0xffff;info->sendIndex=(info->sendIndex+1)&3;}else if(result!=0&&result!=2){info->state=5;return 1;}}}
 else{info->sendIndex=3;WM_SetPortCallback(port,WmDataSharingReceiveCallback_Child,info);}return 0;}
