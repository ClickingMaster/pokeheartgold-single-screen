#pragma force_active on
#include "global.h"
// clang-format off
#include <nitro/code32.h>
// clang-format on
#pragma force_active on
typedef struct {u16 mask;u8 pad[510];} SharingBlock;
typedef struct {SharingBlock blocks[4]; u8 pad[8];u16 current;u8 gap[14];u16 mode,other,state;} Sharing;
typedef struct {u16 unk,error,state;u8 pad[6];void *data;u16 other,aid;u8 gap[8]; Sharing *sharing;} Callback;
extern void WmDataSharingReceiveData(Sharing *,u16,void *);
extern void WmDataSharingSendDataSet(Sharing *,int);
void WmDataSharingReceiveCallback_Parent(Callback *);
#pragma opt_propagation off
#pragma opt_dead_assignments off
void WmDataSharingReceiveCallback_Parent(Callback *cb) {
 Sharing *s=cb->sharing;
 if(!s)return;
 if(!cb->error) {
 switch(cb->state) {
 case 21: WmDataSharingReceiveData(s,cb->aid,cb->data); WmDataSharingSendDataSet(s,0);break;
 case 7: WmDataSharingSendDataSet(s,0);break;
 case 9: case 26: {u16 aid=cb->aid;u32 one=1;OSIntrMode irq=OS_DisableInterrupts(); u16 index=s->current; u32 mask=~(one<<aid); s->blocks[index].mask &=mask; if(s->mode==1)s->blocks[(u16)((index+1)&3)].mask &=mask;OS_RestoreInterrupts(irq);WmDataSharingSendDataSet(s,0);if(s->mode==1)WmDataSharingSendDataSet(s,0);break;}
 case 25:break;
 }
 } else {s->state=5;}
}
