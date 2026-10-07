#pragma force_active on
#include <nitro/types.h>
// clang-format off
#include <nitro/code32.h>
// clang-format on
#define BASE (*(u8 **)0x0380FFF4)
extern void AddTask(int,int);
typedef struct {u16 state,x,result,reason,y,elapsed,step,z;u32 a,b;u8 *req,*confirm;} M35585;
#define MLME ((M35585 *)(BASE+4+0x400))
#define H(p,o) (*(u16 *)((u8 *)(p)+(o)))
extern u8 *MakeAssReqFrame(const void *);extern void MLME_AssTimeOut(void);extern void TxManCtrlFrame(void *),SetupTimeOut(u32,void (*)(void)),ResetTxqPri(int),ClearQueuedPri(int),MessageDeleteTx(int,int),IssueMlmeConfirm(void);
void MLME_AssTask(void);
// @fn MLME_AssTask
void MLME_AssTask(void){M35585 *p=MLME;switch(p->state){case 80:{u8 *frame=MakeAssReqFrame(p->req+16);if(!frame){H(p->confirm,4)=8;p->state=83;AddTask(2,3);}else{p->state=81;TxManCtrlFrame(frame);SetupTimeOut(H(p->req,0x18),MLME_AssTimeOut);}break;}case 83:ResetTxqPri(1);ClearQueuedPri(1);MessageDeleteTx(1,0);p->state=0;IssueMlmeConfirm();break;}}
