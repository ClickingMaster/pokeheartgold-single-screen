#pragma force_active on
// clang-format off
#include "global.h"
#include <nitro/code32.h>
// clang-format on
#pragma force_active on

typedef struct {u16 mask;u8 bytes[510];} DataSet;
typedef struct {DataSet data[4];u16 sequence[4];u16 write,read,unknown,mask,pad[2],port,size,doubleBuffer,pad2,state;} Sharing;
extern u16 WM_GetConnectedAIDs(void);
extern void WmDataSharingSetDataCallback(void*);
extern int WM_SetMPDataToPortEx(void(*)(void*),void*,void*,u16,u16,u16,u16);
void WmDataSharingSendDataSet(Sharing*,int);
void WmDataSharingSendDataSet(Sharing*p,int exclude){OSIntrMode interrupts=OS_DisableInterrupts();int index,next,clear,status;u16 connected;if(!p->data[p->write].mask){connected=WM_GetConnectedAIDs();index=p->write;next=(index+1)&3;if(p->doubleBuffer==1)clear=(next+1)&3;else clear=next;MI_CpuClear16(&p->data[clear],512);p->data[clear].mask=p->mask&(connected|1);p->write=next;p->data[index].mask=p->mask;if(exclude==1)p->data[index].mask&=~1;OS_RestoreInterrupts(interrupts);status=WM_SetMPDataToPortEx(WmDataSharingSetDataCallback,p,&p->data[index],p->port,p->mask&connected,p->size,1);if(status==7){p->sequence[index]=0xffff;p->read=(p->read+1)&3;}else {if(status==0||status==2)return;p->state=5;}}else OS_RestoreInterrupts(interrupts);}
