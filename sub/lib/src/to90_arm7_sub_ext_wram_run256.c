#pragma force_active on
#include <nitro/types.h>
// clang-format off
#include <nitro/code32.h>
// clang-format on
#define BASE (*(u8 **)0x0380FFF4)
extern void AddTask(int,int);
typedef struct {u16 state,x,result,reason,y,elapsed,step,z;u32 a,b;u8 *req,*confirm;} M35587;
#define MLME ((M35587 *)(BASE+4+0x400))
#define H(p,o) (*(u16 *)((u8 *)(p)+(o)))
extern u8 *MakeReAssReqFrame(const void *);extern void MLME_ReAssTimeOut(void);extern void TxManCtrlFrame(void *),SetupTimeOut(u32,void (*)(void)),ResetTxqPri(int),ClearQueuedPri(int),MessageDeleteTx(int,int),IssueMlmeConfirm(void);
void MLME_ReAssTask(void);
// @fn MLME_ReAssTask
void MLME_ReAssTask(void){M35587 *p=MLME;switch(p->state){case 96:{u8 *frame=MakeReAssReqFrame(p->req+16);if(!frame){H(p->confirm,4)=8;p->state=99;AddTask(2,4);}else{p->state=97;TxManCtrlFrame(frame);SetupTimeOut(H(p->req,0x18),MLME_ReAssTimeOut);}break;}case 99:ClearQueuedPri(1);MessageDeleteTx(1,0);p->state=0;IssueMlmeConfirm();break;}}
