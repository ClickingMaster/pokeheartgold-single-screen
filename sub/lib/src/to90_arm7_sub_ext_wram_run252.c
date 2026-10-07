#pragma force_active on
#include <nitro/types.h>
// clang-format off
#include <nitro/code32.h>
// clang-format on
#define BASE (*(u8 **)0x0380FFF4)
extern void AddTask(int,int);
typedef struct {u16 state,x,result,reason,y,elapsed,step,z;u32 a,b;u8 *req,*confirm;} M35583;
#define MLME ((M35583 *)(BASE+4+0x400))
#define H(p,o) (*(u16 *)((u8 *)(p)+(o)))
extern u8 *MakeAuthFrame(const void *,int,int);extern void MLME_AuthTimeOut(void);extern void TxManCtrlFrame(void *),SetupTimeOut(u32,void (*)(void)),ResetTxqPri(int),ClearQueuedPri(int),MessageDeleteTx(int,int),IssueMlmeConfirm(void);
void MLME_AuthTask(void);
// @fn MLME_AuthTask
void MLME_AuthTask(void){M35583 *p=MLME;switch(p->state){case 48:{u8 *frame=MakeAuthFrame(p->req+16,0,0);if(!frame){H(p->confirm,4)=8;p->state=53;AddTask(2,2);}else{H(frame,0x2c)=H(p->req,0x16);H(frame,0x2e)=1;H(frame,0x30)=0;p->state=49;TxManCtrlFrame(frame);SetupTimeOut(H(p->req,0x18),MLME_AuthTimeOut);}break;}case 53:ResetTxqPri(1);ClearQueuedPri(1);MessageDeleteTx(1,0);p->state=0;IssueMlmeConfirm();break;}}
