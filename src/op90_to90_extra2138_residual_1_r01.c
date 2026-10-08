#pragma force_active on
// clang-format off
#include "global.h"
#include <nitro/code32.h>
// clang-format on
#include "WM.h"
typedef WMDataSet DataSet;
typedef WMDataSharingInfo Sharing;
int WMi_CheckStateEx(int,...);
u16 WM_GetAID(void);
u16 WM_GetConnectedAIDs(void);
void WmDataSharingSetDataCallback(void*);
int WM_SetMPDataToPortEx(void(*)(void*),Sharing*,const void*,u16,u16,u16,u16);
void WmDataSharingSendDataSet(Sharing*,int);
void WmDataSharingReceiveData(Sharing*,u32,const void*);
int WM_StepDataSharing(Sharing *s,const void *input,void *output);
typedef char SlotExtentProof[sizeof(DataSet)==512?1:-1];
typedef char ParentExtentProof[sizeof(Sharing)==0x820?1:-1];
static inline int SubmitChildData(Sharing*s,const void*input,int result){int status;
            DataSet *dest=&s->ds[s->sendIndex];
            MIi_CpuCopy16(input,&dest->data[14],s->dataLength);
            status=WM_SetMPDataToPortEx(WmDataSharingSetDataCallback,s,&dest->data[14],s->dataLength,s->aidBitmap,s->port,1);
            s->sendIndex=(s->sendIndex+1u)&3u;
            if(status!=2 && status!=0){s->state=5;result=1;}
 return result;
}
// @fn WM_StepDataSharing
int WM_StepDataSharing(Sharing *s,const void *input,void *output){
    int status;u16 aid,connected;int result;
    status=WMi_CheckStateEx(2,9,10);
    if(status) return status;
    if(!s)return 6;
    if(!input)return 6;
    if(!output)return 6;
    aid=WM_GetAID();
    if(!aid)connected=WM_GetConnectedAIDs();
    if(s->state==5)return 1;
    if(s->state!=1 && s->state!=4)return 3;
    result=5;
    if(!aid){
        int parentReceived=0,empty=0;
        if(s->state==4){
            u32 index;
            s->state=1;index=(s->writeIndex+3u)&3u;
            status=WM_SetMPDataToPortEx(WmDataSharingSetDataCallback,s,&s->ds[index],s->dataSetLength,(u16)(s->aidBitmap&connected),s->port,1);
            if(status==7){s->seqNum[index]=0xffff;s->sendIndex=(s->sendIndex+1u)&3u;}
            else if(status!=0 && status!=2){s->state=5;return 1;}
        }
        if(s->readIndex!=s->sendIndex){
            s->ds[s->readIndex].aidBitmap|=1;
            MIi_CpuCopy16((const u16*)&s->ds[s->readIndex],output,512);
            s->currentSeqNum=s->seqNum[s->readIndex];s->readIndex=(s->readIndex+1u)&3u;
            parentReceived=1;result=0;
            if(!s->doubleMode && connected && s->ds[s->writeIndex].aidBitmap==1)empty=1;else empty=0;
        }
        WmDataSharingSendDataSet(s,0);
        if(parentReceived){
            WmDataSharingReceiveData(s,0,input);
            if(!s->doubleMode)WmDataSharingSendDataSet(s,empty);
        }
    }else{
        int childSubmit=0;
        if(s->state==4){childSubmit=1;s->state=1;}
        else if(s->readIndex!=s->writeIndex){
            if(!(s->ds[s->readIndex].aidBitmap&1))s->ds[s->readIndex].aidBitmap|=1;
            else{MIi_CpuCopy16((const u16*)&s->ds[s->readIndex],output,512);s->currentSeqNum=s->seqNum[s->readIndex];s->readIndex=(s->readIndex+1u)&3u;childSubmit=1;result=0;}
        }
        if(childSubmit){
            result=SubmitChildData(s,input,result);
        }
    }
    return result;
}
