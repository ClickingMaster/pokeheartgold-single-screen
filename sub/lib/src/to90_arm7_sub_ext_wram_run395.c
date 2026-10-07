#pragma force_active on
#include <nitro/types.h>
// clang-format off
#include <nitro/code32.h>
// clang-format on
#define BASE (*(u8 **)0x0380fff4)
#define H(p,o) (*(u16 *)((u8 *)(p)+(o)))
#define W(p,o) (*(u32 *)((u8 *)(p)+(o)))
typedef struct {u16 rest:14;u16 more:1;u16 top:1;} To90W37028Flags;
extern u32 CAM_GetPowerMgtMode(u32),CAM_GetStaState(u32);
extern void CAM_SetDoze(u32),CAM_SetStaState(u16,u32),CAM_ReleaseAID(u16),CAM_Delete(u16);
extern void MLME_IssueAuthIndication(void *,u16),MLME_IssueDeAuthIndication(void *,u16);
extern void MLME_IssueAssIndication(void *,u16,void *),MLME_IssueReAssIndication(void *,u16,void *);
extern u8 *MakeDeAuthFrame(void *,u16,u32);
extern void SetManCtrlFrame(void *),TxqPri(u32),WSetStaState(u32),WClearAids(void),IssueMlmeConfirm(void),CAM_DecFrameCount(void *),ReleaseHeapBuf(void *,void *);
void TxqEndManCtrl(void*,u32);
// Queue frames come from SetManCtrlFrame, which maps CAM miss to slot 0.
// Linked WvrBegin provides 16 CAM slots; valid frame indices are 0..15.
// @fn TxqEndManCtrl
void TxqEndManCtrl(void*arg,u32 schedule) {u8*frame=(u8*)arg;
 u8 *base=BASE,*ctl=base+0x344,*mlme=base+0x404,*queue=base+0x1a0;u32 index=H(frame,2),kind;
 if(!(H(frame,8)&2)){
 W(base,0x53c)++;if(H(frame,0x18)&1)W(base,0x550)++;else W(base,0x54c)++;
 if(CAM_GetPowerMgtMode(index) && !(H(frame,0x14)&0x2000))CAM_SetDoze(index);
 }else W(base,0x540)++;
 if(((To90W37028Flags *)(frame+0x14))->more)W(base,0x554)++;
 W(base,0x544)+=H(frame,0xc)&255;
 kind=H(frame,0x14)&0xfc;
 switch(kind){
 case 0xb0:
 if(index && !(H(frame,8)&2)){
 if(H(frame,0x2c)==0 && H(frame,0x2e)==2 && H(frame,0x30)==0){CAM_SetStaState((u16)index,0x30);MLME_IssueAuthIndication(frame+0x18,H(frame,0x2c));}
 else if(H(frame,0x2c)==1 && H(frame,0x2e)==4 && H(frame,0x30)==0){CAM_SetStaState((u16)index,0x30);MLME_IssueAuthIndication(frame+0x18,H(frame,0x2c));}
 }break;
 case 0x10:case 0x30:
 if(!index)break;
 if(!(H(frame,8)&2)){
 if(H(frame,0x2e)==0 && CAM_GetStaState(index)==0x30){CAM_SetStaState((u16)index,0x40);if(kind==0x10)MLME_IssueAssIndication(frame+0x18,H(frame,0x30),frame+0x14+H(frame,0x12));else MLME_IssueReAssIndication(frame+0x18,H(frame,0x30),frame+0x14+H(frame,0x12));}
 }else{
 u8 *deauth;CAM_ReleaseAID((u16)index);deauth=MakeDeAuthFrame(frame+0x18,1,0);if(deauth){H(deauth,0)=2;if(schedule){SetManCtrlFrame(deauth);TxqPri(1);}else SetManCtrlFrame(deauth);}
 }break;
 case 0xa0:
 if(H(ctl,12)==1){if(index){if(CAM_GetStaState(index)>0x30)CAM_SetStaState((u16)index,0x30);}
 else if(H(frame,0x18)&1){u32 station;for(station=1;station<H(BASE,0x322);station++)if(CAM_GetStaState(station)>0x30)CAM_SetStaState((u16)station,0x30);}}
 else if(H(ctl,8)>0x30){WSetStaState(0x30);WClearAids();}
 if(H(mlme,0)==0x71 && frame==*(u8 **)(mlme+4)){if(!(H(frame,8)&2))H(*(u8 **)(mlme+0x1c),4)=0;else H(*(u8 **)(mlme+0x1c),4)=12;H(mlme,0)=0;IssueMlmeConfirm();}break;
 case 0xc0:
 if(H(ctl,12)==1){if(index){if(CAM_GetStaState(index)>0x20)CAM_SetStaState((u16)index,0x20);}
 else if(H(frame,0x18)&1){u32 station;for(station=1;station<H(BASE,0x322);station++)if(CAM_GetStaState(station)>0x20)CAM_SetStaState((u16)station,0x20);}}
 else if(H(ctl,8)>0x20){WSetStaState(0x20);WClearAids();}
 if(H(mlme,0)==0x41 && frame==*(u8 **)(mlme+4)){if(!(H(frame,8)&2))H(*(u8 **)(mlme+0x1c),4)=0;else H(*(u8 **)(mlme+0x1c),4)=12;H(mlme,0)=0;IssueMlmeConfirm();}
 if(H(frame,0)==1){if(index){u8 *current=BASE;u32 bit=1U<<index;H(current,0x534)&=~bit;CAM_Delete((u16)index);}MLME_IssueDeAuthIndication(frame+0x18,1);}
 else if(H(frame,0)==2)MLME_IssueDeAuthIndication(frame+0x18,H(frame,0x2c));break;
 }
 CAM_DecFrameCount(frame);ReleaseHeapBuf(queue,frame-0x10);H(BASE,0x440)=0;if(schedule && H(queue,8))TxqPri(1);
}
